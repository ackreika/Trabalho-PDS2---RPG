class_name Itens
# IDs dos itens. O que cada um faz está em ItensConcretos.cpp.

const PASSIVOS := ["HALTER", "SUCO", "JAMES_STEWART", "INTEGRAL", "DERIVADA", "INTERPOLACAO", "REFRIGERADOR"]
const ESPECIAIS := ["LASER", "VETOR", "PONTEIRO"]
const DROPA_ITEM_CHAVE := ["3_1"]

# Item que cada professor dá na PRIMEIRA vez que é derrotado.
# Chave = "area_indice" (area 0..3, indice a partir de 1, como no Global).
const RECOMPENSAS := {
	"0_1": ["JAMES_STEWART"],           # Substituto
	"0_2": ["INTEGRAL", "DERIVADA"],    # Xavier
	"0_3": ["HALTER"],                  # Colina Sombria
	"1_1": ["VETOR"],                   # Viktor Bekert
	"1_2": ["INTERPOLACAO"],            # Alexandre
	"2_3": ["LASER"],                   # Ivonete
	"2_4": ["REFRIGERADOR"],            # Slide Man
	"3_1": ["PONTEIRO"],                # Rezeck
}

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
