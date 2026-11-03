#include "MochilaItens.hpp"
#include <godot_cpp/variant/utility_functions.hpp>

MochilaItens::MochilaItens(const Herois& heroi) : heroi(heroi), heroiMutavel(nullptr) {}
MochilaItens::MochilaItens(Herois& heroi) : heroi(heroi), heroiMutavel(&heroi) {}

int MochilaItens::getQuantidadeItensPassivos() const {
    return static_cast<int>(heroi.itensPassivos.size());
}

std::string MochilaItens::getNomeItemPassivo(int indice) const {
    if (indice < 0 || indice >= static_cast<int>(heroi.itensPassivos.size())) return "";
    return heroi.itensPassivos[indice].nome;
}

void MochilaItens::equiparItemEspecial(ItemEspecial item) {
    if (!heroiMutavel) {
        godot::UtilityFunctions::push_warning("Não é possível equipar um item por uma mochila somente de leitura.");
        return;
    }
    heroiMutavel->definirItemEspecial(item);
}

bool MochilaItens::possuiItemEspecialEquipado() const {
    return heroi.possuiItemEspecialAtivo;
}

std::string MochilaItens::getNomeItemEspecialEquipado() const {
    return heroi.possuiItemEspecialAtivo ? heroi.itemEspecialAtivo.nome : "";
}
