#include "doctest.h"
#include "Herois.hpp"

//testes de herois

TEST_CASE("Herois comeca com a vida maxima cheia") {
    Herois heroi("Teste", 100, 10, 20); // nome, vidaMaxima, defesa, danoBase
    CHECK(heroi.getVida() == heroi.getVidaMaxima());
}