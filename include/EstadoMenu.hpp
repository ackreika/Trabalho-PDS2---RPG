#pragma once
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/node.hpp>

namespace godot {

class GerenciadorMenu : public Node
{
    GDCLASS(GerenciadorMenu, Node)

    protected:
    static void _bind_methods();

    public:
    /** @brief Estados de menu que o Godot consulta pra saber qual painel/tela mostrar. */
    enum EstadoMenu {
    MENU_INICIAL,
    MENU_JOGAR,          ///< submenu: Batalhar / Inventario Completo / Voltar
    MENU_CONFIGURACOES,  ///< submenu: Volume / Resolucao / Voltar
    MENU_INVENTARIO,
    EM_BATALHA,
};
    
    GerenciadorMenu();
    ~GerenciadorMenu();

    void set_estado(EstadoMenu estado);
    GerenciadorMenu::EstadoMenu get_estado() const;

    private: 
    EstadoMenu estado_atual = EstadoMenu::MENU_INICIAL;


};

}
VARIANT_ENUM_CAST(godot::GerenciadorMenu::EstadoMenu);

