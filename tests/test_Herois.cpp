#include "doctest.h"
#include "Herois.hpp"

//testes de herois

TEST_CASE("Herois comeca com a vida maxima cheia") {
    Herois heroi("Teste", 100, 10, 20); // nome, vidaMaxima, defesa, danoBase
    CHECK(heroi.getVida() == heroi.getVidaMaxima());
}

TEST_CASE("Herois perde vida ao receber dano") {
    Herois heroi("Teste", 100, 10, 20);
    int vidaAntes = heroi.getVida();
 
    heroi.receberDano(30);
 
    CHECK(heroi.getVida() < vidaAntes);
}
 
TEST_CASE("Herois nao fica com vida negativa") {
    Herois heroi("Teste", 100, 10, 20);
 
    heroi.receberDano(999999);
 
    CHECK(heroi.getVida() >= 0);
}