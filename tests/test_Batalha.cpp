#include "doctest.h"
#include "Batalha.hpp"
#include "Herois.hpp"
#include "Professor.hpp"
 
//testes de batalha
 
TEST_CASE("Batalha comeca no estado ESCOLHENDO_ACAO") {
    Herois heroi("Teste", 100, 10, 20);
    Professor professor("Prof. Teste", "Calculo", 50, 5);
 
    Batalha batalha(&heroi, &professor);
 
    CHECK(batalha.getEstado() == EstadoBatalha::ESCOLHENDO_ACAO);
}

TEST_CASE("Atacar com um movimento causa dano no professor") {
    Herois heroi("Teste", 100, 10, 20);
    Professor professor("Prof. Teste", "Calculo", 50, 5);
    heroi.aprenderMovimento(Movimento("Golpe", 15));
 
    Batalha batalha(&heroi, &professor);
    ResultadoTurno resultado = batalha.executarAcao(0);
 
    CHECK(resultado.acaoValida == true);
    CHECK(resultado.danoCausadoPeloHeroi > 0);
}