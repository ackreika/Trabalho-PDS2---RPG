extends Node
# Autoload — registre em Projeto > Configurações do Projeto > Autoload
# com o nome "Progresso" apontando pra este arquivo.
#
# Guarda os pontos ganhos ao vencer professores e os níveis de upgrade
# permanentes (vida e dano). Tudo é salvo em disco, então sobrevive entre
# runs e entre aberturas do jogo.

const ARQUIVO := "user://progresso.cfg"

# ---------- Balanceamento: mexa só aqui ----------
const PONTOS_COMUM := 10
const PONTOS_MINIBOSS := 25
const PONTOS_BOSS := 50
const PONTOS_BATALHA_FALSA := 5

const BONUS_VIDA_POR_NIVEL := 10   # +10 de vida máxima por nível
const BONUS_DANO_POR_NIVEL := 2    # +2 de dano base por nível

const CUSTO_BASE := 20             # custo do 1º nível
const CUSTO_ACRESCIMO_POR_NIVEL := 10   # cada nível novo custa +10

var pontos: int = 0
var nivel_vida: int = 0
var nivel_dano: int = 0

func _ready() -> void:
	carregar()

# ---------- Ganho de pontos ----------

## Quantos pontos o professor dá ao ser derrotado.
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

## Chame UMA vez quando o jogador vencer um professor. Soma, salva e devolve
## quantos pontos foram ganhos (pra mostrar na tela).
func registrar_vitoria(professor: Professor) -> int:
	var ganhos: int = pontos_por_vitoria(professor)
	pontos += ganhos
	salvar()
	return ganhos

# ---------- Loja de upgrades ----------

func custo_vida() -> int:
	return CUSTO_BASE + CUSTO_ACRESCIMO_POR_NIVEL * nivel_vida

func custo_dano() -> int:
	return CUSTO_BASE + CUSTO_ACRESCIMO_POR_NIVEL * nivel_dano

## Retorna true se a compra foi feita (tinha pontos suficientes).
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

## Aplica os upgrades comprados num herói recém-criado (chame logo depois do
## heroi.inicializar(...), no começo de cada run).
func aplicar_ao_heroi(heroi: Herois) -> void:
	heroi.aplicar_bonus_permanente(
		nivel_vida * BONUS_VIDA_POR_NIVEL,
		nivel_dano * BONUS_DANO_POR_NIVEL)

# ---------- Persistência ----------

func salvar() -> void:
	var cfg := ConfigFile.new()
	cfg.set_value("progresso", "pontos", pontos)
	cfg.set_value("progresso", "nivel_vida", nivel_vida)
	cfg.set_value("progresso", "nivel_dano", nivel_dano)
	cfg.save(ARQUIVO)

func carregar() -> void:
	var cfg := ConfigFile.new()
	if cfg.load(ARQUIVO) != OK:
		return # primeira vez jogando: nada salvo ainda
	pontos = int(cfg.get_value("progresso", "pontos", 0))
	nivel_vida = int(cfg.get_value("progresso", "nivel_vida", 0))
	nivel_dano = int(cfg.get_value("progresso", "nivel_dano", 0))
