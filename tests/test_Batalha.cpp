#include "doctest.h"
#include "Batalha.hpp"
#include "Herois.hpp"
#include "Professor.hpp"
#include "ProfessoresFabrica.hpp"
#include "GerenciadorSemestre.hpp"
#include "ModificadorStatus.hpp"
#include "EstadoMenu.hpp"
#include "Movimento.hpp"

TEST_CASE("Testando a Lógica da Batalha") {
    SUBCASE("Fluxo de turno normal e derrota do professor") {
        Herois heroi("Aluno", 100, 10, 20);
        heroi.aprenderMovimento(Movimento("Prova", 50, -1));

        Professor prof("Carrasco", "Cálculo", 30, 0, TipoProfessor::COMUM, false);
        prof.adicionarMovimento(Movimento("Questão Difícil", 10, -1));

        Batalha batalha(&heroi, &prof);
        CHECK(batalha.getEstado() == EstadoBatalha::ESCOLHENDO_ACAO);
        CHECK(batalha.get_heroi() == &heroi);
        CHECK(batalha.get_professor() == &prof);

        // O herói ataca com 50 de dano em um professor com 30 de vida e 0 de defesa -> Vitória!
        ResultadoTurno res = batalha.executarAcao(0);
        CHECK(res.acaoValida == true);
        CHECK(res.heroiAtacou == true);
        CHECK(res.danoCausadoPeloHeroi == 50);
        CHECK(!prof.estaVivo());
        CHECK(res.estadoResultante == EstadoBatalha::VITORIA);
        CHECK(batalha.getEstado() == EstadoBatalha::VITORIA);
    }

    SUBCASE("Tentativa de usar golpe inválido ou sem usos restantes") {
        Herois heroi("Aluno", 100, 10, 20);
        heroi.aprenderMovimento(Movimento("Cola", 10, 1)); // Só 1 uso

        Professor prof("Vigilante", "Física", 100, 10, TipoProfessor::COMUM, false);
        prof.adicionarMovimento(Movimento("Olhar Severo", 5, -1));

        Batalha batalha(&heroi, &prof);

        // Primeiro uso: válido
        ResultadoTurno res1 = batalha.executarAcao(0);
        CHECK(res1.acaoValida == true);

        // Segundo uso do mesmo golpe: inválido (acabaram os usos)
        ResultadoTurno res2 = batalha.executarAcao(0);
        CHECK(res2.acaoValida == false);
        CHECK(res2.heroiAtacou == false);

        // Índice fora dos limites (ex: golpe 99)
        ResultadoTurno resInvalido = batalha.executarAcao(99);
        CHECK(resInvalido.acaoValida == false);
    }

    SUBCASE("Combate contra Boss invulnerável, item especial e batalha falsa") {
        Herois heroi("Aluno", 100, 10, 20);
        heroi.aprenderMovimento(Movimento("Revisão", 40, -1));
        heroi.definirItemEspecial(ItemEspecial{"Laser", 2.0f, 1, 1});

        Professor boss("Chefe Final", "PDS II", 80, 0, TipoProfessor::BOSS, false);
        boss.adicionarMovimento(Movimento("Ponteiros Selvagens", 10, -1));

        // Boss começa invulnerável
        CHECK(boss.isInvulneravel() == true);
        boss.receberDano(40);
        CHECK(boss.getVida() == 80);

        // Após remover invulnerabilidade, passa a receber dano normalmente
        boss.removerInvulnerabilidade();
        CHECK(boss.isInvulneravel() == false);
        boss.receberDano(40);
        CHECK(boss.getVida() == 40);

        // Usando item especial (índice 3) na batalha
        Batalha batalhaBoss(&heroi, &boss);
        ResultadoTurno resEspecial = batalhaBoss.executarAcao(3);
        CHECK(resEspecial.acaoValida == true);
        CHECK(resEspecial.heroiAtacou == true);
        CHECK(!boss.estaVivo());
        CHECK(resEspecial.estadoResultante == EstadoBatalha::VITORIA);

        // Batalha falsa: dura 2 turnos sem causar dano real ao herói e encerra em vitória
        Professor profFalso("Veterano", "Introdução", 100, 10, TipoProfessor::COMUM, true);
        profFalso.adicionarMovimento(Movimento("Susto", 50, -1));
        Batalha batalhaFalsa(&heroi, &profFalso);

        int vidaAntes = heroi.getVida();
        ResultadoTurno t1 = batalhaFalsa.executarAcao(0);
        CHECK(heroi.getVida() == vidaAntes);
        CHECK(t1.estadoResultante == EstadoBatalha::ESCOLHENDO_ACAO);

        ResultadoTurno t2 = batalhaFalsa.executarAcao(0);
        CHECK(heroi.getVida() == vidaAntes);
        CHECK(t2.estadoResultante == EstadoBatalha::VITORIA);
    }

    SUBCASE("ModificadorStatus, ProfessoresFabrica, GerenciadorSemestre e EstadoMenu") {
        ModificadorStatus modFortalecido(TipoModificador::FORTALECIDO, 2);
        CHECK(modFortalecido.getTipo() == TipoModificador::FORTALECIDO);
        CHECK(modFortalecido.estaAtivo() == true);
        CHECK(modFortalecido.interceptarDano(100) == 120);
        modFortalecido.passarTurno();
        modFortalecido.passarTurno();
        CHECK(modFortalecido.deveSerRemovido() == true);
        CHECK(modFortalecido.estaAtivo() == false);

        // Fábrica de professores e áreas
        CHECK(ProfessoresFabrica::get_quantidade_areas() == 5);
        for (int area = 0; area < ProfessoresFabrica::get_quantidade_areas(); area++) {
            int qtd = ProfessoresFabrica::get_quantidade_batalhas(area);
            CHECK(qtd > 0);
            for (int indice = 1; indice <= qtd; indice++) {
                CHECK(ProfessoresFabrica::batalha_existe(area, indice) == true);
                Professor* p = ProfessoresFabrica::criar_professor(area, indice);
                CHECK(p != nullptr);
                CHECK(p->estaVivo() == true);
            }
            CHECK(ProfessoresFabrica::eh_ultima_da_area(area, qtd) == true);
        }
        CHECK(ProfessoresFabrica::batalha_existe(99, 1) == false);
        Professor* desc = ProfessoresFabrica::criar_professor(99, 1);
        CHECK(desc != nullptr);

        // Progressão do semestre
        GerenciadorSemestre semestre;
        semestre.iniciar(0);
        CHECK(semestre.getIdAreaAtual() == 0);
        CHECK(semestre.getIndiceBatalhaAtual() == 1);
        while (!semestre.venceuOJogo()) {
            semestre.avancarBatalha();
        }
        CHECK(semestre.venceuOJogo() == true);

        // Estado do Menu
        GerenciadorMenu menu;
        menu.set_estado(GerenciadorMenu::MENU_MOVIMENTOS);
        CHECK(menu.get_estado() == GerenciadorMenu::MENU_MOVIMENTOS);
    }
}
