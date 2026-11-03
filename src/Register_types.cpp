#include "Register_types.hpp"
#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

// Inclua os cabeçalhos das suas classes C++
#include "Herois.hpp"
#include "Professor.hpp"
#include "Batalha.hpp"
#include "GerenciadorInterface.hpp"
#include "Recompensas.hpp"
#include "ProfessoresFabrica.hpp"
#include "ProgressoJogador.hpp" // Adicionado para a persistência e drop único
#include "GerenciadorSemestre.hpp"
#include "EstadoMenu.hpp"
#include "GerenciadorQuiz.hpp"

using namespace godot;

void initialize_game_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
    
    // Registre as classes C++ no ClassDB do Godot:
    ClassDB::register_class<GerenciadorInterface>();
    ClassDB::register_class<Herois>();
    ClassDB::register_class<Professor>();
    ClassDB::register_class<Batalha>();
    ClassDB::register_class<Recompensas>();
    ClassDB::register_class<ProfessoresFabrica>();
    ClassDB::register_class<ProgressoJogador>();
    ClassDB::register_class<GerenciadorSemestre>();
    ClassDB::register_class<GerenciadorMenu>();
    ClassDB::register_class<GerenciadorQuiz>();
}

void uninitialize_game_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
}

extern "C" {
    GDExtensionBool GDE_EXPORT game_library_init(
        GDExtensionInterfaceGetProcAddress p_get_proc_address,
        const GDExtensionClassLibraryPtr p_library,
        GDExtensionInitialization *r_initialization) {

        godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

        init_obj.register_initializer(initialize_game_module);
        init_obj.register_terminator(uninitialize_game_module);
        init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

        return init_obj.init();
    }
}
