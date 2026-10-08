#pragma once
#include <godot_cpp/classes/node.hpp>
#include <string>
#include <vector>
#include <map>
#include "Movimento.hpp"
#include "ItemPassivo.hpp"
#include "ItemEspecial.hpp"
#include "EstadosVisuais.hpp"


using namespace godot;

class Professor; // precisa declarar o tipo Professor, usado em Herois::usarItemEspecial()

/**
 * @brief Representa o Estudante (jogador) durante uma run.
 *
 * Guarda status (vida, defesa, dano), os 3 movimentos escolhidos pelo jogador,
 * os itens passivos coletados na run atual, o item especial equipado (4º slot
 * de golpe na batalha) e o item de história usado contra bosses invulneráveis.
 */
class Herois : public Node {
    GDCLASS(Herois, Node)

protected:
    static void _bind_methods();
private:
    std::string nome;
    int nivel;
    int vida;
    int vidaMaxima;
    int vidaMaximaBase; ///< valor original, sem bônus de itens
    int defesa;
    int danoBase;

    std::vector<Movimento> movimentos;
    std::vector<ItemPassivo> itensPassivos;

    bool possuiItemEspecialAtivo;
    ItemEspecial itemEspecialAtivo;

    bool possuiItemChave;

    std::map<EstadoVisual, std::string> spritesPorEstado;

public:
    /**
     * @brief Cria um novo Herois com os status base da run.
     * @param nome Nome exibido do herói.
     * @param vidaMaxima Vida máxima inicial (antes de bônus de itens).
     * @param defesa Defesa base, usada na fórmula de dano recebido.
     * @param danoBase Dano base, usado nos cálculos de ataque.
     */
    
     Herois(std::string nome, int vidaMaxima, int defesa, int danoBase);

    /** @brief Retorna o nome do herói. */
    std::string getNome() const;
    /** @brief Retorna a vida atual. */
    int getVida() const;
    /** @brief Retorna a vida máxima atual (já com bônus de itens aplicados). */
    int getVidaMaxima() const;
    /** @brief Retorna a defesa base do herói. */
    int getDefesa() const;
    /** @brief Retorna o dano base do herói. */
    int getDanoBase() const;
    /** @brief Retorna true se a vida atual for maior que zero. */
    bool estaVivo() const;

    /** @brief Restaura vida, sem ultrapassar a vida máxima. */
    void curar(int quantidade);
    /**
     * @brief Aplica dano ao herói usando a fórmula vida/(vida+defesa).
     * @param danoBase Dano bruto antes de aplicar a fórmula de redução.
     */
    void receberDano(int danoBase);

    /**
     * @brief Adiciona um movimento ao moveset (limite de 3 pois o 4º slot é o item especial).
     * @param novo Movimento a ser aprendido.
     * @param indiceParaSubstituir Se o moveset já estiver cheio, índice do movimento a substituir; -1 para não substituir.
     */
    void aprenderMovimento(const Movimento& novo, int indiceParaSubstituir = -1);
    /** @brief Retorna a lista de movimentos aprendidos (somente leitura). */
    const std::vector<Movimento>& getMovimentos() const;
    /**
     * @brief Retorna um ponteiro editável pro movimento no índice informado.
     * @param indice Posição no moveset (0 a 2).
     * @return Ponteiro pro movimento, ou nullptr se o índice for inválido.
     */
    Movimento* getMovimentoParaEditar(int indice);

    /** @brief Adiciona um item passivo coletado e recalcula a vida máxima. */
    void adicionarItemPassivo(ItemPassivo item);
    /** @brief Retorna quantos itens passivos o herói possui. */
    int getQuantidadeItensPassivos() const;
    /** @brief Retorna o nome do item passivo no índice informado, ou "" se inválido. */
    std::string getNomeItemPassivo(int indice) const;
    /** @brief Aplica os bônus de dano de todos os itens passivos sobre um valor base. */
    int calcularDanoFinal(int danoBase) const;
    /** @brief Aplica os bônus de vida máxima de todos os itens passivos sobre um valor base. */
    int calcularVidaFinal(int vidaBase) const;
    /** @brief Reaplica todos os bônus de vida máxima a partir do valor base; chamado ao equipar um item novo. */
    void recalcularVidaMaxima();

    /** @brief Equipa/troca o item especial ativo (4º slot de golpe). */
    void definirItemEspecial(ItemEspecial item);
    /** @brief Retorna true se o herói tiver algum item especial equipado. */
    bool possuiItemEspecial() const;
    /** @brief Retorna o nome do item especial equipado, ou "" se nenhum. */
    std::string getNomeItemEspecial() const;
    /** @brief Retorna true se houver item especial equipado E com usos restantes. */
    bool itemEspecialPodeUsar() const;
    /**
     * @brief Consome um uso do item especial e aplica dano no alvo.
     * @param alvo Professor que vai receber o dano.
     * @return Dano causado, ou 0 se a ação for inválida.
     */
    int usarItemEspecial(Professor& alvo);

    /** @brief Marca que o herói obteve o item de história (quebra invulnerabilidade de Boss). */
    void obterItemChave();
    /** @brief Retorna true se o herói possuir o item de história no momento. */
    bool possuiItemDeChave() const;
    /** @brief Marca o item de história como consumido/gasto. */
    void consumirItemChave();

    /** @brief Define o caminho do sprite do herói pra uma pose específica. */
    void definirSprite(EstadoVisual estado, std::string caminho);
    /**
     * @brief Retorna o caminho do sprite pra uma pose. Se a pose não tiver sido definida,
     * retorna a pose PARADO.
     */
    std::string getCaminhoSprite(EstadoVisual estado) const;
};