#include "ProgressoJogador.hpp"
#include <fstream>
#include <sstream>

ProgressoJogador::ProgressoJogador() : pontos(0) {}
// Implementação dos métodos da classe ProgressoJogador

int ProgressoJogador::getPontos() const { return pontos; }
//retorna a quantidade de pontos de progresso do jogador

void ProgressoJogador::adicionarPontos(int quantidade) {
    pontos += quantidade;
}//adiciona a quantidade de pontos de progresso do jogador

bool ProgressoJogador::gastarPontos(int quantidade) {
    if (pontos < quantidade) return false;
    pontos -= quantidade;
    return true;
}// retorna false se não tiver pontos suficientes

bool ProgressoJogador::jaObteve(ItemId id) const {
    return itensDesbloqueados.count(static_cast<int>(id)) > 0;
}//retorna true se o jogador já desbloqueou o item com o ID especificado, false caso contrário

void ProgressoJogador::desbloquear(ItemId id) {
    itensDesbloqueados.insert(static_cast<int>(id));
}//adiciona o ID do item ao conjunto de itens desbloqueados do jogador

const std::set<int>& ProgressoJogador::getTodos() const {
    return itensDesbloqueados;
}//retorna o conjunto de IDs de itens que o jogador já desbloqueou

void ProgressoJogador::salvar(const std::string& caminho) const {
    std::ofstream arquivo(caminho);
    arquivo << pontos << "\n";

    bool primeiro = true;
    for (int id : itensDesbloqueados) {
        if (!primeiro) arquivo << ",";
        arquivo << id;
        primeiro = false;
    }
    arquivo << "\n";
}//salva o progresso do jogador em um arquivo no caminho especificado

ProgressoJogador ProgressoJogador::carregar(const std::string& caminho) {
    ProgressoJogador progresso;
    std::ifstream arquivo(caminho);

    if (!arquivo.is_open()) {
        return progresso; // primeira vez jogando, nada salvo ainda
    }

    arquivo >> progresso.pontos;
    arquivo.ignore(); // pula a quebra de linha

    std::string linhaItens;
    std::getline(arquivo, linhaItens);

    std::stringstream ss(linhaItens);
    std::string idStr;
    while (std::getline(ss, idStr, ',')) {
        if (!idStr.empty()) {
            progresso.desbloquear(static_cast<ItemId>(std::stoi(idStr)));
        }
    }

    return progresso;
}//carrega o progresso do jogador a partir de um arquivo no caminho especificado
 //e retorna um objeto ProgressoJogador
