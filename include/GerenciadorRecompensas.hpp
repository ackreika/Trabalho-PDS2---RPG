#pragma once
#include "ItemPassivo.hpp"
#include "Professor.hpp"

/**
 * @brief Sistema de recompensas (GerenciadorRecompensas, no CRC): conhece os
 * drops fixos de cada chefe. Só Miniboss e Boss dropam item (US03).
 */
class GerenciadorRecompensas {
public:
    /**
     * @brief Gera a recompensa por derrotar um professor, se houver.
     * @param idArea Área/disciplina do professor derrotado.
     * @param tipoDerrotado Tipo do professor (Comum nunca dropa nada).
     * @param resultadoSaida Referência onde o item é escrito, se houver drop.
     * @return true se um item foi gerado; false se não há drop pra esse caso.
     */
    static bool gerarRecompensa(int idArea, TipoProfessor tipoDerrotado, ItemPassivo& resultadoSaida);
};