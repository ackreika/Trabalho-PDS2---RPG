/**
 * @file test_GerenciadorRecompensas.cpp
 * @brief Testes unitários para o TAD GerenciadorRecompensas utilizando o framework Doctest.
 * 
 * Disciplina: Programação e Desenvolvimento de Software 2 (PDS 2)
 * Projeto: RPG Acadêmico em C++ e Godot Engine
 */

#include "doctest.h"
#include "GerenciadorRecompensas.hpp"
#include "ItensConcretos.hpp"
#include <set>

namespace {
// Constantes para testes de limites e varredura defensiva
constexpr int AREAS_PARA_VARRER = 6;    // Varre além das 4 áreas reais (0 a 3) para testar robustez
constexpr int INDICES_PARA_VARRER = 8;  // Varre além dos índices de batalha existentes
}

TEST_CASE("GerenciadorRecompensas: Professor com drop único") {
    // Área 0 (Cálculo), Índice 3: Professor Colina Sombria deve dropar o item HALTER
    auto itens = godot::GerenciadorRecompensas::itensDoProfessor(0, 3);
    REQUIRE(itens.size() == 1);
    CHECK(itens[0] == ItemId::HALTER);
}

TEST_CASE("GerenciadorRecompensas: Professor com múltiplos drops mantém a ordem") {
    // Área 0 (Cálculo), Índice 2: Professor Xavier deve dropar INTEGRAL seguido de DERIVADA
    auto itens = godot::GerenciadorRecompensas::itensDoProfessor(0, 2);
    REQUIRE(itens.size() == 2);
    CHECK(itens[0] == ItemId::INTEGRAL);
    CHECK(itens[1] == ItemId::DERIVADA);
}

TEST_CASE("GerenciadorRecompensas: Batalhas sem recompensa ou índices inválidos") {
    SUBCASE("Batalhas válidas mas sem drops") {
        CHECK(godot::GerenciadorRecompensas::itensDoProfessor(0, 0).empty());
        CHECK(godot::GerenciadorRecompensas::itensDoProfessor(0, 4).empty());
    }

    SUBCASE("Índices e áreas inexistentes ou negativos (Programação Defensiva)") {
        CHECK(godot::GerenciadorRecompensas::itensDoProfessor(9, 1).empty());
        CHECK(godot::GerenciadorRecompensas::itensDoProfessor(-1, -1).empty());
        CHECK_FALSE(godot::GerenciadorRecompensas::temRecompensa(9, 1));
        CHECK_FALSE(godot::GerenciadorRecompensas::dropaItemChave(9, 1));
    }
}

TEST_CASE("GerenciadorRecompensas: Item Chave é exclusivo do Professor Rezeck") {
    // O item chave da formatura só pode ser obtido ao derrotar o Rezeck (Área 3 - PDS, Batalha 1)
    for (int area = 0; area < AREAS_PARA_VARRER; ++area) {
        for (int indice = 0; indice < INDICES_PARA_VARRER; ++indice) {
            bool esperado = (area == 3 && indice == 1);
            CHECK(godot::GerenciadorRecompensas::dropaItemChave(area, indice) == esperado);
        }
    }
    CHECK(godot::GerenciadorRecompensas::temRecompensa(3, 1));
}

TEST_CASE("GerenciadorRecompensas: Integridade da tabela - Nenhum item é duplicado entre chefes") {
    // Garante que cada item exclusivo é dropado por exatamente um chefe no jogo inteiro
    std::set<int> itensRegistrados;
    for (int area = 0; area < AREAS_PARA_VARRER; ++area) {
        for (int indice = 0; indice < INDICES_PARA_VARRER; ++indice) {
            for (ItemId id : godot::GerenciadorRecompensas::itensDoProfessor(area, indice)) {
                bool inseridoComSucesso = itensRegistrados.insert(static_cast<int>(id)).second;
                CHECK(inseridoComSucesso);
            }
        }
    }
}

TEST_CASE("GerenciadorRecompensas: Todo drop possui conversão textual válida") {
    // Garante que todos os itens na tabela possuem mapeamento com nome e regras registradas
    for (int area = 0; area < AREAS_PARA_VARRER; ++area) {
        for (int indice = 0; indice < INDICES_PARA_VARRER; ++indice) {
            for (ItemId id : godot::GerenciadorRecompensas::itensDoProfessor(area, indice)) {
                CHECK_FALSE(itemIdParaTexto(id).empty());
            }
        }
    }
}

TEST_CASE("GerenciadorRecompensas: temRecompensa é estritamente consistente com itens e chave") {
    for (int area = 0; area < AREAS_PARA_VARRER; ++area) {
        for (int indice = 0; indice < INDICES_PARA_VARRER; ++indice) {
            bool temItens = !godot::GerenciadorRecompensas::itensDoProfessor(area, indice).empty();
            bool temChave = godot::GerenciadorRecompensas::dropaItemChave(area, indice);
            bool esperado = temItens || temChave;
            CHECK(godot::GerenciadorRecompensas::temRecompensa(area, indice) == esperado);
        }
    }
}
