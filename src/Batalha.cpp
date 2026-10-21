#include "Batalha.hpp"
#include <godot_cpp/core/class_db.hpp>

Batalha::Batalha()
    : heroi(nullptr), professor(nullptr), estado(EstadoBatalha::ESCOLHENDO_ACAO), turnosDecorridos(0) {}

Batalha::Batalha(Herois* h, Professor* p)
    : heroi(h), professor(p), estado(EstadoBatalha::ESCOLHENDO_ACAO), turnosDecorridos(0) {}

void Batalha::inicializar(Herois* heroiDoJogador, Professor* professorAlvo) {
    heroi = heroiDoJogador;
    professor = professorAlvo;
    estado = EstadoBatalha::ESCOLHENDO_ACAO;
    turnosDecorridos = 0;
}

int Batalha::get_estado() const { return static_cast<int>(estado); }
EstadoBatalha Batalha::getEstado() { return estado; }
Professor* Batalha::get_professor() const { return professor; }
Herois* Batalha::get_heroi() const { return heroi; }

Dictionary Batalha::converterParaDictionary(const ResultadoTurno& resultado) const {
    Dictionary d;
    d["acao_valida"] = resultado.acaoValida;
    d["mensagem"] = String(resultado.mensagem.c_str());
    d["heroi_atacou"] = resultado.heroiAtacou;
    d["dano_causado_pelo_heroi"] = resultado.danoCausadoPeloHeroi;
    d["professor_atacou"] = resultado.professorAtacou;
    d["dano_causado_pelo_professor"] = resultado.danoCausadoPeloProfessor;
    d["estado_resultante"] = static_cast<int>(resultado.estadoResultante);
    return d;
}

ResultadoTurno Batalha::executarAcao(int indiceGolpe) {
    ResultadoTurno resultado{};
    resultado.acaoValida = true;
    resultado.heroiAtacou = false;
    resultado.danoCausadoPeloHeroi = 0;
    resultado.professorAtacou = false;
    resultado.danoCausadoPeloProfessor = 0;

    if (heroi == nullptr || professor == nullptr) {
        resultado.acaoValida = false;
        resultado.mensagem = "Batalha nao foi inicializada (chame inicializar() antes).";
        resultado.estadoResultante = estado;
        return resultado;
    }

    if (estado != EstadoBatalha::ESCOLHENDO_ACAO) {
        resultado.acaoValida = false;
        resultado.mensagem = "A batalha terminou.";
        resultado.estadoResultante = estado;
        return resultado;
    }

    int danoBase = 0;
    int curaBase = 0;
    std::string nomeMovimento = "";

    if (indiceGolpe >= 0 && indiceGolpe < 3) {
        Movimento* movimento = heroi->getMovimentoParaEditar(indiceGolpe);
        if (movimento == nullptr || !movimento->podeUsar()) {
            resultado.acaoValida = false;
            resultado.mensagem = "Esse movimento nao possui mais usos disponiveis!";
            resultado.estadoResultante = estado;
            return resultado;
        }
        danoBase = movimento->getDano();
        curaBase = movimento->getCura();
        nomeMovimento = movimento->getNome();
        movimento->usar();
    } else if (indiceGolpe == 3) {
        if (!heroi->itemEspecialPodeUsar()) {
            resultado.acaoValida = false;
            resultado.mensagem = "Nenhum item especial disponivel.";
            resultado.estadoResultante = estado;
            return resultado;
        }
        std::string nomeItem = heroi->getNomeItemEspecial();
        int danoItem = heroi->usarItemEspecial(*professor);
        resultado.heroiAtacou = true;
        resultado.danoCausadoPeloHeroi = danoItem;
        resultado.mensagem = heroi->getNome() + " usou " + nomeItem + "!";

        if (!professor->estaVivo()) {
            estado = EstadoBatalha::VITORIA;
            resultado.estadoResultante = estado;
            return resultado;
        }

        ResultadoTurno resultadoProfessor = processarTurnoProfessor();
        resultadoProfessor.heroiAtacou = resultado.heroiAtacou;
        resultadoProfessor.danoCausadoPeloHeroi = resultado.danoCausadoPeloHeroi;
        resultadoProfessor.mensagem = resultado.mensagem + "\n" + resultadoProfessor.mensagem;
        return resultadoProfessor;
    } else {
        resultado.acaoValida = false;
        resultado.mensagem = "Indice de golpe invalido.";
        resultado.estadoResultante = estado;
        return resultado;
    }

    int danoFinal = 0;
    if (danoBase > 0) {
        danoFinal = heroi->calcularDanoFinal(danoBase);
        professor->receberDano(danoFinal);
    }
    if (curaBase > 0) {
        heroi->curar(curaBase);
    }

    resultado.heroiAtacou = true;
    resultado.danoCausadoPeloHeroi = danoFinal;

    std::string msgHeroi = heroi->getNome() + " usou " + nomeMovimento + "!";
    if (danoFinal > 0 && curaBase > 0) {
        msgHeroi += " (Causou " + std::to_string(danoFinal) + " de dano e recuperou " + std::to_string(curaBase) + " de HP)";
    } else if (danoFinal > 0) {
        msgHeroi += " (Causou " + std::to_string(danoFinal) + " de dano)";
    } else if (curaBase > 0) {
        msgHeroi += " (Recuperou " + std::to_string(curaBase) + " de HP)";
    }
    resultado.mensagem = msgHeroi;

    if (!professor->estaVivo()) {
        estado = EstadoBatalha::VITORIA;
        resultado.estadoResultante = estado;
        return resultado;
    }

    ResultadoTurno resultadoComProfessor = processarTurnoProfessor();
    resultadoComProfessor.heroiAtacou = resultado.heroiAtacou;
    resultadoComProfessor.danoCausadoPeloHeroi = resultado.danoCausadoPeloHeroi;
    resultadoComProfessor.mensagem = msgHeroi + "\n" + resultadoComProfessor.mensagem;
    return resultadoComProfessor;
}

Dictionary Batalha::executar_acao(int indiceGolpe) {
    return converterParaDictionary(executarAcao(indiceGolpe));
}

ResultadoTurno Batalha::processarTurnoProfessor() {
    ResultadoTurno resultado{};
    resultado.acaoValida = true;
    resultado.heroiAtacou = false;
    resultado.danoCausadoPeloHeroi = 0;

    Movimento& golpeProfessor = professor->escolherAtaque();

    if (professor->isBatalhaFalsa()) {
        resultado.professorAtacou = true;
        resultado.danoCausadoPeloProfessor = 0;
        resultado.mensagem = professor->getNome() + " faz uma cena dramatica de " + golpeProfessor.getNome() + ", mas erra feio (batalha falsa).";
    } else {
        heroi->receberDano(golpeProfessor.getDano());
        resultado.professorAtacou = true;
        resultado.danoCausadoPeloProfessor = golpeProfessor.getDano();
        resultado.mensagem = professor->getNome() + " usou " + golpeProfessor.getNome() + "!";
    }

    turnosDecorridos++;

    if (professor->isBatalhaFalsa() && turnosDecorridos >= TURNOS_PARA_VITORIA_BATALHA_FALSA) {
        estado = EstadoBatalha::VITORIA;
    } else if (!heroi->estaVivo()) {
        estado = EstadoBatalha::DERROTA;
    }

    resultado.estadoResultante = estado;
    return resultado;
}

Dictionary Batalha::usar_item_chave() {
    ResultadoTurno resultado{};
    resultado.heroiAtacou = false;
    resultado.danoCausadoPeloHeroi = 0;
    resultado.professorAtacou = false;
    resultado.danoCausadoPeloProfessor = 0;

    if (heroi == nullptr || professor == nullptr) {
        resultado.acaoValida = false;
        resultado.mensagem = "Batalha nao foi inicializada (chame inicializar() antes).";
        resultado.estadoResultante = estado;
        return converterParaDictionary(resultado);
    }

    if (estado != EstadoBatalha::ESCOLHENDO_ACAO) {
        resultado.acaoValida = false;
        resultado.mensagem = "A batalha ja terminou.";
        resultado.estadoResultante = estado;
        return converterParaDictionary(resultado);
    }

    if (!heroi->possuiItemDeChave() || !professor->isInvulneravel()) {
        resultado.acaoValida = false;
        resultado.mensagem = "Nao ha nada pra usar o item de historia aqui.";
        resultado.estadoResultante = estado;
        return converterParaDictionary(resultado);
    }

    professor->removerInvulnerabilidade();
    heroi->consumirItemChave();

    resultado.acaoValida = true;
    resultado.mensagem = professor->getNome() + " nao esta mais invulneravel!";
    resultado.estadoResultante = estado;

    return converterParaDictionary(resultado);
}

Dictionary Batalha::fugir() {
    estado = EstadoBatalha::FUGIU;

    ResultadoTurno resultado{};
    resultado.acaoValida = true;
    resultado.mensagem = (heroi != nullptr ? heroi->getNome() : std::string("O heroi")) + " fugiu da batalha, o semestre foi perdido.";
    resultado.heroiAtacou = false;
    resultado.danoCausadoPeloHeroi = 0;
    resultado.professorAtacou = false;
    resultado.danoCausadoPeloProfessor = 0;
    resultado.estadoResultante = estado;
    return converterParaDictionary(resultado);
}

void Batalha::_bind_methods() {
    ClassDB::bind_method(D_METHOD("inicializar", "heroi", "professor"), &Batalha::inicializar);
    ClassDB::bind_method(D_METHOD("get_estado"), &Batalha::get_estado);
    ClassDB::bind_method(D_METHOD("get_professor"), &Batalha::get_professor);
    ClassDB::bind_method(D_METHOD("get_heroi"), &Batalha::get_heroi);
    ClassDB::bind_method(D_METHOD("executar_acao", "indice_golpe"), &Batalha::executar_acao);
    ClassDB::bind_method(D_METHOD("usar_item_chave"), &Batalha::usar_item_chave);
    ClassDB::bind_method(D_METHOD("fugir"), &Batalha::fugir);
}