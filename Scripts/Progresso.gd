extends Node
# Autoload "Progresso": pontos, upgrades e itens desbloqueados. Tudo salvo em disco.

const ARQUIVO := "user://progresso.cfg"

# ---------- Balanceamento: mexa só aqui ----------
const PONTOS_COMUM := 10
const PONTOS_MINIBOSS := 25
const PONTOS_BOSS := 50
const PONTOS_BATALHA_FALSA := 5

const BONUS_VIDA_POR_NIVEL := 10
const BONUS_DANO_POR_NIVEL := 2

const CUSTO_BASE := 20
const CUSTO_ACRESCIMO_POR_NIVEL := 10

var pontos: int = 0
var nivel_vida: int = 0
var nivel_dano: int = 0

var professores_derrotados: Array = []   # chaves "area_indice" já vencidas
var itens_desbloqueados: Array = []      # IDs dos itens já ganhos
var item_especial_equipado: String = "" # item ativo que começa cada run

func _ready() -> void:
	carregar()

# ---------- Ganho de pontos ----------

func pontos_por_vitoria(professor: Professor) -> int:
	if professor.is_batalha_falsa():
		return PONTOS_BATALHA_FALSA
	match professor.get_tipo(): # 0 = comum, 1 = miniboss, 2 = boss
		1:
			return PONTOS_MINIBOSS
		2:
			return PONTOS_BOSS
		_:
			return PONTOS_COMUM

func registrar_vitoria(professor: Professor) -> int:
	var ganhos: int = pontos_por_vitoria(professor)
	pontos += ganhos
	salvar()
	return ganhos

# ---------- Itens ----------

## Chame após uma vitória. Devolve o ID do item ganho agora, ou "" se esse
## professor já tinha sido vencido antes (ou se ele não dá item).
## Devolve os IDs ganhos agora (lista vazia se o professor já tinha sido vencido).
func desbloquear_por_vitoria(area: int, indice: int) -> Array:
	var chave := "%d_%d" % [area, indice]
	var ganhos: Array = []
	if chave in professores_derrotados:
		return ganhos
	professores_derrotados.append(chave)
	for item in Itens.RECOMPENSAS.get(chave, []):
		if item in itens_desbloqueados:
			continue
		itens_desbloqueados.append(item)
		ganhos.append(item)
		if item in Itens.ESPECIAIS and item_especial_equipado == "":
			item_especial_equipado = item
	salvar()
	return ganhos

## Escolhe qual item ativo começa as próximas runs. Retorna false se não for válido.
func equipar_item_especial(id: String) -> bool:
	if not (id in itens_desbloqueados) or not (id in Itens.ESPECIAIS):
		return false
	item_especial_equipado = id
	salvar()
	return true

# ---------- Loja de upgrades ----------

func custo_vida() -> int:
	return CUSTO_BASE + CUSTO_ACRESCIMO_POR_NIVEL * nivel_vida

func custo_dano() -> int:
	return CUSTO_BASE + CUSTO_ACRESCIMO_POR_NIVEL * nivel_dano

func comprar_vida() -> bool:
	var custo: int = custo_vida()
	if pontos < custo:
		return false
	pontos -= custo
	nivel_vida += 1
	salvar()
	return true

func comprar_dano() -> bool:
	var custo: int = custo_dano()
	if pontos < custo:
		return false
	pontos -= custo
	nivel_dano += 1
	salvar()
	return true

## Aplica upgrades e itens desbloqueados num herói recém-criado (chame logo
## depois do heroi.inicializar(...), no começo de cada run).
func aplicar_ao_heroi(heroi: Herois) -> void:
	heroi.aplicar_bonus_permanente(
		nivel_vida * BONUS_VIDA_POR_NIVEL,
		nivel_dano * BONUS_DANO_POR_NIVEL)
	for id in itens_desbloqueados:
		if id in Itens.PASSIVOS:
			heroi.dar_item_passivo(id)
	if item_especial_equipado != "":
		heroi.dar_item_especial(item_especial_equipado)

# ---------- Persistência ----------

func salvar() -> void:
	var cfg := ConfigFile.new()
	cfg.set_value("progresso", "pontos", pontos)
	cfg.set_value("progresso", "nivel_vida", nivel_vida)
	cfg.set_value("progresso", "nivel_dano", nivel_dano)
	cfg.set_value("progresso", "professores_derrotados", professores_derrotados)
	cfg.set_value("progresso", "itens_desbloqueados", itens_desbloqueados)
	cfg.set_value("progresso", "item_especial_equipado", item_especial_equipado)
	cfg.save(ARQUIVO)

func carregar() -> void:
	var cfg := ConfigFile.new()
	if cfg.load(ARQUIVO) != OK:
		return
	pontos = int(cfg.get_value("progresso", "pontos", 0))
	nivel_vida = int(cfg.get_value("progresso", "nivel_vida", 0))
	nivel_dano = int(cfg.get_value("progresso", "nivel_dano", 0))
	professores_derrotados = Array(cfg.get_value("progresso", "professores_derrotados", []))
	itens_desbloqueados = Array(cfg.get_value("progresso", "itens_desbloqueados", []))
	item_especial_equipado = String(cfg.get_value("progresso", "item_especial_equipado", ""))
