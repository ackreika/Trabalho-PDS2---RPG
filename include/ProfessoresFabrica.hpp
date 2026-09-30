#pragma once
#include "Professor.hpp"

/**
 * @brief Ponto único onde todos os professores/matérias do jogo são definidos.
 */
class ProfessoresFabrica {
public:
    /**
     * @brief Cria o Professor correspondente à área e batalha informadas.
     * @param idArea 0=Cálculo, 1=Física, 2=GAAL, 3=PDS
     * @param indiceBatalha 1 e 2 = comuns, 3 = miniboss, 4 = boss da área
     * @return O Professor pronto (com moveset e sprites definidos).
     */
    static Professor criarProfessor(int idArea, int indiceBatalha);
};