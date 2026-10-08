#include "GerenciadorQuiz.hpp"
#include <cstdlib>

GerenciadorQuiz::GerenciadorQuiz() {
    // Perguntas de exemplo pra área 0 (Cálculo). As demais áreas seguem o mesmo
    // padrão de adicionarPergunta — só faltam ser povoadas.
    adicionarPergunta(0, Pergunta{
        "Qual e a derivada de x^2?",
        {"x", "2x", "x^2", "2"},
        1 // "2x"
    });
    adicionarPergunta(0, Pergunta{
        "A integral de uma constante c em relacao a x e:",
        {"c", "c*x + C", "0", "x"},
        1 // "c*x + C"
    });
}

void GerenciadorQuiz::adicionarPergunta(int idArea, Pergunta pergunta) {
    bancoPerguntas[idArea].push_back(pergunta);
}

bool GerenciadorQuiz::sortearPergunta(int idArea, Pergunta& saida) const {
    auto it = bancoPerguntas.find(idArea);
    if (it == bancoPerguntas.end() || it->second.empty()) {
        return false; // nenhuma pergunta cadastrada pra essa área
    }
    const auto& perguntas = it->second;
    int indice = std::rand() % perguntas.size();
    saida = perguntas[indice];
    return true;
}

bool GerenciadorQuiz::validarResposta(const Pergunta& pergunta, int indiceEscolhido) const {
    return indiceEscolhido == pergunta.indiceCorreto;
}

void GerenciadorQuiz::aplicarResultado(Movimento* movimento, bool acertou, int incrementoDano) const {
    if (acertou && movimento != nullptr) {
        movimento->aumentarPoder(incrementoDano);
    }
    // errar não concede bônus — não faz nada
}