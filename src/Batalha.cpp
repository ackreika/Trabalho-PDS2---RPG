#include "Batalha.hpp"

Batalha::Batalha(Herois& heroi, Professor professor)
    : heroi(heroi), professor(professor), estado(EstadoBatalha::ESCOLHENDO_ACAO), turnosDecorridos(0) {}
    // construtor que inicializa a batalha com o herói do jogador e o professor a ser enfrentado

EstadoBatalha Batalha::getEstado() const { return estado; }
// retorna o estado atual da batalha, usado pelo Godot pra saber que tela/animação mostrar
const Professor& Batalha::getProfessor() const { return professor; }
// retorna uma referência constante para o professor que o jogador está enfrentando na batalha

ResultadoTurno Batalha::executarAcao(int indiceGolpe) {
    ResultadoTurno resultado{}; // inicializa todos os campos com valores padrão
    resultado.acaoValida = true;    // inicializa como true, será ajustado se a ação for inválida
    resultado.heroiAtacou = false;  // inicializa como false, será ajustado se o herói atacar
    resultado.danoCausadoPeloHeroi = 0; // inicializa como 0, será ajustado se o herói causar dano
    resultado.professorAtacou = false;  // inicializa como false, será ajustado se o professor atacar
    resultado.danoCausadoPeloProfessor = 0; // inicializa como 0, será ajustado se o professor causar dano

    if (estado != EstadoBatalha::ESCOLHENDO_ACAO) {
        resultado.acaoValida = false;   // a batalha já terminou, não é possível executar ações
        resultado.mensagem = "A batalha terminou.";  // mensagem de erro para o Godot exibir
        resultado.estadoResultante = estado;  // mantém o estado atual da batalha, que já é VITORIA, DERROTA ou FUGIU
        return resultado;   // retorna o resultado do turno com a ação inválida
    }

    int danoBase = 0;

    if (indiceGolpe >= 0 && indiceGolpe < 3) {
        // Movimento normal escolhido pelo jogador
        Movimento* movimento = heroi.getMovimentoParaEditar(indiceGolpe); // acesso não-const, pra poder consumir o uso
        if (movimento == nullptr || !movimento->podeUsar()) {
            resultado.acaoValida = false;   // a ação não é válida se o índice do golpe for inválido ou se o movimento não puder ser usado
            resultado.mensagem = "Esse movimento nao pode ser usado agora.";    // mensagem de erro para o Godot exibir
            resultado.estadoResultante = estado;    // mantém o estado atual da batalha, que ainda é ESCOLHENDO_ACAO
            return resultado;   // retorna o resultado do turno com a ação inválida
        }
        danoBase = movimento->getDano();
        movimento->usar(); // consome o uso — antes disso nunca era chamado, então movimentos com usosMaximos nunca gastavam
    } else if (indiceGolpe == 3) {
        // 4º slot: item especial
        if (!heroi.itemEspecialPodeUsar()) {
            resultado.acaoValida = false;   // a ação não é válida se não houver item especial ou se ele não puder ser usado
            resultado.mensagem = "Nenhum item especial disponivel.";    // mensagem de erro para o Godot exibir
            resultado.estadoResultante = estado; // mantém o estado atual da batalha, que ainda é ESCOLHENDO_ACAO
            return resultado;   // retorna o resultado do turno com a ação inválida
        }
        std::string nomeItem = heroi.getNomeItemEspecial();
        int danoItem = heroi.usarItemEspecial(professor);    // o item especial é usado, causando dano no professor
        resultado.heroiAtacou = true;   // o herói atacou usando o item especial
        resultado.danoCausadoPeloHeroi = danoItem;  // dano causado pelo herói no professor usando o item especial
        resultado.mensagem = heroi.getNome() + " usou " + nomeItem + "!";    // mensagem de ação para o Godot exibir

        if (!professor.estaVivo()) {
            estado = EstadoBatalha::VITORIA;    // o professor foi derrotado pelo item especial
            resultado.estadoResultante = estado;    // atualiza o estado da batalha para VITORIA
            return resultado;   // retorna o resultado do turno com a vitória do herói
        }

        return processarTurnoProfessor();   // processa a ação do professor e retorna o resultado do turno, já que o herói usou o item especial
    }

    // dano final do movimento normal, passando pelos itens passivos do herói
    int danoFinal = heroi.calcularDanoFinal(danoBase);
    professor.receberDano(danoFinal);   // aplica o dano final no professor, considerando a defesa dele

    resultado.heroiAtacou = true;   // o herói atacou usando o movimento normal
    resultado.danoCausadoPeloHeroi = danoFinal; // dano causado pelo herói no professor usando o movimento normal
    resultado.mensagem = heroi.getNome() + " atacou causando " + std::to_string(danoFinal) + " de dano!";

    if (!professor.estaVivo()) {
        estado = EstadoBatalha::VITORIA;    // o professor foi derrotado pelo movimento normal
        resultado.estadoResultante = estado;    // atualiza o estado da batalha para VITORIA
        return resultado;   // retorna o resultado do turno com a vitória do herói
    }

    ResultadoTurno resultadoComProfessor = processarTurnoProfessor();
    // mescla o que já aconteceu no turno do herói com o resultado do turno do professor
    resultadoComProfessor.heroiAtacou = resultado.heroiAtacou;  // mantém o valor do turno do herói
    resultadoComProfessor.danoCausadoPeloHeroi = resultado.danoCausadoPeloHeroi;    // mantém o valor do turno do herói
    return resultadoComProfessor;   // retorna o resultado do turno completo, incluindo a ação do professor
}

ResultadoTurno Batalha::processarTurnoProfessor() {
    ResultadoTurno resultado{};     // inicializa todos os campos com valores padrão
    resultado.acaoValida = true;    // inicializa como true, será ajustado se a ação for inválida
    resultado.heroiAtacou = false;  // inicializa como false, será ajustado se o herói atacar
    resultado.danoCausadoPeloHeroi = 0; // inicializa como 0, será ajustado se o herói causar dano

    Movimento& golpeProfessor = professor.escolherAtaque(); // o professor escolhe um ataque aleatório para usar contra o herói

    if (professor.isBatalhaFalsa()) {
        // US05: entidade roteirizada, não pode causar dano real
        resultado.professorAtacou = true;
        resultado.danoCausadoPeloProfessor = 0;
        resultado.mensagem = professor.getNome() + " faz uma cena dramática de " + golpeProfessor.getNome() + ", mas erra feio (batalha falsa).";
    } else {
        heroi.receberDano(golpeProfessor.getDano());    // aplica o dano do golpe do professor no herói, considerando a defesa dele
        resultado.professorAtacou = true;   // o professor atacou usando o golpe escolhido
        resultado.danoCausadoPeloProfessor = golpeProfessor.getDano();  // dano causado pelo professor no herói usando o golpe escolhido
        resultado.mensagem = professor.getNome() + " usou " + golpeProfessor.getNome() + "!";   // mensagem de ação para o Godot exibir
    }

    turnosDecorridos++;

    if (professor.isBatalhaFalsa() && turnosDecorridos >= TURNOS_PARA_VITORIA_BATALHA_FALSA) {
        // US05: vitória automática após o número fixo de turnos, independente do HP do professor
        estado = EstadoBatalha::VITORIA;
    } else if (!heroi.estaVivo()) {
        estado = EstadoBatalha::DERROTA;    // o herói foi derrotado pelo golpe do professor
    }

    resultado.estadoResultante = estado;    // atualiza o estado da batalha para DERROTA/VITORIA se aplicável, ou mantém o estado atual
    return resultado;   // retorna o resultado do turno completo, incluindo a ação do professor
}

ResultadoTurno Batalha::usarItemChave() {
    ResultadoTurno resultado{};
    resultado.heroiAtacou = false;
    resultado.danoCausadoPeloHeroi = 0;
    resultado.professorAtacou = false;
    resultado.danoCausadoPeloProfessor = 0;

    if (estado != EstadoBatalha::ESCOLHENDO_ACAO) {
        resultado.acaoValida = false;
        resultado.mensagem = "A batalha ja terminou.";
        resultado.estadoResultante = estado;
        return resultado;
    }

    if (!heroi.possuiItemDeChave() || !professor.isInvulneravel()) {
        resultado.acaoValida = false;
        resultado.mensagem = "Nao ha nada pra usar o item de historia aqui.";
        resultado.estadoResultante = estado;
        return resultado;
    }

    professor.removerInvulnerabilidade();
    heroi.consumirItemChave();

    resultado.acaoValida = true;
    resultado.mensagem = professor.getNome() + " nao esta mais invulneravel!";
    resultado.estadoResultante = estado; // continua ESCOLHENDO_ACAO — o professor ainda nao apanhou

    return resultado;
}

ResultadoTurno Batalha::fugir() {
    estado = EstadoBatalha::FUGIU;  // o jogador escolheu fugir da batalha, a run é perdida

    ResultadoTurno resultado{};
    resultado.acaoValida = true;    // a ação de fugir é sempre válida, não há restrições
    resultado.mensagem = heroi.getNome() + " fugiu da batalha, o semestre foi perdido.";  // mensagem de ação para o Godot exibir
    resultado.heroiAtacou = false;  // o herói não atacou, ele fugiu da batalha
    resultado.danoCausadoPeloHeroi = 0; // o herói não causou dano, ele fugiu da batalha
    resultado.professorAtacou = false;  // o professor não atacou, o herói fugiu antes do turno dele
    resultado.danoCausadoPeloProfessor = 0; // o professor não causou dano, o herói fugiu antes do turno dele
    resultado.estadoResultante = estado;    // atualiza o estado da batalha para FUGIU, indicando que o jogador desistiu da luta
    return resultado;   // retorna o resultado do turno completo, incluindo a ação de fugir do herói
}