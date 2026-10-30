#pragma once
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>

using namespace godot;

/**
 * @brief Ponte entre o GerenciadorRecompensas (C++ puro) e o GDScript.
 *
 * Não precisa ser instanciada: os métodos são estáticos e se chamam direto
 * pelo nome da classe, como em Herois.descrever_item_passivo().
 *
 * Exemplos no GDScript:
 *   Recompensas.itens_do_professor(0, 2)  # -> ["INTEGRAL", "DERIVADA"]
 *   Recompensas.dropa_item_chave(3, 1)    # -> true
 */
class Recompensas : public RefCounted {
    GDCLASS(Recompensas, RefCounted)

protected:
    static void _bind_methods();

public:
    /**
     * @brief Nomes dos itens que o professor dá na 1ª vez que é derrotado ("HALTER", "SUCO"...).
     * @param idArea 0=Cálculo, 1=GAAL, 2=Física, 3=PDS.
     * @param indiceBatalha Posição do professor na área (a partir de 1).
     * @return Lista de nomes de itens; vazia se o professor não dá item.
     */
    static PackedStringArray itens_do_professor(int idArea, int indiceBatalha);

    /** @brief true se derrotar esse professor entrega o item chave. */
    static bool dropa_item_chave(int idArea, int indiceBatalha);
};
