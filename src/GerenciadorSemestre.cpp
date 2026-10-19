#include "GerenciadorSemestre.hpp"
#include "ProfessoresFabrica.hpp"

GerenciadorSemestre::GerenciadorSemestre()
    : idAreaAtual(0), indiceBatalhaAtual(1) {}

int GerenciadorSemestre::getIdAreaAtual() const { return idAreaAtual; }
int GerenciadorSemestre::getIndiceBatalhaAtual() const { return indiceBatalhaAtual; }

Professor GerenciadorSemestre::gerarProximoProfessor() const {
    return ProfessoresFabrica::criarProfessor(idAreaAtual, indiceBatalhaAtual);
}

void GerenciadorSemestre::avancarBatalha() {
    if (venceuOJogo()) {
        return; // já venceu o jogo, não há mais o que avançar
    }

    indiceBatalhaAtual++;
    if (indiceBatalhaAtual > 4) {
        indiceBatalhaAtual = 1;
        idAreaAtual++; // venceu o boss da área — avança pra próxima disciplina
    }
}

bool GerenciadorSemestre::venceuOJogo() const {
    return idAreaAtual >= TOTAL_AREAS;
}
