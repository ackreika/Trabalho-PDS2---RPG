#pragma once
#include <string>

/**
 * @brief Representa um ataque/habilidade do moveset de um personagem
 * (HabilidadeAcademica, no CRC), com controle de usos limitados.
 */
class Movimento {
private:
    std::string nome;
    int dano;
    int usosMaximos;   ///< negativo = ilimitado; positivo = quantidade de usos
    int usosRestantes;

public:
    /**
     * @brief Cria um novo movimento.
     * @param nome Nome exibido do movimento.
     * @param dano Dano base causado ao usar.
     * @param usosMaximos Quantidade de usos permitidos; -1 (padrão) = ilimitado.
     */
    Movimento(std::string nome, int dano, int usosMaximos = -1);

    /** @brief Retorna o nome do movimento. */
    std::string getNome() const;
    /** @brief Retorna o dano atual do movimento. */
    int getDano() const;
    /** @brief Retorna quantos usos ainda restam. */
    int getUsosRestantes() const;
    /** @brief Retorna o limite máximo de usos (-1 = ilimitado). */
    int getUsosMaximos() const;

    /** @brief Retorna true se o movimento ainda puder ser usado. */
    bool podeUsar() const;
    /** @brief Consome um uso do movimento, se ele tiver usos limitados. */
    void usar();
    /** @brief Restaura os usos ao valor máximo. */
    void resetarUsos();
    /** @brief Aumenta o dano do movimento permanentemente (upgrade concedido ao acertar o quiz). */
    void aumentarPoder(int incremento);
};