#include "GerenciadorInterface.hpp"
#include "Herois.hpp"
#include "Movimento.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/memory.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/classes/h_slider.hpp>
#include <godot_cpp/classes/option_button.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/color_rect.hpp>
#include <godot_cpp/classes/margin_container.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/scroll_container.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/label.hpp>
#include <algorithm>

using namespace godot;

void GerenciadorInterface::_bind_methods() 
{
    ClassDB::bind_method(D_METHOD("clicar_jogar"), &GerenciadorInterface::clicar_jogar);
    ClassDB::bind_method(D_METHOD("clicar_configuracoes"), &GerenciadorInterface::clicar_configuracoes);
    ClassDB::bind_method(D_METHOD("clicar_sair"), &GerenciadorInterface::clicar_sair);
    ClassDB::bind_method(D_METHOD("clicar_voltar"), &GerenciadorInterface::clicar_voltar);
    ClassDB::bind_method(D_METHOD("mudar_volume", "valor"), &GerenciadorInterface::mudar_volume);
    ClassDB::bind_method(D_METHOD("mudar_resolucao", "indice"), &GerenciadorInterface::mudar_resolucao);
    
    ClassDB::bind_method(D_METHOD("clicar_mundo_principal"), &GerenciadorInterface::clicar_mundo_principal);
    ClassDB::bind_method(D_METHOD("clicar_quests"), &GerenciadorInterface::clicar_quests);
    ClassDB::bind_method(D_METHOD("clicar_equipamento"), &GerenciadorInterface::clicar_equipamento);

    // Métodos do Menu de Movimentos em C++
    ClassDB::bind_method(D_METHOD("clicar_movimentos"), &GerenciadorInterface::clicar_movimentos);
    ClassDB::bind_method(D_METHOD("clicar_fechar_movimentos"), &GerenciadorInterface::clicar_fechar_movimentos);
    ClassDB::bind_method(D_METHOD("selecionar_slot_movimento", "slot"), &GerenciadorInterface::selecionar_slot_movimento);
    ClassDB::bind_method(D_METHOD("escolher_movimento_catalogo", "id_catalogo"), &GerenciadorInterface::escolher_movimento_catalogo);
}

GerenciadorInterface::GerenciadorInterface()
    : estadoAtual(GerenciadorMenu::EstadoMenu::MENU_INICIAL),
      volume(100.0f),
      indiceResolucao(0),
      painelMovimentos(nullptr),
      labelStatusMovimentos(nullptr),
      slotMovimentoSelecionado(0)
{
    botoesSlot[0] = nullptr;
    botoesSlot[1] = nullptr;
    botoesSlot[2] = nullptr;
}

void GerenciadorInterface::_notification(int p_what)
{
    if (p_what == NOTIFICATION_READY) {
        inicializar_componentes();
    }
}

void GerenciadorInterface::_ready() 
{
    inicializar_componentes();
}

void GerenciadorInterface::inicializar_componentes()
{
    Button* botao_jogar = has_node("ContainerBotoes/BotaoJogar") ? get_node<Button>("ContainerBotoes/BotaoJogar") : nullptr;
    Button* botao_config = has_node("ContainerBotoes/BotaoConfig") ? get_node<Button>("ContainerBotoes/BotaoConfig") : nullptr;
    Button* botao_sair = has_node("ContainerBotoes/BotaoSair") ? get_node<Button>("ContainerBotoes/BotaoSair") : nullptr;
    Button* botao_voltar = has_node("ContainerConfig/BotaoVoltar") ? get_node<Button>("ContainerConfig/BotaoVoltar") : nullptr;
    
    Button* botao_mundo = has_node("ContainerJogar/BotaoMundo") ? get_node<Button>("ContainerJogar/BotaoMundo") : nullptr;
    Button* botao_quests = has_node("ContainerJogar/BotaoQuests") ? get_node<Button>("ContainerJogar/BotaoQuests") : nullptr;
    Button* botao_equipamento = has_node("ContainerJogar/BotaoEquipamento") ? get_node<Button>("ContainerJogar/BotaoEquipamento") : nullptr;
    Button* botao_voltar_jogar = has_node("ContainerJogar/BotaoVoltar") ? get_node<Button>("ContainerJogar/BotaoVoltar") : nullptr;
    
    HSlider* slider_volume = has_node("ContainerConfig/SliderVolume") ? get_node<HSlider>("ContainerConfig/SliderVolume") : nullptr;
    OptionButton* caixa_resolucao = has_node("ContainerConfig/CaixaResolucao") ? get_node<OptionButton>("ContainerConfig/CaixaResolucao") : nullptr;
    
    if (caixa_resolucao) 
    {
        caixa_resolucao->clear();
        caixa_resolucao->add_item("640x480");
        caixa_resolucao->add_item("1280x720");
        caixa_resolucao->add_item("1280x960");
        caixa_resolucao->add_item("1920x1080");
        caixa_resolucao->add_item("1920x1440");
        caixa_resolucao->add_item("2560x1440");

        Callable funcao_resolucao(this, "mudar_resolucao");
        if (!caixa_resolucao->is_connected("item_selected", funcao_resolucao)) {
            caixa_resolucao->connect("item_selected", funcao_resolucao);
        }
    }

    if (botao_jogar && !botao_jogar->is_connected("pressed", Callable(this, "clicar_jogar")))
        botao_jogar->connect("pressed", Callable(this, "clicar_jogar"));
    if (botao_config && !botao_config->is_connected("pressed", Callable(this, "clicar_configuracoes")))
        botao_config->connect("pressed", Callable(this, "clicar_configuracoes"));
    if (botao_sair && !botao_sair->is_connected("pressed", Callable(this, "clicar_sair")))
        botao_sair->connect("pressed", Callable(this, "clicar_sair"));
    if (botao_voltar && !botao_voltar->is_connected("pressed", Callable(this, "clicar_voltar")))
        botao_voltar->connect("pressed", Callable(this, "clicar_voltar"));
    
    if (botao_mundo && !botao_mundo->is_connected("pressed", Callable(this, "clicar_mundo_principal")))
        botao_mundo->connect("pressed", Callable(this, "clicar_mundo_principal"));
    if (botao_quests && !botao_quests->is_connected("pressed", Callable(this, "clicar_quests")))
        botao_quests->connect("pressed", Callable(this, "clicar_quests"));
    if (botao_equipamento && !botao_equipamento->is_connected("pressed", Callable(this, "clicar_equipamento")))
        botao_equipamento->connect("pressed", Callable(this, "clicar_equipamento"));
    if (botao_voltar_jogar && !botao_voltar_jogar->is_connected("pressed", Callable(this, "clicar_voltar")))
        botao_voltar_jogar->connect("pressed", Callable(this, "clicar_voltar"));

    if (slider_volume && !slider_volume->is_connected("value_changed", Callable(this, "mudar_volume")))
        slider_volume->connect("value_changed", Callable(this, "mudar_volume"));

    if (has_node("ContainerBotoes")) {
        Control* container_botoes = get_node<Control>("ContainerBotoes");
        Button* botao_movimentos = has_node("ContainerBotoes/BotaoMovimentos")
                                       ? get_node<Button>("ContainerBotoes/BotaoMovimentos")
                                       : nullptr;
        if (!botao_movimentos && container_botoes) {
            botao_movimentos = memnew(Button);
            botao_movimentos->set_name("BotaoMovimentos");
            botao_movimentos->set_text("Movimentos");
            botao_movimentos->add_theme_font_size_override("font_size", 40);
            container_botoes->add_child(botao_movimentos);
        }
        if (botao_movimentos) {
            Callable acao_movimentos(this, "clicar_movimentos");
            if (!botao_movimentos->is_connected("pressed", acao_movimentos)) {
                botao_movimentos->connect("pressed", acao_movimentos);
            }
        }
    }

    criar_painel_movimentos();
}

void GerenciadorInterface::criar_painel_movimentos()
{
    if (painelMovimentos != nullptr) return;

    painelMovimentos = memnew(Control);
    painelMovimentos->set_name("PainelMovimentos");
    painelMovimentos->set_visible(false);
    painelMovimentos->set_z_index(100);
    add_child(painelMovimentos);

    painelMovimentos->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
    painelMovimentos->set_anchor(SIDE_RIGHT, 1.0f);
    painelMovimentos->set_anchor(SIDE_BOTTOM, 1.0f);
    painelMovimentos->set_h_grow_direction(Control::GROW_DIRECTION_BOTH);
    painelMovimentos->set_v_grow_direction(Control::GROW_DIRECTION_BOTH);

    // Fundo escuro cobrindo toda a tela
    ColorRect* fundo = memnew(ColorRect);
    fundo->set_color(Color(0.06f, 0.08f, 0.14f, 0.96f));
    painelMovimentos->add_child(fundo);
    fundo->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
    fundo->set_anchor(SIDE_RIGHT, 1.0f);
    fundo->set_anchor(SIDE_BOTTOM, 1.0f);

    // Margens responsivas para funcionar em qualquer resolução
    MarginContainer* margem = memnew(MarginContainer);
    painelMovimentos->add_child(margem);
    margem->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
    margem->set_anchor(SIDE_RIGHT, 1.0f);
    margem->set_anchor(SIDE_BOTTOM, 1.0f);
    margem->add_theme_constant_override("margin_left", 40);
    margem->add_theme_constant_override("margin_top", 25);
    margem->add_theme_constant_override("margin_right", 40);
    margem->add_theme_constant_override("margin_bottom", 25);

    VBoxContainer* caixaPrincipal = memnew(VBoxContainer);
    caixaPrincipal->add_theme_constant_override("separation", 10);
    caixaPrincipal->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    caixaPrincipal->set_v_size_flags(Control::SIZE_EXPAND_FILL);
    margem->add_child(caixaPrincipal);

    Label* titulo = memnew(Label);
    titulo->set_text("GRADE CURRICULAR DE MOVIMENTOS (ICEx / UFMG)");
    titulo->set_horizontal_alignment(HORIZONTAL_ALIGNMENT_CENTER);
    titulo->add_theme_font_size_override("font_size", 28);
    caixaPrincipal->add_child(titulo);

    labelStatusMovimentos = memnew(Label);
    labelStatusMovimentos->set_horizontal_alignment(HORIZONTAL_ALIGNMENT_CENTER);
    labelStatusMovimentos->add_theme_font_size_override("font_size", 18);
    caixaPrincipal->add_child(labelStatusMovimentos);

    // Barra superior com os 3 Slots equipados do Estudante
    HBoxContainer* barraSlots = memnew(HBoxContainer);
    barraSlots->add_theme_constant_override("separation", 12);
    barraSlots->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    caixaPrincipal->add_child(barraSlots);

    for (int i = 0; i < 3; i++) {
        Button* btnSlot = memnew(Button);
        btnSlot->set_custom_minimum_size(Vector2(0, 60));
        btnSlot->set_h_size_flags(Control::SIZE_EXPAND_FILL);
        btnSlot->add_theme_font_size_override("font_size", 16);
        btnSlot->connect("pressed", Callable(this, "selecionar_slot_movimento").bind(i));
        barraSlots->add_child(btnSlot);
        botoesSlot[i] = btnSlot;
    }

    // Lista rolável com todos os 12 movimentos do catálogo C++
    ScrollContainer* scroll = memnew(ScrollContainer);
    scroll->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    scroll->set_v_size_flags(Control::SIZE_EXPAND_FILL);
    caixaPrincipal->add_child(scroll);

    VBoxContainer* listaCatalogo = memnew(VBoxContainer);
    listaCatalogo->add_theme_constant_override("separation", 8);
    listaCatalogo->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    scroll->add_child(listaCatalogo);

    botoesCatalogo.clear();
    const auto& catalogo = Movimento::getCatalogoMovimentos();
    for (int id = 0; id < static_cast<int>(catalogo.size()); id++) {
        Button* btnMov = memnew(Button);
        btnMov->set_custom_minimum_size(Vector2(0, 54));
        btnMov->set_h_size_flags(Control::SIZE_EXPAND_FILL);
        btnMov->add_theme_font_size_override("font_size", 16);
        btnMov->connect("pressed", Callable(this, "escolher_movimento_catalogo").bind(id));
        listaCatalogo->add_child(btnMov);
        botoesCatalogo.push_back(btnMov);
    }

    Button* btnVoltar = memnew(Button);
    btnVoltar->set_text("Confirmar Movimentos e Voltar ao Menu");
    btnVoltar->set_custom_minimum_size(Vector2(0, 48));
    btnVoltar->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    btnVoltar->add_theme_font_size_override("font_size", 22);
    btnVoltar->connect("pressed", Callable(this, "clicar_fechar_movimentos"));
    caixaPrincipal->add_child(btnVoltar);

    atualizar_painel_movimentos();
}

void GerenciadorInterface::atualizar_painel_movimentos()
{
    if (!painelMovimentos) return;

    const auto& catalogo = Movimento::getCatalogoMovimentos();

    for (int i = 0; i < 3; i++) {
        if (!botoesSlot[i]) continue;
        int idEquipado = Herois::getMovimentoEscolhido(i);
        const Movimento& m = catalogo[idEquipado];

        std::string prefixo = (i == slotMovimentoSelecionado) ? ">> [EDITANDO SLOT " : "[SLOT ";
        std::string sufixo = (i == slotMovimentoSelecionado) ? "] <<" : "]";
        std::string textoSlot = prefixo + std::to_string(i + 1) + sufixo + "\n" +
                                m.getNome() + " (" + m.getDisciplina() + ")";
        botoesSlot[i]->set_text(String(textoSlot.c_str()));
    }

    if (labelStatusMovimentos) {
        std::string msg = "Escolhendo para o SLOT " + std::to_string(slotMovimentoSelecionado + 1) +
                          " — Clique em qualquer habilidade abaixo para equipar na batalha:";
        labelStatusMovimentos->set_text(String(msg.c_str()));
    }

    for (int id = 0; id < static_cast<int>(botoesCatalogo.size()) && id < static_cast<int>(catalogo.size()); id++) {
        Button* btn = botoesCatalogo[id];
        const Movimento& m = catalogo[id];
        int slotOndeEsta = Herois::encontrarSlotDoMovimento(id);

        std::string statusEquip = "";
        if (slotOndeEsta != -1) {
            statusEquip = "[EQUIPADO NO SLOT " + std::to_string(slotOndeEsta + 1) + "] ";
        }

        std::string textoBtn = statusEquip + "[" + m.getDisciplina() + "] " + m.getNome() +
                               "  |  " + m.getResumoEfeito() + "\n" + m.getDescricao();
        btn->set_text(String(textoBtn.c_str()));
    }
}

void GerenciadorInterface::clicar_movimentos()
{
    if (painelMovimentos == nullptr) {
        criar_painel_movimentos();
    }
    irPara(GerenciadorMenu::EstadoMenu::MENU_MOVIMENTOS);
    slotMovimentoSelecionado = 0;
    atualizar_painel_movimentos();
    atualizar_telas();
    if (painelMovimentos) {
        painelMovimentos->set_visible(true);
        painelMovimentos->move_to_front();
    }
    UtilityFunctions::print(">> Abriu a Grade Curricular de Movimentos (C++)");
}

void GerenciadorInterface::clicar_fechar_movimentos()
{
    if (painelMovimentos) {
        painelMovimentos->set_visible(false);
    }
    irPara(GerenciadorMenu::EstadoMenu::MENU_INICIAL);
    atualizar_telas();
}

void GerenciadorInterface::selecionar_slot_movimento(int slot)
{
    if (slot >= 0 && slot < 3) {
        slotMovimentoSelecionado = slot;
        atualizar_painel_movimentos();
    }
}

void GerenciadorInterface::escolher_movimento_catalogo(int idCatalogo)
{
    Herois::definirMovimentoEscolhido(slotMovimentoSelecionado, idCatalogo);
    slotMovimentoSelecionado = (slotMovimentoSelecionado + 1) % 3;
    atualizar_painel_movimentos();
}

void GerenciadorInterface::atualizar_telas() 
{
    Control* menu_principal = has_node("ContainerBotoes") ? get_node<Control>("ContainerBotoes") : nullptr;
    Control* menu_config = has_node("ContainerConfig") ? get_node<Control>("ContainerConfig") : nullptr;
    Control* menu_jogar = has_node("ContainerJogar") ? get_node<Control>("ContainerJogar") : nullptr;
    Control* titulo = has_node("Titulo") ? get_node<Control>("Titulo") : nullptr;

    TextureRect* fundo_principal = has_node("FundoPrincipal") ? get_node<TextureRect>("FundoPrincipal") : nullptr;
    TextureRect* fundo_config = has_node("FundoConfig") ? get_node<TextureRect>("FundoConfig") : nullptr;
    TextureRect* fundo_jogo = has_node("FundoJogo") ? get_node<TextureRect>("FundoJogo") : nullptr;

    bool is_menu = (estadoAtual == GerenciadorMenu::EstadoMenu::MENU_INICIAL);
    bool is_config = (estadoAtual == GerenciadorMenu::EstadoMenu::MENU_CONFIGURACOES);
    bool is_jogar = (estadoAtual == GerenciadorMenu::EstadoMenu::MENU_JOGAR);
    bool is_movimentos = (estadoAtual == GerenciadorMenu::EstadoMenu::MENU_MOVIMENTOS);

    if (menu_principal) menu_principal->set_visible(is_menu);
    if (menu_config) menu_config->set_visible(is_config);
    if (menu_jogar) menu_jogar->set_visible(is_jogar);
    if (titulo) titulo->set_visible(is_menu);
    if (painelMovimentos) painelMovimentos->set_visible(is_movimentos);

    if (fundo_principal) fundo_principal->set_visible(is_menu || is_movimentos);
    if (fundo_config) fundo_config->set_visible(is_config);
    if (fundo_jogo) fundo_jogo->set_visible(is_jogar);
}

void GerenciadorInterface::clicar_jogar() 
{
    irPara(GerenciadorMenu::EstadoMenu::MENU_JOGAR);
    UtilityFunctions::print(">> Abriu o submenu de jogar");
    atualizar_telas();
}

void GerenciadorInterface::clicar_mundo_principal() 
{
    UtilityFunctions::print(">> Entrou no Mundo Principal!");
    iniciarOuRetomarPartida(); 
    Node* global = has_node("/root/Global") ? get_node<Node>("/root/Global") : nullptr;
    if (global) {
        global->call("iniciar_primeira_batalha");
    }
    atualizar_telas();
}

void GerenciadorInterface::clicar_quests() 
{
    UtilityFunctions::print(">> Abrindo menu de Quests...");
}

void GerenciadorInterface::clicar_equipamento() 
{
    UtilityFunctions::print(">> Abrindo Inventário/Equipamentos...");
}

void GerenciadorInterface::clicar_configuracoes() 
{
    irPara(GerenciadorMenu::EstadoMenu::MENU_CONFIGURACOES);
    UtilityFunctions::print("Abriu configuracoes");
    atualizar_telas();
}

void GerenciadorInterface::clicar_voltar() 
{
    voltar();
    UtilityFunctions::print("Voltou pro menu inicial");
    atualizar_telas();
}

void GerenciadorInterface::clicar_sair() 
{
    UtilityFunctions::print("Fechou o jogo");
    get_tree()->quit();
}

void GerenciadorInterface::mudar_volume(float valor) 
{
    definirVolume(valor);
    UtilityFunctions::print("Volume ajustado para: ", volume);
}

void GerenciadorInterface::mudar_resolucao(int indice) 
{
    definirIndiceResolucao(indice);
    Window* janela = get_window();
    if (!janela) return;

    switch (indice) {
        case 0: janela->set_size(Vector2i(640, 480)); break;
        case 1: janela->set_size(Vector2i(1280, 720)); break;
        case 2: janela->set_size(Vector2i(1280, 960)); break;
        case 3: janela->set_size(Vector2i(1920, 1080)); break;
        case 4: janela->set_size(Vector2i(1920, 1440)); break;
        case 5: janela->set_size(Vector2i(2560, 1440)); break;
    }
    UtilityFunctions::print("Resolução alterada. Índice: ", indice);
}

GerenciadorMenu::EstadoMenu GerenciadorInterface::getEstadoAtual() const { return estadoAtual; }
void GerenciadorInterface::irPara(GerenciadorMenu::EstadoMenu novoEstado) { estadoAtual = novoEstado; }
void GerenciadorInterface::voltar() { estadoAtual = GerenciadorMenu::EstadoMenu::MENU_INICIAL; }
float GerenciadorInterface::getVolume() const { return volume; }
void GerenciadorInterface::definirVolume(float novoVolume) { volume = std::max(0.0f, std::min(100.0f, novoVolume)); }
int GerenciadorInterface::getIndiceResolucao() const { return indiceResolucao; }
void GerenciadorInterface::definirIndiceResolucao(int indice) { indiceResolucao = indice; }
void GerenciadorInterface::iniciarOuRetomarPartida() { estadoAtual = GerenciadorMenu::EstadoMenu::EM_BATALHA; }