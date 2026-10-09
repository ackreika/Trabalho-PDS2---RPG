#include "GerenciadorInterface.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/classes/h_slider.hpp>
#include <godot_cpp/classes/option_button.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include <algorithm>

using namespace godot;
void GerenciadorInterface::_bind_methods() {
    ClassDB::bind_method(D_METHOD("clicar_jogar"), &GerenciadorInterface::clicar_jogar);
    ClassDB::bind_method(D_METHOD("clicar_configuracoes"), &GerenciadorInterface::clicar_configuracoes);
    ClassDB::bind_method(D_METHOD("clicar_sair"), &GerenciadorInterface::clicar_sair);
    ClassDB::bind_method(D_METHOD("clicar_voltar"), &GerenciadorInterface::clicar_voltar);
    ClassDB::bind_method(D_METHOD("mudar_volume", "valor"), &GerenciadorInterface::mudar_volume);
    ClassDB::bind_method(D_METHOD("mudar_resolucao", "indice"), &GerenciadorInterface::mudar_resolucao);
}

GerenciadorInterface::GerenciadorInterface()
    : estadoAtual(GerenciadorMenu::EstadoMenu::MENU_INICIAL), volume(100.0f), indiceResolucao(0) {}


void GerenciadorInterface::_ready() 
{
    // Atenção: ajuste "VBoxContainer/BotaoJogar" pro caminho exato que você criar na árvore do Godot!
    Button* botao_jogar = get_node<Button>("ContainerBotoes/BotaoJogar");
    Button* botao_config = get_node<Button>("ContainerBotoes/BotaoConfig");
    Button* botao_sair = get_node<Button>("ContainerBotoes/BotaoSair"); // Pra usar nos submenus
    Button* botao_voltar = get_node<Button>("ContainerConfig/BotaoVoltar");
    HSlider* slider_volume = get_node<HSlider>("ContainerConfig/SliderVolume");
    OptionButton* caixa_resolucao = get_node<OptionButton>("ContainerConfig/CaixaResolucao");
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
        if (!caixa_resolucao->is_connected("item_selected", funcao_resolucao)) 
        {
            caixa_resolucao->connect("item_selected", funcao_resolucao);
        }
    }
    if (botao_jogar) botao_jogar->connect("pressed", Callable(this, "clicar_jogar"));
    if (botao_config) botao_config->connect("pressed", Callable(this, "clicar_configuracoes"));
    if (botao_sair) botao_sair->connect("pressed", Callable(this, "clicar_sair"));
    if (botao_voltar) botao_voltar->connect("pressed", Callable(this, "clicar_voltar"));
    if (slider_volume) slider_volume->connect("value_changed", Callable(this, "mudar_volume"));
    if (caixa_resolucao) caixa_resolucao->connect("item_selected", Callable(this, "mudar_resolucao"));
}
void GerenciadorInterface::atualizar_telas() 
{
    Control* menu_principal = get_node<Control>("ContainerBotoes");
    Control* menu_config = get_node<Control>("ContainerConfig");

    TextureRect* fundo_principal = get_node<TextureRect>("FundoPrincipal");
    TextureRect* fundo_config = get_node<TextureRect>("FundoConfig");

    if (menu_principal && menu_config) 
    {
        bool menu_inicial = (estadoAtual == GerenciadorMenu::EstadoMenu::MENU_INICIAL);
        menu_principal->set_visible(menu_inicial);
        menu_config->set_visible(!menu_inicial);
        if (fundo_principal) fundo_principal->set_visible(menu_inicial);
        if (fundo_config) fundo_config->set_visible(!menu_inicial);
    }
}
void GerenciadorInterface::clicar_jogar() 
{
    iniciarOuRetomarPartida();
    UtilityFunctions::print("Esta em batalha");
    atualizar_telas();
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
    UtilityFunctions::print("Voltou um menu");
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
