/**
 * @file Recompensas.hpp
 * @brief Adaptador GDExtension da classe GerenciadorRecompensas para a Godot Engine.
 * 
 * Disciplina: Programação e Desenvolvimento de Software 2 (PDS 2)
 * Projeto: RPG Acadêmico em C++ e Godot Engine
 */

#ifndef RECOMPENSAS_HPP
#define RECOMPENSAS_HPP

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/array.hpp>

namespace godot {

/**
 * @class Recompensas
 * @brief Adaptador (Adapter Pattern) que expõe métodos estáticos de recompensas para a Godot.
 * 
 * Permite que scripts GDScript consultem diretamente regras de negócio do backend C++,
 * especificamente a checagem de concessão do item chave da formatura.
 */
class Recompensas : public RefCounted {
    GDCLASS(Recompensas, RefCounted);

protected:
    /**
     * @brief Registra os métodos estáticos expostos à ClassDB do motor gráfico.
     */
    static void _bind_methods();

public:
    /**
     * @brief Consulta se o professor de uma dada área e índice dropa o item chave.
     * @param area Identificador da área acadêmica.
     * @param indice Índice do professor na respectiva área.
     * @return true se dropa o item chave; false caso contrário.
     */
    static bool dropa_item_chave(int area, int indice);
};

} // namespace godot

#endif // RECOMPENSAS_HPP
