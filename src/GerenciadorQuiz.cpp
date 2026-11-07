#include "GerenciadorQuiz.hpp"
#include <cstdlib>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <algorithm>

GerenciadorQuiz::GerenciadorQuiz() {
    // Perguntas por area: 0 = Calculo, 1 = GAAL, 2 = Fisica, 3 = PDS.

    // --- Calculo (0) ---
    adicionarPergunta(0, Pergunta{"Qual eh a derivada de e^x?", {"e^x", "x*e^(x-1)", "ln(x)", "1"}, 0});
    adicionarPergunta(0, Pergunta{"Quanto vale o limite de sen(x)/x quando x tende ao infinito?", {"0", "1", "infinito", "nao existe"}, 0});
    adicionarPergunta(0, Pergunta{"Qual eh a derivada de cos(x)?", {"sen(x)", "cos(x)", "-sen(x)", "-cos(x)"}, 2});
    adicionarPergunta(0, Pergunta{"Qual eh a integral de 1/x?", {"x^2/2 + C", "-1/x^2 + C", "e^x + C", "ln|x| + C"}, 3});

    // --- GAAL (1) ---
    adicionarPergunta(1, Pergunta{"Qual a propriedade de uma matriz identidade?", {"Todos os elementos sao zero", "Multiplicar por ela preserva o vetor", "Ela sempre tem determinante zero", "Ela nao possui diagonal"}, 1});
    adicionarPergunta(1, Pergunta{"Qual eh a soma dos vetores (1, 2) e (3, 4)?", {"(3, 8)", "(4, 6)", "(2, 2)", "(4, 8)"}, 1});
    adicionarPergunta(1, Pergunta{"Uma matriz quadrada eh aquela que:", {"Tem so uma linha", "Tem so zeros", "Tem o mesmo numero de linhas e colunas", "Tem determinante igual a 1"}, 2});
    adicionarPergunta(1, Pergunta{"Qual eh o determinante de uma matriz que tem uma linha inteira de zeros?", {"1", "-1", "2", "0"}, 3});

    // --- Fisica (2) ---
    adicionarPergunta(2, Pergunta{"Qual eh a formula da energia cinetica?", {"m*v^2/2", "m*v", "m*v^2", "m*g*h"}, 0});
    adicionarPergunta(2, Pergunta{"Qual eh a unidade de forca no SI?", {"Joule", "Watt", "Newton", "Pascal"}, 2});
    adicionarPergunta(2, Pergunta{"Qual lei relaciona tensao, corrente e resistencia?", {"Lei de Ohm", "Lei de Gauss", "Lei de Hooke", "Lei de Snell"}, 0});
    adicionarPergunta(2, Pergunta{"O trabalho da forca resultante eh igual a variacao de qual grandeza?", {"energia potencial", "energia cinetica", "momento linear", "potencia"}, 1});

    // --- PDS (3) ---
    adicionarPergunta(3, Pergunta{"Qual palavra-chave eh usada para declarar uma classe em C++?", {"func", "object", "class", "define"}, 2});
    adicionarPergunta(3, Pergunta{"Qual operador libera memoria alocada com new?", {"free", "delete", "remove", "clear"}, 1});
    adicionarPergunta(3, Pergunta{"Qual tipo de dado guarda um numero inteiro em C++?", {"string", "bool", "char", "int"}, 3});
    adicionarPergunta(3, Pergunta{"Um membro declarado como public em uma classe pode ser acessado:", {"De fora da classe", "Somente dentro da classe", "Somente por classes filhas", "Nunca"}, 0});
}

void GerenciadorQuiz::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("sortear_pergunta", "id_area"), &GerenciadorQuiz::sortear_pergunta);
    godot::ClassDB::bind_method(godot::D_METHOD("validar_resposta", "pergunta", "indice"), &GerenciadorQuiz::validar_resposta);
    godot::ClassDB::bind_method(godot::D_METHOD("aplicar_resultado", "heroi", "indice_movimento", "acertou", "incremento_dano"), &GerenciadorQuiz::aplicar_resultado, DEFVAL(3));
}

godot::Dictionary GerenciadorQuiz::sortear_pergunta(int idArea) const {
    Pergunta sorteada;
    godot::Dictionary d;
    if (!sortearPergunta(idArea, sorteada)) return d;
    godot::Array alternativas;
    for (const std::string &opcao : sorteada.alternativas) alternativas.append(godot::String::utf8(opcao.c_str()));
    d["enunciado"] = godot::String::utf8(sorteada.enunciado.c_str());
    d["alternativas"] = alternativas;
    d["indice_correto"] = sorteada.indiceCorreto;
    return d;
}

bool GerenciadorQuiz::validar_resposta(const godot::Dictionary &pergunta, int indiceEscolhido) const {
    if (!pergunta.has("indice_correto") || !pergunta.has("alternativas") || indiceEscolhido < 0) return false;
    const godot::Variant alternativasVariant = pergunta["alternativas"];
    if (alternativasVariant.get_type() != godot::Variant::ARRAY) return false;
    const godot::Array alternativas = alternativasVariant;
    const godot::Variant indiceVariant = pergunta["indice_correto"];
    if (indiceVariant.get_type() != godot::Variant::INT) return false;
    const int64_t correto = indiceVariant;
    if (alternativas.is_empty() || correto < 0 || correto >= alternativas.size() || indiceEscolhido >= alternativas.size()) return false;
    return correto == indiceEscolhido;
}

void GerenciadorQuiz::aplicar_resultado(Herois *heroi, int indiceMovimento, bool acertou, int incrementoDano) const {
    if (!acertou) return;
    if (!heroi || indiceMovimento < 0 || indiceMovimento >= heroi->get_quantidade_movimentos() || incrementoDano <= 0) {
        godot::UtilityFunctions::push_warning("Bônus de quiz ignorado: herói, movimento ou incremento inválido.");
        return;
    }
    heroi->aumentar_poder_movimento(indiceMovimento, incrementoDano);
}

void GerenciadorQuiz::adicionarPergunta(int idArea, Pergunta pergunta) {
    if (idArea < 0 || idArea >= 4 || pergunta.enunciado.empty() || pergunta.alternativas.size() != 4 ||
        pergunta.indiceCorreto < 0 || pergunta.indiceCorreto >= static_cast<int>(pergunta.alternativas.size()) ||
        std::any_of(pergunta.alternativas.begin(), pergunta.alternativas.end(), [](const std::string &opcao) { return opcao.empty(); })) {
        godot::UtilityFunctions::push_warning("Pergunta de quiz inválida ignorada.");
        return;
    }
    bancoPerguntas[idArea].push_back(pergunta);
}

bool GerenciadorQuiz::sortearPergunta(int idArea, Pergunta& saida) const {
    auto it = bancoPerguntas.find(idArea);
    if (it == bancoPerguntas.end() || it->second.empty()) {
        return false; // nenhuma pergunta cadastrada pra essa área
    }
    const auto& perguntas = it->second;
    const size_t indice = static_cast<size_t>(godot::UtilityFunctions::randi_range(0, static_cast<int64_t>(perguntas.size()) - 1));
    saida = perguntas[indice];
    return true;
}

bool GerenciadorQuiz::validarResposta(const Pergunta& pergunta, int indiceEscolhido) const {
    return pergunta.alternativas.size() == 4 && pergunta.indiceCorreto >= 0 &&
        pergunta.indiceCorreto < static_cast<int>(pergunta.alternativas.size()) &&
        indiceEscolhido >= 0 && indiceEscolhido < static_cast<int>(pergunta.alternativas.size()) &&
        indiceEscolhido == pergunta.indiceCorreto;
}

void GerenciadorQuiz::aplicarResultado(Movimento* movimento, bool acertou, int incrementoDano) const {
    if (acertou && movimento != nullptr && incrementoDano > 0) {
        movimento->aumentarPoder(incrementoDano);
    }
    // errar não concede bônus — não faz nada
}
