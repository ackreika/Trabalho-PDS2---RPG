#include "herois.h"

// 2. Inclui o banco de dados de classes da Godot (obrigatório para o _bind_methods)
#include <godot_cpp/core/class_db.hpp>

// 3. Inclui funções utilitárias (como printar no terminal da Godot para debugar)
#include <godot_cpp/variant/utility_functions.hpp>

// Necessário para std::max
#include <algorithm>

using namespace godot;

void Herois::_bind_methods() {
    // Registra os métodos e o Enum para o GDScript
    BIND_ENUM_CONSTANT(FISICO);
    BIND_ENUM_CONSTANT(MAGICO);

    ClassDB::bind_method(D_METHOD("tomar_dano", "dano", "tipo"), &Herois::tomar_dano);
    ClassDB::bind_method(D_METHOD("curar", "cura"), &Herois::curar);
    ClassDB::bind_method(D_METHOD("set_ataque_fisico", "ataque"), &Herois::set_ataque_fisico);
    ClassDB::bind_method(D_METHOD("set_ataque_magico", "ataque"), &Herois::set_ataque_magico);
    ClassDB::bind_method(D_METHOD("set_defesa", "defesa"), &Herois::set_defesa);
}

Herois::Herois() {
    vida = 100;
    max_vida = 100;
    ataque_fisico = 10;
    ataque_magico = 5;
    defesa = 3;
    is_dead = false;
    nivel = 1; // Inicializa o nível do herói
    experiencia = 0; // Inicializa a experiência do herói
}

Herois::~Herois() {}

void Herois::tomar_dano(int dano, TipoDano tipo) {
    int dano_final = dano;

    if (tipo == FISICO) {
        // Reduz o dano pela defesa, garantindo um valor mínimo de 0
        dano_final = std::max(0, dano - defesa);
    }
    // Se tipo == MAGICO, o dano_final permanece igual ao dano inicial (ignora a defesa)

    vida -= dano_final;

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

void Herois::set_subir_nivel(int experiencia, int nivel) {
    if{ nivel == 1 && experiencia >= 100 } {
        nivel++;
        vida += 5;
        max_vida += 5;
        ataque_fisico += 2;
        ataque_magico += 1;
        if{ nivel % 3 == 0 } {
            defesa += 1; // Aumenta a defesa a cada 3 níveis
        }
        experiencia = 0; // Reseta a experiência após subir de nível
    }
    if (nivel > 1 && experiencia >= 100 + nivel * 10) {
        nivel++;
        experiencia = 0; // Reseta a experiência após subir de nível
    }
}

void Herois::set_experiencia(int experiencia) {
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
