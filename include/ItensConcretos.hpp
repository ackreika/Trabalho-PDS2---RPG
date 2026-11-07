#pragma once
#include <string>
#include "ItemPassivo.hpp"
#include "ItemEspecial.hpp"
#include "ItensIds.hpp"

/**
 * @brief Monta o ItemPassivo correspondente ao ID informado.
 * @param id Identificador do item.
 * @return Struct preenchido com nome/tipo/multiplicador desse item.
 */
ItemPassivo criarItemPassivo(ItemId id);

/**
 * @brief Monta o ItemEspecial correspondente ao ID informado.
 * @param id Identificador do item.
 * @return Struct preenchido com nome/multiplicador/usos desse item.
 */
ItemEspecial criarItemEspecial(ItemId id);

/** @brief true se o ID é um item especial (4º slot); false se for passivo. */
bool ehItemEspecial(ItemId id);

/**
 * @brief Converte o nome textual de um item ("HALTER", "LASER"...) no ItemId correspondente.
 *
 * Os nomes são os mesmos usados pelo GDScript (Itens.gd) e pelo arquivo de
 * progresso salvo em disco, então esta é a ponte única entre texto e ItemId.
 * @param texto Nome do item, em maiúsculas (a comparação diferencia maiúsculas de minúsculas).
 * @param saida Recebe o ItemId encontrado; não é alterado se o texto for desconhecido.
 * @return true se o texto corresponde a um item; false caso contrário.
 */
bool itemIdDeTexto(const std::string& texto, ItemId& saida);

/**
 * @brief Converte um ItemId no nome textual correspondente ("HALTER", "LASER"...).
 * @return O nome do item, ou "" se o ID não existir.
 */
std::string itemIdParaTexto(ItemId id);

/** @brief true se o ID é de um item passivo que existe; false se for especial ou desconhecido. */
bool ehItemPassivo(ItemId id);
