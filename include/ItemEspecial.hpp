#pragma once
#include <string>

/**
 * @brief Item especial (ocupa o 4º slot de golpe na batalha). Dado puro,
 * consumido/aplicado por Herois::usarItemEspecial.
 */
struct ItemEspecial {
    std::string nome;
    float multiplicadorDano; ///< dano = heroi.getDanoBase() * multiplicadorDano
    int usosMaximos;
    int usosRestantes;
};