#pragma once
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