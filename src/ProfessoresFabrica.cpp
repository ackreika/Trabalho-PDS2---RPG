#include "ProfessoresFabrica.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/memory.hpp>

using namespace godot;

namespace {
const int QUANTIDADE_AREAS = 4;
const int BATALHAS_POR_AREA[QUANTIDADE_AREAS] = {3, 2, 4, 2};
const char* NOMES_AREAS[QUANTIDADE_AREAS] = {"Calculo", "GAAL", "Fisica", "PDS"};
}

void ProfessoresFabrica::_bind_methods() {
    ClassDB::bind_static_method("ProfessoresFabrica", D_METHOD("criar_professor", "id_area", "indice_batalha"), &ProfessoresFabrica::criar_professor);
    ClassDB::bind_static_method("ProfessoresFabrica", D_METHOD("get_quantidade_areas"), &ProfessoresFabrica::get_quantidade_areas);
    ClassDB::bind_static_method("ProfessoresFabrica", D_METHOD("get_quantidade_batalhas", "id_area"), &ProfessoresFabrica::get_quantidade_batalhas);
    ClassDB::bind_static_method("ProfessoresFabrica", D_METHOD("get_nome_area", "id_area"), &ProfessoresFabrica::get_nome_area);
    ClassDB::bind_static_method("ProfessoresFabrica", D_METHOD("batalha_existe", "id_area", "indice_batalha"), &ProfessoresFabrica::batalha_existe);
    ClassDB::bind_static_method("ProfessoresFabrica", D_METHOD("eh_ultima_da_area", "id_area", "indice_batalha"), &ProfessoresFabrica::eh_ultima_da_area);
}

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

Professor* ProfessoresFabrica::montar(const char* nome, const char* materia, int vida, int defesa,
                                      TipoProfessor tipo, bool batalhaFalsa) {
    // Todo BOSS nasce invulneravel (regra do Professor). Os bosses de AREA chamam
    // removerInvulnerabilidade() depois; so o Fernando mantem a invulnerabilidade.
    return memnew(Professor(nome, materia, vida, defesa, tipo, batalhaFalsa));
}

void ProfessoresFabrica::golpe(Professor* p, const char* nome, int dano, int usosMaximos) {
    p->adicionarMovimento(Movimento(nome, dano, usosMaximos));
}

void ProfessoresFabrica::sprites(Professor* p, const char* parado, const char* atacando,
                                 const char* tomandoDano, const char* vitoria, const char* derrota) {
    p->definirSprite(EstadoVisual::PARADO, parado);
    p->definirSprite(EstadoVisual::ATACANDO, atacando);
    p->definirSprite(EstadoVisual::TOMANDO_DANO, tomandoDano);
    p->definirSprite(EstadoVisual::VITORIA, vitoria);
    p->definirSprite(EstadoVisual::DERROTA, derrota);
}

Professor* ProfessoresFabrica::criarProfessorDesconhecido() {
    Professor* p = montar("Professor Desconhecido", "Materia Desconhecida", 30, 3, TipoProfessor::COMUM);
    golpe(p, "Ataque Basico", 5, -1);
    return p;
}

// ---------------------------------------------------------------------------
// Area 0: Calculo
// ---------------------------------------------------------------------------

Professor* ProfessoresFabrica::criarProfessorCalculo(int indiceBatalha) {
    switch (indiceBatalha) {
        case 1: {
            Professor* p = montar("Substituto", "Calculo I", 45, 4, TipoProfessor::COMUM);
            golpe(p, "Prova Marcada de Ultima Hora", 10, -1);
            golpe(p, "Limite Mal Explicado", 8, 3);
            sprites(p,
                "res://imagens/SubstitutoNormal-removebg-preview.png",    // parado
                "res://imagens/SubstitutoSorrindo-removebg-preview.png",  // atacando
                "res://imagens/SubstitutoConfuso-removebg-preview.png",   // tomando dano
                "res://imagens/SubstitutoConfuso-removebg-preview.png",   // vitoria
                "res://imagens/SubstitutoNormal-removebg-preview.png");   // derrota
            return p;
        }
        case 2: {
            Professor* p = montar("Xavier", "Calculo I", 55, 5, TipoProfessor::COMUM);
            golpe(p, "Regra da Cadeia Relampago", 12, -1);
            golpe(p, "Derivada Implicita Confusa", 10, 3);
            sprites(p,
                "res://imagens/Javier_Normal-removebg-preview.png",
                "res://imagens/Javier_Irritado-removebg-preview.png",
                "res://imagens/Javier_Irritado-removebg-preview.png",
                "res://imagens/Javier_Normal-removebg-preview.png",
                "res://imagens/Javier_Sorrindo-removebg-preview.png");
            return p;
        }
        case 3: {
            Professor* p = montar("Colina Sombria", "Calculo II", 110, 9, TipoProfessor::BOSS);
            p->removerInvulnerabilidade(); // boss de area: vulneravel
            golpe(p, "Serie de Taylor Infinita", 20, 3);
            golpe(p, "Integral Impropria Divergente", 25, 2);
            golpe(p, "Convergencia Sombria", 15, -1);
            sprites(p,
                "res://imagens/MarcosSerio-removebg-preview.png",
                "res://imagens/MarcosSorrindo-removebg-preview.png",
                "res://imagens/MarcosSorrindo-removebg-preview.png",
                "res://imagens/MarcosDecepcionado-removebg-preview.png",
                "res://imagens/MarcosSorrindo-removebg-preview.png");
            return p;
        }
        default:
            return criarProfessorDesconhecido();
    }
}

// ---------------------------------------------------------------------------
// Area 1: GAAL
// ---------------------------------------------------------------------------

Professor* ProfessoresFabrica::criarProfessorGaal(int indiceBatalha) {
    switch (indiceBatalha) {
        case 1: {
            Professor* p = montar("Viktor Bekert", "GAAL", 50, 5, TipoProfessor::COMUM);
            golpe(p, "Produto Vetorial Cortante", 13, -1);
            golpe(p, "Matriz Transposta Traicoeira", 11, 3);
            return p;
        }
        case 2: {
            Professor* p = montar("Alexandre", "Analise Numerica", 120, 10, TipoProfessor::BOSS);
            p->removerInvulnerabilidade();
            golpe(p, "Metodo de Newton-Raphson", 22, 3);
            golpe(p, "Instabilidade Numerica", 18, -1);
            golpe(p, "Erro de Truncamento Fatal", 26, 2);
            return p;
        }
        default:
            return criarProfessorDesconhecido();
    }
}

// ---------------------------------------------------------------------------
// Area 2: Fisica
// ---------------------------------------------------------------------------

Professor* ProfessoresFabrica::criarProfessorFisica(int indiceBatalha) {
    switch (indiceBatalha) {
        case 1: {
            Professor* p = montar("Flavio", "Introducao a Fisica Mecanica", 50, 5, TipoProfessor::COMUM);
            golpe(p, "Segunda Lei de Newton", 12, -1);
            golpe(p, "Forca de Atrito Inesperada", 10, 3);
            return p;
        }
        case 2: {
            // Batalha falsa (US05): sem dano real
            Professor* p = montar("Outro Prof*", "Introducao a Fisica Mecanica", 30, 3, TipoProfessor::COMUM, true);
            golpe(p, "Giz Perdido", 5, -1);
            golpe(p, "Quadro Apagado Sem Querer", 5, 2);
            return p;
        }
        case 3: {
            Professor* p = montar("Ivonete", "Fundamentos de Mecanica", 80, 7, TipoProfessor::MINIBOSS);
            golpe(p, "Torque Desequilibrado", 16, 3);
            golpe(p, "Momento Angular Conservado", 14, -1);
            return p;
        }
        case 4: {
            Professor* p = montar("Slide Man", "Mecanica dos Fluidos", 125, 10, TipoProfessor::BOSS);
            p->removerInvulnerabilidade();
            golpe(p, "Equacao de Bernoulli", 20, 3);
            golpe(p, "Avalanche de Slides", 24, 2);
            golpe(p, "Escoamento Turbulento", 16, -1);
            return p;
        }
        default:
            return criarProfessorDesconhecido();
    }
}

// ---------------------------------------------------------------------------
// Area 3: PDS
// ---------------------------------------------------------------------------

Professor* ProfessoresFabrica::criarProfessorPds(int indiceBatalha) {
    switch (indiceBatalha) {
        case 1: {
            Professor* p = montar("Rezeck", "PDS II", 55, 6, TipoProfessor::COMUM);
            golpe(p, "Code Review Implacavel", 14, -1);
            golpe(p, "Refatoracao Forcada", 12, 3);
            return p;
        }
        case 2: {
            // Boss final: MANTEM a invulnerabilidade ate o jogador usar o item chave
            Professor* p = montar("Fernando", "PDS II", 150, 12, TipoProfessor::BOSS);
            golpe(p, "NullPointerException", 24, 3);
            golpe(p, "Merge Conflict Caotico", 28, 2);
            golpe(p, "Divida Tecnica Acumulada", 18, -1);
            return p;
        }
        default:
            return criarProfessorDesconhecido();
    }
}

// ---------------------------------------------------------------------------
// API publica
// ---------------------------------------------------------------------------

Professor* ProfessoresFabrica::criar_professor(int idArea, int indiceBatalha) {
    switch (idArea) {
        case 0: return criarProfessorCalculo(indiceBatalha);
        case 1: return criarProfessorGaal(indiceBatalha);
        case 2: return criarProfessorFisica(indiceBatalha);
        case 3: return criarProfessorPds(indiceBatalha);
        default: return criarProfessorDesconhecido();
    }
}

int ProfessoresFabrica::get_quantidade_areas() {
    return QUANTIDADE_AREAS;
}

int ProfessoresFabrica::get_quantidade_batalhas(int idArea) {
    if (idArea < 0 || idArea >= QUANTIDADE_AREAS) return 0;
    return BATALHAS_POR_AREA[idArea];
}

String ProfessoresFabrica::get_nome_area(int idArea) {
    if (idArea < 0 || idArea >= QUANTIDADE_AREAS) return String("");
    return String(NOMES_AREAS[idArea]);
}

bool ProfessoresFabrica::batalha_existe(int idArea, int indiceBatalha) {
    return indiceBatalha >= 1 && indiceBatalha <= get_quantidade_batalhas(idArea);
}

bool ProfessoresFabrica::eh_ultima_da_area(int idArea, int indiceBatalha) {
    return batalha_existe(idArea, indiceBatalha) && indiceBatalha == get_quantidade_batalhas(idArea);
}