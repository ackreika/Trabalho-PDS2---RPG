#pragma once
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/string.hpp>
#include "Professor.hpp"

using namespace godot;

/**
 * @brief Fábrica centralizada de professores (inimigos) do jogo, exposta ao GDScript.
 *
 * Quantidade de batalhas por área:
 *   área 0 = Cálculo -> 3 | área 1 = GAAL -> 2 | área 2 = Física -> 4 | área 3 = PDS -> 2
 *
 * Os bosses de área (Colina Sombria, Alexandre, Slide Man) nascem já vulneráveis;
 * só o Fernando (boss final) continua invulnerável até o jogador usar o item chave.
 * A batalha 2 da Física ("Outro Prof*") é uma batalha falsa (US05).
 */
class ProfessoresFabrica : public RefCounted {
    GDCLASS(ProfessoresFabrica, RefCounted)

protected:
    static void _bind_methods();

private:
    static Professor* criarProfessorCalculo(int indiceBatalha);
    static Professor* criarProfessorGaal(int indiceBatalha);
    static Professor* criarProfessorFisica(int indiceBatalha);
    static Professor* criarProfessorPds(int indiceBatalha);
    static Professor* criarProfessorDesconhecido();

    /** @brief Cria o professor já com status básicos (quem chama adiciona golpes e sprites). */
    static Professor* montar(const char* nome, const char* materia, int vida, int defesa,
                             TipoProfessor tipo, bool batalhaFalsa = false);
    /** @brief Atalho pra adicionar um golpe; usosMaximos -1 = ilimitado. */
    static void golpe(Professor* p, const char* nome, int dano, int usosMaximos);
    /** @brief Define os 5 sprites do professor de uma vez (caminhos "res://..."). */
    static void sprites(Professor* p, const char* parado, const char* atacando,
                        const char* tomandoDano, const char* vitoria, const char* derrota);

public:
    /**
     * @brief Cria o Professor de uma área + índice de batalha (índice começa em 1).
     * @param idArea 0 = Calculo, 1 = GAAL, 2 = Fisica, 3 = PDS.
     * @return Professor novo (quem chama é dono dele e deve dar free()), nunca nulo.
     */
    static Professor* criar_professor(int idArea, int indiceBatalha);

    /** @brief Quantas áreas existem. */
    static int get_quantidade_areas();
    /** @brief Quantas batalhas a área tem (0 se a área for inválida). */
    static int get_quantidade_batalhas(int idArea);
    /** @brief Nome da área ("Calculo", "GAAL", ...) ou "" se inválida. */
    static String get_nome_area(int idArea);
    /** @brief true se a batalha existe (área e índice válidos). */
    static bool batalha_existe(int idArea, int indiceBatalha);
    /** @brief true se for a última batalha da área (o boss da área). */
    static bool eh_ultima_da_area(int idArea, int indiceBatalha);
};