#pragma once

/** @brief Estados de menu que o Godot consulta pra saber qual painel/tela mostrar. */
enum class EstadoMenu {
    MENU_INICIAL,
    MENU_JOGAR,          ///< submenu: Batalhar / Inventario Completo / Voltar
    MENU_CONFIGURACOES,  ///< submenu: Volume / Resolucao / Voltar
    MENU_INVENTARIO,
    EM_BATALHA,
};