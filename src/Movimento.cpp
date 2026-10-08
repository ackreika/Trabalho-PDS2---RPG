#include "Movimento.hpp"

Movimento::Movimento(std::string nome, int dano, int usosMaximos)
    : nome(nome), dano(dano), usosMaximos(usosMaximos), usosRestantes(usosMaximos) {}
    // Implementação dos métodos da classe Movimento

std::string Movimento::getNome() const { return nome; }             //retorna o nome da habilidade
int Movimento::getDano() const { return dano; }                     //retorna o dano da habilidade
int Movimento::getUsosRestantes() const { return usosRestantes; }   //retorna a quantidade de usos restantes da habilidade
int Movimento::getUsosMaximos() const { return usosMaximos; }       //retorna a quantidade máxima de usos da habilidade

bool Movimento::podeUsar() const {                                  //retorna true se a habilidade pode ser usada
    return usosMaximos < 0 || usosRestantes > 0;                    //retorna false caso contrário
}

void Movimento::usar() {                                            //decrementa a quantidade de usos restantes da habilidade
    if (usosMaximos >= 0 && usosRestantes > 0) {                    //caso ela tenha usos limitados
        usosRestantes--;
    }
}

void Movimento::resetarUsos() {              //reseta a quantidade de usos restantes da habilidade para o valor máximo de usos
    usosRestantes = usosMaximos;             //caso a habilidade tenha usos limitados
}

void Movimento::aumentarPoder(int incremento) {
    dano += incremento;
}