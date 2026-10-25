#include "GerenciadorRecompensas.hpp"

namespace {
/** Um professor que dá recompensa. Professores fora da tabela não dão nada. */
struct Recompensa {
    int idArea;
    int indiceBatalha;
    std::vector<ItemId> itens;
    bool dropaItemChave;
};

/**
 * Tabela de drops. Para mudar quem dá o quê, basta editar aqui.
 * Nomes dos professores: ver ProfessoresFabrica.cpp.
 */
const std::vector<Recompensa>& tabelaDeRecompensas() {
    static const std::vector<Recompensa> tabela = {
        // Cálculo
        {0, 1, {ItemId::JAMES_STEWART},              false}, // Substituto
        {0, 2, {ItemId::INTEGRAL, ItemId::DERIVADA}, false}, // Xavier
        {0, 3, {ItemId::HALTER},                     false}, // Colina Sombria
        // GAAL / Análise Numérica
        {1, 1, {ItemId::VETOR},                      false}, // Viktor Bekert
        {1, 2, {ItemId::INTERPOLACAO},               false}, // Alexandre
        // Física
        {2, 3, {ItemId::LASER},                      false}, // Ivonete
        {2, 4, {ItemId::REFRIGERADOR},               false}, // Slide Man
        // PDS
        {3, 1, {ItemId::PONTEIRO},                   true},  // Rezeck (entrega também o item chave)
    };
    return tabela;
}

const Recompensa* buscar(int idArea, int indiceBatalha) {
    for (const Recompensa& r : tabelaDeRecompensas()) {
        if (r.idArea == idArea && r.indiceBatalha == indiceBatalha) {
            return &r;
        }
    }
    return nullptr;
}
}

std::vector<ItemId> GerenciadorRecompensas::itensDoProfessor(int idArea, int indiceBatalha) {
    const Recompensa* r = buscar(idArea, indiceBatalha);
    return r ? r->itens : std::vector<ItemId>{};
}

bool GerenciadorRecompensas::dropaItemChave(int idArea, int indiceBatalha) {
    const Recompensa* r = buscar(idArea, indiceBatalha);
    return r != nullptr && r->dropaItemChave;
}

bool GerenciadorRecompensas::temRecompensa(int idArea, int indiceBatalha) {
    const Recompensa* r = buscar(idArea, indiceBatalha);
    return r != nullptr && (!r->itens.empty() || r->dropaItemChave);
}
