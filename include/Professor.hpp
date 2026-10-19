#pragma once
#include <godot_cpp/classes/node2d.hpp>
#include <string>
#include <vector>
#include <map>
#include "Movimento.hpp"
#include "EstadosVisuais.hpp"

using namespace godot;

/**
 * @brief Classificação do professor dentro de uma área/disciplina.
 *
 * COMUM (0): batalha normal (1ª e 2ª luta da área).
 * MINIBOSS (1): 3ª luta da área, status elevados.
 * BOSS (2): 4ª luta da área, status ainda mais altos, começa invulnerável.
 *
 * Do GDScript, passe o valor inteiro correspondente pro parâmetro `tipo`
 * de `inicializar()` (0, 1 ou 2) — o enum em si não é exposto ao Godot.
 */
enum class TipoProfessor {
    COMUM,
    MINIBOSS,
    BOSS,
};

/**
 * @brief Representa o inimigo (ProfessorInimigo, no CRC) que o herói enfrenta numa luta.
 *
 * Conhece seu tipo (Comum/Miniboss/Boss), status de combate, moveset, e se
 * é uma "batalha falsa" roteirizada (US05) que não causa dano real.
 */
class Professor : public Node2D {
    GDCLASS(Professor, Node2D)

protected:
    static void _bind_methods();

private:
    std::string nome;
    std::string materia;
    TipoProfessor tipo;
    int vida;
    int vidaMaxima;
    int defesa;
    bool invulneravel;
    bool ehBatalhaFalsa;
    std::vector<Movimento> movimentos;

    std::map<EstadoVisual, std::string> spritesPorEstado;

public:
    /** @brief Construtor padrão (exigido pelo Godot pra usar `.new()`); chame inicializar() em seguida. */
    Professor();

    /**
     * @brief Cria um novo Professor direto em C++ (ex: usado pela ProfessoresFabrica).
     * @param nome Nome exibido do professor.
     * @param materia Disciplina/matéria associada.
     * @param vidaMaxima Vida máxima do professor.
     * @param defesa Defesa, usada na fórmula de dano recebido.
     * @param tipo Comum, Miniboss ou Boss (Boss começa invulnerável automaticamente).
     * @param batalhaFalsa Se true, a luta é roteirizada (US05) e não causa dano real.
     */
    Professor(std::string nome, std::string materia, int vidaMaxima, int defesa,
              TipoProfessor tipo = TipoProfessor::COMUM, bool batalhaFalsa = false);

    /** @brief Retorna o nome do professor. */
    std::string getNome() const;
    /** @brief Retorna a matéria/disciplina do professor. */
    std::string getMateria() const;
    /** @brief Retorna se o professor é Comum, Miniboss ou Boss. */
    TipoProfessor getTipo() const;
    /** @brief Retorna a vida atual do professor. */
    int getVida() const;
    /** @brief Retorna a vida máxima do professor. */
    int getVidaMaxima() const;
    /** @brief Retorna true se a vida atual for maior que zero. */
    bool estaVivo() const;

    /** @brief Retorna true enquanto o Boss não tiver sido "destravado" pelo item de história. */
    bool isInvulneravel() const;
    /** @brief Remove a invulnerabilidade do Boss (chamado ao usar o item de história na batalha). */
    void removerInvulnerabilidade();

    int get_tipo() const;
    bool is_batalha_falsa() const;

    /** @brief Retorna true se essa for uma batalha roteirizada (US05), sem dano real. */
    bool isBatalhaFalsa() const;

    /** @brief Adiciona um movimento ao moveset do professor. */
    void adicionarMovimento(Movimento movimento);
    /**
     * @brief Sorteia um ataque entre os movimentos que ainda podem ser usados (IA simples).
     * @return Referência pro movimento escolhido.
     */
    Movimento& escolherAtaque();

    /**
     * @brief Aplica dano ao professor usando a fórmula vida/(vida+defesa).
     * Se o professor estiver invulnerável, o dano é ignorado (0).
     * @param danoBase Dano bruto antes de aplicar a fórmula de mitigação.
     */
    void receberDano(int danoBase);

    /** @brief Define o caminho do sprite do professor pra uma pose/estado específico. */
    void definirSprite(EstadoVisual estado, std::string caminho);
    /** @brief Retorna o caminho do sprite pra uma pose, com fallback pra PARADO. */
    std::string getCaminhoSprite(EstadoVisual estado) const;

    // ======================================================================
    // API exposta ao GDScript
    // ======================================================================

    /**
     * @brief Chamar logo após `Professor.new()` no GDScript pra configurar o professor.
     * @param tipo 0 = COMUM, 1 = MINIBOSS, 2 = BOSS.
     */
    void inicializar(String nome, String materia, int vidaMaxima, int defesa, int tipo, bool batalhaFalsa);

    String get_nome() const;
    String get_materia() const;
    int get_vida() const;
    int get_vida_maxima() const;
    bool esta_vivo() const;
    bool is_invulneravel() const;

    /** @brief Cria e adiciona um movimento ao professor direto do GDScript. */
    void adicionar_movimento_simples(String nome, int dano, int usosMaximos);


    /**
     * @brief Define o sprite de uma pose. estado: 0 = TOMANDO_DANO, 1 = VITORIA,
     * 2 = DERROTA, 3 = PARADO (idle), 4 = ATACANDO (mesma ordem do enum EstadoVisual).
     */
    void definir_sprite(int estado, String caminho);
    /** @brief Caminho do sprite da pose (fallback: PARADO). Mesmos valores de estado acima. */
    String get_caminho_sprite(int estado) const;
};