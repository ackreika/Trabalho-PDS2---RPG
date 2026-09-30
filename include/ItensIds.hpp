#pragma once

/**
 * @brief IDs fixos de cada item do jogo, usados pra salvar o progresso
 * desbloqueado entre runs (ver ProgressoJogador).
 * @warning Nunca reordenar ou remover um ID já usado — novos itens sempre
 * entram no final da lista, ou saves antigos ficam inconsistentes.
 */
enum class ItemId {
    HALTER = 0,
    SUCO = 1,
    LASER = 2,
};