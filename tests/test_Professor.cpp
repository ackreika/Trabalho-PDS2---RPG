/**
 * @file test_professor.cpp
 * @brief Testes de unidade (TDD) para a entidade Professor.
 *        Valida a mecânica de chefes finais (Bosses) e invulnerabilidade (US04).
 */
#include "../third_party/doctest.h"
#include "../include/Professor.hpp"

TEST_CASE("Testando Invulnerabilidade de Boss Final") {
    // Instancia um Boss (Boss por default deve instanciar como invulneravel, conforme regra)
    Professor bossFinal("Fernando", "PDS2", 500, 10, TipoProfessor::BOSS, false);

    SUBCASE("Boss comeca invulneravel e nao toma dano") {
        CHECK(bossFinal.isInvulneravel() == true);
        
        // Tenta aplicar dano direto
        bossFinal.receberDano(100);
        // O HP deve permanecer intacto, ou seja, dano foi ignorado (0)
        CHECK(bossFinal.getVida() == 500); 
    }

    SUBCASE("Quebra de invulnerabilidade permite aplicar dano") {
        // Remove a flag protetora
        bossFinal.removerInvulnerabilidade();
        CHECK(bossFinal.isInvulneravel() == false);
        
        // O bossFinal tem 10 de defesa. O dano base é 100.
        // Se a sua fórmula for vida/(vida+defesa), o comportamento no cpp tem que passar.
        bossFinal.receberDano(100);
        
        // Basta checar se a vida reduziu de alguma forma, provando que o dano penetrou.
        CHECK(bossFinal.getVida() < 500);
    }
}