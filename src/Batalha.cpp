#include "Batalha.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/rich_text_label.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/theme_db.hpp>
#include <godot_cpp/classes/theme.hpp>
#include <godot_cpp/classes/font.hpp>
#include <godot_cpp/classes/text_server.hpp>

Batalha::Batalha()
    : heroi(nullptr), professor(nullptr), telaBatalhaAtual(nullptr), estado(EstadoBatalha::DERROTA), turnosDecorridos(0) {}

Batalha::Batalha(Herois* h, Professor* p)
    : heroi(h), professor(p), telaBatalhaAtual(nullptr), estado((h && p) ? EstadoBatalha::ESCOLHENDO_ACAO : EstadoBatalha::DERROTA), turnosDecorridos(0) {}

void Batalha::inicializar(Herois* heroiDoJogador, Professor* professorAlvo) {
	if (!heroiDoJogador || !professorAlvo || !heroiDoJogador->estaVivo() || !professorAlvo->estaVivo()) {
		heroi = nullptr;
		professor = nullptr;
		estado = EstadoBatalha::DERROTA;
		turnosDecorridos = 0;
		UtilityFunctions::push_error("Batalha não inicializada: herói/professor nulo ou sem vida.");
		return;
	}
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
    d["mensagem"] = String::utf8(resultado.mensagem.c_str());
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
        resultado.mensagem = "Batalha não foi inicializada (chame inicializar() antes).";
        resultado.estadoResultante = estado;
        return resultado;
    }
	if (!heroi->estaVivo()) estado = EstadoBatalha::DERROTA;
	else if (!professor->estaVivo()) estado = EstadoBatalha::VITORIA;

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
            resultado.mensagem = "Esse movimento não possui mais usos disponíveis!";
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
            resultado.mensagem = "Nenhum item especial disponível.";
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
        resultado.mensagem = "Índice de golpe inválido.";
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
        resultado.mensagem = professor->getNome() + " faz uma cena dramática de " + golpeProfessor.getNome() + ", mas erra feio (batalha falsa).";
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
        resultado.mensagem = "Batalha não foi inicializada (chame inicializar() antes).";
        resultado.estadoResultante = estado;
        return converterParaDictionary(resultado);
    }
	if (!heroi->estaVivo()) estado = EstadoBatalha::DERROTA;
	else if (!professor->estaVivo()) estado = EstadoBatalha::VITORIA;

    if (estado != EstadoBatalha::ESCOLHENDO_ACAO) {
        resultado.acaoValida = false;
        resultado.mensagem = "A batalha já terminou.";
        resultado.estadoResultante = estado;
        return converterParaDictionary(resultado);
    }

    if (!heroi->possuiItemDeChave() || !professor->isInvulneravel()) {
        resultado.acaoValida = false;
        resultado.mensagem = "Não há nada para usar o item de história aqui.";
        resultado.estadoResultante = estado;
        return converterParaDictionary(resultado);
    }

    professor->removerInvulnerabilidade();
    heroi->consumirItemChave();

    resultado.acaoValida = true;
    resultado.mensagem = professor->getNome() + " não está mais invulnerável!";
    resultado.estadoResultante = estado;

    return converterParaDictionary(resultado);
}

Dictionary Batalha::fugir() {
    ResultadoTurno resultado{};
    resultado.heroiAtacou = false;
    resultado.danoCausadoPeloHeroi = 0;
    resultado.professorAtacou = false;
    resultado.danoCausadoPeloProfessor = 0;
	if (heroi && professor && !heroi->estaVivo()) estado = EstadoBatalha::DERROTA;
	else if (heroi && professor && !professor->estaVivo()) estado = EstadoBatalha::VITORIA;
	if (!heroi || !professor || estado != EstadoBatalha::ESCOLHENDO_ACAO) {
		resultado.acaoValida = false;
		resultado.mensagem = (!heroi || !professor) ? "Batalha não inicializada." : "A batalha já terminou.";
		resultado.estadoResultante = estado;
		return converterParaDictionary(resultado);
	}

	estado = EstadoBatalha::FUGIU;
	resultado.acaoValida = true;
	resultado.mensagem = heroi->getNome() + " fugiu da batalha, o semestre foi perdido.";
    resultado.estadoResultante = estado;
    return converterParaDictionary(resultado);
}

void Batalha::atualizar_botoes_acao(Control* telaBatalha) {
    if (!telaBatalha || !heroi) return;
    telaBatalhaAtual = telaBatalha;

    bool turnoDoJogador = (estado == EstadoBatalha::ESCOLHENDO_ACAO);

    for (int i = 0; i < 3; i++) {
        String caminhoGolpe = "BarraAcoes/BotoesGolpe/Golpe" + String::num_int64(i);
        Button* botaoGolpe = telaBatalha->has_node(caminhoGolpe) ? telaBatalha->get_node<Button>(caminhoGolpe) : nullptr;
        if (!botaoGolpe) continue;

        if (turnoDoJogador && i < heroi->get_quantidade_movimentos()) {
            botaoGolpe->set_disabled(!heroi->movimento_pode_usar(i));
        } else {
            botaoGolpe->set_disabled(true);
        }
    }

    // Garante que o botão "Desistir" sempre volte a ficar ativo no turno do jogador
    Button* botaoFugir = telaBatalha->has_node("BarraAcoes/BotoesGolpe/BotaoFugir")
        ? telaBatalha->get_node<Button>("BarraAcoes/BotoesGolpe/BotaoFugir")
        : nullptr;
    if (botaoFugir) {
        botaoFugir->set_disabled(!turnoDoJogador);
    }
}

void Batalha::posicionar_barras_vida(Control* telaBatalha) {
    if (!telaBatalha) return;
    telaBatalhaAtual = telaBatalha;

    Control* hudHeroi = telaBatalha->has_node("HUDHeroi") ? telaBatalha->get_node<Control>("HUDHeroi") : nullptr;
    Control* hudProfessor = telaBatalha->has_node("HUDProfessor") ? telaBatalha->get_node<Control>("HUDProfessor") : nullptr;

    // Estudante (Jogador) no topo esquerdo
    if (hudHeroi) {
        hudHeroi->set_anchor(SIDE_LEFT, 0.025f);
        hudHeroi->set_anchor(SIDE_TOP, 0.04f);
        hudHeroi->set_anchor(SIDE_RIGHT, 0.36f);
        hudHeroi->set_anchor(SIDE_BOTTOM, 0.20f);
        hudHeroi->set_offset(SIDE_LEFT, 0.0f);
        hudHeroi->set_offset(SIDE_TOP, 0.0f);
        hudHeroi->set_offset(SIDE_RIGHT, 0.0f);
        hudHeroi->set_offset(SIDE_BOTTOM, 0.0f);
    }

    // Professor (Oponente) no lado direito
    if (hudProfessor) {
        hudProfessor->set_anchor(SIDE_LEFT, 0.64f);
        hudProfessor->set_anchor(SIDE_TOP, 0.56f);
        hudProfessor->set_anchor(SIDE_RIGHT, 0.975f);
        hudProfessor->set_anchor(SIDE_BOTTOM, 0.70f);
        hudProfessor->set_offset(SIDE_LEFT, 0.0f);
        hudProfessor->set_offset(SIDE_TOP, 0.0f);
        hudProfessor->set_offset(SIDE_RIGHT, 0.0f);
        hudProfessor->set_offset(SIDE_BOTTOM, 0.0f);
    }

    // Aplica a tipografia global estilo Pixel Art nos rótulos, caixa de diálogo e botões da Batalha
    ThemeDB* bancoTemas = ThemeDB::get_singleton();
    if (!bancoTemas) return;

    Ref<Font> fonteGlobal = bancoTemas->get_fallback_font();
    if (!fonteGlobal.is_valid()) return;

    const char* caminhosNomes[] = {
        "HUDHeroi/Conteudo/LabelHeroiNome",
        "HUDProfessor/Conteudo/LabelProfessorNome"
    };
    for (const char* caminho : caminhosNomes) {
        Label* lbl = telaBatalha->has_node(caminho) ? telaBatalha->get_node<Label>(caminho) : nullptr;
        if (lbl) {
            lbl->add_theme_font_override("font", fonteGlobal);
            lbl->add_theme_font_size_override("font_size", 18);
        }
    }

    const char* caminhosVida[] = {
        "HUDHeroi/Conteudo/LabelHeroiVida",
        "HUDProfessor/Conteudo/LabelProfessorVida"
    };
    for (const char* caminho : caminhosVida) {
        Label* lbl = telaBatalha->has_node(caminho) ? telaBatalha->get_node<Label>(caminho) : nullptr;
        if (lbl) {
            lbl->add_theme_font_override("font", fonteGlobal);
            lbl->add_theme_font_size_override("font_size", 15);
        }
    }

    if (telaBatalha->has_node("CaixaMensagem/LabelMensagem")) {
        Node* noDialogo = telaBatalha->get_node<Node>("CaixaMensagem/LabelMensagem");
        Label* lblMensagem = Object::cast_to<Label>(noDialogo);
        if (lblMensagem) {
            lblMensagem->set_autowrap_mode(TextServer::AUTOWRAP_WORD_SMART);
            lblMensagem->add_theme_font_override("font", fonteGlobal);
            lblMensagem->add_theme_font_size_override("font_size", 16);
        }
        RichTextLabel* richMensagem = Object::cast_to<RichTextLabel>(noDialogo);
        if (richMensagem) {
            richMensagem->add_theme_font_override("normal_font", fonteGlobal);
            richMensagem->add_theme_font_override("bold_font", fonteGlobal);
            richMensagem->add_theme_font_size_override("normal_font_size", 16);
        }
    }

    const char* caminhosBotoes[] = {
        "BarraAcoes/BotoesGolpe/Golpe0",
        "BarraAcoes/BotoesGolpe/Golpe1",
        "BarraAcoes/BotoesGolpe/Golpe2",
        "BarraAcoes/BotoesGolpe/BotaoFugir",
        "BarraAcoes/BotoesGolpe/BotaoItemEspecial",
        "BarraAcoes/BotoesGolpe/BotaoItemChave"
    };
    for (const char* caminho : caminhosBotoes) {
        Button* btn = telaBatalha->has_node(caminho) ? telaBatalha->get_node<Button>(caminho) : nullptr;
        if (btn) {
            btn->add_theme_font_override("font", fonteGlobal);
            btn->add_theme_font_size_override("font_size", 15);
        }
    }
}

void Batalha::alterar_fundo_batalha(int area_atual) {
    if (!telaBatalhaAtual) return;

    TextureRect* fundo = telaBatalhaAtual->has_node("Fundo")
        ? telaBatalhaAtual->get_node<TextureRect>("Fundo")
        : nullptr;
    if (!fundo) return;

    String caminho_da_imagem;
    switch (area_atual) {
        case 0: // Área 1 (Cálculo)
            caminho_da_imagem = String::utf8("res://assets/imagens/Sala_icex.jpeg");
            break;
        case 1: // Área 2 (GAAL / Análise Numérica)
            caminho_da_imagem = String::utf8("res://assets/imagens/Auditorio_icex.jpeg");
            break;
        case 2: // Área 3 (Física / Mecânica)
            caminho_da_imagem = String::utf8("res://assets/imagens/Sala_icex.jpeg");
            break;
        case 3: // Área 4 (PDS)
            caminho_da_imagem = String::utf8("res://assets/imagens/Dcc_padrao.jpeg");
            break;
        case 4: // Desafio secreto da Luciana
            caminho_da_imagem = String::utf8("res://assets/imagens/Auditorio_icex.jpeg");
            break;
        default:
            caminho_da_imagem = String::utf8("res://assets/imagens/Sala_icex.jpeg");
            break;
    }

    ResourceLoader* loader = ResourceLoader::get_singleton();
    if (!loader) return;

    Ref<Resource> recurso = loader->load(caminho_da_imagem);
    Ref<Texture2D> textura = recurso;
    if (textura.is_valid()) {
        fundo->set_expand_mode(TextureRect::EXPAND_IGNORE_SIZE);
        fundo->set_texture(textura);
    }
}

void Batalha::_bind_methods() {
    ClassDB::bind_method(D_METHOD("inicializar", "heroi", "professor"), &Batalha::inicializar);
    ClassDB::bind_method(D_METHOD("get_estado"), &Batalha::get_estado);
    ClassDB::bind_method(D_METHOD("get_professor"), &Batalha::get_professor);
    ClassDB::bind_method(D_METHOD("get_heroi"), &Batalha::get_heroi);
    ClassDB::bind_method(D_METHOD("executar_acao", "indice_golpe"), &Batalha::executar_acao);
    ClassDB::bind_method(D_METHOD("usar_item_chave"), &Batalha::usar_item_chave);
    ClassDB::bind_method(D_METHOD("fugir"), &Batalha::fugir);
    ClassDB::bind_method(D_METHOD("atualizar_botoes_acao", "tela_batalha"), &Batalha::atualizar_botoes_acao);
    ClassDB::bind_method(D_METHOD("posicionar_barras_vida", "tela_batalha"), &Batalha::posicionar_barras_vida);
    ClassDB::bind_method(D_METHOD("alterar_fundo_batalha", "area_atual"), &Batalha::alterar_fundo_batalha);
}
