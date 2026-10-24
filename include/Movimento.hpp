#pragma once
#include <string>
#include <vector>

/**
 * @brief Identificadores dos movimentos disponíveis no catálogo para o estudante escolher.
 */
enum class MovimentoId {
    DERIVADA_RAPIDA = 0,
    INTEGRAL_POR_PARTES = 1,
    LIMITE_FUNDAMENTAL = 2,
    SERIE_DE_FOURIER = 3,
    ESCALONAMENTO_GAUSS = 4,
    AUTOVALOR_CRITICO = 5,
    CURTO_CIRCUITO_TRIFASICO = 6,
    LEI_DE_LENZ = 7,
    ALOCACAO_DINAMICA = 8,
    POLIMORFISMO_PURO = 9,
    BANDEJAO_RU = 10,
    CAFE_DUPLO_CANTINA = 11
};

/**
 * @brief Representa um ataque/habilidade do moveset de um personagem
 * (HabilidadeAcademica, no CRC), com suporte a dano, cura e controle de usos limitados.
 */
class Movimento {
private:
    std::string nome;
    std::string disciplina;
    int dano;
    int cura;
    int usosMaximos;   ///< negativo = ilimitado; positivo = quantidade de usos
    int usosRestantes;
    std::string descricao;

public:
    /**
     * @brief Construtor padrão de ataque (mantém compatibilidade com ProfessoresFabrica e testes).
     */
    Movimento(std::string nome, int dano, int usosMaximos = -1);

    /**
     * @brief Construtor completo para movimentos temáticos com disciplina, dano, cura e descrição.
     */
    Movimento(std::string nome, std::string disciplina, int dano, int cura, int usosMaximos, std::string descricao);

    /** @brief Retorna o nome do movimento. */
    std::string getNome() const;
    /** @brief Retorna a disciplina/área temática do movimento. */
    std::string getDisciplina() const;
    /** @brief Retorna o dano atual do movimento. */
    int getDano() const;
    /** @brief Retorna a cura proporcionada pelo movimento. */
    int getCura() const;
    /** @brief Retorna quantos usos ainda restam. */
    int getUsosRestantes() const;
    /** @brief Retorna o limite máximo de usos (-1 = ilimitado). */
    int getUsosMaximos() const;
    /** @brief Retorna a descrição temática do movimento. */
    std::string getDescricao() const;
    /** @brief Retorna um resumo formatado (Dano, Cura e Usos) para exibir nos menus C++. */
    std::string getResumoEfeito() const;

    /** @brief Retorna true se o movimento ainda puder ser usado. */
    bool podeUsar() const;
    /** @brief Consome um uso do movimento, se ele tiver usos limitados. */
    void usar();
    /** @brief Restaura os usos ao valor máximo. */
    void resetarUsos();
    /** @brief Aumenta o dano do movimento permanentemente (upgrade concedido ao acertar o quiz). */
    void aumentarPoder(int incremento);

    // ======================================================================
    // Catálogo estático de movimentos para o Menu de Movimentos em C++
    // ======================================================================

    /** @brief Retorna a lista completa de movimentos disponíveis para o estudante escolher. */
    static const std::vector<Movimento>& getCatalogoMovimentos();
    /** @brief Retorna a quantidade total de movimentos no catálogo. */
    static int getQuantidadeCatalogo();
    /** @brief Instancia e retorna um movimento do catálogo pelo seu índice/ID. */
    static Movimento criarMovimentoPorId(int id);
};