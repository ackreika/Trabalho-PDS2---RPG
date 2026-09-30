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

    /** @brief Retorna true se o item com esse ID já foi desbloqueado. */
    bool jaObteve(ItemId id) const;
    /** @brief Marca um item como desbloqueado. */
    void desbloquear(ItemId id);
    /** @brief Retorna o conjunto de IDs de todos os itens desbloqueados. */
    const std::set<int>& getTodos() const;

    /** @brief Salva pontos e itens desbloqueados num arquivo de texto. */
    void salvar(const std::string& caminho) const;
    /**
     * @brief Carrega o progresso salvo de um arquivo de texto.
     * @param caminho Caminho do arquivo salvo previamente.
     * @return Um ProgressoJogador vazio (0 pontos) se o arquivo não existir ainda.
     */
    static ProgressoJogador carregar(const std::string& caminho);
};