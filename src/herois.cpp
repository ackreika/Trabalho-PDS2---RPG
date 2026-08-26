#include "herois.h"

// 2. Inclui o banco de dados de classes da Godot (obrigatório para o _bind_methods)
#include <godot_cpp/core/class_db.hpp>

// 3. Inclui funções utilitárias (como printar no terminal da Godot para debugar)
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void Herois::_bind_methods() 
{
    // Registra os métodos da classe para a Godot
}

Herois::Herois() 
{
    // Inicializa os atributos do herói
    vida = 100;
    max_vida = 100;
    ataque_fisico = 10;
    ataque_magico = 5;
    defesa = 3;
    is_dead = false;
}
Herois::~Herois() 
{
    // Destrutor da classe
}
