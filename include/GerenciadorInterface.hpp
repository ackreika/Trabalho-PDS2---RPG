#pragma once

#include <godot_cpp/classes/canvas_layer.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/label.hpp>
#include <vector>
#include "EstadoMenu.hpp"

using namespace godot;

/**
 * @brief Conhece em qual tela de menu o jogador está, guarda as configurações
 * de áudio/vídeo e constrói/controla o Menu de Movimentos em C++.
 */
class GerenciadorInterface : public CanvasLayer {
    GDCLASS(GerenciadorInterface, CanvasLayer)

private:
    GerenciadorMenu::EstadoMenu estadoAtual;
    float volume;
    int indiceResolucao;

    // Componentes do Menu de Movimentos gerenciados em C++
    Control* painelMovimentos;
    Label* labelStatusMovimentos;
    Button* botoesSlot[3];
    std::vector<Button*> botoesCatalogo;
    int slotMovimentoSelecionado;

    void inicializar_componentes();
    void atualizar_telas();
    void criar_painel_movimentos();
    void atualizar_painel_movimentos();

protected:
    static void _bind_methods();

public:
    GerenciadorInterface();

    void _ready() override;
    void _notification(int p_what);

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
    void clicar_equipamento();

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