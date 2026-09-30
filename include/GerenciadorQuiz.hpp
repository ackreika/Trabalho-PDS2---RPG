#pragma once
#include <string>
#include <vector>
#include <map>
#include "Movimento.hpp"

/** @brief Uma pergunta de quiz, com alternativas e o índice da correta. */
struct Pergunta {
    std::string enunciado;
    std::vector<std::string> alternativas;
    int indiceCorreto;
};

/**
 * @brief Sistema de evolução por quiz (GerenciadorQuiz, no CRC): sorteia
 * perguntas por área/disciplina e aplica upgrade de poder no movimento
 * escolhido quando o jogador acerta.
 */
class GerenciadorQuiz {
private:
    std::map<int, std::vector<Pergunta>> bancoPerguntas; ///< idArea -> perguntas (0=Cálculo, 1=Física, 2=GAAL, 3=PDS)

public:
    /** @brief Cria o gerenciador já povoado com perguntas de exemplo (ver .cpp). */
    GerenciadorQuiz();

    /** @brief Adiciona uma pergunta ao banco de uma área. */
    void adicionarPergunta(int idArea, Pergunta pergunta);

    /**
     * @brief Sorteia uma pergunta da área informada.
     * @param idArea Área/disciplina atual do jogador.
     * @param saida Referência onde a pergunta sorteada é escrita, se houver.
     * @return true se havia pergunta cadastrada pra essa área; false caso contrário.
     */
    bool sortearPergunta(int idArea, Pergunta& saida) const;

    /** @brief Retorna true se o índice escolhido for o correto pra essa pergunta. */
    bool validarResposta(const Pergunta& pergunta, int indiceEscolhido) const;

    /**
     * @brief Aplica o upgrade de poder no movimento, somente se o jogador acertou.
     * @param movimento Movimento a receber o upgrade (ignorado se nullptr).
     * @param acertou Resultado da validação da resposta.
     * @param incrementoDano Quanto de dano adicionar ao movimento em caso de acerto.
     */
    void aplicarResultado(Movimento* movimento, bool acertou, int incrementoDano = 3) const;
};