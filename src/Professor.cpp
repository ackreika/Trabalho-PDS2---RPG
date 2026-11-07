#include "Professor.hpp"
#include <cstdlib>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <algorithm>
#include <cmath>
#include <limits>

namespace {
TipoProfessor validarTipoProfessor(TipoProfessor tipo) {
	switch (tipo) {
		case TipoProfessor::COMUM:
		case TipoProfessor::MINIBOSS:
		case TipoProfessor::BOSS:
			return tipo;
		default:
			godot::UtilityFunctions::push_warning("Tipo de professor inválido; usando tipo comum.");
			return TipoProfessor::COMUM;
	}
}
}

Professor::Professor()
    : nome("Professor"), materia("Materia"), tipo(TipoProfessor::COMUM),
      vida(50), vidaMaxima(50), defesa(5), statusInvulnerabilidade(TipoModificador::INVULNERAVEL, 0),
      invulnerabilidadeNaMetadeHabilitada(false), invulnerabilidadeMetadeAtivada(false), ehBatalhaFalsa(false) {}

Professor::Professor(std::string nome, std::string materia, int vidaMaxima, int defesa,
                      TipoProfessor tipo, bool batalhaFalsa)
    : nome(nome.empty() ? "Professor" : nome), materia(materia.empty() ? "Materia" : materia), tipo(validarTipoProfessor(tipo)),
      vida(std::max(1, vidaMaxima)), vidaMaxima(std::max(1, vidaMaxima)), defesa(std::max(0, defesa)),
      statusInvulnerabilidade(TipoModificador::INVULNERAVEL, validarTipoProfessor(tipo) == TipoProfessor::BOSS ? -1 : 0),
      invulnerabilidadeNaMetadeHabilitada(false), invulnerabilidadeMetadeAtivada(false), ehBatalhaFalsa(batalhaFalsa) {}

std::string Professor::getNome() const { return nome; }
std::string Professor::getMateria() const { return materia; }
TipoProfessor Professor::getTipo() const { return tipo; }
int Professor::getVida() const { return vida; }
int Professor::getVidaMaxima() const { return vidaMaxima; }
bool Professor::estaVivo() const { return vida > 0; }

bool Professor::isInvulneravel() const { return statusInvulnerabilidade.estaAtivo(); }
void Professor::removerInvulnerabilidade() {
    statusInvulnerabilidade = ModificadorStatus(TipoModificador::INVULNERAVEL, 0);
    if (invulnerabilidadeMetadeAtivada) invulnerabilidadeNaMetadeHabilitada = false;
}
void Professor::ativarInvulnerabilidadeNaMetadeDaVida() {
    statusInvulnerabilidade = ModificadorStatus(TipoModificador::INVULNERAVEL, 0);
    invulnerabilidadeNaMetadeHabilitada = true;
    invulnerabilidadeMetadeAtivada = false;
}
bool Professor::isBatalhaFalsa() const { return ehBatalhaFalsa; }

void Professor::adicionarMovimento(Movimento movimento) {
    movimentos.push_back(movimento);
}

Movimento& Professor::escolherAtaque() {
    if (movimentos.empty()) {
        static Movimento ataquePadrao("Ataque Basico", 1, -1);
        UtilityFunctions::push_warning("Professor sem movimentos cadastrados; usando ataque básico de segurança.");
        return ataquePadrao;
    }
    std::vector<int> indicesDisponiveis;
    for (size_t i = 0; i < movimentos.size(); i++) {
        if (movimentos[i].podeUsar()) {
            indicesDisponiveis.push_back(static_cast<int>(i));
        }
    }

    if (indicesDisponiveis.empty()) {
        return movimentos[0]; // fallback: sem golpes disponíveis, usa o primeiro mesmo assim
    }

    const int escolhido = static_cast<int>(UtilityFunctions::randi_range(0, static_cast<int64_t>(indicesDisponiveis.size()) - 1));
    return movimentos[escolhido];
}

void Professor::receberDano(int danoBase) {
    if (danoBase <= 0 || vida <= 0) return;
    danoBase = statusInvulnerabilidade.interceptarDano(danoBase);
    if (danoBase <= 0) return;

    const double divisor = static_cast<double>(vida) + std::max(0, defesa);
    if (divisor <= 0.0) return;
    const double fator = static_cast<double>(vida) / divisor;
    const int danoFinal = std::clamp(static_cast<int>(danoBase * fator), 0, vida);
    vida -= danoFinal;
    if (vida < 0) vida = 0;
    if (invulnerabilidadeNaMetadeHabilitada && !invulnerabilidadeMetadeAtivada &&
        vida > 0 && vida <= vidaMaxima / 2) {
        statusInvulnerabilidade = ModificadorStatus(TipoModificador::INVULNERAVEL, -1);
        invulnerabilidadeMetadeAtivada = true;
    }
}

void Professor::definirSprite(EstadoVisual estado, std::string caminho) {
    const int valor = static_cast<int>(estado);
    if (valor < static_cast<int>(EstadoVisual::TOMANDO_DANO) || valor > static_cast<int>(EstadoVisual::ATACANDO)) {
        UtilityFunctions::push_warning("Estado visual de professor fora da faixa; sprite ignorado.");
        return;
    }
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

int Professor::get_tipo() const { return static_cast<int>(tipo); }
bool Professor::is_batalha_falsa() const { return ehBatalhaFalsa; }

// ======================================================================
// Wrappers Godot-friendly (ver Professor.hpp)
// ======================================================================

void Professor::inicializar(String nomeNovo, String materiaNova, int vidaMaximaNova, int defesaNova, int tipoInt, bool batalhaFalsaNova) {
    if (vidaMaximaNova <= 0 || defesaNova < 0 || tipoInt < static_cast<int>(TipoProfessor::COMUM) ||
        tipoInt > static_cast<int>(TipoProfessor::BOSS)) {
        UtilityFunctions::push_error("Professor não inicializado: vida, defesa ou tipo inválido.");
        return;
    }
    nome = nomeNovo.strip_edges().is_empty() ? "Professor" : std::string(nomeNovo.utf8().get_data());
    materia = materiaNova.strip_edges().is_empty() ? "Materia" : std::string(materiaNova.utf8().get_data());
    vidaMaxima = vidaMaximaNova;
    vida = vidaMaximaNova;
    defesa = defesaNova;
    tipo = static_cast<TipoProfessor>(tipoInt);
    ehBatalhaFalsa = batalhaFalsaNova;
    statusInvulnerabilidade = ModificadorStatus(TipoModificador::INVULNERAVEL, tipo == TipoProfessor::BOSS ? -1 : 0);
    invulnerabilidadeNaMetadeHabilitada = false;
    invulnerabilidadeMetadeAtivada = false;
}

String Professor::get_nome() const { return String::utf8(nome.c_str()); }
String Professor::get_materia() const { return String::utf8(materia.c_str()); }
int Professor::get_vida() const { return vida; }
int Professor::get_vida_maxima() const { return vidaMaxima; }
bool Professor::esta_vivo() const { return estaVivo(); }
bool Professor::is_invulneravel() const { return isInvulneravel(); }

void Professor::adicionar_movimento_simples(String nomeMovimento, int dano, int usosMaximos) {
    std::string nomeConvertido = std::string(nomeMovimento.utf8().get_data());
    adicionarMovimento(Movimento(nomeConvertido, dano, usosMaximos));
}

void Professor::definir_sprite(int estado, String caminho) {
    definirSprite(static_cast<EstadoVisual>(estado), std::string(caminho.utf8().get_data()));
}

String Professor::get_caminho_sprite(int estado) const {
    return String::utf8(getCaminhoSprite(static_cast<EstadoVisual>(estado)).c_str());
}

void Professor::_bind_methods() {
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
    ClassDB::bind_method(D_METHOD("get_tipo"), &Professor::get_tipo);
    ClassDB::bind_method(D_METHOD("is_batalha_falsa"), &Professor::is_batalha_falsa);
    ClassDB::bind_method(D_METHOD("remover_invulnerabilidade"), &Professor::removerInvulnerabilidade);  
}
