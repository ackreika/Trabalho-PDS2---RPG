#include "Herois.hpp"
#include "Professor.hpp"
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

Herois::Herois(std::string nome, int vidaMaxima, int defesa, int danoBase)
    : nome(nome), nivel(1), vida(vidaMaxima), vidaMaxima(vidaMaxima), vidaMaximaBase(vidaMaxima),
      defesa(defesa), danoBase(danoBase),
      possuiItemEspecialAtivo(false), itemEspecialAtivo{"", 0.0f, 0, 0},
      possuiItemChave(false) {}

std::string Herois::getNome() const { return nome; }
int Herois::getVida() const { return vida; }
int Herois::getVidaMaxima() const { return vidaMaxima; }
int Herois::getDefesa() const { return defesa; }
int Herois::getDanoBase() const { return danoBase; }
bool Herois::estaVivo() const { return vida > 0; }

void Herois::curar(int quantidade) {
    vida += quantidade;
    if (vida > vidaMaxima) vida = vidaMaxima;
}

void Herois::receberDano(int danoBase) {
    double fator = static_cast<double>(vida) / (vida + defesa);
    int danoFinal = static_cast<int>(danoBase * fator);
    vida -= danoFinal;
    if (vida < 0) vida = 0;
}

void Herois::aprenderMovimento(const Movimento& novo, int indiceParaSubstituir) {
    if (movimentos.size() < 3) {
        movimentos.push_back(novo);
    } else if (indiceParaSubstituir >= 0 && indiceParaSubstituir < static_cast<int>(movimentos.size())) {
        movimentos[indiceParaSubstituir] = novo;
    }
}

const std::vector<Movimento>& Herois::getMovimentos() const {
    return movimentos;
}

Movimento* Herois::getMovimentoParaEditar(int indice) {
    if (indice < 0 || indice >= static_cast<int>(movimentos.size())) {
        return nullptr;
    }
    return &movimentos[indice];
}

void Herois::adicionarItemPassivo(ItemPassivo item) {
    itensPassivos.push_back(item);
    recalcularVidaMaxima();
}

int Herois::getQuantidadeItensPassivos() const {
    return static_cast<int>(itensPassivos.size());
}

std::string Herois::getNomeItemPassivo(int indice) const {
    if (indice < 0 || indice >= static_cast<int>(itensPassivos.size())) {
        return "";
    }
    return itensPassivos[indice].nome;
}

int Herois::calcularDanoFinal(int danoBase) const {
    int dano = danoBase;
    for (const auto& item : itensPassivos) {
        if (item.tipo == TipoEfeito::BONUS_DANO_CAUSADO) {
            dano = static_cast<int>(dano * item.multiplicador);
        }
    }
    return dano;
}

int Herois::calcularVidaFinal(int vidaBase) const {
    int vidaFinal = vidaBase;
    for (const auto& item : itensPassivos) {
        if (item.tipo == TipoEfeito::BONUS_VIDA_MAXIMA) {
            vidaFinal = static_cast<int>(vidaFinal * item.multiplicador);
        }
    }
    return vidaFinal;
}

void Herois::recalcularVidaMaxima() {
    int novaVidaMaxima = calcularVidaFinal(vidaMaximaBase);
    int delta = novaVidaMaxima - vidaMaxima;
    vidaMaxima = novaVidaMaxima;

    if (delta > 0) {
        vida += delta; // ganhar vida máxima também cura a diferença, na hora
    } else if (vida > vidaMaxima) {
        vida = vidaMaxima;
    }
}

void Herois::definirItemEspecial(ItemEspecial item) {
    itemEspecialAtivo = item;
    possuiItemEspecialAtivo = true;
}

bool Herois::possuiItemEspecial() const { return possuiItemEspecialAtivo; }

std::string Herois::getNomeItemEspecial() const {
    return possuiItemEspecialAtivo ? itemEspecialAtivo.nome : "";
}

bool Herois::itemEspecialPodeUsar() const {
    return possuiItemEspecialAtivo && itemEspecialAtivo.usosRestantes > 0;
}

int Herois::usarItemEspecial(Professor& alvo) {
    if (!itemEspecialPodeUsar()) {
        return 0;
    }
    itemEspecialAtivo.usosRestantes--;
    int dano = static_cast<int>(danoBase * itemEspecialAtivo.multiplicadorDano);
    alvo.receberDano(dano);
    return dano;
}

void Herois::obterItemChave() { possuiItemChave = true; }
bool Herois::possuiItemDeChave() const { return possuiItemChave; }
void Herois::consumirItemChave() { possuiItemChave = false; }

void Herois::definirSprite(EstadoVisual estado, std::string caminho) {
    spritesPorEstado[estado] = caminho;
}

std::string Herois::getCaminhoSprite(EstadoVisual estado) const {
    auto it = spritesPorEstado.find(estado);
    if (it != spritesPorEstado.end()) {
        return it->second;
    }
    auto fallback = spritesPorEstado.find(EstadoVisual::PARADO);
    return fallback != spritesPorEstado.end() ? fallback->second : "";
}
void Herois::_bind_methods() {
    // Exemplo: Expõe a função curar para o GDScript e Editor da Godot
    ClassDB::bind_method(D_METHOD("curar", "quantidade"), &Herois::curar);
}