#include "Professor.hpp"
#include <cstdlib>

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