#include "ModificadorStatus.hpp"

ModificadorStatus::ModificadorStatus(TipoModificador tipo, int duracaoTurnos)
    : tipo(tipo), duracaoRestante(duracaoTurnos) {}

TipoModificador ModificadorStatus::getTipo() const { return tipo; }

bool ModificadorStatus::estaAtivo() const {
    return duracaoRestante < 0 || duracaoRestante > 0;
}

void ModificadorStatus::passarTurno() {
    if (duracaoRestante > 0) {
        duracaoRestante--;
    }
}

bool ModificadorStatus::deveSerRemovido() const {
    return duracaoRestante == 0;
}

int ModificadorStatus::interceptarDano(int danoBase) const {
    switch (tipo) {
        case TipoModificador::INVULNERAVEL:
            return 0;
        case TipoModificador::FORTALECIDO:
            return static_cast<int>(danoBase * 1.20);
        default:
            return danoBase;
    }
}
