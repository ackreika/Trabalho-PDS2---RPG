/**
 * @file Recompensas.cpp
 * @brief Implementação dos métodos da classe adaptadora Recompensas.
 * 
 * Disciplina: Programação e Desenvolvimento de Software 2 (PDS 2)
 * Projeto: RPG Acadêmico em C++ e Godot Engine
 */

#include "Recompensas.hpp"
#include "GerenciadorRecompensas.hpp"

using namespace godot;

void Recompensas::_bind_methods() {
    // Expõe o método estático para o GDScript (ex: Recompensas.dropa_item_chave(area, indice))
    ClassDB::bind_static_method(
        "Recompensas",
        D_METHOD("dropa_item_chave", "area", "indice"),
        &Recompensas::dropa_item_chave
    );
}

bool Recompensas::dropa_item_chave(int area, int indice) {
    // Delega a verificação para o núcleo de regras de negócio em C++
    return GerenciadorRecompensas::dropaItemChave(area, indice);
}
