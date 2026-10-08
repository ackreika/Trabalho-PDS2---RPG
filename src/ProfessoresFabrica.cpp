#include "ProfessoresFabrica.hpp"

// Área 0: Cálculo — exemplo completo com as 4 batalhas (2 comuns, 1 miniboss, 1 boss)
static Professor criarProfessorCalculo(int indiceBatalha) {
    switch (indiceBatalha) {
        case 1: {
            Professor p("Prof. Xavier", "Cálculo I", 40, 5, TipoProfessor::COMUM);
            p.adicionarMovimento(Movimento("Derivação implícita", 8));
            p.adicionarMovimento(Movimento("Teste 7", 5));
            p.definirSprite(EstadoVisual::PARADO, "res://assets/professores/calculo_parado.png");
            p.definirSprite(EstadoVisual::ATACANDO, "res://assets/professores/calculo_atacando.png");
            return p;
        }
        case 2: {
            Professor p("Carlos Colina Escura", "Cálculo II", 55, 8, TipoProfessor::COMUM);
            p.adicionarMovimento(Movimento("Coordenadas Polares", 10));
            p.adicionarMovimento(Movimento("Série de Taylor", 14, /*usosMaximos=*/3));
            p.definirSprite(EstadoVisual::PARADO, "res://assets/professores/calculo2_parado.png");
            p.definirSprite(EstadoVisual::ATACANDO, "res://assets/professores/calculo2_atacando.png");
            return p;
        }
        case 3: {
            // Miniboss: status elevados em relação aos comuns da mesma área
            Professor p("Coordenador de Cálculo", "Cálculo III", 90, 12, TipoProfessor::MINIBOSS);
            p.adicionarMovimento(Movimento("Prova Surpresa", 16));
            p.adicionarMovimento(Movimento("Integral Tripla", 22, /*usosMaximos=*/4));
            p.definirSprite(EstadoVisual::PARADO, "res://assets/professores/coordenador_calculo_parado.png");
            p.definirSprite(EstadoVisual::ATACANDO, "res://assets/professores/coordenador_calculo_atacando.png");
            return p;
        }
        case 4: {
            // Boss: status ainda mais altos, começa invulnerável (ver Professor::Professor)
            Professor p("Chefe do Departamento de Matemática", "Cálculo", 150, 18, TipoProfessor::BOSS);
            p.adicionarMovimento(Movimento("Reprovação em Massa", 25));
            p.adicionarMovimento(Movimento("Prova Final Impossível", 35, /*usosMaximos=*/3));
            p.definirSprite(EstadoVisual::PARADO, "res://assets/professores/chefe_calculo_parado.png");
            p.definirSprite(EstadoVisual::ATACANDO, "res://assets/professores/chefe_calculo_atacando.png");
            return p;
        }
        default: {
            Professor p("Monitor de Cálculo", "Cálculo", 30, 4, TipoProfessor::COMUM);
            p.adicionarMovimento(Movimento("Ataque Basico", 5));
            return p;
        }
    }
}

// Áreas 1 (Física), 2 (GAAL) e 3 (PDS) seguem o mesmo padrão de criarProfessorCalculo —
// criem uma função static "criarProfessorFisica", "criarProfessorGaal", "criarProfessorPds"
// cada uma, e chamem aqui embaixo. Deixei só o "case 0" pronto como referência.
Professor ProfessoresFabrica::criarProfessor(int idArea, int indiceBatalha) {
    switch (idArea) {
        case 0:
            return criarProfessorCalculo(indiceBatalha);
        // case 1: return criarProfessorFisica(indiceBatalha);
        // case 2: return criarProfessorGaal(indiceBatalha);
        // case 3: return criarProfessorPds(indiceBatalha);
        default: {
            Professor p("Professor Desconhecido", "Materia Desconhecida", 30, 3, TipoProfessor::COMUM);
            p.adicionarMovimento(Movimento("Ataque Basico", 5));
            p.definirSprite(EstadoVisual::PARADO, "res://assets/professores/generico_parado.png");
            p.definirSprite(EstadoVisual::ATACANDO, "res://assets/professores/generico_atacando.png");
            return p;
        }
    }
}