/**
 * @file ProgressoJogador.hpp
 * @brief Definição da classe ProgressoJogador para persistência e evolução do personagem.
 * 
 * Disciplina: Programação e Desenvolvimento de Software 2 (PDS 2)
 * Projeto: RPG Acadêmico em C++ e Godot Engine
 */

#ifndef PROGRESSO_JOGADOR_HPP
#define PROGRESSO_JOGADOR_HPP

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/classes/node.hpp>
#include <vector>

namespace godot {

/**
 * @class ProgressoJogador
 * @brief TAD responsável pelo gerenciamento de persistência, economia e evolução do jogador.
 * 
 * Controla os pontos acumulados nas batalhas, os níveis de upgrade permanente (vida e dano),
 * o registro de quais professores já foram derrotados para evitar recompensas duplicadas,
 * os itens desbloqueados e o item especial equipado no momento.
 * 
 * Herda de godot::RefCounted para permitir contagem automática de referências e integração
 * transparente com o GDScript através da GDExtension.
 */
class ProgressoJogador : public RefCounted {
    GDCLASS(ProgressoJogador, RefCounted);

private:
    int pontos = 0;                         ///< Saldo atual de pontos para compra de melhorias.
    int nivel_vida = 0;                     ///< Quantidade de upgrades de vida já adquiridos.
    int nivel_dano = 0;                     ///< Quantidade de upgrades de dano já adquiridos.
    String item_especial_equipado = "";     ///< ID do item especial atualmente ativo (slot 4).
    std::vector<String> professores_derrotados; ///< Histórico de batalhas vencidas no formato "area_indice".
    std::vector<String> itens_desbloqueados;    ///< Coleção de IDs de itens obtidos como recompensa.

protected:
    /**
     * @brief Registra métodos e propriedades na ClassDB da Godot Engine.
     */
    static void _bind_methods();

public:
    /**
     * @brief Construtor padrão da classe ProgressoJogador.
     */
    ProgressoJogador();

    /**
     * @brief Destrutor virtual da classe ProgressoJogador.
     */
    ~ProgressoJogador() override;


    // Gestão de Pontos / Economia
    /**
     * @brief Retorna o saldo atual de pontos do jogador.
     * @return Quantidade de pontos inteiros disponíveis.
     */
    int get_pontos() const;

    /**
     * @brief Adiciona pontos ao saldo do jogador (obtidos após derrotar professores).
     * @param qtd Quantidade positiva de pontos a somar.
     */
    void adicionar_pontos(int qtd);

    /**
     * @brief Deduz pontos do saldo caso haja saldo suficiente.
     * @param qtd Quantidade de pontos a debitar.
     * @return true se o débito foi efetuado com sucesso; false caso contrário.
     */
    bool gastar_pontos(int qtd);


    // Upgrades de Atributos Permanentes
    /**
     * @brief Retorna o nível atual de upgrade de vida.
     */
    int get_nivel_vida() const { return nivel_vida; }

    /**
     * @brief Retorna o nível atual de upgrade de dano.
     */
    int get_nivel_dano() const { return nivel_dano; }

    /**
     * @brief Calcula o custo em pontos para o próximo upgrade de vida.
     * @return Custo calculado com base na fórmula progressiva: 20 + 10 * nivel_vida.
     */
    int custo_vida() const;

    /**
     * @brief Calcula o custo em pontos para o próximo upgrade de dano.
     * @return Custo calculado com base na fórmula progressiva: 20 + 10 * nivel_dano.
     */
    int custo_dano() const;

    /**
     * @brief Tenta comprar um nível adicional de vida permanente deduzindo os pontos necessários.
     * @return true se a compra foi concluída com sucesso; false se não houver pontos suficientes.
     */
    bool comprar_vida();

    /**
     * @brief Tenta comprar um nível adicional de dano permanente deduzindo os pontos necessários.
     * @return true se a compra foi concluída com sucesso; false se não houver pontos suficientes.
     */
    bool comprar_dano();


    // Registro de Vitórias e Drops de Itens
    /**
     * @brief Retorna o total de professores/batalhas já vencidas pelo jogador.
     */
    int get_quantidade_vitorias() const { return static_cast<int>(professores_derrotados.size()); }

    /**
     * @brief Verifica se uma batalha específica já foi vencida anteriormente.
     * @param area Identificador da área acadêmica.
     * @param indice Índice do professor/batalha dentro da área.
     * @return true se a batalha já foi superada; false caso contrário.
     */
    bool ja_venceu_professor(int area, int indice) const;

    /**
     * @brief Registra a vitória sobre um professor, liberando suas recompensas na primeira vitória.
     * @param area Identificador da área acadêmica.
     * @param indice Índice do professor/batalha dentro da área.
     * @return Array da Godot contendo os IDs dos itens recém-desbloqueados (para exibição na UI).
     */
    Array registrar_vitoria_professor(int area, int indice);


    // Gestão de Itens Desbloqueados e Especiais
    /**
     * @brief Retorna uma lista com os IDs de todos os itens já conquistados pelo jogador.
     * @return Array contendo strings com os nomes dos itens.
     */
    Array get_todos_itens() const;

    /**
     * @brief Retorna o identificador do item especial atualmente equipado.
     * @return String com o ID do item, ou string vazia caso nenhum esteja selecionado.
     */
    String get_item_especial_equipado() const { return item_especial_equipado; }

    /**
     * @brief Equipa um item especial (método exposto para atalho na interface).
     * @param id Identificador textual do item especial.
     */
    void equipar_item_especial(const String &id) { equipar_item_especial_desbloqueado(id); }

    /**
     * @brief Valida e equipa um item especial que já pertença aos itens desbloqueados.
     * @param id Identificador textual do item a equipar.
     * @return true se o item é válido, especial e foi equipado; false caso contrário.
     */
    bool equipar_item_especial_desbloqueado(const String &id);


    // Aplicação ao Herói em Batalha
    /**
     * @brief Injeta os bônus permanentes e os itens adquiridos no objeto Herois antes da batalha.
     * @param heroi_obj Ponteiro genérico da Godot para o nó/instância da classe Herois.
     */
    void aplicar_ao_heroi(Object *heroi_obj);


    // Persistência em Disco (Save / Load)
    /**
     * @brief Serializa os dados de progresso do jogador em um arquivo de texto.
     * @param caminho Caminho do arquivo (ex: "user://progresso.txt").
     * @return true se o salvamento foi bem-sucedido; false em caso de falha de E/S.
     */
    bool salvar(const String &caminho);

    /**
     * @brief Desserializa os dados de progresso a partir de um arquivo de texto.
     * @param caminho Caminho do arquivo (ex: "user://progresso.txt").
     * @return true se a leitura foi bem-sucedida (ou se o arquivo ainda não existe); false se corrompido.
     */
    bool carregar(const String &caminho);
};

} // namespace godot

#endif // PROGRESSO_JOGADOR_HPP
