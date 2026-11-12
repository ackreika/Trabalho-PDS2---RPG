#include "ProfessoresFabrica.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/memory.hpp>

using namespace godot;

namespace {
const int QUANTIDADE_AREAS = 5;
const int BATALHAS_POR_AREA[QUANTIDADE_AREAS] = {3, 2, 4, 2, 1};
const char* NOMES_AREAS[QUANTIDADE_AREAS] = {"Calculo", "GAAL", "Fisica", "PDS", "Segredo"};
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
                "res://assets/imagens/SubstitutoNormal-removebg-preview.png",    // parado
                "res://assets/imagens/SubstitutoSorrindo-removebg-preview.png",  // atacando
                "res://assets/imagens/SubstitutoConfuso-removebg-preview.png",   // tomando dano
                "res://assets/imagens/SubstitutoConfuso-removebg-preview.png",   // vitoria
                "res://assets/imagens/SubstitutoNormal-removebg-preview.png");   // derrota
            return p;
        }
        case 2: {
            Professor* p = montar("Xavier", "Calculo I", 55, 5, TipoProfessor::COMUM);
            golpe(p, "Regra da Cadeia Relampago", 12, -1);
            golpe(p, "Derivada Implicita Confusa", 10, 3);
            sprites(p,
                "res://assets/imagens/Javier_Normal-removebg-preview.png",
                "res://assets/imagens/Javier_Irritado-removebg-preview.png",
                "res://assets/imagens/Javier_Irritado-removebg-preview.png",
                "res://assets/imagens/Javier_Normal-removebg-preview.png",
                "res://assets/imagens/Javier_Sorrindo-removebg-preview.png");
            return p;
        }
        case 3: {
            Professor* p = montar("Colina Sombria", "Calculo II", 110, 9, TipoProfessor::BOSS);
            p->removerInvulnerabilidade(); // boss de area: vulneravel
            golpe(p, "Serie de Taylor Infinita", 20, 3);
            golpe(p, "Integral Impropria Divergente", 25, 2);
            golpe(p, "Convergencia Sombria", 15, -1);
            sprites(p,
                "res://assets/imagens/MarcosSorrindo-removebg-preview.png",
                "res://assets/imagens/MarcosSorrindo-removebg-preview.png",
                "res://assets/imagens/MarcosSerio.png",
                "res://assets/imagens/MarcosSorrindo-removebg-preview.png",
                "res://assets/imagens/MarcosSorrindo-removebg-preview.png");
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
            sprites(p,
                "res://assets/imagens/BekkertSorrindo-removebg-preview.png",
                "res://assets/imagens/BekkertBravo-removebg-preview.png",
                "res://assets/imagens/BekkertBravo-removebg-preview.png",
                "res://assets/imagens/BekkertPalmas-removebg-preview.png",
                "res://assets/imagens/BekkertSorrindo-removebg-preview.png");
            return p;
        }
        case 2: {
<<<<<<< Updated upstream
            Professor* p = montar("Alexandre", "Analise Numerica", 120, 10, TipoProfessor::BOSS);
=======
            Professor* p = montar("XandeNumerus", "Analise Numerica", 125, 10, TipoProfessor::BOSS);
>>>>>>> Stashed changes
            p->removerInvulnerabilidade();
            golpe(p, "Metodo de Newton-Raphson", 22, 3);
            golpe(p, "Instabilidade Numerica", 18, -1);
            golpe(p, "Erro de Truncamento Fatal", 26, 2);
            sprites(p,
                "res://assets/imagens/AlexandreSorrindo-removebg-preview.png",
                "res://assets/imagens/AlexandreIrritado-removebg-preview.png",
                "res://assets/imagens/AlexandreIrritado-removebg-preview.png",
                "res://assets/imagens/AlexandrePalmas-removebg-preview.png",
                "res://assets/imagens/AlexandreSorrindo-removebg-preview.png");
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
            sprites(p,
                "res://assets/imagens/DuczmalNeutro-removebg-preview.png",
                "res://assets/imagens/DuczmalBravo-removebg-preview.png",
                "res://assets/imagens/DuczmalBravo-removebg-preview.png",
                "res://assets/imagens/DuczmalPalmas-removebg-preview.png",
                "res://assets/imagens/DuczmalFeliz-removebg-preview.png");
            return p;
        }
        case 2: {
            // Batalha falsa (US05): sem dano real
            Professor* p = montar("America", "Introducao a Fisica Mecanica", 30, 3, TipoProfessor::COMUM, true);
            golpe(p, "Giz Perdido", 5, -1);
            golpe(p, "Quadro Apagado Sem Querer", 5, 2);
            sprites(p,
                "res://assets/imagens/PedroNormal-removebg-preview.png", // parado
                "res://assets/imagens/PedroNormal-removebg-preview.png", // atacando
                "res://assets/imagens/PedroBravo-removebg-preview.png",  // tomando dano
                "res://assets/imagens/PedroBravo-removebg-preview.png",  // vitoria
                "res://assets/imagens/PedroNormal-removebg-preview.png"); // derrota
            return p;
        }
        case 3: {
<<<<<<< Updated upstream
            Professor* p = montar("Ivonete", "Fundamentos de Mecanica", 80, 7, TipoProfessor::MINIBOSS);
            golpe(p, "Torque Desequilibrado", 16, 3);
            golpe(p, "Momento Angular Conservado", 14, -1);
=======
            Professor* p = montar("Ivone", "Fundamentos de Mecanica", 115, 7, TipoProfessor::MINIBOSS);
            golpe(p, "Torque Desequilibrado", 17, 3);
            golpe(p, "Momento Angular Conservado", 19, -1);
>>>>>>> Stashed changes
            sprites(p,
                "res://assets/imagens/IvoneteSorrindo-removebg-preview.png",
                "res://assets/imagens/IvoneteSorrindo-removebg-preview.png",
                "res://assets/imagens/IvoneteConfusa-removebg-preview.png",
                "res://assets/imagens/IvoneteDeBraços-removebg-preview.png",
                "res://assets/imagens/IvoneteDeBraços-removebg-preview.png");
            return p;
        }
        case 4: {
            Professor* p = montar("Slide Man", "Mecanica dos Fluidos", 125, 10, TipoProfessor::BOSS);
            p->removerInvulnerabilidade();
            golpe(p, "Equacao de Bernoulli", 20, 3);
            golpe(p, "Avalanche de Slides", 24, 2);
            golpe(p, "Escoamento Turbulento", 16, -1);
            sprites(p,
                "res://assets/imagens/SlideFeliz-removebg-preview.png",
                "res://assets/imagens/SlideTriste-removebg-preview.png",
                "res://assets/imagens/SlideTriste-removebg-preview.png",
                "res://assets/imagens/SlidePalmas-removebg-preview.png",
                "res://assets/imagens/SlideFeliz-removebg-preview.png");
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
<<<<<<< Updated upstream
            Professor* p = montar("Rezeck", "PDS II", 55, 6, TipoProfessor::COMUM);
            golpe(p, "Code Review Implacavel", 14, -1);
            golpe(p, "Refatoracao Forcada", 12, 3);
=======
            Professor* p = montar("Rezende", "PDS II", 125, 7, TipoProfessor::COMUM);
            golpe(p, "Code Review Implacavel", 16, -1);
            golpe(p, "Refatoracao Forcada", 14, 3);
>>>>>>> Stashed changes
            sprites(p,
                "res://assets/imagens/RezeckNeutro-removebg-preview.png",
                "res://assets/imagens/RezeckNeutro-removebg-preview.png",
                "res://assets/imagens/RezeckConfuso-removebg-preview.png",
                "res://assets/imagens/RezeckPalmas-removebg-preview.png",
                "res://assets/imagens/RezeckNeutro-removebg-preview.png");
            return p;
        }
        case 2: {
            // O boss final fica vulneravel ate chegar a metade da vida.
            Professor* p = montar("Fernando", "PDS II", 150, 12, TipoProfessor::BOSS);
            p->ativarInvulnerabilidadeNaMetadeDaVida();
            golpe(p, "NullPointerException", 24, 3);
            golpe(p, "Merge Conflict Caotico", 28, 2);
            golpe(p, "Divida Tecnica Acumulada", 18, -1);
            sprites(p,
                "res://assets/imagens/FernandoSorrisinho-removebg-preview.png",
                "res://assets/imagens/FernandoSorrisinho-removebg-preview.png",
                "res://assets/imagens/FernandoBravo-removebg-preview.png",
                "res://assets/imagens/FernandoSorrindo-removebg-preview.png",
                "res://assets/imagens/FernandoEstranhando-removebg-preview.png");
            return p;
        }
        default:
            return criarProfessorDesconhecido();
    }
}

// Area 4: Batalha secreta, desbloqueada apos concluir a campanha principal.
Professor* ProfessoresFabrica::criarProfessorSecreto(int indiceBatalha) {
    if (indiceBatalha == 1) {
<<<<<<< Updated upstream
        Professor* p = montar("Luciana", "Desafio Secreto", 190, 14, TipoProfessor::BOSS);
=======
        Professor* p = montar("Luci", "Desafio Secreto", 500, 14, TipoProfessor::SUPERBOSS);
>>>>>>> Stashed changes
        p->removerInvulnerabilidade();
        golpe(p, "textinho", 20, -1);
        golpe(p, "hassou tobi", 27, 3);
        golpe(p, "Amp op subtrator", 23, 2);
        sprites(p,
            "res://assets/imagens/LucianaNormal-removebg-preview.png", // parado
            "res://assets/imagens/LucianaNormal-removebg-preview.png", // atacando
            "res://assets/imagens/LucianaBrava-removebg-preview.png",  // tomando dano
            "res://assets/imagens/LucianaRindo-removebg-preview.png",  // vitoria
            "res://assets/imagens/LucianaNormal-removebg-preview.png"); // derrota
        return p;
    }
    return criarProfessorDesconhecido();
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
        case 4: return criarProfessorSecreto(indiceBatalha);
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
    return String::utf8(NOMES_AREAS[idArea]);
}

bool ProfessoresFabrica::batalha_existe(int idArea, int indiceBatalha) {
    return indiceBatalha >= 1 && indiceBatalha <= get_quantidade_batalhas(idArea);
}

bool ProfessoresFabrica::eh_ultima_da_area(int idArea, int indiceBatalha) {
    return batalha_existe(idArea, indiceBatalha) && indiceBatalha == get_quantidade_batalhas(idArea);
}
