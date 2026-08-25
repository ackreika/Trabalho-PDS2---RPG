#include <iostream>
#define MAX_ATAQUES_DISTINTOS_INIMIGOS 10 // Número máximo de ataques distintos que um inimigo poderá escolher entre
#define NUM_INIMIGOS_DISTINTOS 30 // Usado como índice para diferenciar inimigos de tipos diferentes
#define NUM_ATAQUES_DISTINTOS_HEROI 8  // Skillset completo do player 'heroi', incluindo skills passivas


// Espaço para incluir funções

struct inimigos
{
    int vida_inimigo;
    int defesa_inimigo;
    int dano_fisico_inimigo;
    int dano_magico_inimigo;
    int lista_ataques_inimigo[NUM_INIMIGOS_DISTINTOS][MAX_ATAQUES_DISTINTOS_INIMIGOS];
    bool vivo_inimigo;
};
struct heroi
{
    int vida_heroi;
    int defesa_heroi;
    int dano_fisico_heroi;
    int dano_magico_heroi;
    int lista_ataques_heroi[NUM_ATAQUES_DISTINTOS_HEROI];
    bool vivo_heroi;
};


int main(int argc, char const *argv[])
{


    return 0;
}

