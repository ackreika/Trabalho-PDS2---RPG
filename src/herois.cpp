#include "herois.h"

// 2. Inclui o banco de dados de classes da Godot (obrigatório para o _bind_methods)
#include <godot_cpp/core/class_db.hpp>

// 3. Inclui funções utilitárias (como printar no terminal da Godot para debugar)
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void Herois::_bind_methods() {
    // Registra as funções para o Godot 4 / GDScript enxergarem
    ClassDB::bind_method(D_METHOD("tomar_dano", "dano"), &Herois::tomar_dano);
    ClassDB::bind_method(D_METHOD("curar", "cura"), &Herois::curar);
    ClassDB::bind_method(D_METHOD("set_ataque_fisico", "ataque"), &Herois::set_ataque_fisico);
    ClassDB::bind_method(D_METHOD("set_ataque_magico", "ataque"), &Herois::set_ataque_magico);
    ClassDB::bind_method(D_METHOD("set_defesa", "defesa"), &Herois::set_defesa);
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

void Herois::tomar_dano(int dano) {
    vida -= dano;

    if (vida <= 0) {
        vida = 0;
        is_dead = true;
    }
}

void Herois::curar(int cura) {
    if (is_dead) return;

    vida += cura;
    if (vida > max_vida) {
        vida = max_vida;
    }
}

void Herois::set_ataque_fisico(int ataque) {
    ataque_fisico = ataque;
}

void Herois::set_ataque_magico(int ataque) {
    ataque_magico = ataque;
}

void Herois::set_defesa(int p_defesa) {
    defesa = p_defesa;
}