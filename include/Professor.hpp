#pragma once
#include <string>
#include <vector>
#include <map>
#include "Movimento.hpp"
#include "EstadosVisuais.hpp"

/**
 * @brief Classificação do professor dentro de uma área/disciplina.
 *
 * COMUM: batalha normal (1ª e 2ª luta da área).
 * MINIBOSS: 3ª luta da área, status elevados.
 * BOSS: 4ª luta da área, status ainda mais altos, começa invulnerável.
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
class Professor {
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
    /**
     * @brief Cria um novo Professor.
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
};