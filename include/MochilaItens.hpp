#pragma once
#include <string>
#include "Herois.hpp"
#include "ItemEspecial.hpp"

/**
 * @brief Inventário do jogador (MochilaItens, no CRC).
 *
 * Não guarda os itens passivos diretamente — eles já ficam em
 * Herois::itensPassivos assim que coletados (aplicados automaticamente).
 * Esta classe é a "fachada" que o Godot consulta pra exibir isso na tela de
 * Inventário Completo, e é por onde o jogador escolhe/troca o item especial ativo.
 */
class MochilaItens {
private:
    Herois& heroi;

public:
    /** @brief Cria a mochila associada a um herói específico. */
    MochilaItens(Herois& heroi);

    /** @brief Retorna quantos itens passivos o herói possui. */
    int getQuantidadeItensPassivos() const;
    /** @brief Retorna o nome do item passivo no índice informado. */
    std::string getNomeItemPassivo(int indice) const;

    /** @brief Equipa/troca o item especial ativo do herói (4º slot de golpe). */
    void equiparItemEspecial(ItemEspecial item);
    /** @brief Retorna true se o herói tiver algum item especial equipado. */
    bool possuiItemEspecialEquipado() const;
    /** @brief Retorna o nome do item especial equipado, ou "" se nenhum. */
    std::string getNomeItemEspecialEquipado() const;
};