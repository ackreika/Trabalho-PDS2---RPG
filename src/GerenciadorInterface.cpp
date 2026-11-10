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
#include <godot_cpp/classes/panel_container.hpp>
#include <godot_cpp/classes/margin_container.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/scroll_container.hpp>
#include <godot_cpp/classes/h_separator.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/style_box_flat.hpp>
#include <godot_cpp/classes/style_box_line.hpp>
#include <godot_cpp/classes/text_server.hpp>
#include <godot_cpp/classes/font_file.hpp>
#include <godot_cpp/classes/system_font.hpp>
#include <godot_cpp/classes/theme_db.hpp>
#include <godot_cpp/classes/rich_text_label.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/label.hpp>
#include <algorithm>
#include <cmath>
#include <string>

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

    // Métodos para trocar as imagens de fundo do menu de Configurações e do Submenu Principal
    ClassDB::bind_method(D_METHOD("definir_imagem_fundo_config", "caminho_textura"), &GerenciadorInterface::definir_imagem_fundo_config);
    ClassDB::bind_method(D_METHOD("definir_imagem_fundo_submenu", "caminho_textura"), &GerenciadorInterface::definir_imagem_fundo_submenu);
    ClassDB::bind_method(D_METHOD("aplicar_tema_global"), &GerenciadorInterface::aplicar_tema_global);

    // Métodos do Submenu "Escolha a Área" em C++
    ClassDB::bind_method(D_METHOD("abrir_painel_areas"), &GerenciadorInterface::abrir_painel_areas);
    ClassDB::bind_method(D_METHOD("fechar_painel_areas"), &GerenciadorInterface::fechar_painel_areas);
    ClassDB::bind_method(D_METHOD("atualizar_estado_area", "id_area", "desbloqueada"), &GerenciadorInterface::atualizar_estado_area);
    ClassDB::bind_method(D_METHOD("selecionar_area", "id_area"), &GerenciadorInterface::selecionar_area);

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
      caminhoFundoConfig(String::utf8("res://imagens/Icex_padrao.jpeg")),
      caminhoFundoSubmenu(String::utf8("res://imagens/Quadro_prob.jpeg")),
      caminhoFonteGlobal(String::utf8("res://fontes/PixelArt.ttf")),
      painelConfig(nullptr),
      labelVolumeConfig(nullptr),
      labelResolucaoConfig(nullptr),
      sliderVolumeConfig(nullptr),
      caixaResolucaoConfig(nullptr),
      botaoVoltarConfig(nullptr),
      painelAreas(nullptr),
      painelMovimentos(nullptr),
      labelStatusMovimentos(nullptr),
      slotMovimentoSelecionado(0)
{
    for (int i = 0; i < TOTAL_AREAS; i++) {
        cardsAreas[i] = nullptr;
        labelsAreas[i] = nullptr;
        botoesAreas[i] = nullptr;
    }
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

// ---------------------------------------------------------------------------
// Métodos de estilização em C++ (baseados na tela de Upgrades Permanentes)
// ---------------------------------------------------------------------------

Ref<StyleBoxFlat> GerenciadorInterface::criar_estilo_painel_principal(float opacidade) const
{
    Ref<StyleBoxFlat> estilo;
    estilo.instantiate();

    // Mesmas cores de Telaupgrades.gd: fundo #101a2b e borda #355371
    estilo->set_bg_color(Color(0.063f, 0.102f, 0.169f, opacidade));
    estilo->set_border_color(Color(0.208f, 0.325f, 0.443f, 1.0f));
    estilo->set_border_width_all(2);
    estilo->set_corner_radius_all(18);
    return estilo;
}

Ref<StyleBoxFlat> GerenciadorInterface::criar_estilo_card_interno(bool destacado) const
{
    Ref<StyleBoxFlat> estilo;
    estilo.instantiate();

    // Mesmas cores do card interno de Telaupgrades.gd: fundo #17263b e borda #2d4662
    if (destacado) {
        estilo->set_bg_color(Color(0.110f, 0.192f, 0.290f, 1.0f));
        estilo->set_border_color(Color(0.439f, 0.882f, 0.847f, 1.0f));
        estilo->set_border_width_all(2);
    } else {
        estilo->set_bg_color(Color(0.090f, 0.149f, 0.231f, 1.0f));
        estilo->set_border_color(Color(0.176f, 0.275f, 0.384f, 1.0f));
        estilo->set_border_width_all(1);
    }

    estilo->set_corner_radius_all(12);
    estilo->set_content_margin(SIDE_LEFT, 18.0f);
    estilo->set_content_margin(SIDE_RIGHT, 18.0f);
    estilo->set_content_margin(SIDE_TOP, 14.0f);
    estilo->set_content_margin(SIDE_BOTTOM, 14.0f);
    return estilo;
}

Ref<StyleBoxFlat> GerenciadorInterface::criar_estilo_botao(const Color& cor_fundo, const Color& cor_borda) const
{
    Ref<StyleBoxFlat> estilo;
    estilo.instantiate();

    estilo->set_bg_color(cor_fundo);
    estilo->set_border_color(cor_borda);
    estilo->set_border_width_all(1);
    estilo->set_corner_radius_all(10);
    estilo->set_content_margin(SIDE_LEFT, 14.0f);
    estilo->set_content_margin(SIDE_RIGHT, 14.0f);
    estilo->set_content_margin(SIDE_TOP, 8.0f);
    estilo->set_content_margin(SIDE_BOTTOM, 8.0f);
    return estilo;
}

void GerenciadorInterface::estilizar_botao(Button* botao, const Color& cor_normal, const Color& cor_hover, int tamanho_fonte) const
{
    if (!botao) return;

    Ref<StyleBoxFlat> normal = criar_estilo_botao(cor_normal, cor_normal.darkened(0.3f));
    Ref<StyleBoxFlat> hover = criar_estilo_botao(cor_hover, cor_hover.lightened(0.15f));
    Ref<StyleBoxFlat> pressed = criar_estilo_botao(cor_normal.darkened(0.2f), Color(0.439f, 0.882f, 0.847f, 1.0f));
    Ref<StyleBoxFlat> disabled = criar_estilo_botao(Color(0.149f, 0.200f, 0.275f, 1.0f), Color(0.208f, 0.267f, 0.357f, 1.0f));

    botao->add_theme_stylebox_override("normal", normal);
    botao->add_theme_stylebox_override("hover", hover);
    botao->add_theme_stylebox_override("pressed", pressed);
    botao->add_theme_stylebox_override("focus", hover);
    botao->add_theme_stylebox_override("disabled", disabled);

    botao->add_theme_color_override("font_color", Color(0.953f, 0.965f, 1.000f, 1.0f));
    botao->add_theme_color_override("font_hover_color", Color(1.0f, 1.0f, 1.0f, 1.0f));
    botao->add_theme_color_override("font_disabled_color", Color(0.537f, 0.592f, 0.667f, 1.0f));
    botao->add_theme_font_size_override("font_size", tamanho_fonte);
    if (fontePixelArt.is_valid()) {
        botao->add_theme_font_override("font", fontePixelArt);
    }
}

void GerenciadorInterface::estilizar_botao_destaque(Button* botao, bool ativo, int tamanho_fonte) const
{
    if (!botao) return;

    if (!ativo) {
        // Cor padrão de botão secundário (#1b2b43 / #344b69)
        estilizar_botao(botao, Color(0.106f, 0.169f, 0.263f, 1.0f), Color(0.204f, 0.294f, 0.412f, 1.0f), tamanho_fonte);
        return;
    }

    // Botão selecionado ou equipado com borda ciano (#70e1d8)
    Color corCiano(0.439f, 0.882f, 0.847f, 1.0f);
    Ref<StyleBoxFlat> normal = criar_estilo_botao(Color(0.090f, 0.235f, 0.227f, 1.0f), corCiano);
    Ref<StyleBoxFlat> hover = criar_estilo_botao(Color(0.141f, 0.349f, 0.333f, 1.0f), corCiano.lightened(0.2f));
    normal->set_border_width_all(2);
    hover->set_border_width_all(2);

    botao->add_theme_stylebox_override("normal", normal);
    botao->add_theme_stylebox_override("hover", hover);
    botao->add_theme_stylebox_override("pressed", normal);
    botao->add_theme_stylebox_override("focus", hover);

    botao->add_theme_color_override("font_color", Color(0.612f, 0.894f, 0.851f, 1.0f));
    botao->add_theme_color_override("font_hover_color", Color(1.0f, 1.0f, 1.0f, 1.0f));
    botao->add_theme_font_size_override("font_size", tamanho_fonte);
    if (fontePixelArt.is_valid()) {
        botao->add_theme_font_override("font", fontePixelArt);
    }
}

void GerenciadorInterface::aplicar_tema_padrao_submenu(
    PanelContainer* painel,
    Label* titulo,
    Label* subtitulo,
    HSeparator* separador,
    Button* botao_voltar,
    float opacidade) const
{
    if (painel) {
        painel->add_theme_stylebox_override("panel", criar_estilo_painel_principal(opacidade));
    }
    if (titulo) {
        titulo->set_horizontal_alignment(HORIZONTAL_ALIGNMENT_CENTER);
        titulo->add_theme_font_size_override("font_size", 26);
        titulo->add_theme_color_override("font_color", Color(0.439f, 0.882f, 0.847f, 1.0f)); // #70e1d8
        if (fontePixelArt.is_valid()) {
            titulo->add_theme_font_override("font", fontePixelArt);
        }
    }
    if (subtitulo) {
        subtitulo->set_horizontal_alignment(HORIZONTAL_ALIGNMENT_CENTER);
        subtitulo->set_autowrap_mode(TextServer::AUTOWRAP_WORD_SMART);
        subtitulo->add_theme_font_size_override("font_size", 17);
        subtitulo->add_theme_color_override("font_color", Color(0.910f, 0.941f, 1.000f, 1.0f)); // #e8f0ff
        if (fontePixelArt.is_valid()) {
            subtitulo->add_theme_font_override("font", fontePixelArt);
        }
    }
    if (separador) {
        Ref<StyleBoxLine> linha;
        linha.instantiate();
        linha->set_color(Color(0.176f, 0.275f, 0.384f, 1.0f));
        linha->set_thickness(1);
        separador->add_theme_stylebox_override("separator", linha);
    }
    if (botao_voltar) {
        botao_voltar->set_custom_minimum_size(Vector2(0, 50));
        estilizar_botao(botao_voltar, Color(0.106f, 0.169f, 0.263f, 1.0f), Color(0.204f, 0.294f, 0.412f, 1.0f), 16);
    }
}

// ---------------------------------------------------------------------------
// Tema Global e Tipografia Estilo Pixel Art via C++
// ---------------------------------------------------------------------------

Ref<Font> GerenciadorInterface::carregar_fonte_pixel_art() const
{
    ResourceLoader* loader = ResourceLoader::get_singleton();
    if (loader && !caminhoFonteGlobal.is_empty() && loader->exists(caminhoFonteGlobal)) {
        Ref<Resource> recurso = loader->load(caminhoFonteGlobal);
        Ref<Font> fonteArquivo = recurso;
        if (fonteArquivo.is_valid()) {
            FontFile* arquivoDinamico = Object::cast_to<FontFile>(fonteArquivo.ptr());
            if (arquivoDinamico) {
                arquivoDinamico->set_antialiasing(TextServer::FONT_ANTIALIASING_NONE);
                arquivoDinamico->set_subpixel_positioning(TextServer::SUBPIXEL_POSITIONING_DISABLED);
                arquivoDinamico->set_generate_mipmaps(false);
            }
            return fonteArquivo;
        }
    }

    // Caso o arquivo res://fontes/PixelArt.ttf ainda não exista no projeto,
    // constrói uma fonte de sistema monoespaçada sem antialiasing para simular pixel art.
    Ref<SystemFont> fonteSistema;
    fonteSistema.instantiate();

    PackedStringArray familias;
    familias.push_back("Consolas");
    familias.push_back("Lucida Console");
    familias.push_back("Courier New");
    familias.push_back("Monospace");

    fonteSistema->set_font_names(familias);
    fonteSistema->set_antialiasing(TextServer::FONT_ANTIALIASING_NONE);
    fonteSistema->set_subpixel_positioning(TextServer::SUBPIXEL_POSITIONING_DISABLED);
    fonteSistema->set_hinting(TextServer::HINTING_NORMAL);
    fonteSistema->set_force_autohinter(true);
    fonteSistema->set_font_weight(600);
    fonteSistema->set_generate_mipmaps(false);

    return fonteSistema;
}

void GerenciadorInterface::aplicar_fonte_nos_filhos(Node* no_raiz) const
{
    if (!no_raiz || !fontePixelArt.is_valid()) return;

    Control* controle = Object::cast_to<Control>(no_raiz);
    if (controle && controle->get_parent() == this && temaGlobal.is_valid()) {
        controle->set_theme(temaGlobal);
    }

    Button* botao = Object::cast_to<Button>(no_raiz);
    if (botao) {
        botao->add_theme_font_override("font", fontePixelArt);
    }

    Label* rotulo = Object::cast_to<Label>(no_raiz);
    if (rotulo) {
        rotulo->add_theme_font_override("font", fontePixelArt);
    }

    RichTextLabel* textoRico = Object::cast_to<RichTextLabel>(no_raiz);
    if (textoRico) {
        textoRico->add_theme_font_override("normal_font", fontePixelArt);
        textoRico->add_theme_font_override("bold_font", fontePixelArt);
    }

    int totalFilhos = no_raiz->get_child_count();
    for (int i = 0; i < totalFilhos; i++) {
        aplicar_fonte_nos_filhos(no_raiz->get_child(i));
    }
}

void GerenciadorInterface::aplicar_tema_global()
{
    fontePixelArt = carregar_fonte_pixel_art();
    if (!fontePixelArt.is_valid()) return;

    if (!temaGlobal.is_valid()) {
        temaGlobal.instantiate();
    }

    temaGlobal->set_default_font(fontePixelArt);
    temaGlobal->set_default_font_size(16);

    const char* tiposControles[] = {"Button", "Label", "OptionButton", "PopupMenu"};
    for (const char* tipo : tiposControles) {
        temaGlobal->set_font("font", tipo, fontePixelArt);
        temaGlobal->set_font_size("font_size", tipo, 16);
    }
    temaGlobal->set_font("normal_font", "RichTextLabel", fontePixelArt);
    temaGlobal->set_font("bold_font", "RichTextLabel", fontePixelArt);
    temaGlobal->set_font_size("normal_font_size", "RichTextLabel", 16);

    // Aplica também no ThemeDB padrão do Godot para unificar todas as cenas (incluindo Batalha e Upgrades)
    ThemeDB* bancoTemas = ThemeDB::get_singleton();
    if (bancoTemas) {
        bancoTemas->set_fallback_font(fontePixelArt);
        bancoTemas->set_fallback_font_size(16);

        Ref<Theme> temaPadrao = bancoTemas->get_default_theme();
        if (temaPadrao.is_valid()) {
            temaPadrao->set_default_font(fontePixelArt);
            temaPadrao->set_default_font_size(16);
            for (const char* tipo : tiposControles) {
                temaPadrao->set_font("font", tipo, fontePixelArt);
            }
            temaPadrao->set_font("normal_font", "RichTextLabel", fontePixelArt);
            temaPadrao->set_font("bold_font", "RichTextLabel", fontePixelArt);
        }
    }

    aplicar_fonte_nos_filhos(this);
}

void GerenciadorInterface::estilizar_botoes_menus_principais()
{
    const char* caminhosMenuInicial[] = {
        "ContainerBotoes/BotaoJogar",
        "ContainerBotoes/BotaoConfig",
        "ContainerBotoes/BotaoSair"
    };
    const char* textosMenuInicial[] = {
        "Jogar",
        "Configurações",
        "Sair"
    };

    for (int i = 0; i < 3; i++) {
        Button* btn = has_node(caminhosMenuInicial[i]) ? get_node<Button>(caminhosMenuInicial[i]) : nullptr;
        if (!btn) continue;
        btn->set_text(String::utf8(textosMenuInicial[i]));
        btn->set_custom_minimum_size(Vector2(0, 62));
        estilizar_botao(btn, Color(0.063f, 0.102f, 0.169f, 0.88f), Color(0.141f, 0.306f, 0.451f, 0.95f), 28);
    }

    const char* caminhosSubmenuJogar[] = {
        "ContainerJogar/BotaoMundo",
        "ContainerJogar/BotaoMovimentos",
        "ContainerJogar/BotaoUpgrades",
        "ContainerJogar/BotaoItens",
        "ContainerJogar/BotaoVoltar"
    };
    const char* textosSubmenuJogar[] = {
        "Mundo",
        "Movimentos",
        "Upgrades",
        "Itens",
        "Voltar"
    };

    for (int i = 0; i < 5; i++) {
        Button* btn = has_node(caminhosSubmenuJogar[i]) ? get_node<Button>(caminhosSubmenuJogar[i]) : nullptr;
        if (!btn) continue;
        btn->set_text(String::utf8(textosSubmenuJogar[i]));
        btn->set_custom_minimum_size(Vector2(260, 54));
        estilizar_botao(btn, Color(0.063f, 0.102f, 0.169f, 0.88f), Color(0.141f, 0.306f, 0.451f, 0.95f), 24);
    }
}

void GerenciadorInterface::estilizar_titulo_principal()
{
    Label* titulo = has_node("Titulo") ? get_node<Label>("Titulo") : nullptr;
    if (!titulo) return;

    titulo->set_text(String::utf8("Fuja do ICEX"));
    titulo->set_horizontal_alignment(HORIZONTAL_ALIGNMENT_CENTER);
    titulo->add_theme_font_size_override("font_size", 56);
    titulo->add_theme_color_override("font_color", Color(0.92f, 0.97f, 1.0f, 1.0f));
    titulo->add_theme_color_override("font_outline_color", Color(0.06f, 0.10f, 0.17f, 1.0f));
    titulo->add_theme_constant_override("outline_size", 8);
    if (fontePixelArt.is_valid()) {
        titulo->add_theme_font_override("font", fontePixelArt);
    }
}

// ---------------------------------------------------------------------------
// Inicialização dos botões e reformulação do menu de Configurações
// ---------------------------------------------------------------------------

void GerenciadorInterface::inicializar_componentes()
{
    aplicar_tema_global();
    estilizar_titulo_principal();
    estilizar_botoes_menus_principais();

    Button* botao_jogar = has_node("ContainerBotoes/BotaoJogar") ? get_node<Button>("ContainerBotoes/BotaoJogar") : nullptr;
    Button* botao_config = has_node("ContainerBotoes/BotaoConfig") ? get_node<Button>("ContainerBotoes/BotaoConfig") : nullptr;
    Button* botao_sair = has_node("ContainerBotoes/BotaoSair") ? get_node<Button>("ContainerBotoes/BotaoSair") : nullptr;

    if (!botaoVoltarConfig && has_node("ContainerConfig/BotaoVoltar"))
        botaoVoltarConfig = get_node<Button>("ContainerConfig/BotaoVoltar");
    if (!sliderVolumeConfig && has_node("ContainerConfig/SliderVolume"))
        sliderVolumeConfig = get_node<HSlider>("ContainerConfig/SliderVolume");
    if (!caixaResolucaoConfig && has_node("ContainerConfig/CaixaResolucao"))
        caixaResolucaoConfig = get_node<OptionButton>("ContainerConfig/CaixaResolucao");

    Button* botao_quests = has_node("ContainerJogar/BotaoQuests") ? get_node<Button>("ContainerJogar/BotaoQuests") : nullptr;
    Button* botao_voltar_jogar = has_node("ContainerJogar/BotaoVoltar") ? get_node<Button>("ContainerJogar/BotaoVoltar") : nullptr;

    if (caixaResolucaoConfig) {
        caixaResolucaoConfig->clear();
        caixaResolucaoConfig->add_item("640x480");
        caixaResolucaoConfig->add_item("1280x720");
        caixaResolucaoConfig->add_item("1280x960");
        caixaResolucaoConfig->add_item("1920x1080");
        caixaResolucaoConfig->add_item("1920x1440");
        caixaResolucaoConfig->add_item("2560x1440");
        caixaResolucaoConfig->add_item("Tela Cheia (Exclusive)");
        caixaResolucaoConfig->add_item("Tela Cheia (Borderless)");
    }

    if (botao_jogar && !botao_jogar->is_connected("pressed", Callable(this, "clicar_jogar")))
        botao_jogar->connect("pressed", Callable(this, "clicar_jogar"));
    if (botao_config && !botao_config->is_connected("pressed", Callable(this, "clicar_configuracoes")))
        botao_config->connect("pressed", Callable(this, "clicar_configuracoes"));
    if (botao_sair && !botao_sair->is_connected("pressed", Callable(this, "clicar_sair")))
        botao_sair->connect("pressed", Callable(this, "clicar_sair"));
    if (botaoVoltarConfig && !botaoVoltarConfig->is_connected("pressed", Callable(this, "clicar_voltar")))
        botaoVoltarConfig->connect("pressed", Callable(this, "clicar_voltar"));

    // BotaoMundo é conectado pelo cena_principal.gd, que abre o seletor de áreas.
    if (botao_quests && !botao_quests->is_connected("pressed", Callable(this, "clicar_quests")))
        botao_quests->connect("pressed", Callable(this, "clicar_quests"));
    if (botao_voltar_jogar && !botao_voltar_jogar->is_connected("pressed", Callable(this, "clicar_voltar")))
        botao_voltar_jogar->connect("pressed", Callable(this, "clicar_voltar"));

    if (sliderVolumeConfig && !sliderVolumeConfig->is_connected("value_changed", Callable(this, "mudar_volume")))
        sliderVolumeConfig->connect("value_changed", Callable(this, "mudar_volume"));

    estilizar_menu_configuracoes();
    definir_imagem_fundo_submenu(caminhoFundoSubmenu);
    criar_painel_areas();
    criar_painel_movimentos();
    aplicar_fonte_nos_filhos(this);
}

void GerenciadorInterface::definir_imagem_fundo_config(const String& caminho_textura)
{
    String caminho = caminho_textura.strip_edges();
    if (caminho.is_empty()) {
        caminho = caminhoFundoConfig;
    }

    TextureRect* fundo_config = has_node("FundoConfig") ? get_node<TextureRect>("FundoConfig") : nullptr;
    if (!fundo_config) return;

    ResourceLoader* loader = ResourceLoader::get_singleton();
    if (!loader) return;

    Ref<Resource> recurso = loader->load(caminho);
    Ref<Texture2D> textura = recurso;
    if (textura.is_valid()) {
        fundo_config->set_expand_mode(TextureRect::EXPAND_IGNORE_SIZE);
        fundo_config->set_texture(textura);
        caminhoFundoConfig = caminho;
    } else {
        UtilityFunctions::push_warning(String::utf8("Não foi possível carregar a textura de fundo: ") + caminho);
    }
}

void GerenciadorInterface::definir_imagem_fundo_submenu(const String& caminho_textura)
{
    String caminho = caminho_textura.strip_edges();
    if (caminho.is_empty()) {
        caminho = caminhoFundoSubmenu;
    }

    TextureRect* fundo_jogo = has_node("FundoJogo") ? get_node<TextureRect>("FundoJogo") : nullptr;
    if (!fundo_jogo) return;

    ResourceLoader* loader = ResourceLoader::get_singleton();
    if (!loader) return;

    Ref<Resource> recurso = loader->load(caminho);
    Ref<Texture2D> textura = recurso;
    if (textura.is_valid()) {
        fundo_jogo->set_expand_mode(TextureRect::EXPAND_IGNORE_SIZE);
        fundo_jogo->set_texture(textura);
        caminhoFundoSubmenu = caminho;
    } else {
        UtilityFunctions::push_warning(String::utf8("Não foi possível carregar a textura do submenu: ") + caminho);
    }
}

void GerenciadorInterface::atualizar_texto_volume()
{
    if (!labelVolumeConfig) return;
    int valorInt = static_cast<int>(std::round(volume));
    std::string txt = "VOLUME\nNível atual: " + std::to_string(valorInt) + "%";
    labelVolumeConfig->set_text(String::utf8(txt.c_str()));
}

void GerenciadorInterface::estilizar_menu_configuracoes()
{
    if (painelConfig != nullptr) return;

    Control* container_antigo = has_node("ContainerConfig") ? get_node<Control>("ContainerConfig") : nullptr;
    if (!container_antigo) return;

    if (!labelVolumeConfig && has_node("ContainerConfig/LabelVolume"))
        labelVolumeConfig = get_node<Label>("ContainerConfig/LabelVolume");
    if (!sliderVolumeConfig && has_node("ContainerConfig/SliderVolume"))
        sliderVolumeConfig = get_node<HSlider>("ContainerConfig/SliderVolume");
    if (!labelResolucaoConfig && has_node("ContainerConfig/LabelResolucao"))
        labelResolucaoConfig = get_node<Label>("ContainerConfig/LabelResolucao");
    if (!caixaResolucaoConfig && has_node("ContainerConfig/CaixaResolucao"))
        caixaResolucaoConfig = get_node<OptionButton>("ContainerConfig/CaixaResolucao");
    if (!botaoVoltarConfig && has_node("ContainerConfig/BotaoVoltar"))
        botaoVoltarConfig = get_node<Button>("ContainerConfig/BotaoVoltar");

    // Cria o painel centralizado sobre o FundoConfig (com opacidade 0.90 pra ver a imagem atrás)
    painelConfig = memnew(PanelContainer);
    painelConfig->set_name("PainelConfig");
    painelConfig->set_visible(false);
    add_child(painelConfig);

    painelConfig->set_anchors_preset(Control::PRESET_FULL_RECT);
    painelConfig->set_anchor(SIDE_LEFT, 0.15f);
    painelConfig->set_anchor(SIDE_TOP, 0.08f);
    painelConfig->set_anchor(SIDE_RIGHT, 0.85f);
    painelConfig->set_anchor(SIDE_BOTTOM, 0.92f);
    painelConfig->set_offset(SIDE_LEFT, 0.0f);
    painelConfig->set_offset(SIDE_TOP, 0.0f);
    painelConfig->set_offset(SIDE_RIGHT, 0.0f);
    painelConfig->set_offset(SIDE_BOTTOM, 0.0f);
    painelConfig->set_h_grow_direction(Control::GROW_DIRECTION_BOTH);
    painelConfig->set_v_grow_direction(Control::GROW_DIRECTION_BOTH);

    MarginContainer* conteudo = memnew(MarginContainer);
    conteudo->add_theme_constant_override("margin_left", 34);
    conteudo->add_theme_constant_override("margin_top", 28);
    conteudo->add_theme_constant_override("margin_right", 34);
    conteudo->add_theme_constant_override("margin_bottom", 28);
    painelConfig->add_child(conteudo);

    VBoxContainer* coluna = memnew(VBoxContainer);
    coluna->add_theme_constant_override("separation", 16);
    conteudo->add_child(coluna);

    Label* titulo = memnew(Label);
    titulo->set_text(String::utf8("CONFIGURAÇÕES"));
    coluna->add_child(titulo);

    Label* subtitulo = memnew(Label);
    subtitulo->set_text(String::utf8("Ajustes de áudio e resolução da janela"));
    coluna->add_child(subtitulo);

    HSeparator* separador = memnew(HSeparator);
    coluna->add_child(separador);

    // Card interno de Volume
    PanelContainer* cardVolume = memnew(PanelContainer);
    cardVolume->add_theme_stylebox_override("panel", criar_estilo_card_interno(false));
    coluna->add_child(cardVolume);

    VBoxContainer* boxVolume = memnew(VBoxContainer);
    boxVolume->add_theme_constant_override("separation", 8);
    cardVolume->add_child(boxVolume);

    if (labelVolumeConfig) {
        if (labelVolumeConfig->get_parent())
            labelVolumeConfig->get_parent()->remove_child(labelVolumeConfig);
        boxVolume->add_child(labelVolumeConfig);
        labelVolumeConfig->add_theme_font_size_override("font_size", 19);
        labelVolumeConfig->add_theme_color_override("font_color", Color(0.863f, 0.910f, 0.969f, 1.0f));
    }

    if (sliderVolumeConfig) {
        volume = static_cast<float>(sliderVolumeConfig->get_value());
        if (sliderVolumeConfig->get_parent())
            sliderVolumeConfig->get_parent()->remove_child(sliderVolumeConfig);
        sliderVolumeConfig->set_custom_minimum_size(Vector2(0, 32));
        boxVolume->add_child(sliderVolumeConfig);
    }
    atualizar_texto_volume();

    // Card interno de Resolução
    PanelContainer* cardResolucao = memnew(PanelContainer);
    cardResolucao->add_theme_stylebox_override("panel", criar_estilo_card_interno(false));
    coluna->add_child(cardResolucao);

    VBoxContainer* boxResolucao = memnew(VBoxContainer);
    boxResolucao->add_theme_constant_override("separation", 8);
    cardResolucao->add_child(boxResolucao);

    if (labelResolucaoConfig) {
        if (labelResolucaoConfig->get_parent())
            labelResolucaoConfig->get_parent()->remove_child(labelResolucaoConfig);
        boxResolucao->add_child(labelResolucaoConfig);
        labelResolucaoConfig->set_text(String::utf8("RESOLUÇÃO\nEscolha o tamanho da janela ou tela cheia"));
        labelResolucaoConfig->add_theme_font_size_override("font_size", 19);
        labelResolucaoConfig->add_theme_color_override("font_color", Color(0.863f, 0.910f, 0.969f, 1.0f));
    }

    if (caixaResolucaoConfig) {
        if (caixaResolucaoConfig->get_parent())
            caixaResolucaoConfig->get_parent()->remove_child(caixaResolucaoConfig);
        caixaResolucaoConfig->set_custom_minimum_size(Vector2(0, 50));
        estilizar_botao(caixaResolucaoConfig, Color(0.141f, 0.306f, 0.451f, 1.0f), Color(0.224f, 0.498f, 0.690f, 1.0f), 16);
        boxResolucao->add_child(caixaResolucaoConfig);
    }

    // Espaço pra empurrar o botão de voltar pro final do painel
    Control* espacador = memnew(Control);
    espacador->set_v_size_flags(Control::SIZE_EXPAND_FILL);
    coluna->add_child(espacador);

    if (botaoVoltarConfig) {
        if (botaoVoltarConfig->get_parent())
            botaoVoltarConfig->get_parent()->remove_child(botaoVoltarConfig);
        botaoVoltarConfig->set_text(String::utf8("Voltar ao menu"));
        coluna->add_child(botaoVoltarConfig);
    }

    aplicar_tema_padrao_submenu(painelConfig, titulo, subtitulo, separador, botaoVoltarConfig, 0.90f);
    definir_imagem_fundo_config(caminhoFundoConfig);
    container_antigo->set_visible(false);
}

// ---------------------------------------------------------------------------
// Submenu "Escolha a Área" em C++
// ---------------------------------------------------------------------------

void GerenciadorInterface::criar_painel_areas()
{
    if (painelAreas != nullptr) return;

    painelAreas = memnew(Control);
    painelAreas->set_name("PainelAreas");
    painelAreas->set_visible(false);
    painelAreas->set_z_index(95);
    add_child(painelAreas);

    painelAreas->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
    painelAreas->set_anchor(SIDE_RIGHT, 1.0f);
    painelAreas->set_anchor(SIDE_BOTTOM, 1.0f);
    painelAreas->set_h_grow_direction(Control::GROW_DIRECTION_BOTH);
    painelAreas->set_v_grow_direction(Control::GROW_DIRECTION_BOTH);

    // Fundo escuro cobrindo a tela anterior (mesma cor da TelaUpgrades.tscn)
    ColorRect* fundo = memnew(ColorRect);
    fundo->set_color(Color(0.035f, 0.055f, 0.09f, 1.0f));
    painelAreas->add_child(fundo);
    fundo->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
    fundo->set_anchor(SIDE_RIGHT, 1.0f);
    fundo->set_anchor(SIDE_BOTTOM, 1.0f);

    PanelContainer* painel = memnew(PanelContainer);
    painelAreas->add_child(painel);
    painel->set_anchors_preset(Control::PRESET_FULL_RECT);
    painel->set_anchor(SIDE_LEFT, 0.15f);
    painel->set_anchor(SIDE_TOP, 0.06f);
    painel->set_anchor(SIDE_RIGHT, 0.85f);
    painel->set_anchor(SIDE_BOTTOM, 0.94f);
    painel->set_offset(SIDE_LEFT, 0.0f);
    painel->set_offset(SIDE_TOP, 0.0f);
    painel->set_offset(SIDE_RIGHT, 0.0f);
    painel->set_offset(SIDE_BOTTOM, 0.0f);
    painel->set_h_grow_direction(Control::GROW_DIRECTION_BOTH);
    painel->set_v_grow_direction(Control::GROW_DIRECTION_BOTH);

    MarginContainer* conteudo = memnew(MarginContainer);
    conteudo->add_theme_constant_override("margin_left", 34);
    conteudo->add_theme_constant_override("margin_top", 24);
    conteudo->add_theme_constant_override("margin_right", 34);
    conteudo->add_theme_constant_override("margin_bottom", 24);
    painel->add_child(conteudo);

    VBoxContainer* coluna = memnew(VBoxContainer);
    coluna->add_theme_constant_override("separation", 14);
    conteudo->add_child(coluna);

    Label* titulo = memnew(Label);
    titulo->set_text(String::utf8("ESCOLHA A ÁREA"));
    coluna->add_child(titulo);

    Label* subtitulo = memnew(Label);
    subtitulo->set_text(String::utf8("Selecione a disciplina para iniciar as batalhas do semestre"));
    coluna->add_child(subtitulo);

    HSeparator* separador = memnew(HSeparator);
    coluna->add_child(separador);

    ScrollContainer* scroll = memnew(ScrollContainer);
    scroll->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    scroll->set_v_size_flags(Control::SIZE_EXPAND_FILL);
    scroll->set_horizontal_scroll_mode(ScrollContainer::SCROLL_MODE_DISABLED);
    coluna->add_child(scroll);

    VBoxContainer* listaAreas = memnew(VBoxContainer);
    listaAreas->add_theme_constant_override("separation", 12);
    listaAreas->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    scroll->add_child(listaAreas);

    for (int id = 0; id < TOTAL_AREAS; id++) {
        PanelContainer* card = memnew(PanelContainer);
        card->set_h_size_flags(Control::SIZE_EXPAND_FILL);
        card->add_theme_stylebox_override("panel", criar_estilo_card_interno(false));
        listaAreas->add_child(card);
        cardsAreas[id] = card;

        VBoxContainer* boxCard = memnew(VBoxContainer);
        boxCard->add_theme_constant_override("separation", 8);
        boxCard->set_h_size_flags(Control::SIZE_EXPAND_FILL);
        card->add_child(boxCard);

        Label* infoArea = memnew(Label);
        infoArea->set_autowrap_mode(TextServer::AUTOWRAP_WORD_SMART);
        infoArea->set_h_size_flags(Control::SIZE_EXPAND_FILL);
        infoArea->add_theme_font_size_override("font_size", 16);
        infoArea->add_theme_color_override("font_color", Color(0.863f, 0.910f, 0.969f, 1.0f));
        if (fontePixelArt.is_valid()) {
            infoArea->add_theme_font_override("font", fontePixelArt);
        }
        boxCard->add_child(infoArea);
        labelsAreas[id] = infoArea;

        Button* botaoArea = memnew(Button);
        botaoArea->set_custom_minimum_size(Vector2(0, 50));
        botaoArea->set_h_size_flags(Control::SIZE_EXPAND_FILL);
        botaoArea->connect("pressed", Callable(this, "selecionar_area").bind(id));
        boxCard->add_child(botaoArea);
        botoesAreas[id] = botaoArea;
    }

    Button* botaoVoltar = memnew(Button);
    botaoVoltar->set_text(String::utf8("Voltar ao menu"));
    botaoVoltar->connect("pressed", Callable(this, "fechar_painel_areas"));
    coluna->add_child(botaoVoltar);

    aplicar_tema_padrao_submenu(painel, titulo, subtitulo, separador, botaoVoltar, 1.0f);

    for (int id = 0; id < TOTAL_AREAS; id++) {
        atualizar_estado_area(id, id == 0);
    }
}

void GerenciadorInterface::atualizar_estado_area(int id_area, bool desbloqueada)
{
    if (id_area < 0 || id_area >= TOTAL_AREAS) return;
    if (painelAreas == nullptr) {
        criar_painel_areas();
    }

    const char* nomes[TOTAL_AREAS] = {
        "Cálculo",
        "GAAL / Análise Numérica",
        "Física",
        "PDS",
        "Luciana (secreta)"
    };

    const char* descricoes[TOTAL_AREAS] = {
        "ÁREA 1  ·  Ciclo Básico de Matemática",
        "ÁREA 2  ·  Vetores, Matrizes e Métodos Numéricos",
        "ÁREA 3  ·  Mecânica e Eletromagnetismo",
        "ÁREA 4  ·  Programação e Desenvolvimento de Software II",
        "PÓS-JOGO  ·  Um desafio secreto aguarda no ICEx"
    };

    // A área secreta fica oculta até a vitória de Fernando estar salva.
    if (id_area == 4 && cardsAreas[id_area]) {
        cardsAreas[id_area]->set_visible(desbloqueada);
    }

    if (labelsAreas[id_area]) {
        std::string status = desbloqueada ? "Disponível" : "Bloqueada — vença o chefe da área anterior";
        std::string textoLabel = std::string(descricoes[id_area]) + "  (" + status + ")";
        labelsAreas[id_area]->set_text(String::utf8(textoLabel.c_str()));
    }

    if (botoesAreas[id_area]) {
        Button* btn = botoesAreas[id_area];
        btn->set_disabled(!desbloqueada);
        if (desbloqueada) {
            btn->set_text(String::utf8(nomes[id_area]));
        } else {
            std::string txtBloqueada = std::string(nomes[id_area]) + " (bloqueada)";
            btn->set_text(String::utf8(txtBloqueada.c_str()));
        }
        estilizar_botao(btn, Color(0.141f, 0.306f, 0.451f, 1.0f), Color(0.224f, 0.498f, 0.690f, 1.0f), 16);
    }
}

void GerenciadorInterface::abrir_painel_areas()
{
    if (painelAreas == nullptr) {
        criar_painel_areas();
    }
    irPara(GerenciadorMenu::EstadoMenu::MENU_AREAS);
    atualizar_telas();
    if (painelAreas) {
        painelAreas->set_visible(true);
        painelAreas->move_to_front();
    }
}

void GerenciadorInterface::fechar_painel_areas()
{
    if (painelAreas) {
        painelAreas->set_visible(false);
    }
    irPara(GerenciadorMenu::EstadoMenu::MENU_JOGAR);
    atualizar_telas();
}

void GerenciadorInterface::selecionar_area(int id_area)
{
    if (id_area < 0 || id_area >= TOTAL_AREAS) return;
    if (has_method("_on_area_escolhida")) {
        call("_on_area_escolhida", id_area);
    }
}

// ---------------------------------------------------------------------------
// Submenu "Grade Curricular de Movimentos (ICEx / UFMG)" em C++
// ---------------------------------------------------------------------------

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

    // Fundo escuro cobrindo toda a tela (mesma cor da TelaUpgrades.tscn)
    ColorRect* fundo = memnew(ColorRect);
    fundo->set_color(Color(0.035f, 0.055f, 0.09f, 1.0f));
    painelMovimentos->add_child(fundo);
    fundo->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
    fundo->set_anchor(SIDE_RIGHT, 1.0f);
    fundo->set_anchor(SIDE_BOTTOM, 1.0f);

    // Painel centralizado no padrão de Upgrades Permanentes
    PanelContainer* painel = memnew(PanelContainer);
    painelMovimentos->add_child(painel);
    painel->set_anchors_preset(Control::PRESET_FULL_RECT);
    painel->set_anchor(SIDE_LEFT, 0.10f);
    painel->set_anchor(SIDE_TOP, 0.05f);
    painel->set_anchor(SIDE_RIGHT, 0.90f);
    painel->set_anchor(SIDE_BOTTOM, 0.95f);
    painel->set_offset(SIDE_LEFT, 0.0f);
    painel->set_offset(SIDE_TOP, 0.0f);
    painel->set_offset(SIDE_RIGHT, 0.0f);
    painel->set_offset(SIDE_BOTTOM, 0.0f);
    painel->set_h_grow_direction(Control::GROW_DIRECTION_BOTH);
    painel->set_v_grow_direction(Control::GROW_DIRECTION_BOTH);

    MarginContainer* margem = memnew(MarginContainer);
    margem->add_theme_constant_override("margin_left", 32);
    margem->add_theme_constant_override("margin_top", 22);
    margem->add_theme_constant_override("margin_right", 32);
    margem->add_theme_constant_override("margin_bottom", 22);
    painel->add_child(margem);

    VBoxContainer* caixaPrincipal = memnew(VBoxContainer);
    caixaPrincipal->add_theme_constant_override("separation", 12);
    caixaPrincipal->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    caixaPrincipal->set_v_size_flags(Control::SIZE_EXPAND_FILL);
    margem->add_child(caixaPrincipal);

    Label* titulo = memnew(Label);
    titulo->set_text(String::utf8("GRADE CURRICULAR DE MOVIMENTOS (ICEx / UFMG)"));
    caixaPrincipal->add_child(titulo);

    labelStatusMovimentos = memnew(Label);
    caixaPrincipal->add_child(labelStatusMovimentos);

    HSeparator* separador = memnew(HSeparator);
    caixaPrincipal->add_child(separador);

    // Card superior com os 3 Slots equipados do Estudante
    PanelContainer* cardSlots = memnew(PanelContainer);
    cardSlots->add_theme_stylebox_override("panel", criar_estilo_card_interno(false));
    caixaPrincipal->add_child(cardSlots);

    HBoxContainer* barraSlots = memnew(HBoxContainer);
    barraSlots->add_theme_constant_override("separation", 12);
    barraSlots->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    cardSlots->add_child(barraSlots);

    for (int i = 0; i < 3; i++) {
        Button* btnSlot = memnew(Button);
        btnSlot->set_custom_minimum_size(Vector2(0, 60));
        btnSlot->set_h_size_flags(Control::SIZE_EXPAND_FILL);
        btnSlot->connect("pressed", Callable(this, "selecionar_slot_movimento").bind(i));
        barraSlots->add_child(btnSlot);
        botoesSlot[i] = btnSlot;
    }

    // Lista rolável com cards para os 12 movimentos do catálogo C++
    ScrollContainer* scroll = memnew(ScrollContainer);
    scroll->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    scroll->set_v_size_flags(Control::SIZE_EXPAND_FILL);
    scroll->set_horizontal_scroll_mode(ScrollContainer::SCROLL_MODE_DISABLED);
    caixaPrincipal->add_child(scroll);

    VBoxContainer* listaCatalogo = memnew(VBoxContainer);
    listaCatalogo->add_theme_constant_override("separation", 10);
    listaCatalogo->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    scroll->add_child(listaCatalogo);

    cardsCatalogo.clear();
    labelsCatalogo.clear();
    botoesCatalogo.clear();

    const auto& catalogo = Movimento::getCatalogoMovimentos();
    for (int id = 0; id < static_cast<int>(catalogo.size()); id++) {
        PanelContainer* cardMov = memnew(PanelContainer);
        cardMov->set_h_size_flags(Control::SIZE_EXPAND_FILL);
        cardMov->add_theme_stylebox_override("panel", criar_estilo_card_interno(false));
        listaCatalogo->add_child(cardMov);
        cardsCatalogo.push_back(cardMov);

        VBoxContainer* boxMov = memnew(VBoxContainer);
        boxMov->add_theme_constant_override("separation", 8);
        boxMov->set_h_size_flags(Control::SIZE_EXPAND_FILL);
        cardMov->add_child(boxMov);

        Label* labelMov = memnew(Label);
        labelMov->set_autowrap_mode(TextServer::AUTOWRAP_WORD_SMART);
        labelMov->set_h_size_flags(Control::SIZE_EXPAND_FILL);
        labelMov->add_theme_font_size_override("font_size", 16);
        labelMov->add_theme_color_override("font_color", Color(0.863f, 0.910f, 0.969f, 1.0f));
        if (fontePixelArt.is_valid()) {
            labelMov->add_theme_font_override("font", fontePixelArt);
        }
        boxMov->add_child(labelMov);
        labelsCatalogo.push_back(labelMov);

        Button* btnMov = memnew(Button);
        btnMov->set_custom_minimum_size(Vector2(0, 50));
        btnMov->set_h_size_flags(Control::SIZE_EXPAND_FILL);
        btnMov->connect("pressed", Callable(this, "escolher_movimento_catalogo").bind(id));
        boxMov->add_child(btnMov);
        botoesCatalogo.push_back(btnMov);
    }

    Button* btnVoltar = memnew(Button);
    btnVoltar->set_text(String::utf8("Confirmar Movimentos e Voltar ao Menu"));
    btnVoltar->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    btnVoltar->connect("pressed", Callable(this, "clicar_fechar_movimentos"));
    caixaPrincipal->add_child(btnVoltar);

    aplicar_tema_padrao_submenu(painel, titulo, labelStatusMovimentos, separador, btnVoltar, 1.0f);
    titulo->add_theme_font_size_override("font_size", 24);
    labelStatusMovimentos->add_theme_font_size_override("font_size", 16);

    atualizar_painel_movimentos();
}

void GerenciadorInterface::atualizar_painel_movimentos()
{
    if (!painelMovimentos) return;

    const auto& catalogo = Movimento::getCatalogoMovimentos();
    if (catalogo.empty()) {
        UtilityFunctions::push_error(String::utf8("Não foi possível atualizar os movimentos: catálogo vazio."));
        for (Button* btn : botoesSlot) {
            if (btn) btn->set_text(String::utf8("Movimentos indisponíveis"));
        }
        return;
    }

    for (int i = 0; i < 3; i++) {
        if (!botoesSlot[i]) continue;
        int idEquipado = Herois::getMovimentoEscolhido(i);
        if (idEquipado < 0 || idEquipado >= static_cast<int>(catalogo.size())) {
            UtilityFunctions::push_warning(String::utf8("Movimento equipado inválido; restaurando o primeiro movimento do catálogo."));
            Herois::definirMovimentoEscolhido(i, 0);
            idEquipado = 0;
        }
        const Movimento& m = catalogo[idEquipado];
        bool selecionado = (i == slotMovimentoSelecionado);

        std::string prefixo = selecionado ? ">> [EDITANDO SLOT " : "[SLOT ";
        std::string sufixo = selecionado ? "] <<" : "]";
        std::string textoSlot = prefixo + std::to_string(i + 1) + sufixo + "\n" +
                                m.getNome() + " (" + m.getDisciplina() + ")";
        botoesSlot[i]->set_text(String::utf8(textoSlot.c_str()));
        estilizar_botao_destaque(botoesSlot[i], selecionado, 15);
    }

    if (labelStatusMovimentos) {
        int vitorias = Herois::getVitoriasJogador();
        std::string msg = "Escolhendo para o SLOT " + std::to_string(slotMovimentoSelecionado + 1) +
                          "  (Vitórias: " + std::to_string(vitorias) +
                          ") — Clique em uma habilidade liberada para equipar:";
        labelStatusMovimentos->set_text(String::utf8(msg.c_str()));
    }

    for (int id = 0; id < static_cast<int>(botoesCatalogo.size()) && id < static_cast<int>(catalogo.size()); id++) {
        Button* btn = botoesCatalogo[id];
        if (!btn) continue;

        const Movimento& m = catalogo[id];
        bool desbloqueado = Herois::movimentoEstaDesbloqueado(id);
        int slotOndeEsta = Herois::encontrarSlotDoMovimento(id);
        bool equipado = desbloqueado && (slotOndeEsta != -1);

        if (id < static_cast<int>(cardsCatalogo.size()) && cardsCatalogo[id]) {
            cardsCatalogo[id]->add_theme_stylebox_override("panel", criar_estilo_card_interno(equipado));
        }

        if (id < static_cast<int>(labelsCatalogo.size()) && labelsCatalogo[id]) {
            std::string textoCard = m.getNome() + "  ·  " + m.getDisciplina() + "\n" + m.getDescricao();
            labelsCatalogo[id]->set_text(String::utf8(textoCard.c_str()));
        }

        btn->set_disabled(!desbloqueado);

        if (!desbloqueado) {
            int req = m.getVitoriasNecessarias();
            std::string palavraVit = (req == 1) ? " vitória" : " vitórias";
            std::string textoBloqueado = "[BLOQUEADO] - Requer " + std::to_string(req) + palavraVit +
                                         "  |  [" + m.getDisciplina() + "] " + m.getNome();
            btn->set_text(String::utf8(textoBloqueado.c_str()));
            estilizar_botao(btn, Color(0.141f, 0.306f, 0.451f, 1.0f), Color(0.224f, 0.498f, 0.690f, 1.0f), 16);
        } else {
            std::string statusEquip = "";
            if (equipado) {
                statusEquip = "[EQUIPADO NO SLOT " + std::to_string(slotOndeEsta + 1) + "] ";
            }

            std::string textoBtn = statusEquip + "[" + m.getDisciplina() + "] " + m.getNome() +
                                   "  |  " + m.getResumoEfeito();
            btn->set_text(String::utf8(textoBtn.c_str()));

            if (equipado) {
                estilizar_botao_destaque(btn, true, 16);
            } else {
                estilizar_botao(btn, Color(0.141f, 0.306f, 0.451f, 1.0f), Color(0.224f, 0.498f, 0.690f, 1.0f), 16);
            }
        }
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
    UtilityFunctions::print(String::utf8(">> Abriu a Grade Curricular de Movimentos (C++)"));
}

void GerenciadorInterface::clicar_fechar_movimentos()
{
    if (painelMovimentos) {
        painelMovimentos->set_visible(false);
    }
    irPara(GerenciadorMenu::EstadoMenu::MENU_JOGAR);
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
    if (slotMovimentoSelecionado < 0 || slotMovimentoSelecionado >= 3 ||
        idCatalogo < 0 || idCatalogo >= Movimento::getQuantidadeCatalogo()) {
        UtilityFunctions::push_warning(String::utf8("Seleção de movimento fora da faixa; ação ignorada."));
        return;
    }
    if (!Herois::movimentoEstaDesbloqueado(idCatalogo)) {
        return;
    }
    Herois::definirMovimentoEscolhido(slotMovimentoSelecionado, idCatalogo);
    slotMovimentoSelecionado = (slotMovimentoSelecionado + 1) % 3;
    atualizar_painel_movimentos();
}

// ---------------------------------------------------------------------------
// Controle de visibilidade das telas
// ---------------------------------------------------------------------------

void GerenciadorInterface::atualizar_telas()
{
    Control* menu_principal = has_node("ContainerBotoes") ? get_node<Control>("ContainerBotoes") : nullptr;
    Control* menu_config_antigo = has_node("ContainerConfig") ? get_node<Control>("ContainerConfig") : nullptr;
    Control* menu_jogar = has_node("ContainerJogar") ? get_node<Control>("ContainerJogar") : nullptr;
    Control* titulo = has_node("Titulo") ? get_node<Control>("Titulo") : nullptr;

    TextureRect* fundo_principal = has_node("FundoPrincipal") ? get_node<TextureRect>("FundoPrincipal") : nullptr;
    TextureRect* fundo_config = has_node("FundoConfig") ? get_node<TextureRect>("FundoConfig") : nullptr;
    TextureRect* fundo_jogo = has_node("FundoJogo") ? get_node<TextureRect>("FundoJogo") : nullptr;

    bool is_menu = (estadoAtual == GerenciadorMenu::EstadoMenu::MENU_INICIAL);
    bool is_config = (estadoAtual == GerenciadorMenu::EstadoMenu::MENU_CONFIGURACOES);
    bool is_jogar = (estadoAtual == GerenciadorMenu::EstadoMenu::MENU_JOGAR);
    bool is_movimentos = (estadoAtual == GerenciadorMenu::EstadoMenu::MENU_MOVIMENTOS);
    bool is_areas = (estadoAtual == GerenciadorMenu::EstadoMenu::MENU_AREAS);

    if (menu_principal) menu_principal->set_visible(is_menu);
    if (menu_config_antigo) menu_config_antigo->set_visible(false);
    if (painelConfig) painelConfig->set_visible(is_config);
    if (menu_jogar) menu_jogar->set_visible(is_jogar);
    if (titulo) titulo->set_visible(is_menu);
    if (painelMovimentos) painelMovimentos->set_visible(is_movimentos);
    if (painelAreas) painelAreas->set_visible(is_areas);

    if (fundo_principal) fundo_principal->set_visible(is_menu);
    if (fundo_config) fundo_config->set_visible(is_config);
    if (fundo_jogo) fundo_jogo->set_visible(is_jogar);
}

void GerenciadorInterface::clicar_jogar()
{
    if (painelAreas) painelAreas->set_visible(false);
    if (painelMovimentos) painelMovimentos->set_visible(false);
    definir_imagem_fundo_submenu(caminhoFundoSubmenu);
    irPara(GerenciadorMenu::EstadoMenu::MENU_JOGAR);
    UtilityFunctions::print(">> Abriu o submenu de jogar");
    atualizar_telas();
}

void GerenciadorInterface::clicar_mundo_principal()
{
    abrir_painel_areas();
}

void GerenciadorInterface::clicar_quests()
{
    UtilityFunctions::print(">> Abrindo menu de Quests...");
}

void GerenciadorInterface::clicar_configuracoes()
{
    if (painelConfig == nullptr) {
        estilizar_menu_configuracoes();
    }
    definir_imagem_fundo_config(caminhoFundoConfig);
    irPara(GerenciadorMenu::EstadoMenu::MENU_CONFIGURACOES);
    atualizar_texto_volume();
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
    if (!std::isfinite(valor)) {
        UtilityFunctions::push_warning("Valor de volume inválido ignorado.");
        return;
    }
    definirVolume(valor);
    atualizar_texto_volume();
    UtilityFunctions::print("Volume ajustado para: ", volume);
}

void GerenciadorInterface::mudar_resolucao(int indice)
{
    if (indice < 0 || indice > 7) {
        UtilityFunctions::push_warning("Índice de resolução fora da faixa; configuração mantida.");
        return;
    }
    definirIndiceResolucao(indice);
    Window* janela = get_window();
    if (!janela) return;

    if (indice <= 5) {
        janela->set_mode(Window::MODE_WINDOWED);

        switch (indice) {
            case 0: janela->set_size(Vector2i(640, 480)); break;
            case 1: janela->set_size(Vector2i(1280, 720)); break;
            case 2: janela->set_size(Vector2i(1280, 960)); break;
            case 3: janela->set_size(Vector2i(1920, 1080)); break;
            case 4: janela->set_size(Vector2i(1920, 1440)); break;
            case 5: janela->set_size(Vector2i(2560, 1440)); break;
        }
    } else if (indice == 6) {
        // Tela Cheia Exclusiva
        janela->set_mode(Window::MODE_EXCLUSIVE_FULLSCREEN);
    } else if (indice == 7) {
        // Tela Cheia
        janela->set_mode(Window::MODE_FULLSCREEN);
    }

    UtilityFunctions::print("Resolução alterada. Índice: ", indice);
}

GerenciadorMenu::EstadoMenu GerenciadorInterface::getEstadoAtual() const { return estadoAtual; }

void GerenciadorInterface::irPara(GerenciadorMenu::EstadoMenu novoEstado)
{
    const int valor = static_cast<int>(novoEstado);
    if (valor < static_cast<int>(GerenciadorMenu::EstadoMenu::MENU_INICIAL) ||
        valor > static_cast<int>(GerenciadorMenu::EstadoMenu::MENU_AREAS)) {
        UtilityFunctions::push_warning("Estado de menu inválido; retornando ao menu inicial.");
        estadoAtual = GerenciadorMenu::EstadoMenu::MENU_INICIAL;
        return;
    }
    estadoAtual = novoEstado;
}

void GerenciadorInterface::voltar() { estadoAtual = GerenciadorMenu::EstadoMenu::MENU_INICIAL; }
float GerenciadorInterface::getVolume() const { return volume; }

void GerenciadorInterface::definirVolume(float novoVolume)
{
    if (!std::isfinite(novoVolume)) {
        UtilityFunctions::push_warning("Volume não finito ignorado.");
        return;
    }
    volume = std::max(0.0f, std::min(100.0f, novoVolume));
}

int GerenciadorInterface::getIndiceResolucao() const { return indiceResolucao; }

void GerenciadorInterface::definirIndiceResolucao(int indice)
{
    if (indice < 0 || indice > 7) {
        UtilityFunctions::push_warning("Índice de resolução inválido; usando a resolução padrão.");
        indiceResolucao = 0;
        return;
    }
    indiceResolucao = indice;
}

void GerenciadorInterface::iniciarOuRetomarPartida() { estadoAtual = GerenciadorMenu::EstadoMenu::EM_BATALHA; }
