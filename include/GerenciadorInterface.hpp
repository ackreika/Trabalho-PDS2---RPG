#pragma once
#include <godot_cpp/classes/ref_counted.hpp>
#include "EstadoMenu.hpp"

using namespace godot;

/**
 * @brief Conhece em qual tela de menu o jogador está e guarda as configurações
 * de áudio/vídeo (GerenciadorInterface, no CRC).
 *
 * A aplicação real do volume/resolução no motor (AudioServer, DisplayServer)
 * é feita pelo Godot; aqui só fica o DADO da configuração.
 *
 * EstadoMenu, do lado do GDScript, é só um número (int) — a correspondência é:
 * 0 = MENU_INICIAL, 1 = MENU_JOGAR, 2 = MENU_CONFIGURACOES, 3 = MENU_INVENTARIO, 4 = EM_BATALHA
 */
class GerenciadorInterface : public RefCounted {
    GDCLASS(GerenciadorInterface, RefCounted)

private:
    GerenciadorMenu::EstadoMenu estadoAtual;
    float volume;
    int indiceResolucao;

protected:
    /** @brief Registra os métodos públicos pro Godot/GDScript enxergar. */
    static void _bind_methods();

public:
    /** @brief Cria o gerenciador já no MENU_INICIAL, volume 1.0 e resolução padrão. */
    GerenciadorInterface();

    /** @brief Retorna o int correspondente ao EstadoMenu atual. */
    GerenciadorMenu::EstadoMenu getEstadoAtual() const;
    /** @brief Muda o estado do menu (recebe o int, converte pra EstadoMenu internamente). */
    void irPara(GerenciadorMenu::EstadoMenu novoEstado);
    /** @brief Volta pro MENU_INICIAL — usado pelos botões "Voltar" dos submenus. */
    void voltar();

    /** @brief Retorna o volume configurado (0.0 a 1.0). */
    float getVolume() const;
    /** @brief Define o volume, travado entre 0.0 e 1.0. */
    void definirVolume(float novoVolume);

    /** @brief Retorna o índice da resolução escolhida. */
    int getIndiceResolucao() const;
    /** @brief Define o índice da resolução escolhida. */
    void definirIndiceResolucao(int indice);

    /** @brief Sinaliza início/retomada de partida; muda o estado pra EM_BATALHA. */
    void iniciarOuRetomarPartida();
};