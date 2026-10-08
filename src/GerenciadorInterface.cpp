#include "GerenciadorInterface.hpp"
#include <algorithm>

using namespace godot;

GerenciadorInterface::GerenciadorInterface()
    : estadoAtual(GerenciadorMenu::EstadoMenu::MENU_INICIAL), volume(1.0f), indiceResolucao(0) {}

GerenciadorMenu::EstadoMenu GerenciadorInterface::getEstadoAtual() const { return estadoAtual; }

void GerenciadorInterface::irPara(GerenciadorMenu::EstadoMenu novoEstado) {
    estadoAtual = novoEstado;
}

void GerenciadorInterface::voltar() {
    estadoAtual = GerenciadorMenu::EstadoMenu::MENU_INICIAL;
}

float GerenciadorInterface::getVolume() const { return volume; }

void GerenciadorInterface::definirVolume(float novoVolume) {
    volume = std::max(0.0f, std::min(1.0f, novoVolume)); // trava entre 0 e 1
}

int GerenciadorInterface::getIndiceResolucao() const { return indiceResolucao; }

void GerenciadorInterface::definirIndiceResolucao(int indice) {
    indiceResolucao = indice;
}

void GerenciadorInterface::iniciarOuRetomarPartida() {
    estadoAtual = GerenciadorMenu::EstadoMenu::EM_BATALHA;
}
