#include "ModificadorStatus.hpp"
#include <algorithm>
#include <limits>

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
	if (danoBase <= 0) return 0;
    if (!estaAtivo()) return danoBase;
    switch (tipo) {
        case TipoModificador::INVULNERAVEL:
            return 0;
		case TipoModificador::FORTALECIDO: {
			const double fortalecido = static_cast<double>(danoBase) * 1.20;
			return static_cast<int>(std::min(fortalecido, static_cast<double>(std::numeric_limits<int>::max())));
		}
        default:
            return danoBase;
    }
}
