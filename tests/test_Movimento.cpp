/**
 * @file test_movimento.cpp
 * @brief Testes de unidade (TDD) para a mecânica das habilidades/movimentos do herói.
 */
#include "../third_party/doctest.h"
#include "../include/Movimento.hpp"

TEST_CASE("Testando o Controle de Usos e Upgrade do Movimento") {
    // Instancia um movimento com 3 usos máximos
    Movimento golpe("Derivada", 25, 3); // nome, dano, usosMaximos

    SUBCASE("Consumo de usos") {
        CHECK(golpe.getUsosRestantes() == 3);
        CHECK(golpe.podeUsar() == true);
        
        golpe.usar();
        CHECK(golpe.getUsosRestantes() == 2);
    }

    SUBCASE("Restauracao de usos e Upgrade de poder (Quiz US08)") {
        golpe.usar(); // cai para 2
        golpe.resetarUsos();
        CHECK(golpe.getUsosRestantes() == 3);
        
        // O GerenciadorQuiz acerta e aplica aumento de poder
        golpe.aumentarPoder(5);
        CHECK(golpe.getDano() == 30);
    }
}