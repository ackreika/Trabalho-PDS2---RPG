/**
 * @file test_herois.cpp
 * @brief Testes de unidade para a entidade principal do jogador, verificação de dano e itens.
 */
#include "../third_party/doctest.h"
#include "../include/Herois.hpp"
#include "../include/ItemPassivo.hpp"

TEST_CASE("Testando a Entidade Herois e seus Status") {
    Herois estudante("Gustavo", 100, 15, 20); // Nome, vidaMaxima, defesa, danoBase

    SUBCASE("Verificação de Status Iniciais") {
        CHECK(estudante.getNome() == "Gustavo");
        CHECK(estudante.getVida() == 100);
        CHECK(estudante.estaVivo() == true);
    }

    SUBCASE("Lógica de Recebimento de Dano") {
        estudante.receberDano(50);
        // O dano real será menor que 50 devido à fórmula vida/(vida+defesa)
        CHECK(estudante.getVida() < 100);
        
        // Testa a morte do herói
        estudante.receberDano(9999);
        CHECK(estudante.getVida() == 0);
        CHECK(estudante.estaVivo() == false);
    }

    SUBCASE("Equipamento de Itens Passivos") {
        ItemPassivo caderno{"Caderno", TipoEfeito::BONUS_VIDA_MAXIMA, 1.20f};
        estudante.adicionarItemPassivo(caderno);
        
        // A vida máxima base é 100, com 20% de bônus deve ser 120
        CHECK(estudante.getVidaMaxima() == 120);
        CHECK(estudante.getQuantidadeItensPassivos() == 1);
    }
}