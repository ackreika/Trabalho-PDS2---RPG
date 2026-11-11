/**
 * @file test_ProgressoJogador.cpp
 * @brief Testes unitários para o TAD ProgressoJogador utilizando o framework Doctest.
 * 
 * Disciplina: Programação e Desenvolvimento de Software 2 (PDS 2)
 * Projeto: RPG Acadêmico em C++ e Godot Engine
 */

#include "doctest.h"
#include "ProgressoJogador.hpp"
#include "Herois.hpp"

using namespace godot;

TEST_CASE("ProgressoJogador: Gestão de Pontos e Economia") {
    ProgressoJogador progresso;

    SUBCASE("Saldo inicial e adição de pontos") {
        CHECK(progresso.get_pontos() == 0);

        progresso.adicionar_pontos(50);
        CHECK(progresso.get_pontos() == 50);

        // Adicionar valor nulo ou negativo não altera o saldo
        progresso.adicionar_pontos(0);
        CHECK(progresso.get_pontos() == 50);
        progresso.adicionar_pontos(-20);
        CHECK(progresso.get_pontos() == 50);
    }

    SUBCASE("Gasto de pontos com validação de saldo") {
        progresso.adicionar_pontos(100);

        // Gasto com saldo suficiente
        CHECK(progresso.gastar_pontos(40) == true);
        CHECK(progresso.get_pontos() == 60);

        // Tentativa de gastar mais do que o saldo disponível
        CHECK(progresso.gastar_pontos(70) == false);
        CHECK(progresso.get_pontos() == 60);

        // Gasto de valor inválido ou zero
        CHECK(progresso.gastar_pontos(0) == false);
        CHECK(progresso.gastar_pontos(-10) == false);
        CHECK(progresso.get_pontos() == 60);
    }
}

TEST_CASE("ProgressoJogador: Sistema de Upgrades Permanentes de Vida e Dano") {
    ProgressoJogador progresso;

    SUBCASE("Níveis e custos iniciais") {
        CHECK(progresso.get_nivel_vida() == 0);
        CHECK(progresso.get_nivel_dano() == 0);
        // Custo inicial: 20 + 10 * 0 = 20 pontos
        CHECK(progresso.custo_vida() == 20);
        CHECK(progresso.custo_dano() == 20);
    }

    SUBCASE("Compra de upgrades sem saldo suficiente") {
        CHECK(progresso.comprar_vida() == false);
        CHECK(progresso.get_nivel_vida() == 0);

        CHECK(progresso.comprar_dano() == false);
        CHECK(progresso.get_nivel_dano() == 0);
    }

    SUBCASE("Compra sucessiva e progressão aritmética do custo") {
        progresso.adicionar_pontos(150);

        // 1ª compra de vida: gasta 20 pontos, sobram 130
        CHECK(progresso.comprar_vida() == true);
        CHECK(progresso.get_nivel_vida() == 1);
        CHECK(progresso.custo_vida() == 30); // 20 + 10 * 1 = 30
        CHECK(progresso.get_pontos() == 130);

        // 2ª compra de vida: gasta 30 pontos, sobram 100
        CHECK(progresso.comprar_vida() == true);
        CHECK(progresso.get_nivel_vida() == 2);
        CHECK(progresso.custo_vida() == 40); // 20 + 10 * 2 = 40
        CHECK(progresso.get_pontos() == 100);

        // 1ª compra de dano: gasta 20 pontos, sobram 80
        CHECK(progresso.comprar_dano() == true);
        CHECK(progresso.get_nivel_dano() == 1);
        CHECK(progresso.custo_dano() == 30);
        CHECK(progresso.get_pontos() == 80);
    }
}

TEST_CASE("ProgressoJogador: Registro de Vitórias e Concessão de Recompensas") {
    ProgressoJogador progresso;

    SUBCASE("Vitória inédita concede recompensas e atualiza contadores") {
        CHECK(progresso.get_quantidade_vitorias() == 0);
        CHECK(progresso.ja_venceu_professor(0, 3) == false);

        // Batalha (0, 3) corresponde à Colina Sombria
        progresso.registrar_vitoria_professor(0, 3);
        CHECK(progresso.get_quantidade_vitorias() == 1);
        CHECK(progresso.ja_venceu_professor(0, 3) == true);

        // Batalha repetida não incrementa vitórias
        progresso.registrar_vitoria_professor(0, 3);
        CHECK(progresso.get_quantidade_vitorias() == 1);
    }

    SUBCASE("Validação defensiva para batalhas inexistentes") {
        progresso.registrar_vitoria_professor(99, 99);
        CHECK(progresso.get_quantidade_vitorias() == 0);
        CHECK(progresso.ja_venceu_professor(99, 99) == false);
    }
}
