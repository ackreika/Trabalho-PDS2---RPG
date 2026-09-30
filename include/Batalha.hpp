#pragma once
#include <string>
#include "Herois.hpp"
#include "Professor.hpp"

/** @brief Estado geral da batalha; o Godot consulta isso pra saber que tela/animação mostrar. */
enum class EstadoBatalha {
    ESCOLHENDO_ACAO, ///< aguardando o jogador escolher um golpe ou fugir
    VITORIA,         ///< professor derrotado
    DERROTA,         ///< herói derrotado
    FUGIU,           ///< jogador desistiu — run perdida
};

/**
 * @brief Resultado de um turno completo (ação do herói + resposta do professor,
 * quando aplicável). O Godot usa isso pra saber o que animar/exibir na UI.
 */
struct ResultadoTurno {
    bool acaoValida;        ///< false se, por ex., tentou usar item sem usos restantes
    std::string mensagem;   ///< texto simples pra exibir na caixa de diálogo da batalha

    bool heroiAtacou;
    int danoCausadoPeloHeroi;

    bool professorAtacou;
    int danoCausadoPeloProfessor;

    EstadoBatalha estadoResultante; ///< estado da batalha depois do turno
};

/**
 * @brief Orquestra uma luta entre o Herois do jogador e um Professor (SistemaBatalha, no CRC).
 *
 * Recebe a ação escolhida pelo jogador (um dos 4 golpes, usar item de história
 * ou fugir), resolve o dano, aplica o turno do professor em seguida, e sinaliza
 * o estado resultante (vitória, derrota ou fuga) pro Godot reagir.
 */
class Batalha {
private:
    Herois& heroi;
    Professor professor;
    EstadoBatalha estado;
    int turnosDecorridos;

    static const int TURNOS_PARA_VITORIA_BATALHA_FALSA = 2;

    /** @brief Executa o turno do professor: escolhe um ataque e aplica no herói. */
    ResultadoTurno processarTurnoProfessor();

public:
    /**
     * @brief Inicia uma batalha entre o herói do jogador e o professor informado.
     * @param heroi Referência pro herói do jogador (não é copiado).
     * @param professor Professor a ser enfrentado (copiado por valor).
     */
    Batalha(Herois& heroi, Professor professor);

    /** @brief Retorna o estado atual da batalha. */
    EstadoBatalha getEstado() const;
    /** @brief Retorna uma referência constante pro professor sendo enfrentado. */
    const Professor& getProfessor() const;

    /**
     * @brief Executa a ação escolhida pelo jogador no turno atual.
     * @param indiceGolpe 0, 1 ou 2 = movimento do herói; 3 = item especial (4º slot).
     * @return O resultado do turno (dano causado/recebido e o novo estado da batalha).
     */
    ResultadoTurno executarAcao(int indiceGolpe);

    /**
     * @brief Usa o item de história pra quebrar a invulnerabilidade de um Boss (US04).
     * @return Resultado indicando se a ação foi válida.
     */
    ResultadoTurno usarItemChave();

    /**
     * @brief O jogador desiste da luta; a run é perdida.
     * @return Resultado com o estado FUGIU.
     */
    ResultadoTurno fugir();
};