#pragma once
#include <set>
#include <string>
#include "ItensIds.hpp"

/**
 * @brief Meta-progresso entre runs: pontos acumulados
 * e itens já desbloqueados. Guarda em um arquivo .txt simples.
 */
class ProgressoJogador {
private:
    int pontos;
    std::set<int> itensDesbloqueados;

public:
    /** @brief Cria um progresso vazio: 0 pontos, nenhum item desbloqueado. */
    ProgressoJogador();

    /** @brief Retorna a quantidade de pontos acumulados. */
    int getPontos() const;
    /** @brief Adiciona pontos ao total acumulado. */
    void adicionarPontos(int quantidade);
    /**
     * @brief Tenta gastar pontos num upgrade.
     * @return true se havia pontos suficientes (e eles foram descontados); false caso contrário.
     */
    bool gastarPontos(int quantidade);

<<<<<<< Updated upstream
    /** @brief Retorna true se o item com esse ID já foi desbloqueado. */
    bool jaObteve(ItemId id) const;
    /** @brief Marca um item como desbloqueado. */
    void desbloquear(ItemId id);
    /** @brief Retorna o conjunto de IDs de todos os itens desbloqueados. */
    const std::set<int>& getTodos() const;
=======
    int get_nivel_vida() const { return nivel_vida; }
    int get_nivel_dano() const { return nivel_dano; }
    int get_quantidade_vitorias() const { return static_cast<int>(professores_derrotados.size()); }
    String get_item_especial_equipado() const { return item_especial_equipado; }
    void equipar_item_especial(const String &id) { equipar_item_especial_desbloqueado(id); }
    Array get_todos_itens() const;
    bool ja_venceu_professor(int area, int indice) const;
    Array registrar_vitoria_professor(int area, int indice);
    bool equipar_item_especial_desbloqueado(const String &id);
>>>>>>> Stashed changes

    /** @brief Salva pontos e itens desbloqueados num arquivo de texto. */
    void salvar(const std::string& caminho) const;
    /**
     * @brief Carrega o progresso salvo de um arquivo de texto.
     * @param caminho Caminho do arquivo salvo previamente.
     * @return Um ProgressoJogador vazio (0 pontos) se o arquivo não existir ainda.
     */
    static ProgressoJogador carregar(const std::string& caminho);
};