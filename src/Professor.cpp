#include "Professor.hpp"
#include <cstdlib>
#include <godot_cpp/core/class_db.hpp>

Professor::Professor()
    : nome("Professor"), materia("Materia"), tipo(TipoProfessor::COMUM),
      vida(50), vidaMaxima(50), defesa(5), invulneravel(false), ehBatalhaFalsa(false) {}

Professor::Professor(std::string nome, std::string materia, int vidaMaxima, int defesa,
                      TipoProfessor tipo, bool batalhaFalsa)
    : nome(nome), materia(materia), tipo(tipo), vida(vidaMaxima), vidaMaxima(vidaMaxima), defesa(defesa),
      invulneravel(tipo == TipoProfessor::BOSS), ehBatalhaFalsa(batalhaFalsa) {}

std::string Professor::getNome() const { return nome; }
std::string Professor::getMateria() const { return materia; }
TipoProfessor Professor::getTipo() const { return tipo; }
int Professor::getVida() const { return vida; }
int Professor::getVidaMaxima() const { return vidaMaxima; }
bool Professor::estaVivo() const { return vida > 0; }

bool Professor::isInvulneravel() const { return invulneravel; }
void Professor::removerInvulnerabilidade() { invulneravel = false; }
bool Professor::isBatalhaFalsa() const { return ehBatalhaFalsa; }

void Professor::adicionarMovimento(Movimento movimento) {
    movimentos.push_back(movimento);
}

Movimento& Professor::escolherAtaque() {
    std::vector<int> indicesDisponiveis;
    for (size_t i = 0; i < movimentos.size(); i++) {
        if (movimentos[i].podeUsar()) {
            indicesDisponiveis.push_back(static_cast<int>(i));
        }
    }

    if (indicesDisponiveis.empty()) {
        return movimentos[0]; // fallback: sem golpes disponíveis, usa o primeiro mesmo assim
    }

    int escolhido = indicesDisponiveis[std::rand() % indicesDisponiveis.size()];
    return movimentos[escolhido];
}

void Professor::receberDano(int danoBase) {
    if (invulneravel) return; // Boss protegido até o item de história ser usado — dano recebido = 0

    double fator = static_cast<double>(vida) / (vida + defesa);
    int danoFinal = static_cast<int>(danoBase * fator);
    vida -= danoFinal;
    if (vida < 0) vida = 0;
}

void Professor::definirSprite(EstadoVisual estado, std::string caminho) {
    spritesPorEstado[estado] = caminho;
}

std::string Professor::getCaminhoSprite(EstadoVisual estado) const {
    auto it = spritesPorEstado.find(estado);
    if (it != spritesPorEstado.end()) {
        return it->second;
    }
    auto fallback = spritesPorEstado.find(EstadoVisual::PARADO);
    return fallback != spritesPorEstado.end() ? fallback->second : "";
}

// ======================================================================
// Wrappers Godot-friendly (ver Professor.hpp)
// ======================================================================

void Professor::inicializar(String nomeNovo, String materiaNova, int vidaMaximaNova, int defesaNova, int tipoInt, bool batalhaFalsaNova) {
    nome = std::string(nomeNovo.utf8().get_data());
    materia = std::string(materiaNova.utf8().get_data());
    vidaMaxima = vidaMaximaNova;
    vida = vidaMaximaNova;
    defesa = defesaNova;
    tipo = static_cast<TipoProfessor>(tipoInt);
    ehBatalhaFalsa = batalhaFalsaNova;
    invulneravel = (tipo == TipoProfessor::BOSS);
}

String Professor::get_nome() const { return String(nome.c_str()); }
String Professor::get_materia() const { return String(materia.c_str()); }
int Professor::get_vida() const { return vida; }
int Professor::get_vida_maxima() const { return vidaMaxima; }
bool Professor::esta_vivo() const { return estaVivo(); }
bool Professor::is_invulneravel() const { return invulneravel; }

void Professor::adicionar_movimento_simples(String nomeMovimento, int dano, int usosMaximos) {
    std::string nomeConvertido = std::string(nomeMovimento.utf8().get_data());
    adicionarMovimento(Movimento(nomeConvertido, dano, usosMaximos));
}

int Professor::get_tipo() const { return static_cast<int>(tipo); }
bool Professor::is_batalha_falsa() const { return ehBatalhaFalsa; }

void Professor::definir_sprite(int estado, String caminho) {
    definirSprite(static_cast<EstadoVisual>(estado), std::string(caminho.utf8().get_data()));
}

String Professor::get_caminho_sprite(int estado) const {
    return String(getCaminhoSprite(static_cast<EstadoVisual>(estado)).c_str());
}

void Professor::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_tipo"), &Professor::get_tipo);
    ClassDB::bind_method(D_METHOD("is_batalha_falsa"), &Professor::is_batalha_falsa);
    ClassDB::bind_method(D_METHOD("definir_sprite", "estado", "caminho"), &Professor::definir_sprite);
    ClassDB::bind_method(D_METHOD("get_caminho_sprite", "estado"), &Professor::get_caminho_sprite);
    ClassDB::bind_method(D_METHOD("inicializar", "nome", "materia", "vida_maxima", "defesa", "tipo", "batalha_falsa"), &Professor::inicializar);
    ClassDB::bind_method(D_METHOD("get_nome"), &Professor::get_nome);
    ClassDB::bind_method(D_METHOD("get_materia"), &Professor::get_materia);
    ClassDB::bind_method(D_METHOD("get_vida"), &Professor::get_vida);
    ClassDB::bind_method(D_METHOD("get_vida_maxima"), &Professor::get_vida_maxima);
    ClassDB::bind_method(D_METHOD("esta_vivo"), &Professor::esta_vivo);
    ClassDB::bind_method(D_METHOD("is_invulneravel"), &Professor::is_invulneravel);
    ClassDB::bind_method(D_METHOD("adicionar_movimento_simples", "nome", "dano", "usos_maximos"), &Professor::adicionar_movimento_simples);
}