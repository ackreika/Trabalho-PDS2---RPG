#include "Recompensas.hpp"
#include "GerenciadorRecompensas.hpp"
#include "ItensConcretos.hpp"
#include <godot_cpp/core/class_db.hpp>

PackedStringArray Recompensas::itens_do_professor(int idArea, int indiceBatalha) {
    PackedStringArray nomes;
    for (ItemId id : GerenciadorRecompensas::itensDoProfessor(idArea, indiceBatalha)) {
        nomes.push_back(String(itemIdParaTexto(id).c_str()));
    }
    return nomes;
}

bool Recompensas::dropa_item_chave(int idArea, int indiceBatalha) {
    return GerenciadorRecompensas::dropaItemChave(idArea, indiceBatalha);
}

void Recompensas::_bind_methods() {
    ClassDB::bind_static_method("Recompensas", D_METHOD("itens_do_professor", "id_area", "indice_batalha"), &Recompensas::itens_do_professor);
    ClassDB::bind_static_method("Recompensas", D_METHOD("dropa_item_chave", "id_area", "indice_batalha"), &Recompensas::dropa_item_chave);
}
