#include "GerenciadorQuiz.hpp"
#include <cstdlib>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <algorithm>

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
    adicionarPergunta(1, Pergunta{"Qual lei relaciona tensao, corrente e resistencia?", {"Lei de Ohm", "Lei de Gauss", "Lei de Hooke", "Lei de Snell"}, 0});
    adicionarPergunta(2, Pergunta{"Qual propriedade de uma matriz identidade?", {"Todos os elementos sao zero", "Multiplicar por ela preserva o vetor", "Ela sempre tem determinante zero", "Ela nao possui diagonal"}, 1});
    adicionarPergunta(3, Pergunta{"Em C++, para que serve um ponteiro?", {"Guardar uma referencia de memoria", "Ordenar um vetor automaticamente", "Criar uma classe abstrata", "Imprimir uma string"}, 0});
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
