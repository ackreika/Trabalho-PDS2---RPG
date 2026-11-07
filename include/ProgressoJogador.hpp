#ifndef PROGRESSO_JOGADOR_HPP
#define PROGRESSO_JOGADOR_HPP

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/classes/node.hpp>
#include <vector>

namespace godot {

class ProgressoJogador : public RefCounted {
    GDCLASS(ProgressoJogador, RefCounted);

private:
    int pontos = 0;
    int nivel_vida = 0;
    int nivel_dano = 0;
    String item_especial_equipado = "";
    std::vector<String> professores_derrotados;
    std::vector<String> itens_desbloqueados;

protected:
    static void _bind_methods();

public:
    ProgressoJogador();
    ~ProgressoJogador();

    int get_pontos() const;
    void adicionar_pontos(int qtd);
    bool gastar_pontos(int qtd);

    int get_nivel_vida() const { return nivel_vida; }
    int get_nivel_dano() const { return nivel_dano; }
    int get_quantidade_vitorias() const { return static_cast<int>(professores_derrotados.size()); }
    String get_item_especial_equipado() const { return item_especial_equipado; }
    void equipar_item_especial(const String &id) { equipar_item_especial_desbloqueado(id); }
    Array get_todos_itens() const;
    bool ja_venceu_professor(int area, int indice) const;
    Array registrar_vitoria_professor(int area, int indice);
    bool equipar_item_especial_desbloqueado(const String &id);

    bool comprar_vida();
    bool comprar_dano();
    int custo_vida() const;
    int custo_dano() const;

    void aplicar_ao_heroi(Object *heroi_obj);

    bool salvar(const String &caminho);
    bool carregar(const String &caminho);
};

}

#endif
