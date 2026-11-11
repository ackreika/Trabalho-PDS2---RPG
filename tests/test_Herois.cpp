#include "doctest.h"
#include "Herois.hpp"
#include "MochilaItens.hpp"
#include "ProgressoJogador.hpp"
#include "Movimento.hpp"
#include "ItemPassivo.hpp"

TEST_CASE("Testando a classe Herois (Estudante)") {
    Herois heroi("Estudante", 100, 20, 15);

    SUBCASE("Atributos iniciais, Dano e Cura") {
        CHECK(heroi.getNome() == "Estudante");
        CHECK(heroi.getDefesa() == 20);
        CHECK(heroi.getDanoBase() == 15);
        CHECK(heroi.getVida() == 100);
        CHECK(heroi.getVidaMaxima() == 100);
        CHECK(heroi.estaVivo() == true);

        // Recebendo dano (leva em conta a fórmula de defesa interna)
        heroi.receberDano(30);
        CHECK(heroi.getVida() < 100);
        CHECK(heroi.getVida() > 0);

        int vidaAposDano = heroi.getVida();
        heroi.curar(10);
        CHECK(heroi.getVida() == vidaAposDano + 10);

        // Não pode curar além da vida máxima
        heroi.curar(500);
        CHECK(heroi.getVida() == 100);
    }

    SUBCASE("Gerenciamento de Movimentos (Máximo 3)") {
        heroi.aprenderMovimento(Movimento("Golpe 1", 10));
        heroi.aprenderMovimento(Movimento("Golpe 2", 20));
        heroi.aprenderMovimento(Movimento("Golpe 3", 30));

        CHECK(heroi.getMovimentos().size() == 3);

        // Tentando aprender um 4º movimento substituindo o índice 1 ("Golpe 2")
        heroi.aprenderMovimento(Movimento("Golpe Novo", 50), 1);
        CHECK(heroi.getMovimentos().size() == 3);
        CHECK(heroi.getMovimentos()[1].getNome() == "Golpe Novo");
        CHECK(heroi.getMovimentos()[1].getDano() == 50);
    }

    SUBCASE("Desbloqueio de movimentos por vitórias e loadout") {
        Herois::definirVitoriasJogador(0);
        CHECK(Herois::getVitoriasJogador() == 0);
        CHECK(Herois::movimentoEstaDesbloqueado(0) == true);
        CHECK(Herois::movimentoEstaDesbloqueado(3) == false);

        // Tentando equipar movimento bloqueado (ID 3) no slot 0 -> deve ser ignorado
        Herois::definirMovimentoEscolhido(0, 0);
        Herois::definirMovimentoEscolhido(0, 3);
        CHECK(Herois::getMovimentoEscolhido(0) == 0);

        // Após conquistar 1 vitória, o movimento ID 3 passa a poder ser equipado
        Herois::definirVitoriasJogador(1);
        CHECK(Herois::movimentoEstaDesbloqueado(3) == true);
        Herois::definirMovimentoEscolhido(0, 3);
        CHECK(Herois::getMovimentoEscolhido(0) == 3);
        CHECK(Herois::movimentoJaEquipadoEmOutroSlot(3, 1) == true);

        heroi.carregarMovimentosEscolhidos();
        CHECK(heroi.getMovimentos().size() == 3);
        CHECK(heroi.getMovimentos()[0].getNome() == "Série de Fourier");

        // Restaura estado padrão dos slots para não afetar outros testes
        Herois::definirMovimentoEscolhido(0, 0);
        Herois::definirMovimentoEscolhido(1, 1);
        Herois::definirMovimentoEscolhido(2, 2);
    }

    SUBCASE("Itens Passivos, MochilaItens e ProgressoJogador") {
        ItemPassivo itemDano{"Halter", TipoEfeito::BONUS_DANO_CAUSADO, 1.5f}; // +50% dano
        heroi.adicionarItemPassivo(itemDano);

        CHECK(heroi.calcularDanoFinal(20) == 30);
        CHECK(heroi.getQuantidadeItensPassivos() == 1);
        CHECK(heroi.getNomeItemPassivo(0) == "Halter");

        ItemPassivo itemVida{"Suco", TipoEfeito::BONUS_VIDA_MAXIMA, 1.2f}; // +20% vida máxima
        heroi.adicionarItemPassivo(itemVida);

        CHECK(heroi.getVidaMaxima() == 120);

        MochilaItens mochila(heroi);
        CHECK(mochila.getQuantidadeItensPassivos() == 2);
        CHECK(mochila.getNomeItemPassivo(1) == "Suco");
        CHECK(mochila.possuiItemEspecialEquipado() == false);
        mochila.equiparItemEspecial(ItemEspecial{"Vetor", 1.8f, 2, 2});
        CHECK(mochila.possuiItemEspecialEquipado() == true);
        CHECK(mochila.getNomeItemEspecialEquipado() == "Vetor");

        // Progresso permanente do jogador (pontos e upgrades)
        ProgressoJogador prog;
        CHECK(prog.get_pontos() == 0);
        prog.adicionar_pontos(50);
        CHECK(prog.get_pontos() == 50);
        CHECK(prog.comprar_vida() == true);
        CHECK(prog.get_nivel_vida() == 1);
        CHECK(prog.comprar_dano() == true);
        CHECK(prog.get_nivel_dano() == 1);
    }

    SUBCASE("Controle de Item Chave e Sprites") {
        CHECK(heroi.possuiItemDeChave() == false);
        heroi.obterItemChave();
        CHECK(heroi.possuiItemDeChave() == true);
        heroi.consumirItemChave();
        CHECK(heroi.possuiItemDeChave() == false);

        heroi.definirSprite(EstadoVisual::PARADO, "res://sprite_parado.png");
        CHECK(heroi.getCaminhoSprite(EstadoVisual::PARADO) == "res://sprite_parado.png");
        // Fallback para PARADO quando o estado consultado não tem sprite específico
        CHECK(heroi.getCaminhoSprite(EstadoVisual::VITORIA) == "res://sprite_parado.png");
    }
}
