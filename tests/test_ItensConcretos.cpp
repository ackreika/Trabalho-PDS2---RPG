#include "../third_party/doctest.h"
#include "../include/ItensConcretos.hpp"

namespace {
const int TOTAL_DE_ITENS = 10; // ItemId vai de 0 (HALTER) a 9 (PONTEIRO)
}

TEST_CASE("ItensConcretos: conversao ida e volta entre ItemId e texto") {
    for (int i = 0; i < TOTAL_DE_ITENS; i++) {
        ItemId id = static_cast<ItemId>(i);
        std::string texto = itemIdParaTexto(id);
        CHECK_FALSE(texto.empty());

        ItemId volta = ItemId::HALTER;
        CHECK(itemIdDeTexto(texto, volta));
        CHECK(static_cast<int>(volta) == i);
    }
}

TEST_CASE("ItensConcretos: texto desconhecido nao e convertido e nao altera a saida") {
    ItemId saida = ItemId::LASER;
    CHECK_FALSE(itemIdDeTexto("NAO_EXISTE", saida));
    CHECK_FALSE(itemIdDeTexto("", saida));
    CHECK_FALSE(itemIdDeTexto("halter", saida)); // diferencia maiusculas de minusculas
    CHECK(static_cast<int>(saida) == static_cast<int>(ItemId::LASER));
}

TEST_CASE("ItensConcretos: ID fora da lista nao tem nome") {
    CHECK(itemIdParaTexto(static_cast<ItemId>(99)).empty());
    CHECK_FALSE(ehItemPassivo(static_cast<ItemId>(99)));
}

TEST_CASE("ItensConcretos: todo item existente e passivo ou especial, nunca os dois") {
    int passivos = 0;
    int especiais = 0;
    for (int i = 0; i < TOTAL_DE_ITENS; i++) {
        ItemId id = static_cast<ItemId>(i);
        CHECK(ehItemPassivo(id) != ehItemEspecial(id));
        if (ehItemPassivo(id)) passivos++;
        if (ehItemEspecial(id)) especiais++;
    }
    CHECK(passivos == 7);
    CHECK(especiais == 3);
}

TEST_CASE("ItensConcretos: todo item tem definicao concreta") {
    for (int i = 0; i < TOTAL_DE_ITENS; i++) {
        ItemId id = static_cast<ItemId>(i);
        if (ehItemEspecial(id)) {
            CHECK(criarItemEspecial(id).nome != "Item Desconhecido");
        } else {
            CHECK(criarItemPassivo(id).nome != "Item Desconhecido");
        }
    }
}
