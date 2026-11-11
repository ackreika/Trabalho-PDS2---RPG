/**
 * @file ProgressoJogador.cpp
 * @brief Implementação dos métodos da classe ProgressoJogador.
 * 
 * Disciplina: Programação e Desenvolvimento de Software 2 (PDS 2)
 * Projeto: RPG Acadêmico em C++ e Godot Engine
 */

#include "ProgressoJogador.hpp"
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include "GerenciadorRecompensas.hpp"
#include "ProfessoresFabrica.hpp"
#include "ItensConcretos.hpp"
#include "Herois.hpp"
#include <algorithm>
#include <limits>
#include <string>
#include <vector>

using namespace godot;

namespace {

// Limite razoável para prevenir estouro em saves corrompidos
constexpr int NIVEL_MAXIMO = 10000;

/**
 * @brief Converte uma string da Godot para um inteiro não negativo com validação de limite.
 */
bool lerInteiroNaoNegativo(const String &texto, int maximo, int &saida) {
    const String limpa = texto.strip_edges();
    if (limpa.is_empty() || !limpa.is_valid_int()) {
        return false;
    }
    const int64_t valor = limpa.to_int();
    if (valor < 0 || valor > maximo) {
        return false;
    }
    saida = static_cast<int>(valor);
    return true;
}

/**
 * @brief Verifica se um ID textual corresponde a um ItemId válido registrado no sistema.
 */
bool idItemValido(const String &id, ItemId &saida) {
    if (id.is_empty()) {
        return false;
    }
    return itemIdDeTexto(std::string(id.utf8().get_data()), saida);
}

/**
 * @brief Função auxiliar para busca linear em vetores de String.
 */
bool contem(const std::vector<String> &lista, const String &valor) {
    return std::find(lista.begin(), lista.end(), valor) != lista.end();
}

/**
 * @brief Emite um aviso no console da Godot caso o arquivo de save esteja mal formatado.
 */
void avisarProgressoInvalido(const String &caminho) {
    UtilityFunctions::push_warning("Arquivo de save inválido ou corrompido. O estado atual foi preservado: ", caminho);
}

} // namespace anônimo


// Registro de Métodos na Godot Engine (GDExtension)

void ProgressoJogador::_bind_methods() {
    // Gestão de Pontos
    ClassDB::bind_method(D_METHOD("get_pontos"), &ProgressoJogador::get_pontos);
    ClassDB::bind_method(D_METHOD("adicionar_pontos", "qtd"), &ProgressoJogador::adicionar_pontos);
    ClassDB::bind_method(D_METHOD("gastar_pontos", "qtd"), &ProgressoJogador::gastar_pontos);

    // Upgrades Permanentes
    ClassDB::bind_method(D_METHOD("get_nivel_vida"), &ProgressoJogador::get_nivel_vida);
    ClassDB::bind_method(D_METHOD("get_nivel_dano"), &ProgressoJogador::get_nivel_dano);
    ClassDB::bind_method(D_METHOD("comprar_vida"), &ProgressoJogador::comprar_vida);
    ClassDB::bind_method(D_METHOD("comprar_dano"), &ProgressoJogador::comprar_dano);
    ClassDB::bind_method(D_METHOD("custo_vida"), &ProgressoJogador::custo_vida);
    ClassDB::bind_method(D_METHOD("custo_dano"), &ProgressoJogador::custo_dano);

    // Registro de Batalhas e Vitórias
    ClassDB::bind_method(D_METHOD("get_quantidade_vitorias"), &ProgressoJogador::get_quantidade_vitorias);
    ClassDB::bind_method(D_METHOD("ja_venceu_professor", "area", "indice"), &ProgressoJogador::ja_venceu_professor);
    ClassDB::bind_method(D_METHOD("registrar_vitoria_professor", "area", "indice"), &ProgressoJogador::registrar_vitoria_professor);

    // Gestão de Itens
    ClassDB::bind_method(D_METHOD("get_todos_itens"), &ProgressoJogador::get_todos_itens);
    ClassDB::bind_method(D_METHOD("get_item_especial_equipado"), &ProgressoJogador::get_item_especial_equipado);
    ClassDB::bind_method(D_METHOD("equipar_item_especial", "id"), &ProgressoJogador::equipar_item_especial);
    ClassDB::bind_method(D_METHOD("equipar_item_especial_desbloqueado", "id"), &ProgressoJogador::equipar_item_especial_desbloqueado);

    // Integração com o Herói e Persistência
    ClassDB::bind_method(D_METHOD("aplicar_ao_heroi", "heroi"), &ProgressoJogador::aplicar_ao_heroi);
    ClassDB::bind_method(D_METHOD("salvar", "caminho"), &ProgressoJogador::salvar);
    ClassDB::bind_method(D_METHOD("carregar", "caminho"), &ProgressoJogador::carregar);
}

// Construtor e Destrutor
ProgressoJogador::ProgressoJogador() {}

ProgressoJogador::~ProgressoJogador() {}


// Gestão de Pontos / Economia
int ProgressoJogador::get_pontos() const {
    return pontos;
}

void ProgressoJogador::adicionar_pontos(int qtd) {
    if (qtd > 0) {
        pontos += qtd;
    }
}

bool ProgressoJogador::gastar_pontos(int qtd) {
    if (qtd > 0 && pontos >= qtd) {
        pontos -= qtd;
        return true;
    }
    return false;
}


// Upgrades de Vida e Dano
int ProgressoJogador::custo_vida() const {
    // Fórmula de progressão de custo: base de 20 pontos + 10 por nível já comprado
    return 20 + 10 * nivel_vida;
}

int ProgressoJogador::custo_dano() const {
    // Fórmula de progressão de custo: base de 20 pontos + 10 por nível já comprado
    return 20 + 10 * nivel_dano;
}

bool ProgressoJogador::comprar_vida() {
    if (nivel_vida >= NIVEL_MAXIMO) {
        return false;
    }
    int custo = custo_vida();
    if (gastar_pontos(custo)) {
        nivel_vida++;
        return true;
    }
    return false;
}

bool ProgressoJogador::comprar_dano() {
    if (nivel_dano >= NIVEL_MAXIMO) {
        return false;
    }
    int custo = custo_dano();
    if (gastar_pontos(custo)) {
        nivel_dano++;
        return true;
    }
    return false;
}


// Gestão de Vitórias e Recompensas
bool ProgressoJogador::ja_venceu_professor(int area, int indice) const {
    if (!ProfessoresFabrica::batalha_existe(area, indice)) {
        return false;
    }
    const String chave = String::num_int64(area) + "_" + String::num_int64(indice);
    return contem(professores_derrotados, chave);
}

Array ProgressoJogador::registrar_vitoria_professor(int area, int indice) {
    Array novosItensDesbloqueados;

    // Valida se a batalha solicitada existe
    if (!ProfessoresFabrica::batalha_existe(area, indice)) {
        UtilityFunctions::push_warning("Tentativa de registrar vitória em área/índice inexistente.");
        return novosItensDesbloqueados;
    }

    // Se já derrotou este professor anteriormente, não concede recompensas repetidas
    if (ja_venceu_professor(area, indice)) {
        return novosItensDesbloqueados;
    }

    // Registra a chave da batalha na lista de professores derrotados
    const String chaveBatalha = String::num_int64(area) + "_" + String::num_int64(indice);
    professores_derrotados.push_back(chaveBatalha);

    // Consulta a tabela de drops do professor via GerenciadorRecompensas
    std::vector<ItemId> drops = GerenciadorRecompensas::itensDoProfessor(area, indice);

    for (ItemId item : drops) {
        std::string nomeItem = itemIdParaTexto(item);
        if (nomeItem.empty()) {
            UtilityFunctions::push_error("Item desconhecido retornado pela tabela de recompensas.");
            continue;
        }

        String idString = String(nomeItem.c_str());

        // Desbloqueia o item caso ainda não esteja no inventário permanente
        if (!contem(itens_desbloqueados, idString)) {
            itens_desbloqueados.push_back(idString);
            novosItensDesbloqueados.append(idString);

            // Se o item for especial e o jogador ainda não tiver nenhum equipado, equipa-o automaticamente
            if (item_especial_equipado.is_empty() && ehItemEspecial(item)) {
                item_especial_equipado = idString;
            }
        }
    }

    // Atualiza o contador global de vitórias no TAD Herois (usado para liberar novos golpes)
    Herois::definirVitoriasJogador(static_cast<int>(professores_derrotados.size()));

    return novosItensDesbloqueados;
}


// Gestão de Itens e Equipamentos
Array ProgressoJogador::get_todos_itens() const {
    Array lista;
    for (const String &id : itens_desbloqueados) {
        lista.append(id);
    }
    return lista;
}

bool ProgressoJogador::equipar_item_especial_desbloqueado(const String &id) {
    ItemId itemId;
    // O item precisa ser válido, ser do tipo especial e já estar desbloqueado pelo jogador
    if (!idItemValido(id, itemId) || !ehItemEspecial(itemId) || !contem(itens_desbloqueados, id)) {
        return false;
    }
    item_especial_equipado = id;
    return true;
}

void ProgressoJogador::aplicar_ao_heroi(Object *heroi_obj) {
    Herois *heroi = Object::cast_to<Herois>(heroi_obj);
    if (!heroi) {
        UtilityFunctions::push_error("Falha ao aplicar progresso: objeto fornecido não é uma instância de Herois.");
        return;
    }

    // 1. Aplica bônus acumulados de vida (+10 por nível) e dano base (+2 por nível)
    heroi->aplicar_bonus_permanente(nivel_vida * 10, nivel_dano * 2);

    // 2. Concede todos os itens passivos desbloqueados
    for (const String &id : itens_desbloqueados) {
        ItemId itemId;
        if (idItemValido(id, itemId) && ehItemPassivo(itemId)) {
            heroi->dar_item_passivo(id);
        }
    }

    // 3. Equipa o item especial ativo no quarto slot do herói
    ItemId itemEspecialId;
    if (idItemValido(item_especial_equipado, itemEspecialId) &&
        ehItemEspecial(itemEspecialId) &&
        contem(itens_desbloqueados, item_especial_equipado) &&
        !heroi->possuiItemEspecial()) {
        heroi->dar_item_especial(item_especial_equipado);
    }
}

// =============================================================================
// Persistência em Disco (Save / Load)
// Formato do arquivo (6 linhas):
// Linha 1: Pontos acumulados
// Linha 2: Nível de upgrade de vida
// Linha 3: Nível de upgrade de dano
// Linha 4: ID do item especial equipado
// Linha 5: Lista de professores derrotados ("area_indice,area_indice,...")
// Linha 6: Lista de itens desbloqueados ("ITEM1,ITEM2,...")
// =============================================================================
bool ProgressoJogador::salvar(const String &caminho) {
    if (caminho.strip_edges().is_empty()) {
        UtilityFunctions::push_error("Não foi possível salvar: caminho do arquivo está vazio.");
        return false;
    }

    Ref<FileAccess> file = FileAccess::open(caminho, FileAccess::WRITE);
    if (!file.is_valid()) {
        UtilityFunctions::push_error("Erro ao abrir arquivo para escrita: ", caminho);
        return false;
    }

    // Monta a linha de professores derrotados separados por vírgula
    String linhaVencidos;
    for (size_t i = 0; i < professores_derrotados.size(); ++i) {
        if (i > 0) linhaVencidos += ",";
        linhaVencidos += professores_derrotados[i];
    }

    // Monta a linha de itens desbloqueados separados por vírgula
    String linhaItens;
    for (size_t i = 0; i < itens_desbloqueados.size(); ++i) {
        if (i > 0) linhaItens += ",";
        linhaItens += itens_desbloqueados[i];
    }

    // Gravação sequencial das 6 linhas
    file->store_line(String::num_int64(pontos));
    file->store_line(String::num_int64(nivel_vida));
    file->store_line(String::num_int64(nivel_dano));
    file->store_line(item_especial_equipado);
    file->store_line(linhaVencidos);
    file->store_line(linhaItens);
    file->flush();

    bool sucesso = (file->get_error() == OK);
    if (!sucesso) {
        UtilityFunctions::push_error("Falha durante a gravação dos dados no arquivo: ", caminho);
    }
    return sucesso;
}

bool ProgressoJogador::carregar(const String &caminho) {
    if (caminho.strip_edges().is_empty()) {
        UtilityFunctions::push_error("Não foi possível carregar: caminho do arquivo está vazio.");
        return false;
    }

    // Se o arquivo de save ainda não existe (primeira inicialização do jogo), inicia com os valores padrão
    if (!FileAccess::file_exists(caminho)) {
        return true;
    }

    Ref<FileAccess> file = FileAccess::open(caminho, FileAccess::READ);
    if (!file.is_valid()) {
        UtilityFunctions::push_error("Erro ao abrir arquivo para leitura: ", caminho);
        return false;
    }

    // Leitura das 6 linhas esperadas
    String linhas[6];
    for (String &linha : linhas) {
        if (file->eof_reached()) {
            avisarProgressoInvalido(caminho);
            return false;
        }
        linha = file->get_line();
        if (file->get_error() != OK && file->get_error() != ERR_FILE_EOF) {
            UtilityFunctions::push_error("Erro de E/S na leitura do arquivo de save: ", caminho);
            return false;
        }
    }

    // 1. Validação dos valores numéricos (Pontos, Vida, Dano)
    int novosPontos = 0;
    int novoNivelVida = 0;
    int novoNivelDano = 0;

    if (!lerInteiroNaoNegativo(linhas[0], std::numeric_limits<int>::max(), novosPontos) ||
        !lerInteiroNaoNegativo(linhas[1], NIVEL_MAXIMO, novoNivelVida) ||
        !lerInteiroNaoNegativo(linhas[2], NIVEL_MAXIMO, novoNivelDano)) {
        avisarProgressoInvalido(caminho);
        return false;
    }

    // 2. Validação da lista de professores derrotados
    std::vector<String> novosVencidos;
    const String linhaVencidos = linhas[4].strip_edges();
    if (!linhaVencidos.is_empty()) {
        const PackedStringArray tokens = linhaVencidos.split(",", false);
        for (int i = 0; i < tokens.size(); ++i) {
            const PackedStringArray partesChave = tokens[i].split("_", false);
            int area = -1;
            int indice = -1;
            if (partesChave.size() != 2 ||
                !lerInteiroNaoNegativo(partesChave[0], ProfessoresFabrica::get_quantidade_areas() - 1, area) ||
                !lerInteiroNaoNegativo(partesChave[1], std::numeric_limits<int>::max(), indice) ||
                !ProfessoresFabrica::batalha_existe(area, indice)) {
                avisarProgressoInvalido(caminho);
                return false;
            }
            const String normalizada = String::num_int64(area) + "_" + String::num_int64(indice);
            if (!contem(novosVencidos, normalizada)) {
                novosVencidos.push_back(normalizada);
            }
        }
    }

    // 3. Validação da lista de itens desbloqueados
    std::vector<String> novosItens;
    const String linhaItens = linhas[5].strip_edges();
    if (!linhaItens.is_empty()) {
        const PackedStringArray tokensItens = linhaItens.split(",", false);
        for (int i = 0; i < tokensItens.size(); ++i) {
            const String id = tokensItens[i].strip_edges();
            ItemId itemId;
            if (!idItemValido(id, itemId)) {
                avisarProgressoInvalido(caminho);
                return false;
            }
            if (!contem(novosItens, id)) {
                novosItens.push_back(id);
            }
        }
    }

    // 4. Validação do item especial equipado
    const String novoItemEquipado = linhas[3].strip_edges();
    if (!novoItemEquipado.is_empty()) {
        ItemId itemId;
        if (!idItemValido(novoItemEquipado, itemId) ||
            !ehItemEspecial(itemId) ||
            !contem(novosItens, novoItemEquipado)) {
            avisarProgressoInvalido(caminho);
            return false;
        }
    }

    // 5. Atualização atômica do estado após todas as validações terem sucesso
    pontos = novosPontos;
    nivel_vida = novoNivelVida;
    nivel_dano = novoNivelDano;
    item_especial_equipado = novoItemEquipado;
    professores_derrotados = std::move(novosVencidos);
    itens_desbloqueados = std::move(novosItens);

    // Sincroniza contador de vitórias com Herois
    Herois::definirVitoriasJogador(static_cast<int>(professores_derrotados.size()));

    return true;
}
