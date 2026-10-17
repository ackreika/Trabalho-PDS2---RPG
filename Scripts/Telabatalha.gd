extends Control
# Anexe este script ao nó raiz da cena de batalha (ex: Batalha.tscn).

# Referência para o nó de imagem do professor na cena (adicione um TextureRect chamado ProfessorSprite)
@onready var professor_sprite_node: TextureRect = $ProfessorSprite

# Constantes numéricas mapeadas do seu enum class EstadoVisual em C++:
# TOMANDO_DANO = 0, VITORIA = 1, DERROTA = 2, PARADO = 3, ATACANDO = 4
const ESTADO_TOMANDO_DANO = 0
const ESTADO_VITORIA = 1
const ESTADO_DERROTA = 2
const ESTADO_PARADO = 3
const ESTADO_ATACANDO = 4

func _ready() -> void:
	if Global.batalha_atual == null:
		push_error("Entrou na tela de batalha sem Global.iniciar_primeira_batalha() ter sido chamado antes.")
		return
	atualizar_tela()
	
	# Define o sprite inicial do professor como PARADO (estado 3)
	atualizar_sprite_professor(ESTADO_PARADO)
	$BotaoItemEspecial.pressed.connect(_on_botao_item_especial_pressed)
	$BotaoItemChave.pressed.connect(_on_botao_item_chave_pressed)

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

	atualizar_botoes_item()

func _on_golpe_0_pressed() -> void: _executar_turno(0)
func _on_golpe_1_pressed() -> void: _executar_turno(1)
func _on_golpe_2_pressed() -> void: _executar_turno(2)
func _on_item_especial_pressed() -> void: _executar_turno(3)

func _on_item_chave_pressed() -> void:
	var resultado: Dictionary = Global.batalha_atual.usar_item_chave()
	$LabelMensagem.text = resultado["mensagem"]
	atualizar_tela()
	_verificar_fim_de_batalha(resultado["estado_resultante"])

func atualizar_botoes_item() -> void:
	var heroi: Herois = Global.batalha_atual.get_heroi()
	$BotaoItemEspecial.visible = heroi.possui_item_especial()
	$BotaoItemEspecial.text = "%s (%d)" % [heroi.get_nome_item_especial(), heroi.get_usos_item_especial()]
	$BotaoItemEspecial.disabled = heroi.get_usos_item_especial() <= 0
	$BotaoItemChave.visible = heroi.possui_item_chave()
	$BotaoItemChave.disabled = false

func _executar_turno(indice: int) -> void:
	# Quando o jogador ataca, podemos mudar temporariamente o sprite do professor para ATACANDO (estado 4)
	atualizar_sprite_professor(ESTADO_ATACANDO)
	
	var resultado: Dictionary = Global.batalha_atual.executar_acao(indice)
	$LabelMensagem.text = resultado["mensagem"]
	atualizar_tela()
	
	# Aguarda um pequeno instante (ex: 0.6 segundos) para o sprite voltar ao normal
	await get_tree().create_timer(0.6).timeout
	if Global.batalha_atual.get_professor().esta_vivo():
		atualizar_sprite_professor(ESTADO_PARADO)

	_verificar_fim_de_batalha(resultado["estado_resultante"])

func _on_botao_fugir_pressed() -> void:
	var resultado: Dictionary = Global.batalha_atual.fugir()
	$LabelMensagem.text = resultado["mensagem"]
	_verificar_fim_de_batalha(resultado["estado_resultante"])

## estado_resultante: 0 = ESCOLHENDO_ACAO, 1 = VITORIA, 2 = DERROTA, 3 = FUGIU
func _verificar_fim_de_batalha(estado: int) -> void:
	match estado:
		1:
			$LabelMensagem.text += "\nVoce venceu! Preparando proxima batalha..."
			_desabilitar_botoes()
			
			# Aguarda 2 segundos para o jogador ler a mensagem de vitória
			await get_tree().create_timer(2.0).timeout
			
			# Tenta avançar para a próxima batalha usando o Global
			var tem_proxima = Global.avancar_para_proxima_batalha()
			
			if tem_proxima:
				# Se ainda houver batalhas (ex: case 2 de Cálculo), recarrega a tela com o novo professor
				atualizar_tela()
				# Reseta o sprite do novo professor para o estado PARADO (3)
				atualizar_sprite_professor(ESTADO_PARADO)
				# Reabilita os botões para a nova luta
				_reabilitar_botoes()
			else:
				$LabelMensagem.text = "Parabéns! Você concluiu todas as matérias do semestre!"
		2:
			$LabelMensagem.text += "\nVoce foi derrotado..."
			atualizar_sprite_professor(ESTADO_DERROTA)
			_desabilitar_botoes()
		3:
			$LabelMensagem.text += "\nVoce fugiu. O semestre foi perdido."
			_desabilitar_botoes()

func _reabilitar_botoes() -> void:
	for i in range(3):
		get_node("BotoesGolpe/Golpe%d" % i).disabled = false
	$BotaoFugir.disabled = false

func atualizar_sprite_professor(estado: int) -> void:
	if Global.batalha_atual == null:
		return
	var professor: Professor = Global.batalha_atual.get_professor()
	if professor:
		var caminho: String = professor.get_caminho_sprite(estado)
		if caminho != "":
			var textura = load(caminho)
			if textura and professor_sprite_node:
				professor_sprite_node.texture = textura

func _desabilitar_botoes() -> void:
	for i in range(3):
		get_node("BotoesGolpe/Golpe%d" % i).disabled = true
	$BotaoFugir.disabled = true


func _on_botao_item_especial_pressed() -> void:
	pass # Replace with function body.


func _on_botao_item_chave_pressed() -> void:
	pass # Replace with function body.
