extends Node

const ARQUIVO := "user://progresso.txt"
const BONUS_VIDA_POR_NIVEL := 10
const BONUS_DANO_POR_NIVEL := 2
const PONTOS_COMUM := 10
const PONTOS_MINIBOSS := 25
const PONTOS_BOSS := 50
const PONTOS_BATALHA_FALSA := 5

var _cpp: ProgressoJogador
var _id_heroi_aplicado: int = 0
var nivel_vida: int:
	get: return get_nivel_vida()
var nivel_dano: int:
	get: return get_nivel_dano()
var item_especial_equipado: String:
	get: return _cpp.get_item_especial_equipado() if _cpp else ""

func _ready() -> void:
	if ClassDB.class_exists("ProgressoJogador"):
		_cpp = ProgressoJogador.new()
		_cpp.carregar(ARQUIVO)
	else:
		push_error("A extensão C++ não registrou ProgressoJogador; progresso persistente está indisponível.")

func get_pontos() -> int:
	return _cpp.get_pontos() if _cpp else 0

func get_nivel_vida() -> int:
	return _cpp.get_nivel_vida() if _cpp else 0

func get_nivel_dano() -> int:
	return _cpp.get_nivel_dano() if _cpp else 0

func custo_vida() -> int:
	return _cpp.custo_vida() if _cpp else 0

func custo_dano() -> int:
	return _cpp.custo_dano() if _cpp else 0

func comprar_vida() -> bool:
	if not _cpp or not _cpp.comprar_vida():
		return false
	_salvar_progresso()
	return true

func comprar_dano() -> bool:
	if not _cpp or not _cpp.comprar_dano():
		return false
	_salvar_progresso()
	return true

func equipar_item_especial(id: String) -> bool:
	if not _cpp or id.strip_edges().is_empty() or not _cpp.equipar_item_especial_desbloqueado(id):
		return false
	_salvar_progresso()
	return true

func aplicar_ao_heroi(heroi: Herois) -> void:
	if not is_instance_valid(heroi):
		return
		
	var id_atual = heroi.get_instance_id()
	
	if _cpp and id_atual != _id_heroi_aplicado:
		_cpp.aplicar_ao_heroi(heroi)
		_id_heroi_aplicado = id_atual

func registrar_vitoria(professor: Professor) -> int:
	if not is_instance_valid(professor):
		push_error("Vitória não registrada: referência de professor inválida.")
		return 0
	var ganhos := PONTOS_BATALHA_FALSA if professor.is_batalha_falsa() else PONTOS_COMUM
	if not professor.is_batalha_falsa():
		match professor.get_tipo():
			1: ganhos = PONTOS_MINIBOSS
			2: ganhos = PONTOS_BOSS
			0: pass
			_:
				push_error("Vitória não registrada: tipo de professor desconhecido.")
				return 0
	if _cpp:
		_cpp.adicionar_pontos(ganhos)
		_salvar_progresso()
	return ganhos

func desbloquear_por_vitoria(area: int, indice: int) -> Array:
	if not _cpp or not ProfessoresFabrica.batalha_existe(area, indice):
		return []
	if ja_venceu_professor(area, indice):
		return []
	var novos: Array = _cpp.registrar_vitoria_professor(area, indice)
	_salvar_progresso()
	return novos

func get_todos_itens() -> Array:
	return _cpp.get_todos_itens() if _cpp else []

func ja_venceu_professor(area: int, indice: int) -> bool:
	return _cpp.ja_venceu_professor(area, indice) if _cpp and ProfessoresFabrica.batalha_existe(area, indice) else false

func dropa_item_chave(area: int, indice: int) -> bool:
	return Recompensas.dropa_item_chave(area, indice) if ProfessoresFabrica.batalha_existe(area, indice) else false

func _salvar_progresso() -> void:
	if _cpp and not _cpp.salvar(ARQUIVO):
		push_error("O progresso mudou nesta sessão, mas não foi possível gravar o save.")
		
func resetar_progresso() -> void:
	if FileAccess.file_exists(ARQUIVO):
		DirAccess.remove_absolute(ARQUIVO)
	if ClassDB.class_exists("ProgressoJogador"):
		_cpp = ProgressoJogador.new()
