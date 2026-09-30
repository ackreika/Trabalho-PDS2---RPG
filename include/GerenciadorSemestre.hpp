#pragma once
#include "Professor.hpp"
#include "ProgressoJogador.hpp"

/**
 * @brief Orquestra a progressão da run (GerenciadorSemestre, no CRC): sabe em
 * qual área o jogador está, em qual das 4 batalhas daquela área, e quando a
 * run inteira foi vencida ("Fuga do ICEx").
 *
 * idArea: 0=Cálculo, 1=Física, 2=GAAL, 3=PDS (nessa ordem fixa)
 */
class GerenciadorSemestre {
private:
    int idAreaAtual;
    int indiceBatalhaAtual; ///< 1 a 4
    static const int TOTAL_AREAS = 4;

public:
    /** @brief Inicia a progressão na área 0 (Cálculo), batalha 1. */
    GerenciadorSemestre();

    /** @brief Retorna o índice da área atual (0 a 3). */
    int getIdAreaAtual() const;
    /** @brief Retorna o índice da batalha atual dentro da área (1 a 4). */
    int getIndiceBatalhaAtual() const;

    /** @brief Cria (via ProfessoresFabrica) o Professor correspondente à batalha atual. */
    Professor gerarProximoProfessor() const;

    /**
     * @brief Avança a progressão após uma VITORIA na Batalha.
     * Avança pra próxima luta, ou pra próxima área se o boss (índice 4)
     * acabou de ser vencido. Não faz nada se o jogo já foi vencido.
     */
    void avancarBatalha();

    /** @brief Retorna true se o boss da última área já foi vencido ("Fuga do ICEx"). */
    bool venceuOJogo() const;
};