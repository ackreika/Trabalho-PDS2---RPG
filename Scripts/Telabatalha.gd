extends Control
# Anexe este script ao nó raiz da cena de batalha (ex: Batalha.tscn).

# Referência para o nó de imagem do professor na cena (adicione um TextureRect chamado ProfessorSprite)
@onready var professor_sprite_node: TextureRect = $ProfessorSprite
var _ocupado := false       # true enquanto um turno está sendo processado
var _fim_tratado := false   # true quando o fim da batalha atual já foi processado

# Constantes numéricas mapeadas do seu enum class EstadoVisual em C++:
# TOMANDO_DANO = 0, VITORIA = 1, DERROTA = 2, PARADO = 3, ATACANDO = 4
const ESTADO_TOMANDO_DANO = 0
const ESTADO_VITORIA = 1
const ESTADO_DERROTA = 2
const ESTADO_PARADO = 3
const ESTADO_ATACANDO = 4
const CENA_MENU := "res://CenaPrincipal.tscn"

func _ready() -> void:
	if Global.batalha_atual == null:
		push_error("Entrou na tela de batalha sem Global.iniciar_primeira_batalha() ter sido chamado antes.")
		return
	atualizar_tela()
	
	# Define o sprite inicial do professor como PARADO (estado 3)
	atualizar_sprite_professor(ESTADO_PARADO)

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

func atualizar_botoes_item() -> void:
	var heroi: Herois = Global.batalha_atual.get_heroi()
	$BotaoItemEspecial.visible = heroi.possui_item_especial()
	$BotaoItemEspecial.text = "%s (%d)" % [heroi.get_nome_item_especial(), heroi.get_usos_item_especial()]
	$BotaoItemEspecial.disabled = heroi.get_usos_item_especial() <= 0
	$BotaoItemChave.visible = heroi.possui_item_chave()
	$BotaoItemChave.disabled = false

func _on_golpe_0_pressed() -> void: _executar_turno(0)
func _on_golpe_1_pressed() -> void: _executar_turno(1)
func _on_golpe_2_pressed() -> void: _executar_turno(2)

# Conectados pelo editor (aba Sinais) aos botões novos.
func _on_botao_item_especial_pressed() -> void:
	_executar_turno(3)

func _on_botao_item_chave_pressed() -> void:
	if _ocupado:
		return
	var resultado: Dictionary = Global.batalha_atual.usar_item_chave()
	$LabelMensagem.text = resultado["mensagem"]
	atualizar_tela()
	_verificar_fim_de_batalha(resultado["estado_resultante"])

func _executar_turno(indice: int) -> void:
	if _ocupado:
		return
	_ocupado = true
	atualizar_sprite_professor(ESTADO_ATACANDO)

	var resultado: Dictionary = Global.batalha_atual.executar_acao(indice)
	$LabelMensagem.text = resultado["mensagem"]
	atualizar_tela()
	_desabilitar_botoes()   # trava os botões também visualmente durante a pausa

	# Aguarda um pequeno instante para o sprite voltar ao normal
	await get_tree().create_timer(0.6).timeout
	if Global.batalha_atual.get_professor().esta_vivo():
		atualizar_sprite_professor(ESTADO_PARADO)

	var estado: int = resultado["estado_resultante"]
	if estado == 0:        # batalha continua: devolve o controle ao jogador
		atualizar_tela()
		_ocupado = false
	_verificar_fim_de_batalha(estado)

func _on_botao_fugir_pressed() -> void:
	if _ocupado:
		return
	var resultado: Dictionary = Global.batalha_atual.fugir()
	$LabelMensagem.text = resultado["mensagem"]
	_verificar_fim_de_batalha(resultado["estado_resultante"])

## estado_resultante: 0 = ESCOLHENDO_ACAO, 1 = VITORIA, 2 = DERROTA, 3 = FUGIU
func _verificar_fim_de_batalha(estado: int) -> void:
	if estado != 0:
		if _fim_tratado:
			return          # esse fim de batalha já foi processado, ignora repetição
		_fim_tratado = true
	match estado:
		1:
			$LabelMensagem.text += "\nVoce venceu! Preparando proxima batalha..."
			_desabilitar_botoes()

			# Pontos e itens (ANTES de avançar: o avanço troca o professor)
			var prof: Professor = Global.batalha_atual.get_professor()
			var heroi: Herois = Global.batalha_atual.get_heroi()
			var ganhos: int = Progresso.registrar_vitoria(prof)
			$LabelMensagem.text += "\n+%d pontos" % ganhos

			# Itens da primeira vitória (um professor pode dar mais de um)
			var novos: Array = Progresso.desbloquear_por_vitoria(
				Global.id_area_atual, Global.indice_batalha_atual)
			for item_id in novos:
				if item_id in Itens.PASSIVOS:
					heroi.dar_item_passivo(item_id)
					$LabelMensagem.text += "\nNovo item passivo: %s" % Itens.descricao_passivo(item_id)
				elif item_id in Itens.ESPECIAIS:
					if Progresso.item_especial_equipado == item_id and not heroi.possui_item_especial():
						heroi.dar_item_especial(item_id)
					$LabelMensagem.text += "\nNovo item ativo: %s" % Itens.descricao_especial(item_id)

			# Item chave: entregue em TODA run por quem estiver em Itens.DROPA_ITEM_CHAVE
			var chave_prof := "%d_%d" % [Global.id_area_atual, Global.indice_batalha_atual]
			if chave_prof in Itens.DROPA_ITEM_CHAVE:
				heroi.obter_item_chave()
				$LabelMensagem.text += "\nVoce obteve o item chave! Use-o contra o chefe final."
			
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
			$LabelMensagem.text += "\nVoce foi derrotado... Voltando ao menu."
			atualizar_sprite_professor(ESTADO_DERROTA)
			_desabilitar_botoes()
			await get_tree().create_timer(2.5).timeout
			_voltar_ao_menu()
		3:
			$LabelMensagem.text += "\nVoce fugiu. O semestre foi perdido. Voltando ao menu."
			_desabilitar_botoes()
			await get_tree().create_timer(2.5).timeout
			_voltar_ao_menu()

func _reabilitar_botoes() -> void:
	for i in range(3):
		get_node("BotoesGolpe/Golpe%d" % i).disabled = false
	$BotaoFugir.disabled = false
	atualizar_botoes_item()
	_ocupado = false
	_fim_tratado = false

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
	$BotaoItemEspecial.disabled = true
	$BotaoItemChave.disabled = true

func _voltar_ao_menu() -> void:
	Global.encerrar_run()  # libera herói e professor da run que acabou
	get_tree().change_scene_to_file(CENA_MENU)
