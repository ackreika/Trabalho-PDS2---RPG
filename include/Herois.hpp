#pragma once
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <string>
#include <vector>
#include <map>
#include "Movimento.hpp"
#include "ItemPassivo.hpp"
#include "ItemEspecial.hpp"
#include "EstadosVisuais.hpp"

using namespace godot;

class Professor;

/**
 * @brief Representa o Estudante (jogador) durante uma run.
 */
class Herois : public Node2D {
    GDCLASS(Herois, Node2D)

protected:
    static void _bind_methods();

private:
    std::string nome;
    int nivel;
    int vida;
    int vidaMaxima;
    int vidaMaximaBase;
    int defesa;
    int danoBase;

    std::vector<Movimento> movimentos;
    std::vector<ItemPassivo> itensPassivos;

    bool possuiItemEspecialAtivo;
    ItemEspecial itemEspecialAtivo;

    bool possuiItemChave;

    std::map<EstadoVisual, std::string> spritesPorEstado;

    /** @brief Guarda os IDs dos 3 movimentos escolhidos pelo jogador no Menu de Movimentos C++. */
    static int idsMovimentosEscolhidos[3];

public:
    Herois();
    Herois(std::string nome, int vidaMaxima, int defesa, int danoBase);

    std::string getNome() const;
    int getVida() const;
    int getVidaMaxima() const;
    int getDefesa() const;
    int getDanoBase() const;
    bool estaVivo() const;

    void curar(int quantidade);
    void receberDano(int danoBase);

    void aprenderMovimento(const Movimento& novo, int indiceParaSubstituir = -1);
    const std::vector<Movimento>& getMovimentos() const;
    Movimento* getMovimentoParaEditar(int indice);

    // Controle do Loadout de Movimentos (Menu de Movimentos em C++)
    static void definirMovimentoEscolhido(int slot, int idCatalogo);
    static int getMovimentoEscolhido(int slot);
    static bool movimentoJaEquipadoEmOutroSlot(int idCatalogo, int slotIgnorado = -1);
    static int encontrarSlotDoMovimento(int idCatalogo);
    void carregarMovimentosEscolhidos();

    void adicionarItemPassivo(ItemPassivo item);
    int getQuantidadeItensPassivos() const;
    std::string getNomeItemPassivo(int indice) const;
    int calcularDanoFinal(int danoBase) const;
    int calcularVidaFinal(int vidaBase) const;
    void recalcularVidaMaxima();

    void definirItemEspecial(ItemEspecial item);
    bool possuiItemEspecial() const;
    std::string getNomeItemEspecial() const;
    bool itemEspecialPodeUsar() const;
    int usarItemEspecial(Professor& alvo);

    void obterItemChave();
    bool possuiItemDeChave() const;
    void consumirItemChave();

    void definirSprite(EstadoVisual estado, std::string caminho);
    std::string getCaminhoSprite(EstadoVisual estado) const;

    // ======================================================================
    // API exposta ao GDScript
    // ======================================================================

    void inicializar(String nome, int vidaMaxima, int defesa, int danoBase);

    String get_nome() const;
    int get_vida() const;
    int get_vida_maxima() const;
    int get_defesa() const;
    int get_dano_base() const;
    bool esta_vivo() const;

    void aprender_movimento_simples(String nome, int dano, int usosMaximos);
    void carregar_movimentos_escolhidos();
    int get_quantidade_movimentos() const;
    String get_nome_movimento(int indice) const;
    int get_dano_movimento(int indice) const;
    int get_cura_movimento(int indice) const;

    int get_usos_restantes_movimento(int indice) const;
    bool movimento_pode_usar(int indice) const;

    static int get_quantidade_catalogo_movimentos();
    static Dictionary descrever_movimento_catalogo(int idCatalogo);
    static void selecionar_movimento_catalogo(int slot, int idCatalogo);
    static int get_id_movimento_slot(int slot);

    void definir_sprite(int estado, String caminho);
    String get_caminho_sprite(int estado) const;
    
    String dar_item_passivo(String id);
    String dar_item_especial(String id);
    static Dictionary descrever_item_passivo(String id);
    static Dictionary descrever_item_especial(String id);

    int get_quantidade_itens_passivos() const;
    String get_nome_item_passivo(int indice) const;
    bool possui_item_especial() const;
    String get_nome_item_especial() const;
    int get_usos_item_especial() const;

    void obter_item_chave();
    bool possui_item_chave() const;
    void aplicar_bonus_permanente(int bonusVida, int bonusDano);
};