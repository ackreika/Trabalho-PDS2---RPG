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

TEST_CASE("Herois aprende um movimento novo") {
    Herois heroi("Teste", 100, 10, 20);
    Movimento golpe("Teste de Calculo", 15);
 
    heroi.aprenderMovimento(golpe);
 
    CHECK(heroi.getMovimentos().size() == 1);
    CHECK(heroi.getMovimentos()[0].getNome() == "Teste de Calculo");
}
 
TEST_CASE("Item passivo de vida aumenta a vida maxima do heroi") {
    Herois heroi("Teste", 100, 10, 20);
    ItemPassivo suco{"Suco", TipoEfeito::BONUS_VIDA_MAXIMA, 1.10f};
 
    heroi.adicionarItemPassivo(suco);
 
    CHECK(heroi.getVidaMaxima() > 100);
}