/**
 * @file GerenciadorRecompensas.hpp
 * @brief Definição da classe GerenciadorRecompensas para consulta de drops e itens de chefes.
 * 
 * Disciplina: Programação e Desenvolvimento de Software 2 (PDS 2)
 * Projeto: RPG Acadêmico em C++ e Godot Engine
 */

#ifndef GERENCIADOR_RECOMPENSAS_HPP
#define GERENCIADOR_RECOMPENSAS_HPP

#include <vector>
#include "ItensIds.hpp"

namespace godot {

/**
 * @class GerenciadorRecompensas
 * @brief Classe utilitária responsável pela tabela de drops e recompensas de batalhas.
 * 
 * Aplica o princípio da Responsabilidade Única (SRP): centraliza quais itens são concedidos
 * por cada professor ao ser derrotado, desacoplando essa lógica da fábrica de professores
 * (ProfessoresFabrica) e da classe de persistência (ProgressoJogador).
 */
class GerenciadorRecompensas {
public:
    /**
     * @brief Retorna os itens obtidos ao derrotar o professor de uma dada área e índice.
     * @param idArea Identificador numérico da área acadêmica (ex: 0 = Cálculo, 1 = Álgebra, etc.).
     * @param indiceBatalha Índice do professor dentro da respectiva área.
     * @return Vetor de ItemId contendo os drops do professor (vazio caso o professor não drope itens).
     */
    static std::vector<ItemId> itensDoProfessor(int idArea, int indiceBatalha);

    /**
     * @brief Informa se o professor derrotado entrega o Item Chave da formatura.
     * @param idArea Identificador numérico da área acadêmica.
     * @param indiceBatalha Índice do professor dentro da área.
     * @return true se o professor entrega o item chave (Professor Rezeck em PDS 2); false caso contrário.
     */
    static bool dropaItemChave(int idArea, int indiceBatalha);

    /**
     * @brief Verifica se uma batalha possui qualquer tipo de recompensa (itens normais ou item chave).
     * @param idArea Identificador numérico da área acadêmica.
     * @param indiceBatalha Índice do professor dentro da área.
     * @return true se houver ao menos um drop ou item chave cadastrado; false caso contrário.
     */
    static bool temRecompensa(int idArea, int indiceBatalha);
};

} // namespace godot

#endif // GERENCIADOR_RECOMPENSAS_HPP
