extends Node
# Autoload (singleton) — registre em Projeto > Configurações do Projeto > Autoload
# com o nome "Global" apontando pra este arquivo.
#
# Guarda o herói, o professor atual e a batalha em andamento, pra não se perder
# quando o Godot troca de cena (menu -> tela de batalha). Variáveis locais de
# um script morrem junto com a cena; um autoload continua vivo o jogo inteiro.

var heroi: Herois
var professor_atual: Professor
var batalha_atual: Batalha

# Progressão atual na run (poderia vir do GerenciadorSemestre quando ele for
# adaptado pro Godot também; por enquanto controlamos aqui mesmo).
var id_area_atual: int = 0      # 0=Calculo, 1=GAAL, 2=Fisica, 3=PDS
var indice_batalha_atual: int = 1

## Quantas batalhas cada área tem (os times não são todos do mesmo tamanho).
const BATALHAS_POR_AREA := {0: 3, 1: 2, 2: 4, 3: 2}

## Monta o herói (se ainda não existir) e a primeira batalha da área/índice atuais.
## Chame isso quando o jogador clicar em "Jogar".
func iniciar_primeira_batalha() -> void:
	if heroi == null:
		heroi = Herois.new()
		heroi.inicializar("Estudante", 100, 10, 20) # nome, vida_maxima, defesa, dano_base
		heroi.aprender_movimento_simples("Derivada Rapida", 15, -1)      # -1 = usos ilimitados
		heroi.aprender_movimento_simples("Integral por Partes", 25, 3)   # só 3 usos
		heroi.aprender_movimento_simples("Limite Fundamental", 10, -1)

	id_area_atual = 0
	indice_batalha_atual = 1
	_iniciar_batalha_atual()

## Avança pra próxima batalha (chamar depois de uma VITORIA). Troca de área
## automaticamente quando a área atual acaba. Retorna false se o jogo já acabou.
func avancar_para_proxima_batalha() -> bool:
	indice_batalha_atual += 1
	if indice_batalha_atual > BATALHAS_POR_AREA[id_area_atual]:
		id_area_atual += 1
		indice_batalha_atual = 1
		if id_area_atual >= BATALHAS_POR_AREA.size():
			return false # acabaram todas as áreas — fim de jogo
	_iniciar_batalha_atual()
	return true

func _iniciar_batalha_atual() -> void:
	professor_atual = criar_professor(id_area_atual, indice_batalha_atual)
	batalha_atual = Batalha.new()
	batalha_atual.inicializar(heroi, professor_atual)

## Cria o Professor correspondente a uma área + índice de batalha (começa em 1).
## tipo do inicializar(): 0 = COMUM, 1 = MINIBOSS, 2 = BOSS.
func criar_professor(id_area: int, indice_batalha: int) -> Professor:
	var p := Professor.new()

	match id_area:
		0: # ---------------- CALCULO ----------------
			match indice_batalha:
				1: # Substituto (Calculo I) - comum
					p.inicializar("Substituto", "Calculo I", 45, 4, 0, false)
					p.adicionar_movimento_simples("Prova Marcada de Ultima Hora", 10, -1)
					p.adicionar_movimento_simples("Limite Mal Explicado", 8, 3)
				2: # Xavier (Calculo I) - comum
					p.inicializar("Xavier", "Calculo I", 55, 5, 0, false)
					p.adicionar_movimento_simples("Regra da Cadeia Relampago", 12, -1)
					p.adicionar_movimento_simples("Derivada Implicita Confusa", 10, 3)
				3: # Colina Sombria (Calculo II) - boss da area
					p.inicializar("Colina Sombria", "Calculo II", 110, 9, 2, false)
					p.adicionar_movimento_simples("Serie de Taylor Infinita", 20, 3)
					p.adicionar_movimento_simples("Integral Impropria Divergente", 25, 2)
					p.adicionar_movimento_simples("Convergencia Sombria", 15, -1)

		1: # ---------------- GAAL ----------------
			match indice_batalha:
				1: # Viktor Bekert (GAAL) - comum
					p.inicializar("Viktor Bekert", "GAAL", 50, 5, 0, false)
					p.adicionar_movimento_simples("Produto Vetorial Cortante", 13, -1)
					p.adicionar_movimento_simples("Matriz Transposta Traicoeira", 11, 3)
				2: # Alexandre (Analise Numerica) - boss da area
					p.inicializar("Alexandre", "Analise Numerica", 120, 10, 2, false)
					p.adicionar_movimento_simples("Metodo de Newton-Raphson", 22, 3)
					p.adicionar_movimento_simples("Instabilidade Numerica", 18, -1)
					p.adicionar_movimento_simples("Erro de Truncamento Fatal", 26, 2)

		2: # ---------------- FISICA ----------------
			match indice_batalha:
				1: # Flavio (Int. a Fisica Mecanica) - comum
					p.inicializar("Flavio", "Introducao a Fisica Mecanica", 50, 5, 0, false)
					p.adicionar_movimento_simples("Segunda Lei de Newton", 12, -1)
					p.adicionar_movimento_simples("Forca de Atrito Inesperada", 10, 3)
				2: # "Outro prof*" (Int. a Fisica Mecanica) - batalha falsa (US05), sem dano real
					p.inicializar("Outro Prof*", "Introducao a Fisica Mecanica", 30, 3, 0, true)
					p.adicionar_movimento_simples("Giz Perdido", 5, -1)
					p.adicionar_movimento_simples("Quadro Apagado Sem Querer", 5, 2)
				3: # Ivonete (Fundamentos de Mecanica) - miniboss
					p.inicializar("Ivonete", "Fundamentos de Mecanica", 80, 7, 1, false)
					p.adicionar_movimento_simples("Torque Desequilibrado", 16, 3)
					p.adicionar_movimento_simples("Momento Angular Conservado", 14, -1)
				4: # Slide Man (Mecanica dos Fluidos) - boss da area
					p.inicializar("Slide Man", "Mecanica dos Fluidos", 125, 10, 2, false)
					p.adicionar_movimento_simples("Equacao de Bernoulli", 20, 3)
					p.adicionar_movimento_simples("Avalanche de Slides", 24, 2)
					p.adicionar_movimento_simples("Escoamento Turbulento", 16, -1)

		3: # ---------------- PDS ----------------
			match indice_batalha:
				1: # Rezeck (PDS II) - comum
					p.inicializar("Rezeck", "PDS II", 55, 6, 0, false)
					p.adicionar_movimento_simples("Code Review Implacavel", 14, -1)
					p.adicionar_movimento_simples("Refatoracao Forcada", 12, 3)
				2: # Fernando (PDS II) - boss final do jogo
					p.inicializar("Fernando", "PDS II", 150, 12, 2, false)
					p.adicionar_movimento_simples("NullPointerException", 24, 3)
					p.adicionar_movimento_simples("Merge Conflict Caotico", 28, 2)
					p.adicionar_movimento_simples("Divida Tecnica Acumulada", 18, -1)

	return p
