#include "EstadoMenu.hpp"

using namespace godot;


void GerenciadorMenu::_bind_methods() {

    ClassDB::bind_integer_constant(get_class_static(), "EstadoMenu", "MENU_INICIAL", MENU_INICIAL, true);
    ClassDB::bind_integer_constant(get_class_static(), "EstadoMenu", "MENU_JOGAR", MENU_JOGAR, true);
    ClassDB::bind_integer_constant(get_class_static(), "EstadoMenu", "MENU_CONFIGURACOES", MENU_CONFIGURACOES, true);
    ClassDB::bind_integer_constant(get_class_static(), "EstadoMenu", "MENU_INVENTARIO", MENU_INVENTARIO, true);
    ClassDB::bind_integer_constant(get_class_static(), "EstadoMenu", "EM_BATALHA", EM_BATALHA, true);

    ClassDB::bind_method(D_METHOD("set_estado", "estado"), &GerenciadorMenu::set_estado);
    ClassDB::bind_method(D_METHOD("get_estado"), &GerenciadorMenu::get_estado);

}

GerenciadorMenu::GerenciadorMenu()
{

}
GerenciadorMenu::~GerenciadorMenu()
{

}

void GerenciadorMenu::set_estado(EstadoMenu estado)
{
    estado_atual = estado;
}
GerenciadorMenu::EstadoMenu GerenciadorMenu::get_estado() const
{
    return estado_atual;
}