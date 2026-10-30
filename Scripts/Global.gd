extends Node
# Autoload (singleton) — registre em Projeto > Configurações do Projeto > Autoload
# com o nome "Global" apontando pra este arquivo.
#
# Guarda o herói, o professor atual e a batalha em andamento, pra não se perder
# quando o Godot troca de cena (menu -> tela de batalha). Variáveis locais de
# um script morrem junto com a cena; um autoload continua vivo o jogo inteiro.
# Os professores (status, golpes e sprites) são criados em C++ pela ProfessoresFabrica.

var heroi: Herois
var professor_atual: Professor
var batalha_atual: Batalha

var id_area_atual: int = 0      # 0=Calculo, 1=GAAL, 2=Fisica, 3=PDS
var indice_batalha_atual: int = 1
var voltar_para_submenu: bool = false

## Monta o herói (se ainda não existir) e a primeira batalha da área/índice atuais.
## Chame isso quando o jogador escolher uma área no menu.
## id_area: 0=Calculo, 1=GAAL, 2=Fisica, 3=PDS (padrão 0, como antes).
func iniciar_primeira_batalha(id_area: int = 0) -> void:
	encerrar_run()  # limpa o herói e o professor da run anterior

	# é sempre criado do zero, então o bloco fica sem indentação extra)
	heroi = Herois.new()
	heroi.inicializar("Estudante", 100, 10, 20) # nome, vida_maxima, defesa, dano_base
	Progresso.aplicar_ao_heroi(heroi)           # aplica os upgrades comprados
	heroi.carregar_movimentos_escolhidos()      # carrega do C++ os 3 movimentos escolhidos no Menu de Movimentos

	id_area_atual = clampi(id_area, 0, ProfessoresFabrica.get_quantidade_areas() - 1)
	indice_batalha_atual = 1
	_iniciar_batalha_atual()


## Uma área está liberada se for a primeira (Cálculo) ou se o boss dela (a última
## batalha da área anterior) já foi vencido em alguma run. Isso persiste em disco,
## pois usa a lista Progresso.professores_derrotados, que já é salva.
func area_desbloqueada(id_area: int) -> bool:
	if id_area <= 0:
		return true
	var anterior: int = id_area - 1
	var chave := "%d_%d" % [anterior, ProfessoresFabrica.get_quantidade_batalhas(anterior)]
	return chave in Progresso.professores_derrotados


## Libera a run atual (herói, professor e batalha).
func encerrar_run() -> void:
	batalha_atual = null
	if is_instance_valid(professor_atual):
		professor_atual.free()
	if is_instance_valid(heroi):
		heroi.free()
	professor_atual = null
	heroi = null

## Avança pra próxima batalha (chamar depois de uma VITORIA). Troca de área
## automaticamente quando a área atual acaba. Retorna false se o jogo já acabou.
func avancar_para_proxima_batalha() -> bool:
	indice_batalha_atual += 1
	if indice_batalha_atual > ProfessoresFabrica.get_quantidade_batalhas(id_area_atual):
		id_area_atual += 1
		indice_batalha_atual = 1
		if id_area_atual >= ProfessoresFabrica.get_quantidade_areas():
			return false # acabaram todas as áreas — fim de jogo
	_iniciar_batalha_atual()
	return true


func _iniciar_batalha_atual() -> void:
	professor_atual = ProfessoresFabrica.criar_professor(id_area_atual, indice_batalha_atual)
	batalha_atual = Batalha.new()
	batalha_atual.inicializar(heroi, professor_atual)
