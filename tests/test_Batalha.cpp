/**
 * @file test_batalha.cpp
 * @brief Testes de unidade para a orquestração dos turnos de combate e seus estados.
 */
#include "../third_party/doctest.h"
#include "../include/Batalha.hpp"
#include "../include/Herois.hpp"
#include "../include/Professor.hpp"

TEST_CASE("Testando a Máquina de Estados da Batalha") {
    // Instancia um herói e um professor base (Valores fictícios para TDD)
    Herois heroi("Gustavo", 100, 10, 20);
    Professor prof("Javier", "Cálculo", 200, 10, TipoProfessor::COMUM, false);
    
    Batalha luta(heroi, prof);

    SUBCASE("Inicialização correta da Batalha") {
        CHECK(luta.getEstado() == EstadoBatalha::ESCOLHENDO_ACAO);
    }

    SUBCASE("Fuga do Herói resulta em fim de jogo") {
        ResultadoTurno resultado = luta.fugir();
        
        CHECK(resultado.acaoValida == true);
        CHECK(resultado.estadoResultante == EstadoBatalha::FUGIU);
        CHECK(luta.getEstado() == EstadoBatalha::FUGIU);
    }
}