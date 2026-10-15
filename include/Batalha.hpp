#pragma once
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <string>
#include "Herois.hpp"
#include "Professor.hpp"

using namespace godot;

/**
 * @brief Estado geral da batalha. Em get_estado()/no Dictionary de resultado,
 * é representado como int:
 *   0 = ESCOLHENDO_ACAO (aguardando o jogador escolher um golpe ou fugir)
 *   1 = VITORIA (professor derrotado)
 *   2 = DERROTA (herói derrotado)
 *   3 = FUGIU (jogador desistiu — run perdida)
 */
enum class EstadoBatalha {
    ESCOLHENDO_ACAO,
    VITORIA,
    DERROTA,
    FUGIU,
};

/**
 * @brief Resultado de um turno completo (ação do herói + resposta do professor,
 * quando aplicável). Usado internamente em C++; pro GDScript, os métodos
 * públicos de Batalha convertem isso pra um Dictionary com as mesmas chaves
 * em snake_case: acao_valida, mensagem, heroi_atacou, dano_causado_pelo_heroi,
 * professor_atacou, dano_causado_pelo_professor, estado_resultante.
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
 *
 * É um RefCounted (não um Node) porque não precisa existir na árvore de cena —
 * é só um objeto de lógica que o GDScript cria com Batalha.new() e guarda numa
 * variável (ex: no autoload Global).
 */
class Batalha : public RefCounted {
    GDCLASS(Batalha, RefCounted)

protected:
    static void _bind_methods();

private:
    Herois* heroi;
    Professor* professor;
    EstadoBatalha estado;
    int turnosDecorridos;

    static const int TURNOS_PARA_VITORIA_BATALHA_FALSA = 2;

    /** @brief Executa o turno do professor: escolhe um ataque e aplica no herói. */
    ResultadoTurno processarTurnoProfessor();

    /** @brief Converte o resultado interno num Dictionary que o GDScript consegue ler. */
    Dictionary converterParaDictionary(const ResultadoTurno& resultado) const;

public:
    /** @brief Construtor padrão (exigido pelo Godot); chame inicializar() em seguida. */
    Batalha();

    /**
     * @brief Prepara a batalha. Precisa ser chamado logo após `Batalha.new()` no GDScript,
     * antes de qualquer outra chamada.
     * @param heroiDoJogador Ponteiro pro Herois do jogador (não é copiado, nem a Batalha é dona dele).
     * @param professorAlvo Ponteiro pro Professor a ser enfrentado.
     */
    void inicializar(Herois* heroiDoJogador, Professor* professorAlvo);

    /** @brief Retorna o estado atual da batalha (ver os valores no comentário de EstadoBatalha). */
    int get_estado() const;
    /** @brief Retorna o Professor sendo enfrentado (pra ler nome/vida na UI). */
    Professor* get_professor() const;
    /** @brief Retorna o Herois que está lutando (pra ler nome/vida na UI). */
    Herois* get_heroi() const;

    /**
     * @brief Executa a ação escolhida pelo jogador no turno atual.
     * @param indiceGolpe 0, 1 ou 2 = movimento do herói; 3 = item especial (4º slot).
     * @return Dictionary com: acao_valida (bool), mensagem (String), heroi_atacou (bool),
     * dano_causado_pelo_heroi (int), professor_atacou (bool), dano_causado_pelo_professor (int),
     * estado_resultante (int).
     */
    Dictionary executar_acao(int indiceGolpe);

    /**
     * @brief Usa o item de história pra quebrar a invulnerabilidade de um Boss (US04).
     * @return Dictionary no mesmo formato de executar_acao().
     */
    Dictionary usar_item_chave();

    /**
     * @brief O jogador desiste da luta; a run é perdida.
     * @return Dictionary com estado_resultante = 3 (FUGIU).
     */
    Dictionary fugir();
};