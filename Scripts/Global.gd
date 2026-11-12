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

var _semestre: GerenciadorSemestre
var id_area_atual: int:
	get: return _semestre.get_id_area_atual() if _semestre else 0
var indice_batalha_atual: int:
	get: return _semestre.get_indice_batalha_atual() if _semestre else 1
var voltar_para_submenu: bool = false

## Monta o herói (se ainda não existir) e a primeira batalha da área/índice atuais.
## Chame isso quando o jogador escolher uma área no menu.
## id_area: 0=Calculo, 1=GAAL, 2=Fisica, 3=PDS, 4=desafio secreto.
func iniciar_primeira_batalha(id_area: int = 0) -> bool:
	if not ClassDB.class_exists("GerenciadorSemestre") or not ClassDB.class_exists("ProfessoresFabrica") or not ClassDB.class_exists("Herois") or not ClassDB.class_exists("Batalha"):
		push_error("Não foi possível iniciar a batalha: extensão C++ incompleta ou não carregada.")
		return false
	if id_area < 0 or id_area >= ProfessoresFabrica.get_quantidade_areas():
		push_error("Não foi possível iniciar a batalha: área selecionada inválida.")
		return false
	if not area_desbloqueada(id_area):
		push_warning("Esta área ainda está bloqueada.")
		return false
	encerrar_run()  # limpa o herói e o professor da run anterior
	_semestre = GerenciadorSemestre.new()
	_semestre.iniciar(id_area)

	# é sempre criado do zero, então o bloco fica sem indentação extra)
	heroi = Herois.new()
	heroi.inicializar("Estudante", 100, 10, 20) # nome, vida_maxima, defesa, dano_base
	Progresso.aplicar_ao_heroi(heroi)           # aplica os upgrades comprados
	heroi.carregar_movimentos_escolhidos()      # carrega do C++ os 3 movimentos escolhidos no Menu de Movimentos

	return _iniciar_batalha_atual()


## Uma área está liberada se for a primeira (Cálculo) ou se o boss dela (a última
## batalha da área anterior) já foi vencido em alguma run. Isso persiste em disco,
## pois usa a lista Progresso.professores_derrotados, que já é salva.
func area_desbloqueada(id_area: int) -> bool:
	if id_area < 0 or id_area >= ProfessoresFabrica.get_quantidade_areas():
		return false
	if id_area == 0:
		return true
	var anterior: int = id_area - 1
	var boss_indice: int = ProfessoresFabrica.get_quantidade_batalhas(anterior)
	return Progresso.ja_venceu_professor(anterior, boss_indice)

## Libera a run atual (herói, professor e batalha).
func encerrar_run() -> void:
	batalha_atual = null
	if is_instance_valid(professor_atual):
		professor_atual.free()
	if is_instance_valid(heroi):
		heroi.free()
	professor_atual = null
	heroi = null
	_semestre = null

## Avança pra próxima batalha (chamar depois de uma VITORIA). Troca de área
## automaticamente quando a área atual acaba. Retorna false se o jogo já acabou.
func avancar_para_proxima_batalha() -> bool:
	print("--- DEBUG GLOBAL: AVANÇANDO BATALHA ---")
	
	if not is_instance_valid(_semestre):
		push_error("ERRO: _semestre é inválido ou não foi criado.")
		return false
		
	if not is_instance_valid(heroi):
		push_error("ERRO: heroi é inválido.")
		return false
		
	var avancou = _semestre.avancar_batalha()
	if not avancou:
		print(">> FALHOU: _semestre.avancar_batalha() retornou FALSE. (O semestre achou que não tem mais luta ou não conseguiu trocar de área)")
		return false
		
	print(">> Semestre avançou com sucesso. Tentando instanciar a batalha...")
	var iniciou = _iniciar_batalha_atual()
	
	if not iniciou:
		print(">> FALHOU: _iniciar_batalha_atual() retornou FALSE. (Provavelmente o id_area_atual ou indice_batalha_atual desincronizaram do C++)")
		return false
		
	print(">> SUCESSO: Próxima batalha carregada e pronta!")
	return true
func semestre_concluido() -> bool:
	return is_instance_valid(_semestre) and _semestre.venceu_o_jogo()


func _iniciar_batalha_atual() -> bool:
	if not is_instance_valid(_semestre) or not is_instance_valid(heroi):
		push_error("Não foi possível criar a batalha: semestre ou herói inválido.")
		return false
	professor_atual = ProfessoresFabrica.criar_professor(id_area_atual, indice_batalha_atual)
	if not is_instance_valid(professor_atual):
		push_error("A fábrica não conseguiu criar um professor para esta batalha.")
		return false
	batalha_atual = Batalha.new()
	if not is_instance_valid(batalha_atual):
		professor_atual.free()
		professor_atual = null
		push_error("Não foi possível alocar o estado da batalha.")
		return false
	batalha_atual.inicializar(heroi, professor_atual)
	if batalha_atual.get_estado() != 0:
		batalha_atual = null
		professor_atual.free()
		professor_atual = null
		push_error("A batalha rejeitou os personagens por estado inválido.")
		return false
	return true
