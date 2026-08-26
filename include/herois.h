#ifndef HEROIS_H
#define HEROIS_H

#include <godot_cpp/classes/node.hpp>

namespace godot {
class Herois : public Node {
    GDCLASS(Herois, Node);

    private:
        int vida;
        int max_vida;
        int ataque_fisico;
        int ataque_magico;
        int defesa;
        bool is_dead;

    protected:
        static void _bind_methods();
    
    public:
        Herois();
        ~Herois();

        void tomar_dano(int dano);
        void curar(int cura);
        void set_ataque_fisico(int ataque);
        void set_ataque_magico(int ataque);
        void set_defesa(int defesa);
};
}

#endif // HEROIS_H