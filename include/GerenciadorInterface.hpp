#pragma once

#include <godot_cpp/classes/canvas_layer.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/panel_container.hpp>
#include <godot_cpp/classes/margin_container.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/scroll_container.hpp>
#include <godot_cpp/classes/h_separator.hpp>
#include <godot_cpp/classes/h_slider.hpp>
#include <godot_cpp/classes/option_button.hpp>
#include <godot_cpp/classes/style_box_flat.hpp>
#include <godot_cpp/classes/style_box_line.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/variant/color.hpp>
#include <godot_cpp/variant/string.hpp>
#include <vector>
#include "EstadoMenu.hpp"

using namespace godot;

/**
 * @brief Conhece em qual tela de menu o jogador está, guarda as configurações
 * de áudio/vídeo e constrói/estiliza os submenus do jogo em C++ seguindo o
 * padrão visual da tela de Upgrades Permanentes.
 */
class GerenciadorInterface : public CanvasLayer {
    GDCLASS(GerenciadorInterface, CanvasLayer)

private:
    static const int TOTAL_AREAS = 4;

    GerenciadorMenu::EstadoMenu estadoAtual;
    float volume;
    int indiceResolucao;

    // Guarda o caminho da imagem de fundo da tela de configurações
    String caminhoFundoConfig;

    // Componentes do Menu de Configurações estilizados em C++
    PanelContainer* painelConfig;
    Label* labelVolumeConfig;
    Label* labelResolucaoConfig;
    HSlider* sliderVolumeConfig;
    OptionButton* caixaResolucaoConfig;
    Button* botaoVoltarConfig;

    // Componentes do Submenu "Escolha a Área" gerenciados em C++
    Control* painelAreas;
    PanelContainer* cardsAreas[TOTAL_AREAS];
    Label* labelsAreas[TOTAL_AREAS];
    Button* botoesAreas[TOTAL_AREAS];

    // Componentes do Menu de Movimentos gerenciados em C++
    Control* painelMovimentos;
    Label* labelStatusMovimentos;
    Button* botoesSlot[3];
    std::vector<PanelContainer*> cardsCatalogo;
    std::vector<Label*> labelsCatalogo;
    std::vector<Button*> botoesCatalogo;
    int slotMovimentoSelecionado;

    void inicializar_componentes();
    void atualizar_telas();

    void estilizar_titulo_principal();
    void estilizar_menu_configuracoes();
    void atualizar_texto_volume();

    void criar_painel_areas();
    void criar_painel_movimentos();
    void atualizar_painel_movimentos();

protected:
    static void _bind_methods();

public:
    GerenciadorInterface();

    void _ready() override;
    void _notification(int p_what);

    // Métodos auxiliares de estilização em C++ (padrão visual de Upgrades Permanentes)
    Ref<StyleBoxFlat> criar_estilo_painel_principal(float opacidade = 1.0f) const;
    Ref<StyleBoxFlat> criar_estilo_card_interno(bool destacado = false) const;
    Ref<StyleBoxFlat> criar_estilo_botao(const Color& cor_fundo, const Color& cor_borda) const;
    void estilizar_botao(Button* botao, const Color& cor_normal, const Color& cor_hover, int tamanho_fonte = 16) const;
    void estilizar_botao_destaque(Button* botao, bool ativo, int tamanho_fonte = 16) const;
    void aplicar_tema_padrao_submenu(PanelContainer* painel, Label* titulo, Label* subtitulo,
                                     HSeparator* separador, Button* botao_voltar, float opacidade = 1.0f) const;

    // Lógica dos botões principais
    void clicar_jogar();
    void iniciar_batalha();
    void clicar_configuracoes();
    void clicar_sair();
    void clicar_voltar();
    void mudar_volume(float valor);
    void mudar_resolucao(int indice);
    void clicar_mundo_principal();
    void clicar_quests();

    // Troca a imagem de fundo do menu de Configurações mantendo suporte ao TextureRect
    void definir_imagem_fundo_config(const String& caminho_textura);

    // Lógica do Submenu "Escolha a Área" em C++
    void abrir_painel_areas();
    void fechar_painel_areas();
    void atualizar_estado_area(int id_area, bool desbloqueada);
    void selecionar_area(int id_area);

    // Lógica do Menu de Movimentos em C++
    void clicar_movimentos();
    void clicar_fechar_movimentos();
    void selecionar_slot_movimento(int slot);
    void escolher_movimento_catalogo(int idCatalogo);

    GerenciadorMenu::EstadoMenu getEstadoAtual() const;
    void irPara(GerenciadorMenu::EstadoMenu novoEstado);
    void voltar();

    float getVolume() const;
    void definirVolume(float novoVolume);

    int getIndiceResolucao() const;
    void definirIndiceResolucao(int indice);

    void iniciarOuRetomarPartida();
};
