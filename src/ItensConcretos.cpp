#include "ItensConcretos.hpp"

ItemPassivo criarItemPassivo(ItemId id) {
    switch (id) {
        case ItemId::HALTER:
            return ItemPassivo{"Halter", TipoEfeito::BONUS_DANO_CAUSADO, 1.05f}; // +5% de dano
        case ItemId::SUCO:
            return ItemPassivo{"Suco", TipoEfeito::BONUS_VIDA_MAXIMA, 1.05f}; // +5% de vida maxima
        default:
            return ItemPassivo{"Item Desconhecido", TipoEfeito::BONUS_DANO_CAUSADO, 1.0f};
    }
}

ItemEspecial criarItemEspecial(ItemId id) {
    switch (id) {
        case ItemId::LASER:
            return ItemEspecial{"Laser", 2.5f, /*usosMaximos=*/1, /*usosRestantes=*/1};
        default:
            return ItemEspecial{"Item Desconhecido", 1.0f, 0, 0};
    }
}