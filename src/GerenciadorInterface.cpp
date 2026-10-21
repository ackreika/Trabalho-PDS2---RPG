#include "GerenciadorInterface.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/classes/h_slider.hpp>
#include <godot_cpp/classes/option_button.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/button.hpp>
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
}

GerenciadorInterface::GerenciadorInterface()
    : estadoAtual(GerenciadorMenu::EstadoMenu::MENU_INICIAL), volume(100.0f), indiceResolucao(0) {}

void GerenciadorInterface::_ready() 
{
    Button* botao_jogar = get_node<Button>("ContainerBotoes/BotaoJogar");
   // Button* botao_config = get_node<Button>("ContainerBotoes/BotaoConfig");
   // Button* botao_sair = get_node<Button>("ContainerBotoes/BotaoSair"); 
    Button* botao_voltar = get_node<Button>("ContainerConfig/BotaoVoltar");
    
    Button* botao_mundo = get_node<Button>("ContainerJogar/BotaoMundo");
   // Button* botao_quests = get_node<Button>("ContainerJogar/BotaoQuests");
    //Button* botao_equipamento = get_node<Button>("ContainerJogar/BotaoEquipamento");
    Button* botao_voltar_jogar = get_node<Button>("ContainerJogar/BotaoVoltar");
    
    //HSlider* slider_volume = get_node<HSlider>("ContainerConfig/SliderVolume");
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
        caixa_resolucao->add_item("Tela Cheia (Exclusive)"); 
        caixa_resolucao->add_item("Tela Cheia (Borderless)");
       // Callable funcao_resolucao(this, "mudar_resolucao");
       // if (!caixa_resolucao->is_connected("item_selected", funcao_resolucao)) {
        //    caixa_resolucao->connect("item_selected", funcao_resolucao);
        //}
    }

    if (botao_jogar) botao_jogar->connect("pressed", Callable(this, "clicar_jogar"));
  //  if (botao_config) botao_config->connect("pressed", Callable(this, "clicar_configuracoes"));
  //  if (botao_sair) botao_sair->connect("pressed", Callable(this, "clicar_sair"));
    if (botao_voltar) botao_voltar->connect("pressed", Callable(this, "clicar_voltar"));
    
    if (botao_mundo) botao_mundo->connect("pressed", Callable(this, "clicar_mundo_principal"));
   // if (botao_quests) botao_quests->connect("pressed", Callable(this, "clicar_quests"));
   // if (botao_equipamento) botao_equipamento->connect("pressed", Callable(this, "clicar_equipamento"));
    if (botao_voltar_jogar) botao_voltar_jogar->connect("pressed", Callable(this, "clicar_voltar"));

  //  if (slider_volume) slider_volume->connect("value_changed", Callable(this, "mudar_volume"));
}

void GerenciadorInterface::atualizar_telas() 
{
    Control* menu_principal = get_node<Control>("ContainerBotoes");
    Control* menu_config = get_node<Control>("ContainerConfig");
    Control* menu_jogar = get_node<Control>("ContainerJogar"); 

    TextureRect* fundo_principal = get_node<TextureRect>("FundoPrincipal");
    TextureRect* fundo_config = get_node<TextureRect>("FundoConfig");
    TextureRect* fundo_jogo = get_node<TextureRect>("FundoJogo"); // Puxando o novo fundo!

    if (menu_principal && menu_config) 
    {
        bool is_menu = (estadoAtual == GerenciadorMenu::EstadoMenu::MENU_INICIAL);
        bool is_config = (estadoAtual == GerenciadorMenu::EstadoMenu::MENU_CONFIGURACOES);
        bool is_jogar = (estadoAtual == GerenciadorMenu::EstadoMenu::MENU_JOGAR);

        menu_principal->set_visible(is_menu);
        menu_config->set_visible(is_config);
        
        if (menu_jogar) {
            menu_jogar->set_visible(is_jogar);
        }

        // Controlando qual fundo aparece
        if (fundo_principal) fundo_principal->set_visible(is_menu);
        if (fundo_config) fundo_config->set_visible(is_config);
        if (fundo_jogo) fundo_jogo->set_visible(is_jogar); // Mostra o fundo novo na tela de Jogar
    }
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
    Node* global = get_node<Node>("/root/Global");
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

    if (indice <= 5) 
    {
        janela->set_mode(Window::MODE_WINDOWED);
        
        switch (indice) {
            case 0: janela->set_size(Vector2i(640, 480)); break;
            case 1: janela->set_size(Vector2i(1280, 720)); break;
            case 2: janela->set_size(Vector2i(1280, 960)); break;
            case 3: janela->set_size(Vector2i(1920, 1080)); break;
            case 4: janela->set_size(Vector2i(1920, 1440)); break;
            case 5: janela->set_size(Vector2i(2560, 1440)); break;
        }
    } 
    else if (indice == 6) 
    {
        // Tela Cheia Exclusiva
        janela->set_mode(Window::MODE_EXCLUSIVE_FULLSCREEN);
    } 
    else if (indice == 7) 
    {
        // Tela Cheia
        janela->set_mode(Window::MODE_FULLSCREEN); 
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