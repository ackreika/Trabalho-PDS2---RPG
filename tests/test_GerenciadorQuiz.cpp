#include "doctest.h"
#include "GerenciadorQuiz.hpp"
#include "Movimento.hpp"

TEST_CASE("Testando o Gerenciador de Quiz") {
    GerenciadorQuiz quiz;

    SUBCASE("Sorteio de perguntas das áreas válidas (0 a 3) e rejeição de área inválida") {
        Pergunta sorteada;

        // As áreas 0 (Cálculo), 1 (GAAL), 2 (Física) e 3 (PDS) já vêm povoadas pelo construtor
        for (int area = 0; area < 4; area++) {
            CHECK(quiz.sortearPergunta(area, sorteada) == true);
            CHECK(!sorteada.enunciado.empty());
            CHECK(sorteada.alternativas.size() == 4);
            CHECK(sorteada.indiceCorreto >= 0);
            CHECK(sorteada.indiceCorreto < 4);
        }

        // Adicionando uma nova pergunta válida (4 alternativas) na área 0
        Pergunta novaPergunta;
        novaPergunta.enunciado = "Qual a derivada de x^2?";
        novaPergunta.alternativas = {"x", "2x", "x^2", "2"};
        novaPergunta.indiceCorreto = 1;
        quiz.adicionarPergunta(0, novaPergunta);
        CHECK(quiz.sortearPergunta(0, sorteada) == true);

        // Área inexistente (ex: 99 ou negativa) não deve sortear pergunta
        Pergunta naoSorteada;
        quiz.adicionarPergunta(99, novaPergunta);
        CHECK(quiz.sortearPergunta(99, naoSorteada) == false);
        CHECK(quiz.sortearPergunta(-1, naoSorteada) == false);
    }

    SUBCASE("Validação de resposta correta, incorreta e fora dos limites") {
        Pergunta p{"O que é um vetor?", {"Escalar", "Módulo, direção e sentido", "Apenas um número", "Uma matriz 3x3"}, 1};

        CHECK(quiz.validarResposta(p, 1) == true);
        CHECK(quiz.validarResposta(p, 0) == false);
        CHECK(quiz.validarResposta(p, 2) == false);
        CHECK(quiz.validarResposta(p, -1) == false);
        CHECK(quiz.validarResposta(p, 5) == false);

        // Pergunta malformada (menos de 4 alternativas) deve retornar falso
        Pergunta pIncompleta{"Pergunta incompleta", {"A", "B"}, 0};
        CHECK(quiz.validarResposta(pIncompleta, 0) == false);
    }

    SUBCASE("Aplicando resultado do quiz em um movimento") {
        Movimento golpe("Derivada", 20, -1);

        // Se errou, o dano não muda
        quiz.aplicarResultado(&golpe, false, 5);
        CHECK(golpe.getDano() == 20);

        // Se acertou, o dano sobe com o incremento
        quiz.aplicarResultado(&golpe, true, 5);
        CHECK(golpe.getDano() == 25);

        // Ponteiro nulo não deve causar erro
        quiz.aplicarResultado(nullptr, true, 5);
    }
}
