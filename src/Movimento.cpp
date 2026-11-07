#include "Movimento.hpp"
#include <algorithm>
#include <limits>

Movimento::Movimento(std::string nome, int dano, int usosMaximos)
    : nome(nome.empty() ? "Movimento" : nome), disciplina("Geral"), dano(std::max(0, dano)), cura(0),
      usosMaximos(usosMaximos), usosRestantes(usosMaximos), descricao(""), vitoriasNecessarias(0) {}

Movimento::Movimento(std::string nome, std::string disciplina, int dano, int cura, int usosMaximos,
                     std::string descricao, int vitoriasNecessarias)
    : nome(nome.empty() ? "Movimento" : nome), disciplina(disciplina.empty() ? "Geral" : disciplina),
      dano(std::max(0, dano)), cura(std::max(0, cura)), usosMaximos(usosMaximos),
      usosRestantes(usosMaximos), descricao(descricao), vitoriasNecessarias(std::max(0, vitoriasNecessarias)) {}

std::string Movimento::getNome() const { return nome; }
std::string Movimento::getDisciplina() const { return disciplina; }
int Movimento::getDano() const { return dano; }
int Movimento::getCura() const { return cura; }
int Movimento::getUsosRestantes() const { return usosRestantes; }
int Movimento::getUsosMaximos() const { return usosMaximos; }
std::string Movimento::getDescricao() const { return descricao; }
int Movimento::getVitoriasNecessarias() const { return vitoriasNecessarias; }

bool Movimento::estaDesbloqueado(int vitoriasJogador) const {
    return vitoriasJogador >= vitoriasNecessarias;
}

std::string Movimento::getResumoEfeito() const {
    std::string resumo = "";
    if (dano > 0) {
        resumo += "Dano: " + std::to_string(dano);
    }
    if (cura > 0) {
        if (!resumo.empty()) resumo += " | ";
        resumo += "Cura: +" + std::to_string(cura) + " HP";
    }
    if (!resumo.empty()) resumo += " | ";
    if (usosMaximos < 0) {
        resumo += "Usos: Ilimitado";
    } else {
        resumo += "Usos: " + std::to_string(usosMaximos);
    }
    return resumo;
}

bool Movimento::podeUsar() const {
    return usosMaximos < 0 || usosRestantes > 0;
}

void Movimento::usar() {
    if (usosMaximos >= 0 && usosRestantes > 0) {
        usosRestantes--;
    }
}

void Movimento::resetarUsos() {
    usosRestantes = usosMaximos;
}

void Movimento::aumentarPoder(int incremento) {
    if (incremento <= 0) return;
    const long long danoNovo = static_cast<long long>(dano) + incremento;
    dano = static_cast<int>(std::min<long long>(danoNovo, std::numeric_limits<int>::max()));
}

const std::vector<Movimento>& Movimento::getCatalogoMovimentos() {
    static const std::vector<Movimento> catalogo = {
        // 0 - Cálculo (Inicial: 0 vitórias)
        Movimento("Derivada Rápida", "Cálculo", 15, 0, -1,
                  "Taxa de variação instantânea na paciência do professor.", 0),
        // 1 - Cálculo (Inicial: 0 vitórias)
        Movimento("Integral por Partes", "Cálculo", 28, 0, 3,
                  "Integrando as partes de defesa do professor.", 0),
        // 2 - Cálculo (Inicial: 0 vitórias)
        Movimento("Limite Fundamental", "Cálculo", 12, 5, -1,
                  "Tende ao infinito causando dano leve e recuperando o fôlego.", 0),
        // 3 - Cálculo / Elétrica (Requer 1 vitória)
        Movimento("Série de Fourier", "Cálculo", 35, 0, 2,
                  "Decompõe a questão inteira em senos e cossenos devastadores.", 1),
        // 4 - GAAL (Requer 2 vitórias)
        Movimento("Escalonamento de Gauss", "GAAL", 20, 0, 5,
                  "Pivoteia a matriz e zera as linhas de defesa do professor.", 2),
        // 5 - GAAL / Numérica (Requer 3 vitórias)
        Movimento("Autovalor Crítico", "GAAL", 32, 0, 2,
                  "Transformação linear que diagonaliza a barra de vida inimiga.", 3),
        // 6 - Física / Engenharia Elétrica (Requer 4 vitórias)
        Movimento("Curto-Circuito Trifásico", "Física / Elétrica", 38, 0, 2,
                  "Sobrecarga direta no disjuntor do ICEx! Alto impacto elétrico.", 4),
        // 7 - Física / Eletromagnetismo (Requer 5 vitórias)
        Movimento("Lei de Lenz", "Física / Elétrica", 16, 12, 4,
                  "Força contra-eletromotriz que se opõe à reprovação: ataca e cura.", 5),
        // 8 - PDS II (Requer 6 vitórias)
        Movimento("Alocação Dinâmica (new)", "PDS II", 30, 0, 3,
                  "Aloca ponteiros na cabeça do professor e trava a correção.", 6),
        // 9 - PDS II (Requer 7 vitórias)
        Movimento("Polimorfismo Puro", "PDS II", 22, 0, 4,
                  "Sobrescreve o método da prova em tempo de execução.", 7),
        // 10 - Suporte / Vivência UFMG (Requer 2 vitórias)
        Movimento("Bandejão Completo do RU", "Vivência UFMG", 0, 35, 2,
                  "Arroz, feijão, proteína e refresco do RU: recupera 35 de HP.", 2),
        // 11 - Suporte / Vivência UFMG (Requer 4 vitórias)
        Movimento("Café Duplo da Lanchonete", "Vivência UFMG", 10, 25, 3,
                  "Cafeína pura entre aulas: recupera 25 de HP e garante 10 de dano.", 4)
    };
    return catalogo;
}

int Movimento::getQuantidadeCatalogo() {
    return static_cast<int>(getCatalogoMovimentos().size());
}

Movimento Movimento::criarMovimentoPorId(int id) {
    const auto& catalogo = getCatalogoMovimentos();
    if (id < 0 || id >= static_cast<int>(catalogo.size())) {
        return catalogo[0];
    }
    return catalogo[id];
}
