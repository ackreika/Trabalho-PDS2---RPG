#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
#include <godot_cpp/godot.hpp>
#include <cstdlib>
#include <cstring>

namespace {

// Funções auxiliares vazias (stubs) para permitir instanciar classes que herdam
// de godot::Node2D ou godot::RefCounted diretamente no terminal de testes,
// sem precisar abrir o motor da Godot.

void stub_vazio() {}

void stub_print_error(const char*, const char*, const char*, int32_t, GDExtensionBool) {}
void stub_print_error_msg(const char*, const char*, const char*, const char*, int32_t, GDExtensionBool) {}

void stub_get_godot_version(GDExtensionGodotVersion* r_version) {
    if (r_version) {
        r_version->major = 4;
        r_version->minor = 99;
        r_version->patch = 99;
        r_version->string = "4.99.99-test";
    }
}

void* stub_mem_alloc(size_t p_bytes) { return std::malloc(p_bytes); }
void* stub_mem_realloc(void* p_ptr, size_t p_bytes) { return std::realloc(p_ptr, p_bytes); }
void stub_mem_free(void* p_ptr) { std::free(p_ptr); }

void stub_ptr_constructor(GDExtensionUninitializedTypePtr r_base, const GDExtensionConstTypePtr*) {
    if (r_base) {
        std::memset(r_base, 0, sizeof(void*));
    }
}

void stub_ptr_destructor(GDExtensionTypePtr) {}

void stub_variant_from_type(GDExtensionUninitializedVariantPtr, GDExtensionTypePtr) {}
void stub_variant_to_type(GDExtensionUninitializedTypePtr, GDExtensionVariantPtr) {}

void stub_ptr_builtin_method(GDExtensionTypePtr, const GDExtensionConstTypePtr*, GDExtensionTypePtr r_return, int) {
    if (r_return) {
        std::memset(r_return, 0, sizeof(int64_t));
    }
}

void stub_ptr_operator_evaluator(GDExtensionConstTypePtr p_left, GDExtensionConstTypePtr p_right, GDExtensionTypePtr r_return) {
    if (r_return) {
        bool iguais = (p_left && p_right) ? (std::memcmp(p_left, p_right, sizeof(void*)) == 0) : false;
        *reinterpret_cast<GDExtensionBool*>(r_return) = iguais ? 1 : 0;
    }
}

void stub_ptr_setter(GDExtensionTypePtr, GDExtensionConstTypePtr) {}
void stub_ptr_getter(GDExtensionConstTypePtr, GDExtensionTypePtr r_ret) {
    if (r_ret) {
        std::memset(r_ret, 0, sizeof(int64_t));
    }
}

GDExtensionVariantFromTypeConstructorFunc stub_get_variant_from_type(GDExtensionVariantType) {
    return &stub_variant_from_type;
}

GDExtensionTypeFromVariantConstructorFunc stub_get_variant_to_type(GDExtensionVariantType) {
    return &stub_variant_to_type;
}

GDExtensionPtrConstructor stub_get_ptr_constructor(GDExtensionVariantType, int32_t) {
    return &stub_ptr_constructor;
}

GDExtensionPtrDestructor stub_get_ptr_destructor(GDExtensionVariantType) {
    return &stub_ptr_destructor;
}

GDExtensionPtrBuiltInMethod stub_get_ptr_builtin_method(GDExtensionVariantType, GDExtensionConstStringNamePtr, GDExtensionInt) {
    return &stub_ptr_builtin_method;
}

GDExtensionPtrOperatorEvaluator stub_get_ptr_operator_evaluator(GDExtensionVariantOperator, GDExtensionVariantType, GDExtensionVariantType) {
    return &stub_ptr_operator_evaluator;
}

GDExtensionPtrSetter stub_get_ptr_setter(GDExtensionVariantType, GDExtensionConstStringNamePtr) {
    return &stub_ptr_setter;
}

GDExtensionPtrGetter stub_get_ptr_getter(GDExtensionVariantType, GDExtensionConstStringNamePtr) {
    return &stub_ptr_getter;
}

GDExtensionPtrUtilityFunction stub_get_ptr_utility_function(GDExtensionConstStringNamePtr, GDExtensionInt) {
    return nullptr;
}

void stub_string_name_new_latin1(GDExtensionUninitializedStringNamePtr r_dest, const char* p_contents, GDExtensionBool) {
    if (r_dest) {
        *reinterpret_cast<const char**>(r_dest) = p_contents;
    }
}

GDExtensionObjectPtr stub_classdb_construct_object(GDExtensionConstStringNamePtr) {
    return nullptr;
}

GDExtensionMethodBindPtr stub_classdb_get_method_bind(GDExtensionConstStringNamePtr, GDExtensionConstStringNamePtr, GDExtensionInt) {
    return nullptr;
}

GDExtensionInterfaceFunctionPtr mock_get_proc_address(const char* p_name) {
    if (!p_name) return &stub_vazio;
    if (std::strcmp(p_name, "get_godot_version") == 0) return reinterpret_cast<GDExtensionInterfaceFunctionPtr>(&stub_get_godot_version);
    if (std::strcmp(p_name, "mem_alloc") == 0) return reinterpret_cast<GDExtensionInterfaceFunctionPtr>(&stub_mem_alloc);
    if (std::strcmp(p_name, "mem_realloc") == 0) return reinterpret_cast<GDExtensionInterfaceFunctionPtr>(&stub_mem_realloc);
    if (std::strcmp(p_name, "mem_free") == 0) return reinterpret_cast<GDExtensionInterfaceFunctionPtr>(&stub_mem_free);
    if (std::strcmp(p_name, "print_error") == 0) return reinterpret_cast<GDExtensionInterfaceFunctionPtr>(&stub_print_error);
    if (std::strcmp(p_name, "print_error_with_message") == 0) return reinterpret_cast<GDExtensionInterfaceFunctionPtr>(&stub_print_error_msg);
    if (std::strcmp(p_name, "print_warning") == 0) return reinterpret_cast<GDExtensionInterfaceFunctionPtr>(&stub_print_error);
    if (std::strcmp(p_name, "print_warning_with_message") == 0) return reinterpret_cast<GDExtensionInterfaceFunctionPtr>(&stub_print_error_msg);
    if (std::strcmp(p_name, "get_variant_from_type_constructor") == 0) return reinterpret_cast<GDExtensionInterfaceFunctionPtr>(&stub_get_variant_from_type);
    if (std::strcmp(p_name, "get_variant_to_type_constructor") == 0) return reinterpret_cast<GDExtensionInterfaceFunctionPtr>(&stub_get_variant_to_type);
    if (std::strcmp(p_name, "variant_get_ptr_constructor") == 0) return reinterpret_cast<GDExtensionInterfaceFunctionPtr>(&stub_get_ptr_constructor);
    if (std::strcmp(p_name, "variant_get_ptr_destructor") == 0) return reinterpret_cast<GDExtensionInterfaceFunctionPtr>(&stub_get_ptr_destructor);
    if (std::strcmp(p_name, "variant_get_ptr_builtin_method") == 0) return reinterpret_cast<GDExtensionInterfaceFunctionPtr>(&stub_get_ptr_builtin_method);
    if (std::strcmp(p_name, "variant_get_ptr_operator_evaluator") == 0) return reinterpret_cast<GDExtensionInterfaceFunctionPtr>(&stub_get_ptr_operator_evaluator);
    if (std::strcmp(p_name, "variant_get_ptr_setter") == 0) return reinterpret_cast<GDExtensionInterfaceFunctionPtr>(&stub_get_ptr_setter);
    if (std::strcmp(p_name, "variant_get_ptr_getter") == 0) return reinterpret_cast<GDExtensionInterfaceFunctionPtr>(&stub_get_ptr_getter);
    if (std::strcmp(p_name, "variant_get_ptr_utility_function") == 0) return reinterpret_cast<GDExtensionInterfaceFunctionPtr>(&stub_get_ptr_utility_function);
    if (std::strcmp(p_name, "string_name_new_with_latin1_chars") == 0) return reinterpret_cast<GDExtensionInterfaceFunctionPtr>(&stub_string_name_new_latin1);
    if (std::strcmp(p_name, "classdb_construct_object") == 0) return reinterpret_cast<GDExtensionInterfaceFunctionPtr>(&stub_classdb_construct_object);
    if (std::strcmp(p_name, "classdb_get_method_bind") == 0) return reinterpret_cast<GDExtensionInterfaceFunctionPtr>(&stub_classdb_get_method_bind);
    return &stub_vazio;
}

void callback_init_teste(godot::ModuleInitializationLevel) {}

void inicializar_ambiente_godot_teste() {
    static GDExtensionInitialization inicializacao{};
    godot::GDExtensionBinding::InitObject init_obj(&mock_get_proc_address, nullptr, &inicializacao);
    init_obj.register_initializer(&callback_init_teste);
    init_obj.register_terminator(&callback_init_teste);
    init_obj.set_minimum_library_initialization_level(godot::MODULE_INITIALIZATION_LEVEL_SCENE);
    init_obj.init();
}

} // namespace

int main(int argc, char** argv) {
    inicializar_ambiente_godot_teste();

    doctest::Context context;
    context.applyCommandLine(argc, argv);
    return context.run();
}
