extends Control
# Anexe este script ao nó raiz da cena de batalha (ex: Batalha.tscn).
# Ajuste os caminhos $NomeDoNo abaixo pra bater com os nomes reais dos nós
# que você criar na cena (ver instruções na conversa).

func _ready() -> void:
	if Global.batalha_atual == null:
		push_error("Entrou na tela de batalha sem Global.iniciar_primeira_batalha() ter sido chamado antes.")
		return
	atualizar_tela()

func atualizar_tela() -> void:
	var batalha: Batalha = Global.batalha_atual
	var professor: Professor = batalha.get_professor()
	var heroi: Herois = batalha.get_heroi()

	$LabelProfessorNome.text = professor.get_nome()
	$LabelProfessorVida.text = "HP: %d / %d" % [professor.get_vida(), professor.get_vida_maxima()]
	$LabelHeroiVida.text = "HP: %d / %d" % [heroi.get_vida(), heroi.get_vida_maxima()]

	for i in range(3):
		var botao: Button = get_node("BotoesGolpe/Golpe%d" % i)
		if i < heroi.get_quantidade_movimentos():
			botao.text = heroi.get_nome_movimento(i)
			botao.disabled = false
		else:
			botao.text = "-"
			botao.disabled = true

func _on_golpe_0_pressed() -> void: _executar_turno(0)
func _on_golpe_1_pressed() -> void: _executar_turno(1)
func _on_golpe_2_pressed() -> void: _executar_turno(2)

func _executar_turno(indice: int) -> void:
	var resultado: Dictionary = Global.batalha_atual.executar_acao(indice)
	$LabelMensagem.text = resultado["mensagem"]
	atualizar_tela()
	_verificar_fim_de_batalha(resultado["estado_resultante"])

func _on_botao_fugir_pressed() -> void:
	var resultado: Dictionary = Global.batalha_atual.fugir()
	$LabelMensagem.text = resultado["mensagem"]
	_verificar_fim_de_batalha(resultado["estado_resultante"])

## estado_resultante: 0 = ESCOLHENDO_ACAO, 1 = VITORIA, 2 = DERROTA, 3 = FUGIU
func _verificar_fim_de_batalha(estado: int) -> void:
	match estado:
		1:
			$LabelMensagem.text += "\nVoce venceu a primeira batalha!"
			_desabilitar_botoes()
		2:
			$LabelMensagem.text += "\nVoce foi derrotado..."
			_desabilitar_botoes()
		3:
			$LabelMensagem.text += "\nVoce fugiu. O semestre foi perdido."
			_desabilitar_botoes()

func _desabilitar_botoes() -> void:
	for i in range(3):
		get_node("BotoesGolpe/Golpe%d" % i).disabled = true
	$BotaoFugir.disabled = true
