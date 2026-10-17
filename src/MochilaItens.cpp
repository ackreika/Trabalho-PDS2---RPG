#include "MochilaItens.hpp"

MochilaItens::MochilaItens(Herois& heroi) : heroi(heroi) {}

int MochilaItens::getQuantidadeItensPassivos() const {
    return heroi.getQuantidadeItensPassivos();
}

std::string MochilaItens::getNomeItemPassivo(int indice) const {
    return heroi.getNomeItemPassivo(indice);
}

void MochilaItens::equiparItemEspecial(ItemEspecial item) {
    heroi.definirItemEspecial(item);
}

bool MochilaItens::possuiItemEspecialEquipado() const {
    return heroi.possuiItemEspecial();
}

std::string MochilaItens::getNomeItemEspecialEquipado() const {
    return heroi.getNomeItemEspecial();
}