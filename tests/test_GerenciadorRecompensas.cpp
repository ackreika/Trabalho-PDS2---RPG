#include "../third_party/doctest.h"
#include "../include/GerenciadorRecompensas.hpp"
#include "../include/ItensConcretos.hpp"
#include <set>

namespace {
const int AREAS_PARA_VARRER = 6;    // varre alem das 4 areas reais, de proposito
const int INDICES_PARA_VARRER = 8;  // idem para os indices de batalha
}

TEST_CASE("GerenciadorRecompensas: professor com um unico drop") {
    auto itens = GerenciadorRecompensas::itensDoProfessor(0, 3); // Colina Sombria
    REQUIRE(itens.size() == 1);
    CHECK(static_cast<int>(itens[0]) == static_cast<int>(ItemId::HALTER));
}

TEST_CASE("GerenciadorRecompensas: professor com mais de um drop mantem a ordem") {
    auto itens = GerenciadorRecompensas::itensDoProfessor(0, 2); // Xavier
    REQUIRE(itens.size() == 2);
    CHECK(static_cast<int>(itens[0]) == static_cast<int>(ItemId::INTEGRAL));
    CHECK(static_cast<int>(itens[1]) == static_cast<int>(ItemId::DERIVADA));
}

TEST_CASE("GerenciadorRecompensas: professor fora da tabela nao da nada") {
    CHECK(GerenciadorRecompensas::itensDoProfessor(0, 4).empty());
    CHECK(GerenciadorRecompensas::itensDoProfessor(0, 0).empty());
    CHECK(GerenciadorRecompensas::itensDoProfessor(9, 1).empty());
    CHECK(GerenciadorRecompensas::itensDoProfessor(-1, -1).empty());
    CHECK_FALSE(GerenciadorRecompensas::temRecompensa(9, 1));
    CHECK_FALSE(GerenciadorRecompensas::dropaItemChave(9, 1));
}

TEST_CASE("GerenciadorRecompensas: o item chave so cai do Rezeck (PDS, 1a batalha)") {
    for (int area = 0; area < AREAS_PARA_VARRER; area++) {
        for (int indice = 0; indice < INDICES_PARA_VARRER; indice++) {
            bool esperado = (area == 3 && indice == 1);
            CHECK(GerenciadorRecompensas::dropaItemChave(area, indice) == esperado);
        }
    }
    CHECK(GerenciadorRecompensas::temRecompensa(3, 1));
}

TEST_CASE("GerenciadorRecompensas: nenhum item e dropado por dois professores") {
    std::set<int> jaVistos;
    for (int area = 0; area < AREAS_PARA_VARRER; area++) {
        for (int indice = 0; indice < INDICES_PARA_VARRER; indice++) {
            for (ItemId id : GerenciadorRecompensas::itensDoProfessor(area, indice)) {
                bool inserido = jaVistos.insert(static_cast<int>(id)).second;
                CHECK(inserido);
            }
        }
    }
}

TEST_CASE("GerenciadorRecompensas: todo drop e um item que existe") {
    for (int area = 0; area < AREAS_PARA_VARRER; area++) {
        for (int indice = 0; indice < INDICES_PARA_VARRER; indice++) {
            for (ItemId id : GerenciadorRecompensas::itensDoProfessor(area, indice)) {
                CHECK_FALSE(itemIdParaTexto(id).empty());
            }
        }
    }
}

TEST_CASE("GerenciadorRecompensas: temRecompensa concorda com os itens e a chave") {
    for (int area = 0; area < AREAS_PARA_VARRER; area++) {
        for (int indice = 0; indice < INDICES_PARA_VARRER; indice++) {
            bool esperado = !GerenciadorRecompensas::itensDoProfessor(area, indice).empty()
                            || GerenciadorRecompensas::dropaItemChave(area, indice);
            CHECK(GerenciadorRecompensas::temRecompensa(area, indice) == esperado);
        }
    }
}
