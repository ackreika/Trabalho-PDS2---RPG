class_name Itens

const PASSIVOS := ["HALTER", "SUCO"]
const ESPECIAIS := ["LASER"]

static func sortear_passivo() -> String:
	return PASSIVOS.pick_random()

static func sortear_especial() -> String:
	return ESPECIAIS.pick_random()
