#ifndef GERENCIADOR_RECOMPENSAS_HPP
#define GERENCIADOR_RECOMPENSAS_HPP

#include <vector>
#include "ItensIds.hpp"
#include "ProgressoJogador.hpp" // Incluir a definição de ProgressoJogador

namespace godot {

class GerenciadorRecompensas {
public:
    static std::vector<ItemId> itensDoProfessor(int idArea, int indiceBatalha);
    static bool dropaItemChave(int idArea, int indiceBatalha);
    static bool temRecompensa(int idArea, int indiceBatalha);
};

}

using namespace godot;

#endif
