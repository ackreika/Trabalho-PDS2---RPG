#ifndef RECOMPENSAS_HPP
#define RECOMPENSAS_HPP

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/array.hpp>

namespace godot {

class Recompensas : public RefCounted {
    GDCLASS(Recompensas, RefCounted);

protected:
    static void _bind_methods();

public:
    static bool dropa_item_chave(int area, int indice);
};

}

#endif
