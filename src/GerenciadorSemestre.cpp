#include "GerenciadorSemestre.hpp"
#include "ProfessoresFabrica.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

GerenciadorSemestre::GerenciadorSemestre()
    : idAreaAtual(0), indiceBatalhaAtual(1) {}

void GerenciadorSemestre::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("iniciar", "id_area"), &GerenciadorSemestre::iniciar);
    godot::ClassDB::bind_method(godot::D_METHOD("get_id_area_atual"), &GerenciadorSemestre::get_id_area_atual);
    godot::ClassDB::bind_method(godot::D_METHOD("get_indice_batalha_atual"), &GerenciadorSemestre::get_indice_batalha_atual);
    godot::ClassDB::bind_method(godot::D_METHOD("avancar_batalha"), &GerenciadorSemestre::avancar_batalha);
    godot::ClassDB::bind_method(godot::D_METHOD("venceu_o_jogo"), &GerenciadorSemestre::venceu_o_jogo);
}

void GerenciadorSemestre::iniciar(int idArea) {
    const int total = ProfessoresFabrica::get_quantidade_areas();
    if (total <= 0) {
        idAreaAtual = 0;
        indiceBatalhaAtual = 1;
        godot::UtilityFunctions::push_error("Não há áreas de batalha cadastradas.");
        return;
    }
    if (idArea < 0 || idArea >= total) {
        godot::UtilityFunctions::push_warning("Área inicial inválida; começando pela primeira área.");
        idArea = 0;
    }
    idAreaAtual = idArea;
    indiceBatalhaAtual = 1;
}

int GerenciadorSemestre::getIdAreaAtual() const { return idAreaAtual; }
int GerenciadorSemestre::getIndiceBatalhaAtual() const { return indiceBatalhaAtual; }

Professor* GerenciadorSemestre::gerarProximoProfessor() const {
    return ProfessoresFabrica::criar_professor(idAreaAtual, indiceBatalhaAtual);
}

void GerenciadorSemestre::avancarBatalha() {
    if (venceuOJogo()) {
        return; // já venceu o jogo, não há mais o que avançar
    }

    const int totalBatalhas = ProfessoresFabrica::get_quantidade_batalhas(idAreaAtual);
    if (totalBatalhas <= 0 || indiceBatalhaAtual < 1 || indiceBatalhaAtual > totalBatalhas) {
        godot::UtilityFunctions::push_error("Progressão inválida; encerrando a run com segurança.");
        idAreaAtual = ProfessoresFabrica::get_quantidade_areas();
        indiceBatalhaAtual = 1;
        return;
    }

    if (indiceBatalhaAtual >= totalBatalhas) {
        // A campanha principal termina em PDS. A área secreta é iniciada
        // separadamente pelo menu e não entra automaticamente na campanha.
        if (idAreaAtual == 3) {
            idAreaAtual = ProfessoresFabrica::get_quantidade_areas();
            indiceBatalhaAtual = 1;
            return;
        }
        indiceBatalhaAtual = 1;
        idAreaAtual++; // venceu o boss da área — avança pra próxima disciplina
    } else {
        indiceBatalhaAtual++;
    }
}

bool GerenciadorSemestre::venceuOJogo() const {
    return idAreaAtual >= ProfessoresFabrica::get_quantidade_areas();
}

int GerenciadorSemestre::get_id_area_atual() const { return getIdAreaAtual(); }
int GerenciadorSemestre::get_indice_batalha_atual() const { return getIndiceBatalhaAtual(); }
bool GerenciadorSemestre::avancar_batalha() {
    avancarBatalha();
    return !venceuOJogo();
}
bool GerenciadorSemestre::venceu_o_jogo() const { return venceuOJogo(); }
