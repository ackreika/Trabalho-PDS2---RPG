#pragma once
#include <string>

/** @brief O que um item passivo bonifica no herói. */
enum class TipoEfeito {
    BONUS_DANO_CAUSADO,
    BONUS_VIDA_MAXIMA,
};

/**
 * @brief Item passivo (ItemAcademico, no CRC): bônus permanente aplicado ao
 * herói assim que coletado. É um dado puro — o efeito é interpretado em
 * Herois::calcularDanoFinal e Herois::calcularVidaFinal.
 */
struct ItemPassivo {
    std::string nome;
    TipoEfeito tipo;
    float multiplicador; ///< ex: 1.05 = +5%
};