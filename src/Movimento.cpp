#include "Movimento.hpp"

Movimento::Movimento(std::string nome, int dano, int usosMaximos)
    : nome(nome), disciplina("Geral"), dano(dano), cura(0),
      usosMaximos(usosMaximos), usosRestantes(usosMaximos), descricao("") {}

Movimento::Movimento(std::string nome, std::string disciplina, int dano, int cura, int usosMaximos, std::string descricao)
    : nome(nome), disciplina(disciplina), dano(dano), cura(cura),
      usosMaximos(usosMaximos), usosRestantes(usosMaximos), descricao(descricao) {}

std::string Movimento::getNome() const { return nome; }
std::string Movimento::getDisciplina() const { return disciplina; }
int Movimento::getDano() const { return dano; }
int Movimento::getCura() const { return cura; }
int Movimento::getUsosRestantes() const { return usosRestantes; }
int Movimento::getUsosMaximos() const { return usosMaximos; }
std::string Movimento::getDescricao() const { return descricao; }

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
    dano += incremento;
}

const std::vector<Movimento>& Movimento::getCatalogoMovimentos() {
    static const std::vector<Movimento> catalogo = {
        // 0 - Cálculo
        Movimento("Derivada Rapida", "Calculo", 15, 0, -1,
                  "Taxa de variacao instantanea na paciencia do professor."),
        // 1 - Cálculo
        Movimento("Integral por Partes", "Calculo", 28, 0, 3,
                  "Integrando as partes de defesa do professor."),
        // 2 - Cálculo
        Movimento("Limite Fundamental", "Calculo", 12, 5, -1,
                  "Tende ao infinito causando dano leve e recuperando o folego."),
        // 3 - Cálculo / Elétrica
        Movimento("Serie de Fourier", "Calculo", 35, 0, 2,
                  "Decompoe a questao inteira em senos e cossenos devastadores."),
        // 4 - GAAL
        Movimento("Escalonamento de Gauss", "GAAL", 20, 0, 5,
                  "Pivoteia a matriz e zera as linhas de defesa do professor."),
        // 5 - GAAL / Numérica
        Movimento("Autovalor Critico", "GAAL", 32, 0, 2,
                  "Transformacao linear que diagonaliza a barra de vida inimiga."),
        // 6 - Física / Engenharia Elétrica
        Movimento("Curto-Circuito Trifasico", "Fisica / Eletrica", 38, 0, 2,
                  "Sobrecarga direta no disjuntor do ICEx! Alto impacto eletrico."),
        // 7 - Física / Eletromagnetismo
        Movimento("Lei de Lenz", "Fisica / Eletrica", 16, 12, 4,
                  "Forca contra-eletromotriz que se opoe a reprovacao: ataca e cura."),
        // 8 - PDS II
        Movimento("Alocacao Dinamica (new)", "PDS II", 30, 0, 3,
                  "Aloca ponteiros na cabeca do professor e trava a correcao."),
        // 9 - PDS II
        Movimento("Polimorfismo Puro", "PDS II", 22, 0, 4,
                  "Sobrescreve o metodo da prova em tempo de execucao."),
        // 10 - Suporte / Vivência UFMG
        Movimento("Bandejao Completo do RU", "Vivencia UFMG", 0, 35, 2,
                  "Arroz, feijao, proteina e refresco do RU: recupera 35 de HP."),
        // 11 - Suporte / Vivência UFMG
        Movimento("Cafe Duplo da Lanchonete", "Vivencia UFMG", 10, 25, 3,
                  "Cafeina pura entre aulas: recupera 25 de HP e garante 10 de dano.")
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