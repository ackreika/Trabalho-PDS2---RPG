/**
 * @file test_gerenciador_quiz.cpp
 * @brief Testes de unidade para o sistema de evolução baseado em perguntas (US08).
 */
#include "../third_party/doctest.h"
#include "../include/GerenciadorQuiz.hpp"
#include "../include/Movimento.hpp"

TEST_CASE("Testando o Gerenciador de Quiz") {
    GerenciadorQuiz quiz;

    SUBCASE("Adição e Sorteio de Perguntas") {
        Pergunta p;
        p.enunciado = "Qual a derivada de x^2?";
        p.alternativas = {"x", "2x", "x^2"};
        p.indiceCorreto = 1;

        // Usamos a área 99 para garantir que não vai misturar com as perguntas de exemplo do seu .cpp
        quiz.adicionarPergunta(99, p); 

        Pergunta sorteada;
        bool temPergunta = quiz.sortearPergunta(99, sorteada);

        CHECK(temPergunta == true);
        CHECK(sorteada.enunciado == "Qual a derivada de x^2?");
    }

    SUBCASE("Validação de Resposta do Jogador") {
        Pergunta p{"O que é um vetor?", {"Escalar", "Módulo, direção e sentido"}, 1};
        
        CHECK(quiz.validarResposta(p, 1) == true);
        CHECK(quiz.validarResposta(p, 0) == false);
    }
}