#include "Recompensas.hpp"
#include "GerenciadorRecompensas.hpp"

using namespace godot;

void Recompensas::_bind_methods() {
    ClassDB::bind_static_method("Recompensas", D_METHOD("dropa_item_chave", "area", "indice"), &Recompensas::dropa_item_chave);
}

bool Recompensas::dropa_item_chave(int area, int indice) {
    return GerenciadorRecompensas::dropaItemChave(area, indice);
}
