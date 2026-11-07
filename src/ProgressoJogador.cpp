#include "ProgressoJogador.hpp"
#include <fstream>
#include <sstream>

ProgressoJogador::ProgressoJogador() : pontos(0) {}
// Implementação dos métodos da classe ProgressoJogador

int ProgressoJogador::getPontos() const { return pontos; }
//retorna a quantidade de pontos de progresso do jogador

void ProgressoJogador::adicionarPontos(int quantidade) {
    pontos += quantidade;
}//adiciona a quantidade de pontos de progresso do jogador

bool ProgressoJogador::gastarPontos(int quantidade) {
    if (pontos < quantidade) return false;
    pontos -= quantidade;
    return true;
}// retorna false se não tiver pontos suficientes

bool ProgressoJogador::jaObteve(ItemId id) const {
    return itensDesbloqueados.count(static_cast<int>(id)) > 0;
}//retorna true se o jogador já desbloqueou o item com o ID especificado, false caso contrário

void ProgressoJogador::desbloquear(ItemId id) {
    itensDesbloqueados.insert(static_cast<int>(id));
}//adiciona o ID do item ao conjunto de itens desbloqueados do jogador

const std::set<int>& ProgressoJogador::getTodos() const {
    return itensDesbloqueados;
}//retorna o conjunto de IDs de itens que o jogador já desbloqueou

void ProgressoJogador::salvar(const std::string& caminho) const {
    std::ofstream arquivo(caminho);
    arquivo << pontos << "\n";

<<<<<<< Updated upstream
    bool primeiro = true;
    for (int id : itensDesbloqueados) {
        if (!primeiro) arquivo << ",";
        arquivo << id;
        primeiro = false;
    }
    arquivo << "\n";
}//salva o progresso do jogador em um arquivo no caminho especificado
=======
    ClassDB::bind_method(D_METHOD("get_nivel_vida"), &ProgressoJogador::get_nivel_vida);
    ClassDB::bind_method(D_METHOD("get_nivel_dano"), &ProgressoJogador::get_nivel_dano);
    ClassDB::bind_method(D_METHOD("get_quantidade_vitorias"), &ProgressoJogador::get_quantidade_vitorias);
    ClassDB::bind_method(D_METHOD("get_item_especial_equipado"), &ProgressoJogador::get_item_especial_equipado);
    ClassDB::bind_method(D_METHOD("equipar_item_especial", "id"), &ProgressoJogador::equipar_item_especial);
    ClassDB::bind_method(D_METHOD("get_todos_itens"), &ProgressoJogador::get_todos_itens);
    ClassDB::bind_method(D_METHOD("ja_venceu_professor", "area", "indice"), &ProgressoJogador::ja_venceu_professor);
    ClassDB::bind_method(D_METHOD("registrar_vitoria_professor", "area", "indice"), &ProgressoJogador::registrar_vitoria_professor);
    ClassDB::bind_method(D_METHOD("equipar_item_especial_desbloqueado", "id"), &ProgressoJogador::equipar_item_especial_desbloqueado);

    ClassDB::bind_method(D_METHOD("comprar_vida"), &ProgressoJogador::comprar_vida);
    ClassDB::bind_method(D_METHOD("comprar_dano"), &ProgressoJogador::comprar_dano);
    ClassDB::bind_method(D_METHOD("custo_vida"), &ProgressoJogador::custo_vida);
    ClassDB::bind_method(D_METHOD("custo_dano"), &ProgressoJogador::custo_dano);

    ClassDB::bind_method(D_METHOD("aplicar_ao_heroi", "heroi"), &ProgressoJogador::aplicar_ao_heroi);

    ClassDB::bind_method(D_METHOD("salvar", "caminho"), &ProgressoJogador::salvar);
    ClassDB::bind_method(D_METHOD("carregar", "caminho"), &ProgressoJogador::carregar);
}

ProgressoJogador::ProgressoJogador() {}
ProgressoJogador::~ProgressoJogador() {}

int ProgressoJogador::get_pontos() const { return pontos; }
void ProgressoJogador::adicionar_pontos(int qtd) {
    if (qtd <= 0) return;
    const int64_t total = static_cast<int64_t>(pontos) + qtd;
    pontos = static_cast<int>(std::min<int64_t>(total, std::numeric_limits<int>::max()));
}

Array ProgressoJogador::get_todos_itens() const {
    Array result;
    for (const String &id : itens_desbloqueados) result.append(id);
    return result;
}

bool ProgressoJogador::ja_venceu_professor(int area, int indice) const {
    if (!ProfessoresFabrica::batalha_existe(area, indice)) return false;
    const String chave = String::num_int64(area) + "_" + String::num_int64(indice);
    for (const String &s : professores_derrotados) if (s == chave) return true;
    return false;
}

Array ProgressoJogador::registrar_vitoria_professor(int area, int indice) {
    Array novos;
    if (!ProfessoresFabrica::batalha_existe(area, indice)) {
        UtilityFunctions::push_warning("Vitória ignorada: área ou índice de batalha inválido.");
        return novos;
    }
    if (ja_venceu_professor(area, indice)) return novos;
        std::vector<String> novosVencidos = professores_derrotados;
        std::vector<String> novosItens = itens_desbloqueados;
        String novoItemEquipado = item_especial_equipado;
        novosVencidos.push_back(String::num_int64(area) + "_" + String::num_int64(indice));
        const char *nomes[] = {"HALTER", "SUCO", "LASER", "JAMES_STEWART", "INTEGRAL", "DERIVADA", "VETOR", "INTERPOLACAO", "REFRIGERADOR", "PONTEIRO"};
        for (ItemId item : GerenciadorRecompensas::itensDoProfessor(area, indice)) {
            const int indiceItem = static_cast<int>(item);
            if (indiceItem < 0 || indiceItem >= static_cast<int>(sizeof(nomes) / sizeof(nomes[0]))) {
                UtilityFunctions::push_error("A tabela de recompensas contém um ItemId inválido.");
                continue;
            }
            const String id = nomes[indiceItem];
            if (contem(novosItens, id)) continue;
            novosItens.push_back(id);
            novos.append(id);
            ItemId itemId;
            if (novoItemEquipado.is_empty() && idItemValido(id, itemId) && ehItemEspecial(itemId)) {
                novoItemEquipado = id;
            }
        }
        professores_derrotados = std::move(novosVencidos);
        itens_desbloqueados = std::move(novosItens);
        item_especial_equipado = std::move(novoItemEquipado);
        Herois::definirVitoriasJogador(static_cast<int>(professores_derrotados.size()));
    return novos;
}
>>>>>>> Stashed changes

ProgressoJogador ProgressoJogador::carregar(const std::string& caminho) {
    ProgressoJogador progresso;
    std::ifstream arquivo(caminho);

    if (!arquivo.is_open()) {
        return progresso; // primeira vez jogando, nada salvo ainda
    }

    arquivo >> progresso.pontos;
    arquivo.ignore(); // pula a quebra de linha

    std::string linhaItens;
    std::getline(arquivo, linhaItens);

    std::stringstream ss(linhaItens);
    std::string idStr;
    while (std::getline(ss, idStr, ',')) {
        if (!idStr.empty()) {
            progresso.desbloquear(static_cast<ItemId>(std::stoi(idStr)));
        }
    }

<<<<<<< Updated upstream
    return progresso;
}//carrega o progresso do jogador a partir de um arquivo no caminho especificado
 //e retorna um objeto ProgressoJogador
=======
    ItemId itemEspecial;
    if (idItemValido(item_especial_equipado, itemEspecial) && ehItemEspecial(itemEspecial) &&
        contem(itens_desbloqueados, item_especial_equipado) && !heroi->possuiItemEspecial()) {
        heroi->dar_item_especial(item_especial_equipado);
    }
}

bool ProgressoJogador::salvar(const String &caminho) {
    if (caminho.strip_edges().is_empty()) {
        UtilityFunctions::push_error("Não foi possível salvar o progresso: caminho vazio.");
        return false;
    }
    Ref<FileAccess> file = FileAccess::open(caminho, FileAccess::WRITE);
    if (!file.is_valid()) {
        UtilityFunctions::push_error("Não foi possível abrir o arquivo de progresso para escrita:", caminho,
                                     "Erro:", static_cast<int>(FileAccess::get_open_error()));
        return false;
    }

    String vencidos;
    for (const String &s : professores_derrotados) {
        if (!vencidos.is_empty()) vencidos += ",";
        vencidos += s;
    }
    String itens;
    for (const String &s : itens_desbloqueados) {
        if (!itens.is_empty()) itens += ",";
        itens += s;
    }

    bool sucesso = true;
    file->store_line(String::num_int64(pontos));
    file->store_line(String::num_int64(nivel_vida));
    file->store_line(String::num_int64(nivel_dano));
    file->store_line(item_especial_equipado);
    file->store_line(vencidos);
    file->store_line(itens);
    file->flush();
    sucesso = sucesso && file->get_error() == OK;
    if (!sucesso) UtilityFunctions::push_error("Falha ao gravar o arquivo de progresso:", caminho);
    return sucesso;
}

bool ProgressoJogador::carregar(const String &caminho) {
    if (caminho.strip_edges().is_empty()) {
        UtilityFunctions::push_error("Não foi possível carregar o progresso: caminho vazio.");
        return false;
    }
    if (!FileAccess::file_exists(caminho)) return true;

    Ref<FileAccess> file = FileAccess::open(caminho, FileAccess::READ);
    if (!file.is_valid()) {
        UtilityFunctions::push_error("Não foi possível abrir o arquivo de progresso:", caminho,
                                     "Erro:", static_cast<int>(FileAccess::get_open_error()));
        return false;
    }

    String linhas[6];
    for (String &linha : linhas) {
        if (file->eof_reached()) {
            avisarProgressoInvalido(caminho);
            return false;
        }
        linha = file->get_line();
        if (file->get_error() != OK && file->get_error() != ERR_FILE_EOF) {
            UtilityFunctions::push_error("Erro de leitura no arquivo de progresso:", caminho);
            return false;
        }
    }

    int novosPontos = 0;
    int novoNivelVida = 0;
    int novoNivelDano = 0;
    if (!lerInteiroNaoNegativo(linhas[0], std::numeric_limits<int>::max(), novosPontos) ||
        !lerInteiroNaoNegativo(linhas[1], NIVEL_MAXIMO, novoNivelVida) ||
        !lerInteiroNaoNegativo(linhas[2], NIVEL_MAXIMO, novoNivelDano)) {
        avisarProgressoInvalido(caminho);
        return false;
    }

    std::vector<String> novosVencidos;
    const String linhaVencidos = linhas[4].strip_edges();
    if (!linhaVencidos.is_empty()) {
        const PackedStringArray values = linhaVencidos.split(",", false);
        for (int i = 0; i < values.size(); ++i) {
            const PackedStringArray chave = values[i].split("_", false);
            int area = -1;
            int indice = -1;
            if (chave.size() != 2 || !lerInteiroNaoNegativo(chave[0], ProfessoresFabrica::get_quantidade_areas() - 1, area) ||
                !lerInteiroNaoNegativo(chave[1], std::numeric_limits<int>::max(), indice) ||
                !ProfessoresFabrica::batalha_existe(area, indice)) {
                avisarProgressoInvalido(caminho);
                return false;
            }
            const String normalizada = String::num_int64(area) + "_" + String::num_int64(indice);
            if (!contem(novosVencidos, normalizada)) novosVencidos.push_back(normalizada);
        }
    }

    std::vector<String> novosItens;
    const String linhaItens = linhas[5].strip_edges();
    if (!linhaItens.is_empty()) {
        const PackedStringArray values = linhaItens.split(",", false);
        for (int i = 0; i < values.size(); ++i) {
            const String id = values[i].strip_edges();
            ItemId itemId;
            if (!idItemValido(id, itemId)) {
                avisarProgressoInvalido(caminho);
                return false;
            }
            if (!contem(novosItens, id)) novosItens.push_back(id);
        }
    }

    const String novoItemEquipado = linhas[3].strip_edges();
    if (!novoItemEquipado.is_empty()) {
        ItemId itemId;
        if (!idItemValido(novoItemEquipado, itemId) || !ehItemEspecial(itemId) ||
            !contem(novosItens, novoItemEquipado)) {
            avisarProgressoInvalido(caminho);
            return false;
        }
    }

    pontos = novosPontos;
    nivel_vida = novoNivelVida;
    nivel_dano = novoNivelDano;
    item_especial_equipado = novoItemEquipado;
    professores_derrotados = std::move(novosVencidos);
    itens_desbloqueados = std::move(novosItens);
    Herois::definirVitoriasJogador(static_cast<int>(professores_derrotados.size()));
    return true;
}
>>>>>>> Stashed changes
