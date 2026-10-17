#include "GerenciadorRecompensas.hpp"
#include "ItensConcretos.hpp"

bool GerenciadorRecompensas::gerarRecompensa(int idArea, TipoProfessor tipoDerrotado, ItemPassivo& resultadoSaida) {
    if (tipoDerrotado == TipoProfessor::COMUM) {
        return false; // Comum não dropa nada, só Miniboss e Boss
    }

    // Drop temático por área — exemplo pra área 0 (Cálculo).
    // Ao adicionarem Física/GAAL/PDS, sigam o mesmo padrão aqui.
    if (idArea == 0) {
        if (tipoDerrotado == TipoProfessor::MINIBOSS) {
            resultadoSaida = criarItemPassivo(ItemId::HALTER);
            return true;
        }
        if (tipoDerrotado == TipoProfessor::BOSS) {
            resultadoSaida = criarItemPassivo(ItemId::SUCO);
            return true;
        }
    }

    return false; // área sem drop definido ainda
}