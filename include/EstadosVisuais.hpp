#pragma once

/**
 * @brief Poses/estados visuais que um personagem (Herois ou Professor) pode ter.
 * Cada estado mapeia pra um caminho de sprite diferente, escolhido pelo Godot
 * conforme a tela/ação atual.
 */
enum class EstadoVisual {
    TOMANDO_DANO,
    VITORIA,
    DERROTA,
    PARADO,
    ATACANDO,
};