#include "ItensConcretos.hpp"

namespace {
/** Tabela única ItemId <-> texto. Ao criar um item novo em ItensIds.hpp, acrescente-o aqui também. */
struct NomeDeItem {
    ItemId id;
    const char* texto;
};

const NomeDeItem NOMES_DE_ITENS[] = {
    {ItemId::HALTER,        "HALTER"},
    {ItemId::SUCO,          "SUCO"},
    {ItemId::LASER,         "LASER"},
    {ItemId::JAMES_STEWART, "JAMES_STEWART"},
    {ItemId::INTEGRAL,      "INTEGRAL"},
    {ItemId::DERIVADA,      "DERIVADA"},
    {ItemId::VETOR,         "VETOR"},
    {ItemId::INTERPOLACAO,  "INTERPOLACAO"},
    {ItemId::REFRIGERADOR,  "REFRIGERADOR"},
    {ItemId::PONTEIRO,      "PONTEIRO"},
};
}

ItemPassivo criarItemPassivo(ItemId id) {
    switch (id) {
        case ItemId::HALTER:
            return ItemPassivo{"Halter", TipoEfeito::BONUS_DANO_CAUSADO, 1.05f};
        case ItemId::SUCO:
            return ItemPassivo{"Suco", TipoEfeito::BONUS_VIDA_MAXIMA, 1.05f};
        case ItemId::JAMES_STEWART:
            return ItemPassivo{"James Stewart", TipoEfeito::BONUS_VIDA_MAXIMA, 1.10f};
        case ItemId::INTEGRAL:
            return ItemPassivo{"Integral", TipoEfeito::BONUS_VIDA_MAXIMA, 1.10f};
        case ItemId::DERIVADA:
            return ItemPassivo{"Derivada", TipoEfeito::BONUS_DANO_CAUSADO, 1.10f};
        case ItemId::INTERPOLACAO:
            return ItemPassivo{"Interpolacao", TipoEfeito::BONUS_VIDA_MAXIMA, 1.10f};
        case ItemId::REFRIGERADOR:
            return ItemPassivo{"Refrigerador", TipoEfeito::BONUS_VIDA_MAXIMA, 1.15f};
        default:
            return ItemPassivo{"Item Desconhecido", TipoEfeito::BONUS_DANO_CAUSADO, 1.0f};
    }
}

ItemEspecial criarItemEspecial(ItemId id) {
    switch (id) {
        case ItemId::LASER:
            return ItemEspecial{"Laser", 2.5f, /*usosMaximos=*/1, /*usosRestantes=*/1};
        case ItemId::VETOR:
            return ItemEspecial{"Vetor", 1.8f, 2, 2};
        case ItemId::PONTEIRO:
            return ItemEspecial{"Ponteiro", 3.0f, 1, 1};
        default:
            return ItemEspecial{"Item Desconhecido", 1.0f, 0, 0};
    }
}

bool ehItemEspecial(ItemId id) {
    switch (id) {
        case ItemId::LASER:
        case ItemId::VETOR:
        case ItemId::PONTEIRO:
            return true;
        default:
            return false;
    }
}

bool itemIdDeTexto(const std::string& texto, ItemId& saida) {
    for (const NomeDeItem& entrada : NOMES_DE_ITENS) {
        if (texto == entrada.texto) {
            saida = entrada.id;
            return true;
        }
    }
    return false;
}

std::string itemIdParaTexto(ItemId id) {
    for (const NomeDeItem& entrada : NOMES_DE_ITENS) {
        if (entrada.id == id) {
            return entrada.texto;
        }
    }
    return "";
}

bool ehItemPassivo(ItemId id) {
    return !itemIdParaTexto(id).empty() && !ehItemEspecial(id);
}
