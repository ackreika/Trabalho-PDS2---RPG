#pragma once

/** @brief Tipo de buff/debuff aplicável (ModificadorStatus, no CRC). */
enum class TipoModificador {
    INVULNERAVEL,
    FORTALECIDO,
};

/**
 * @brief Buff/debuff genérico com duração em turnos (ou permanente, se -1).
 *
 * Independente da invulnerabilidade fixa do Boss (essa já é tratada direto
 * em Professor::invulneravel) — este é reutilizável pra qualquer efeito
 * futuro com duração, como "Fortalecido por 3 turnos".
 */
class ModificadorStatus {
private:
    TipoModificador tipo;
    int duracaoRestante; ///< em turnos; -1 = permanente até ser removido manualmente

public:
    /**
     * @brief Cria um novo modificador de status.
     * @param tipo Invulnerável ou Fortalecido.
     * @param duracaoTurnos Duração em turnos; -1 (padrão) = permanente.
     */
    ModificadorStatus(TipoModificador tipo, int duracaoTurnos = -1);

    /** @brief Retorna o tipo do modificador. */
    TipoModificador getTipo() const;
    /** @brief Retorna true se o modificador ainda estiver em efeito. */
    bool estaAtivo() const;

    /** @brief Decrementa a duração; chamado no fim de cada turno. */
    void passarTurno();
    /** @brief Retorna true quando a duração chegou a 0 e o modificador deve ser removido. */
    bool deveSerRemovido() const;

    /**
     * @brief Aplica o efeito desse modificador sobre um valor de dano.
     * INVULNERAVEL sempre zera o dano; FORTALECIDO aumenta em 20%.
     */
    int interceptarDano(int danoBase) const;
};