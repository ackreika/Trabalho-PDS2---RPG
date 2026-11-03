#pragma once
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include "Professor.hpp"

/**
 * @brief Orquestra a progressão da run (GerenciadorSemestre, no CRC): sabe em
 * qual área o jogador está, em qual das 4 batalhas daquela área, e quando a
 * run inteira foi vencida ("Fuga do ICEx").
 *
 * idArea: 0=Cálculo, 1=GAAL, 2=Física, 3=PDS (nessa ordem fixa)
 */
class GerenciadorSemestre : public godot::RefCounted {
    GDCLASS(GerenciadorSemestre, godot::RefCounted)

protected:
    static void _bind_methods();

private:
    int idAreaAtual;
    int indiceBatalhaAtual; ///< 1 a 4

public:
    /** @brief Inicia a progressão na área 0 (Cálculo), batalha 1. */
    GerenciadorSemestre();
    void iniciar(int idArea = 0);

    /** @brief Retorna o índice da área atual (0 a 3). */
    int getIdAreaAtual() const;
    /** @brief Retorna o índice da batalha atual dentro da área (1 a 4). */
    int getIndiceBatalhaAtual() const;

    /** @brief Cria (via ProfessoresFabrica) o Professor correspondente à batalha atual. */
    Professor* gerarProximoProfessor() const;

    /**
     * @brief Avança a progressão após uma VITORIA na Batalha.
     * Avança pra próxima luta, ou pra próxima área se o boss (índice 4)
     * acabou de ser vencido. Não faz nada se o jogo já foi vencido.
     */
    void avancarBatalha();

    /** @brief Retorna true se o boss da última área já foi vencido ("Fuga do ICEx"). */
    bool venceuOJogo() const;

    int get_id_area_atual() const;
    int get_indice_batalha_atual() const;
    bool avancar_batalha();
    bool venceu_o_jogo() const;
};
