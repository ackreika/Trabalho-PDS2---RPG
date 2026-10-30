class_name Itens
# IDs dos itens. O que cada um faz está em ItensConcretos.cpp.

const PASSIVOS := ["HALTER", "SUCO", "JAMES_STEWART", "INTEGRAL", "DERIVADA", "INTERPOLACAO", "REFRIGERADOR"]
const ESPECIAIS := ["LASER", "VETOR", "PONTEIRO"]
# A tabela de drops (qual professor dá qual item, e quem dá o item chave) fica em
# C++ (GerenciadorRecompensas.cpp) e é consultada por Recompensas.itens_do_professor()
# e Recompensas.dropa_item_chave().

static func nome_item(id: String) -> String:
	var d: Dictionary = Herois.descrever_item_passivo(id) if id in PASSIVOS else Herois.descrever_item_especial(id)
	return d["nome"] if d["valido"] else id

static func descricao_passivo(id: String) -> String:
	var d: Dictionary = Herois.descrever_item_passivo(id)
	if not d["valido"]:
		return id
	var pct := int(round((d["multiplicador"] - 1.0) * 100.0))
	var o_que := "dano causado" if d["efeito"] == 0 else "vida maxima"
	return "%s: +%d%% de %s" % [d["nome"], pct, o_que]

static func descricao_especial(id: String) -> String:
	var d: Dictionary = Herois.descrever_item_especial(id)
	if not d["valido"]:
		return id
	return "%s: golpe de %.1fx o dano base, %d uso(s) por run" % [d["nome"], d["multiplicador"], d["usos"]]
