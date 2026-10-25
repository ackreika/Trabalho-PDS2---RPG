#pragma once
#include <vector>
#include "ItensIds.hpp"

/**
 * @brief Sistema de recompensas (GerenciadorRecompensas, no CRC): é a ÚNICA fonte
 * da tabela de drops do jogo — quem dá qual item, e quem dá o item chave.
 *
 * Cada professor é identificado por (idArea, indiceBatalha), os mesmos valores
 * usados pelo Global.gd: idArea 0=Cálculo, 1=GAAL, 2=Física, 3=PDS; o índice
 * da batalha começa em 1 dentro de cada área.
 *
 * Esta classe só diz O QUE cada professor dá. Saber se é a PRIMEIRA vez que
 * ele é derrotado (e guardar isso em disco) é papel do Progresso. O GDScript
 * acessa estes métodos pela classe Recompensas (ver Recompensas.hpp).
 */
class GerenciadorRecompensas {
public:
    /**
     * @brief Itens que o professor dá ao ser derrotado pela primeira vez.
     * @param idArea Área do professor (0 a 3).
     * @param indiceBatalha Posição do professor na área (a partir de 1).
     * @return Lista de itens (pode ter mais de um); vazia se o professor não dá item.
     */
    static std::vector<ItemId> itensDoProfessor(int idArea, int indiceBatalha);

    /**
     * @brief Indica se derrotar esse professor entrega o item chave (usado contra o chefe final).
     * @return true se entrega o item chave; false caso contrário.
     */
    static bool dropaItemChave(int idArea, int indiceBatalha);

    /** @brief true se o professor dá algum item ou o item chave. */
    static bool temRecompensa(int idArea, int indiceBatalha);
};
