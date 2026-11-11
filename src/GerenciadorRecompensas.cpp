/**
 * @file GerenciadorRecompensas.cpp
 * @brief Implementação da tabela de drops e regras de recompensa de chefes.
 * 
 * Disciplina: Programação e Desenvolvimento de Software 2 (PDS 2)
 * Projeto: RPG Acadêmico em C++ e Godot Engine
 */

#include "GerenciadorRecompensas.hpp"

using namespace godot;

namespace {

/**
 * @struct Recompensa
 * @brief Estrutura interna que mapeia uma batalha aos itens e propriedades concedidas.
 */
struct Recompensa {
    int idArea;                     ///< Área à qual o professor pertence.
    int indiceBatalha;              ///< Posição da batalha na área.
    std::vector<ItemId> itens;      ///< Lista de itens passivos ou especiais dropados.
    bool dropaItemChave;            ///< Flag indicando se entrega a chave mestra do curso.
};

/**
 * @brief Tabela estática de recompensas do jogo.
 * 
 * Centraliza todo o balanceamento de itens obtidos ao derrotar os chefes e minibosses.
 * Utiliza o padrão Meyer's Singleton (static local) para inicialização segura sob demanda.
 */
const std::vector<Recompensa>& getTabelaDeRecompensas() {
    static const std::vector<Recompensa> tabela = {
        // Área 0: Cálculo
        {0, 1, {ItemId::JAMES_STEWART},              false}, // Substituto de Cálculo
        {0, 2, {ItemId::INTEGRAL, ItemId::DERIVADA}, false}, // Xavier
        {0, 3, {ItemId::HALTER},                     false}, // Colina Sombria

        // Área 1: GAAL / Álgebra Linear
        {1, 1, {ItemId::VETOR},                      false}, // Viktor Bekert
        {1, 2, {ItemId::INTERPOLACAO},               false}, // Alexandre

        // Área 2: Física
        {2, 3, {ItemId::LASER},                      false}, // Ivonete
        {2, 4, {ItemId::REFRIGERADOR},               false}, // Slide Man

        // Área 3: Programação e Desenvolvimento de Software (PDS 2)
        {3, 1, {ItemId::PONTEIRO},                   true},  // Rezeck (entrega o item chave da formatura)
    };
    return tabela;
}

/**
 * @brief Realiza busca linear na tabela estática por área e índice.
 * @return Ponteiro constante para a Recompensa encontrada, ou nullptr caso não cadastrada.
 */
const Recompensa* buscarRecompensa(int idArea, int indiceBatalha) {
    for (const Recompensa &r : getTabelaDeRecompensas()) {
        if (r.idArea == idArea && r.indiceBatalha == indiceBatalha) {
            return &r;
        }
    }
    return nullptr;
}

} // namespace anônimo

// =============================================================================
// Métodos Públicos do GerenciadorRecompensas
// =============================================================================

std::vector<ItemId> GerenciadorRecompensas::itensDoProfessor(int idArea, int indiceBatalha) {
    const Recompensa *r = buscarRecompensa(idArea, indiceBatalha);
    return r ? r->itens : std::vector<ItemId>{};
}

bool GerenciadorRecompensas::dropaItemChave(int idArea, int indiceBatalha) {
    const Recompensa *r = buscarRecompensa(idArea, indiceBatalha);
    return (r != nullptr) && r->dropaItemChave;
}

bool GerenciadorRecompensas::temRecompensa(int idArea, int indiceBatalha) {
    const Recompensa *r = buscarRecompensa(idArea, indiceBatalha);
    return (r != nullptr) && (!r->itens.empty() || r->dropaItemChave);
}
