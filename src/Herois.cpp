#include "Herois.hpp"
#include "MochilaItens.hpp"
#include "Professor.hpp"
#include "ItensConcretos.hpp"
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <algorithm>
#include <cmath>
#include <limits>

namespace {
/** Converte o ID textual vindo do GDScript ("HALTER"...) em ItemId, usando a tabela única de ItensConcretos. */
bool idParaItem(const String& id, ItemId& saida) {
    return itemIdDeTexto(std::string(id.utf8().get_data()), saida);
}
}

// Por padrão, o estudante começa com os 3 primeiros movimentos do catálogo (0, 1 e 2)
int Herois::idsMovimentosEscolhidos[3] = {0, 1, 2};
int Herois::vitoriasAcumuladas = 0;

Herois::Herois()
    : nome("Estudante"), nivel(1), vida(100), vidaMaxima(100), vidaMaximaBase(100),
      defesa(10), danoBase(15), possuiItemEspecialAtivo(false), itemEspecialAtivo{"", 1.0f, 0, 0},
      possuiItemChave(false) {}

Herois::Herois(std::string nome, int vidaMaxima, int defesa, int danoBase)
    : nome(nome.empty() ? "Estudante" : nome), nivel(1), vida(std::max(1, vidaMaxima)),
      vidaMaxima(std::max(1, vidaMaxima)), vidaMaximaBase(std::max(1, vidaMaxima)),
      defesa(std::max(0, defesa)), danoBase(std::max(0, danoBase)), possuiItemEspecialAtivo(false),
      itemEspecialAtivo{"", 1.0f, 0, 0}, possuiItemChave(false) {}

std::string Herois::getNome() const { return nome; }
int Herois::getVida() const { return vida; }
int Herois::getVidaMaxima() const { return vidaMaxima; }
int Herois::getDefesa() const { return defesa; }
int Herois::getDanoBase() const { return danoBase; }
bool Herois::estaVivo() const { return vida > 0; }

void Herois::curar(int quantidade) {
    if (quantidade <= 0 || vida <= 0) return;
    const int64_t vidaNova = static_cast<int64_t>(vida) + quantidade;
    vida = static_cast<int>(std::min<int64_t>(vidaNova, vidaMaxima));
}

void Herois::receberDano(int danoBase) {
    if (danoBase <= 0 || vida <= 0) return;
    const double divisor = static_cast<double>(vida) + std::max(0, defesa);
    if (divisor <= 0.0) return;
    const double fator = static_cast<double>(vida) / divisor;
    const int danoFinal = std::clamp(static_cast<int>(danoBase * fator), 0, vida);
    vida -= danoFinal;
}

void Herois::aprenderMovimento(const Movimento& novo, int indiceParaSubstituir) {
    if (movimentos.size() < 3) {
        movimentos.push_back(novo);
    } else if (indiceParaSubstituir >= 0 && indiceParaSubstituir < static_cast<int>(movimentos.size())) {
        movimentos[indiceParaSubstituir] = novo;
    }
}

const std::vector<Movimento>& Herois::getMovimentos() const {
    return movimentos;
}

Movimento* Herois::getMovimentoParaEditar(int indice) {
    if (indice < 0 || indice >= static_cast<int>(movimentos.size())) {
        return nullptr;
    }
    return &movimentos[indice];
}

void Herois::definirMovimentoEscolhido(int slot, int idCatalogo) {
    if (slot < 0 || slot >= 3) return;
    if (idCatalogo < 0 || idCatalogo >= Movimento::getQuantidadeCatalogo()) return;
    if (!movimentoEstaDesbloqueado(idCatalogo)) return;

    // Se o movimento já estava em outro slot, faz a troca (swap) entre os slots
    int slotAnterior = encontrarSlotDoMovimento(idCatalogo);
    if (slotAnterior != -1 && slotAnterior != slot) {
        idsMovimentosEscolhidos[slotAnterior] = idsMovimentosEscolhidos[slot];
    }
    idsMovimentosEscolhidos[slot] = idCatalogo;
}

void Herois::definirVitoriasJogador(int vitorias) {
    vitoriasAcumuladas = std::max(0, vitorias);
}

int Herois::getVitoriasJogador() {
    return vitoriasAcumuladas;
}

bool Herois::movimentoEstaDesbloqueado(int idCatalogo) {
    const auto& catalogo = Movimento::getCatalogoMovimentos();
    if (idCatalogo < 0 || idCatalogo >= static_cast<int>(catalogo.size())) {
        return false;
    }
    return catalogo[idCatalogo].estaDesbloqueado(vitoriasAcumuladas);
}

int Herois::getMovimentoEscolhido(int slot) {
    if (slot < 0 || slot >= 3) return 0;
    return idsMovimentosEscolhidos[slot];
}

bool Herois::movimentoJaEquipadoEmOutroSlot(int idCatalogo, int slotIgnorado) {
    for (int i = 0; i < 3; i++) {
        if (i != slotIgnorado && idsMovimentosEscolhidos[i] == idCatalogo) {
            return true;
        }
    }
    return false;
}

int Herois::encontrarSlotDoMovimento(int idCatalogo) {
    for (int i = 0; i < 3; i++) {
        if (idsMovimentosEscolhidos[i] == idCatalogo) {
            return i;
        }
    }
    return -1;
}

void Herois::carregarMovimentosEscolhidos() {
    movimentos.clear();
    for (int i = 0; i < 3; i++) {
        movimentos.push_back(Movimento::criarMovimentoPorId(idsMovimentosEscolhidos[i]));
    }
}

void Herois::adicionarItemPassivo(ItemPassivo item) {
    if (item.nome.empty() || !std::isfinite(item.multiplicador) || item.multiplicador <= 0.0f) {
        UtilityFunctions::push_warning("Item passivo inválido ignorado.");
        return;
    }
    for (const ItemPassivo &existente : itensPassivos) {
        if (existente.nome == item.nome) return;
    }
    itensPassivos.push_back(item);
    recalcularVidaMaxima();
}

int Herois::getQuantidadeItensPassivos() const {
    return MochilaItens(*this).getQuantidadeItensPassivos();
}

std::string Herois::getNomeItemPassivo(int indice) const {
    return MochilaItens(*this).getNomeItemPassivo(indice);
}

int Herois::calcularDanoFinal(int danoBase) const {
    if (danoBase <= 0) return 0;
    double dano = danoBase;
    for (const auto& item : itensPassivos) {
        if (item.tipo == TipoEfeito::BONUS_DANO_CAUSADO && std::isfinite(item.multiplicador) && item.multiplicador > 0.0f) {
            dano *= item.multiplicador;
            if (dano >= std::numeric_limits<int>::max()) return std::numeric_limits<int>::max();
        }
    }
    return std::max(0, static_cast<int>(dano));
}

int Herois::calcularVidaFinal(int vidaBase) const {
    if (vidaBase <= 0) return 1;
    double vidaFinal = vidaBase;
    for (const auto& item : itensPassivos) {
        if (item.tipo == TipoEfeito::BONUS_VIDA_MAXIMA && std::isfinite(item.multiplicador) && item.multiplicador > 0.0f) {
            vidaFinal *= item.multiplicador;
            if (vidaFinal >= std::numeric_limits<int>::max()) return std::numeric_limits<int>::max();
        }
    }
    return std::max(1, static_cast<int>(vidaFinal));
}

void Herois::recalcularVidaMaxima() {
    int novaVidaMaxima = calcularVidaFinal(vidaMaximaBase);
    int delta = novaVidaMaxima - vidaMaxima;
    vidaMaxima = novaVidaMaxima;

    if (delta > 0) {
        vida += delta;
    } else if (vida > vidaMaxima) {
        vida = vidaMaxima;
    }
}

void Herois::definirItemEspecial(ItemEspecial item) {
    if (item.nome.empty() || !std::isfinite(item.multiplicadorDano) || item.multiplicadorDano <= 0.0f ||
        item.usosMaximos <= 0 || item.usosRestantes < 0 || item.usosRestantes > item.usosMaximos) {
        UtilityFunctions::push_warning("Item especial inválido; equipamento não alterado.");
        return;
    }
    itemEspecialAtivo = item;
    possuiItemEspecialAtivo = true;
}

bool Herois::possuiItemEspecial() const { return possuiItemEspecialAtivo; }

std::string Herois::getNomeItemEspecial() const {
    return possuiItemEspecialAtivo ? itemEspecialAtivo.nome : "";
}

bool Herois::itemEspecialPodeUsar() const {
    return possuiItemEspecialAtivo && itemEspecialAtivo.usosRestantes > 0;
}

int Herois::usarItemEspecial(Professor& alvo) {
    if (!itemEspecialPodeUsar() || !alvo.estaVivo()) {
        return 0;
    }
    itemEspecialAtivo.usosRestantes--;
    const double danoCalculado = static_cast<double>(danoBase) * itemEspecialAtivo.multiplicadorDano;
    const int dano = static_cast<int>(std::clamp(danoCalculado, 0.0, static_cast<double>(std::numeric_limits<int>::max())));
    alvo.receberDano(dano);
    return dano;
}

void Herois::obterItemChave() { possuiItemChave = true; }
bool Herois::possuiItemDeChave() const { return possuiItemChave; }
void Herois::consumirItemChave() { possuiItemChave = false; }

void Herois::definirSprite(EstadoVisual estado, std::string caminho) {
    spritesPorEstado[estado] = caminho;
}

std::string Herois::getCaminhoSprite(EstadoVisual estado) const {
    auto it = spritesPorEstado.find(estado);
    if (it != spritesPorEstado.end()) {
        return it->second;
    }
    auto fallback = spritesPorEstado.find(EstadoVisual::PARADO);
    return fallback != spritesPorEstado.end() ? fallback->second : "";
}



void Herois::inicializar(String nomeNovo, int vidaMaximaNova, int defesaNova, int danoBaseNovo) {
    if (vidaMaximaNova <= 0 || defesaNova < 0 || danoBaseNovo < 0) {
        UtilityFunctions::push_error("Herói não inicializado: vida máxima, defesa ou dano inválido.");
        return;
    }
    nome = nomeNovo.strip_edges().is_empty() ? "Estudante" : std::string(nomeNovo.utf8().get_data());
    vidaMaximaBase = vidaMaximaNova;
    vidaMaxima = vidaMaximaNova;
    vida = vidaMaximaNova;
    defesa = defesaNova;
    danoBase = danoBaseNovo;

    // Já carrega automaticamente os 3 movimentos escolhidos no Menu de Movimentos C++
    carregarMovimentosEscolhidos();
}

String Herois::get_nome() const { return String::utf8(nome.c_str()); }
int Herois::get_vida() const { return vida; }
int Herois::get_vida_maxima() const { return vidaMaxima; }
int Herois::get_defesa() const { return defesa; }
int Herois::get_dano_base() const { return danoBase; }
bool Herois::esta_vivo() const { return estaVivo(); }

void Herois::aprender_movimento_simples(String nomeMovimento, int dano, int usosMaximos) {
    std::string nomeConvertido = std::string(nomeMovimento.utf8().get_data());
    aprenderMovimento(Movimento(nomeConvertido, dano, usosMaximos));
}

void Herois::carregar_movimentos_escolhidos() {
    carregarMovimentosEscolhidos();
}

int Herois::get_quantidade_movimentos() const {
    return static_cast<int>(movimentos.size());
}

String Herois::get_nome_movimento(int indice) const {
    if (indice < 0 || indice >= static_cast<int>(movimentos.size())) {
        return String("");
    }
    const Movimento& m = movimentos[indice];
    std::string texto = m.getNome() + "\n(";
    if (m.getDano() > 0) {
        texto += std::to_string(m.getDano()) + " Dano";
    }
    if (m.getCura() > 0) {
        if (m.getDano() > 0) texto += " ";
        texto += "+" + std::to_string(m.getCura()) + " HP";
    }
    if (m.getUsosMaximos() < 0) {
        texto += " | Ilimitado)";
    } else {
        texto += " | " + std::to_string(m.getUsosRestantes()) + "/" + std::to_string(m.getUsosMaximos()) + ")";
    }
    return String::utf8(texto.c_str());
}

int Herois::get_dano_movimento(int indice) const {
    if (indice < 0 || indice >= static_cast<int>(movimentos.size())) {
        return 0;
    }
    return movimentos[indice].getDano();
}

int Herois::get_cura_movimento(int indice) const {
    if (indice < 0 || indice >= static_cast<int>(movimentos.size())) {
        return 0;
    }
    return movimentos[indice].getCura();
}

int Herois::get_usos_restantes_movimento(int indice) const {
    if (indice < 0 || indice >= static_cast<int>(movimentos.size())) {
        return 0;
    }
    if (movimentos[indice].getUsosMaximos() < 0) {
        return -1;
    }
    return movimentos[indice].getUsosRestantes();
}

bool Herois::movimento_pode_usar(int indice) const {
    if (indice < 0 || indice >= static_cast<int>(movimentos.size())) {
        return false;
    }
    return movimentos[indice].podeUsar();
}

int Herois::get_quantidade_catalogo_movimentos() {
    return Movimento::getQuantidadeCatalogo();
}

Dictionary Herois::descrever_movimento_catalogo(int idCatalogo) {
    Dictionary d;
    const auto& catalogo = Movimento::getCatalogoMovimentos();
    if (idCatalogo < 0 || idCatalogo >= static_cast<int>(catalogo.size())) {
        d["valido"] = false;
        return d;
    }
    const Movimento& m = catalogo[idCatalogo];
    d["valido"] = true;
    d["id"] = idCatalogo;
    d["nome"] = String::utf8(m.getNome().c_str());
    d["disciplina"] = String::utf8(m.getDisciplina().c_str());
    d["dano"] = m.getDano();
    d["cura"] = m.getCura();
    d["usos_maximos"] = m.getUsosMaximos();
    d["descricao"] = String::utf8(m.getDescricao().c_str());
    d["slot_equipado"] = encontrarSlotDoMovimento(idCatalogo);
    return d;
}

void Herois::selecionar_movimento_catalogo(int slot, int idCatalogo) {
    definirMovimentoEscolhido(slot, idCatalogo);
}

int Herois::get_id_movimento_slot(int slot) {
    return getMovimentoEscolhido(slot);
}

void Herois::definir_sprite(int estado, String caminho) {
    definirSprite(static_cast<EstadoVisual>(estado), std::string(caminho.utf8().get_data()));
}

int Herois::get_quantidade_itens_passivos() const { return getQuantidadeItensPassivos(); }
String Herois::get_nome_item_passivo(int indice) const { return String::utf8(getNomeItemPassivo(indice).c_str()); }

void Herois::aplicar_bonus_permanente(int bonusVida, int bonusDano) {
    if (bonusVida < 0 || bonusDano < 0) {
        UtilityFunctions::push_warning("Bônus permanente negativo ignorado.");
        return;
    }
    vidaMaximaBase = static_cast<int>(std::min<int64_t>(static_cast<int64_t>(vidaMaximaBase) + bonusVida,
                                                         std::numeric_limits<int>::max()));
    danoBase = static_cast<int>(std::min<int64_t>(static_cast<int64_t>(danoBase) + bonusDano,
                                                   std::numeric_limits<int>::max()));
    recalcularVidaMaxima();
    vida = vidaMaxima;
}

void Herois::aumentar_poder_movimento(int indice, int incremento) {
    Movimento* movimento = getMovimentoParaEditar(indice);
    if (movimento && incremento > 0) movimento->aumentarPoder(incremento);
}

String Herois::dar_item_passivo(String id) {
    ItemId itemId;
    if (!idParaItem(id, itemId) || ehItemEspecial(itemId)) return String("");
    ItemPassivo item = criarItemPassivo(itemId);
    adicionarItemPassivo(item);
    return String::utf8(item.nome.c_str());
}

String Herois::dar_item_especial(String id) {
    ItemId itemId;
    if (!idParaItem(id, itemId) || !ehItemEspecial(itemId)) return String("");
    ItemEspecial item = criarItemEspecial(itemId);
    definirItemEspecial(item);
    return String::utf8(item.nome.c_str());
}

Dictionary Herois::descrever_item_passivo(String id) {
    Dictionary d;
    ItemId itemId;
    if (!idParaItem(id, itemId) || ehItemEspecial(itemId)) { d["valido"] = false; return d; }
    ItemPassivo item = criarItemPassivo(itemId);
    d["valido"] = true;
    d["nome"] = String::utf8(item.nome.c_str());
    d["efeito"] = (item.tipo == TipoEfeito::BONUS_VIDA_MAXIMA) ? 1 : 0;
    d["multiplicador"] = item.multiplicador;
    return d;
}

Dictionary Herois::descrever_item_especial(String id) {
    Dictionary d;
    ItemId itemId;
    if (!idParaItem(id, itemId) || !ehItemEspecial(itemId)) { d["valido"] = false; return d; }
    ItemEspecial item = criarItemEspecial(itemId);
    d["valido"] = true;
    d["nome"] = String::utf8(item.nome.c_str());
    d["multiplicador"] = item.multiplicadorDano;
    d["usos"] = item.usosMaximos;
    return d;
}

bool Herois::possui_item_especial() const { return MochilaItens(*this).possuiItemEspecialEquipado(); }
String Herois::get_nome_item_especial() const { return String::utf8(MochilaItens(*this).getNomeItemEspecialEquipado().c_str()); }
int Herois::get_usos_item_especial() const { return possuiItemEspecialAtivo ? itemEspecialAtivo.usosRestantes : 0; }

void Herois::obter_item_chave() { obterItemChave(); }
bool Herois::possui_item_chave() const { return possuiItemDeChave(); }

String Herois::get_caminho_sprite(int estado) const {
    return String::utf8(getCaminhoSprite(static_cast<EstadoVisual>(estado)).c_str());
}

void Herois::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_usos_restantes_movimento", "indice"), &Herois::get_usos_restantes_movimento);
    ClassDB::bind_method(D_METHOD("movimento_pode_usar", "indice"), &Herois::movimento_pode_usar);
    ClassDB::bind_method(D_METHOD("definir_sprite", "estado", "caminho"), &Herois::definir_sprite);
    ClassDB::bind_method(D_METHOD("get_caminho_sprite", "estado"), &Herois::get_caminho_sprite);
    ClassDB::bind_method(D_METHOD("inicializar", "nome", "vida_maxima", "defesa", "dano_base"), &Herois::inicializar);
    ClassDB::bind_method(D_METHOD("get_nome"), &Herois::get_nome);
    ClassDB::bind_method(D_METHOD("get_vida"), &Herois::get_vida);
    ClassDB::bind_method(D_METHOD("get_vida_maxima"), &Herois::get_vida_maxima);
    ClassDB::bind_method(D_METHOD("get_defesa"), &Herois::get_defesa);
    ClassDB::bind_method(D_METHOD("get_dano_base"), &Herois::get_dano_base);
    ClassDB::bind_method(D_METHOD("esta_vivo"), &Herois::esta_vivo);
    ClassDB::bind_method(D_METHOD("curar", "quantidade"), &Herois::curar);
    ClassDB::bind_method(D_METHOD("receber_dano", "dano_base"), &Herois::receberDano);
    ClassDB::bind_method(D_METHOD("aprender_movimento_simples", "nome", "dano", "usos_maximos"), &Herois::aprender_movimento_simples);
    ClassDB::bind_method(D_METHOD("carregar_movimentos_escolhidos"), &Herois::carregar_movimentos_escolhidos);
    ClassDB::bind_method(D_METHOD("get_quantidade_movimentos"), &Herois::get_quantidade_movimentos);
    ClassDB::bind_method(D_METHOD("get_nome_movimento", "indice"), &Herois::get_nome_movimento);
    ClassDB::bind_method(D_METHOD("get_dano_movimento", "indice"), &Herois::get_dano_movimento);
    ClassDB::bind_method(D_METHOD("get_cura_movimento", "indice"), &Herois::get_cura_movimento);
    ClassDB::bind_method(D_METHOD("get_quantidade_itens_passivos"), &Herois::get_quantidade_itens_passivos);
    ClassDB::bind_method(D_METHOD("get_nome_item_passivo", "indice"), &Herois::get_nome_item_passivo);
    ClassDB::bind_method(D_METHOD("possui_item_especial"), &Herois::possui_item_especial);
    ClassDB::bind_method(D_METHOD("get_nome_item_especial"), &Herois::get_nome_item_especial);
    ClassDB::bind_method(D_METHOD("get_usos_item_especial"), &Herois::get_usos_item_especial);
    ClassDB::bind_method(D_METHOD("obter_item_chave"), &Herois::obter_item_chave);
    ClassDB::bind_method(D_METHOD("possui_item_chave"), &Herois::possui_item_chave);
    ClassDB::bind_method(D_METHOD("aplicar_bonus_permanente", "bonus_vida", "bonus_dano"), &Herois::aplicar_bonus_permanente);
    ClassDB::bind_method(D_METHOD("aumentar_poder_movimento", "indice", "incremento"), &Herois::aumentar_poder_movimento);
    ClassDB::bind_method(D_METHOD("dar_item_passivo", "id"), &Herois::dar_item_passivo);
    ClassDB::bind_method(D_METHOD("dar_item_especial", "id"), &Herois::dar_item_especial);
    ClassDB::bind_static_method("Herois", D_METHOD("descrever_item_passivo", "id"), &Herois::descrever_item_passivo);
    ClassDB::bind_static_method("Herois", D_METHOD("descrever_item_especial", "id"), &Herois::descrever_item_especial);
    ClassDB::bind_static_method("Herois", D_METHOD("get_quantidade_catalogo_movimentos"), &Herois::get_quantidade_catalogo_movimentos);
    ClassDB::bind_static_method("Herois", D_METHOD("descrever_movimento_catalogo", "id_catalogo"), &Herois::descrever_movimento_catalogo);
    ClassDB::bind_static_method("Herois", D_METHOD("selecionar_movimento_catalogo", "slot", "id_catalogo"), &Herois::selecionar_movimento_catalogo);
    ClassDB::bind_static_method("Herois", D_METHOD("get_id_movimento_slot", "slot"), &Herois::get_id_movimento_slot);
}
