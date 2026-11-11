extends Control
# Anexe este script ao nó raiz da cena de batalha (ex: Batalha.tscn).

# Referência para o nó de imagem do professor na cena
@onready var professor_sprite_node: TextureRect = $ProfessorSprite
@onready var fundo_batalha_node: TextureRect = $Fundo

const FUNDO_BATALHA_PADRAO: Texture2D = preload("res://assets/imagens/Israel.png")
const FUNDO_SALA_ICEX: Texture2D = preload("res://assets/imagens/Sala_icex.jpeg")
const SHADER_FUNDO_MADEIRA: Shader = preload("res://assets/shaders/FundoMadeira.gdshader")

var _ocupado := false       # true enquanto um turno está sendo processado
var _fim_tratado := false   # true quando o fim da batalha atual já foi processado
var _area_estilizada := -1

# Constantes numéricas mapeadas do enum class EstadoVisual em C++:
const ESTADO_TOMANDO_DANO = 0
const ESTADO_VITORIA = 1
const ESTADO_DERROTA = 2
const ESTADO_PARADO = 3
const ESTADO_ATACANDO = 4

const CENA_MENU := "res://cenas/CenaPrincipal.tscn"

func _ready() -> void:
	_chamar_musica("tocar_intro")
	if not is_instance_valid(Global.batalha_atual):
		push_error("Entrou na tela de batalha sem Global.iniciar_primeira_batalha() ter sido chamado antes.")
		return
	Global.batalha_atual.posicionar_barras_vida(self)
	atualizar_tela()
	_atualizar_fundo_batalha()
	atualizar_sprite_professor(ESTADO_PARADO)
	_estilizar_hud_pokemon()
	$CaixaMensagem/LabelMensagem.text = "Escolha uma técnica para começar."

func atualizar_tela() -> void:
	if not is_instance_valid(Global.batalha_atual):
		push_error("Atualização da tela ignorada: batalha inválida.")
		return
	var batalha: Batalha = Global.batalha_atual
	var professor: Professor = batalha.get_professor()
	var heroi: Herois = batalha.get_heroi()
	if not is_instance_valid(professor) or not is_instance_valid(heroi):
		push_error("Atualização da tela ignorada: personagens da batalha inválidos.")
		return

	$HUDProfessor/Conteudo/LabelProfessorNome.text = professor.get_nome().to_upper()
	$HUDProfessor/Conteudo/LabelProfessorVida.text = "HP  %d / %d" % [professor.get_vida(), professor.get_vida_maxima()]
	$HUDProfessor/Conteudo/BarraVida.max_value = professor.get_vida_maxima()
	$HUDProfessor/Conteudo/BarraVida.value = professor.get_vida()
	$HUDHeroi/Conteudo/LabelHeroiNome.text = heroi.get_nome().to_upper()
	$HUDHeroi/Conteudo/LabelHeroiVida.text = "HP  %d / %d" % [heroi.get_vida(), heroi.get_vida_maxima()]
	$HUDHeroi/Conteudo/BarraVida.max_value = heroi.get_vida_maxima()
	$HUDHeroi/Conteudo/BarraVida.value = heroi.get_vida()

	batalha.atualizar_botoes_acao(self)
	atualizar_botoes_item()
	_atualizar_rotulos_ataque()
	_estilizar_botoes_combate()
	_atualizar_cores_barras_vida()

func atualizar_botoes_item() -> void:
	if not is_instance_valid(Global.batalha_atual):
		return
	var heroi: Herois = Global.batalha_atual.get_heroi()
	if not is_instance_valid(heroi):
		return
	$BarraAcoes/BotoesGolpe/BotaoItemEspecial.visible = heroi.possui_item_especial()
	$BarraAcoes/BotoesGolpe/BotaoItemEspecial.text = "%s (%d)" % [heroi.get_nome_item_especial(), heroi.get_usos_item_especial()]
	$BarraAcoes/BotoesGolpe/BotaoItemEspecial.disabled = heroi.get_usos_item_especial() <= 0
	$BarraAcoes/BotoesGolpe/BotaoItemChave.visible = heroi.possui_item_chave()
	$BarraAcoes/BotoesGolpe/BotaoItemChave.disabled = false

func _on_golpe_0_pressed() -> void: _executar_turno(0)
func _on_golpe_1_pressed() -> void: _executar_turno(1)
func _on_golpe_2_pressed() -> void: _executar_turno(2)

func _on_botao_item_especial_pressed() -> void:
	_executar_turno(3)

func _on_botao_item_chave_pressed() -> void:
	if _ocupado:
		return
	if not is_instance_valid(Global.batalha_atual):
		push_error("Não foi possível usar o item: batalha inválida.")
		return
	var resultado: Dictionary = Global.batalha_atual.usar_item_chave()
	$CaixaMensagem/LabelMensagem.text = resultado["mensagem"]
	if resultado["acao_valida"]:
		$CaixaMensagem/LabelMensagem.text += "\nO Fernando voltou a ficar vulneravel!"
	atualizar_tela()
	_verificar_fim_de_batalha(resultado["estado_resultante"])

func _executar_turno(indice: int) -> void:
	if _ocupado:
		return
	if not is_instance_valid(Global.batalha_atual):
		push_error("Ação cancelada: batalha inválida ou encerrada.")
		return
	_ocupado = true
	_chamar_musica("iniciar_musica_batalha")
	var professor: Professor = Global.batalha_atual.get_professor()
	if not is_instance_valid(professor):
		_ocupado = false
		push_error("Ação cancelada: professor da batalha inválido.")
		return
	var invulneravel_antes: bool = professor.is_invulneravel()
	var vida_professor_antes: int = professor.get_vida()
	var resultado: Dictionary = Global.batalha_atual.executar_acao(indice)
	var linhas_resultado: PackedStringArray = str(resultado["mensagem"]).split("\n")
	$CaixaMensagem/LabelMensagem.text = linhas_resultado[0]
	var aviso_invulnerabilidade := ""
	if not invulneravel_antes and professor.is_invulneravel():
		aviso_invulnerabilidade = "Fernando ativou a invulnerabilidade na metade da vida. Use o item chave recebido de Rezeck."
	atualizar_tela()
	_desabilitar_botoes()

	var estado: int = resultado["estado_resultante"]
	if resultado["acao_valida"]:
		if resultado["professor_atacou"]:
			$CaixaMensagem/LabelMensagem.text += "\n%s prepara uma resposta..." % professor.get_nome()
		await _animar_turno_professor(resultado, professor, vida_professor_antes)
		$CaixaMensagem/LabelMensagem.text = linhas_resultado[0]
		if not aviso_invulnerabilidade.is_empty():
			$CaixaMensagem/LabelMensagem.text += "\n" + aviso_invulnerabilidade
		if resultado["professor_atacou"]:
			if linhas_resultado.size() > 1:
				$CaixaMensagem/LabelMensagem.text += "\n" + linhas_resultado[1]
			$CaixaMensagem/LabelMensagem.text += "\n" + _fala_apos_ataque(professor.get_nome(), int(resultado["dano_causado_pelo_professor"]), vida_professor_antes > professor.get_vida(), professor.is_invulneravel())
		elif professor.esta_vivo():
			$CaixaMensagem/LabelMensagem.text += "\n" + _fala_apos_ataque(professor.get_nome(), 0, vida_professor_antes > professor.get_vida(), professor.is_invulneravel())
	else:
		$CaixaMensagem/LabelMensagem.text = resultado["mensagem"]

	if estado == 0:
		atualizar_tela()
		_ocupado = false
	_verificar_fim_de_batalha(estado)

func _on_botao_fugir_pressed() -> void:
	if _ocupado:
		return
	if not is_instance_valid(Global.batalha_atual):
		push_error("Não foi possível desistir: batalha inválida.")
		return
	var resultado: Dictionary = Global.batalha_atual.fugir()
	$CaixaMensagem/LabelMensagem.text = resultado["mensagem"]
	_verificar_fim_de_batalha(resultado["estado_resultante"])

## estado_resultante: 0 = ESCOLHENDO_ACAO, 1 = VITORIA, 2 = DERROTA, 3 = FUGIU
func _verificar_fim_de_batalha(estado: int) -> void:
	if estado != 0:
		if _fim_tratado:
			return
		_fim_tratado = true

	match estado:
		1:
			var fala_vitoria: String = _fala_de_vitoria(Global.batalha_atual.get_professor().get_nome())
			$CaixaMensagem/LabelMensagem.text += "\n" + fala_vitoria + "\nVoce venceu! Preparando a proxima batalha..."
			_desabilitar_botoes()

			var prof: Professor = Global.batalha_atual.get_professor()
			var heroi: Herois = Global.batalha_atual.get_heroi()
			var ganhos: int = Progresso.registrar_vitoria(prof)
			$CaixaMensagem/LabelMensagem.text += "\n+%d pontos" % ganhos

			# Drops de primeira vitória via C++ Recompensas
			var novos: Array = Progresso.desbloquear_por_vitoria(
				Global.id_area_atual, Global.indice_batalha_atual)

			for item_id in novos:
				var id_str: String = str(item_id)

				if id_str in Itens.PASSIVOS:
					heroi.dar_item_passivo(id_str)
					$CaixaMensagem/LabelMensagem.text += "\nNovo item passivo: %s" % Itens.descricao_passivo(id_str)
				elif id_str in Itens.ESPECIAIS:
					if Progresso.item_especial_equipado == id_str and not heroi.possui_item_especial():
						heroi.dar_item_especial(id_str)
					$CaixaMensagem/LabelMensagem.text += "\nNovo item ativo: %s" % Itens.descricao_especial(id_str)

			# Item chave entregue pela configuração nativa
			if Progresso.dropa_item_chave(Global.id_area_atual, Global.indice_batalha_atual):
				heroi.obter_item_chave()
				$CaixaMensagem/LabelMensagem.text += "\nVoce obteve o item chave! Use-o contra o chefe final."
			await _oferecer_quiz(heroi)
			
			await get_tree().create_timer(2.0).timeout
			
			var tem_proxima = Global.avancar_para_proxima_batalha()
			
			if tem_proxima:
				_reabilitar_botoes()
				atualizar_tela()
				_atualizar_fundo_batalha()
				atualizar_sprite_professor(ESTADO_PARADO)
				$CaixaMensagem/LabelMensagem.text = "Um novo professor entrou na arena. Escolha sua proxima tecnica!"
			elif Global.semestre_concluido():
				$CaixaMensagem/LabelMensagem.text = "%s\nParabens! Voce concluiu esta fase! Sua jornada foi brilhante!" % fala_vitoria
				await get_tree().create_timer(3.0).timeout
				_voltar_ao_menu()
			else:
				push_error("A próxima batalha não pôde ser iniciada; o semestre não foi marcado como concluído.")
				$CaixaMensagem/LabelMensagem.text = "Ocorreu um erro ao iniciar a próxima batalha. Voltando ao menu."
				await get_tree().create_timer(2.5).timeout
				_voltar_ao_menu()
		2:
			$CaixaMensagem/LabelMensagem.text += "\nVoce foi derrotado... Voltando ao menu."
			var professor_vitorioso: Professor = Global.batalha_atual.get_professor()
			if professor_vitorioso and professor_vitorioso.get_nome() == "Colina Sombria":
				atualizar_sprite_professor(ESTADO_TOMANDO_DANO)
			elif professor_vitorioso and professor_vitorioso.get_nome() == "Ivonete":
				atualizar_sprite_professor(ESTADO_DERROTA)
			elif professor_vitorioso and professor_vitorioso.get_nome() == "Fernando":
				atualizar_sprite_professor(ESTADO_DERROTA)
			else:
				atualizar_sprite_professor(ESTADO_PARADO)
			_desabilitar_botoes()
			await get_tree().create_timer(2.5).timeout
			_voltar_ao_menu()
		3:
			$CaixaMensagem/LabelMensagem.text += "\nVoce fugiu. O semestre foi perdido. Voltando ao menu."
			_desabilitar_botoes()
			await get_tree().create_timer(2.5).timeout
			_voltar_ao_menu()

func _reabilitar_botoes() -> void:
	for i in range(3):
		get_node("BarraAcoes/BotoesGolpe/Golpe%d" % i).disabled = false
	$BarraAcoes/BotoesGolpe/BotaoFugir.disabled = false
	atualizar_botoes_item()
	_ocupado = false
	_fim_tratado = false

func _atualizar_fundo_batalha() -> void:
	if is_instance_valid(Global.batalha_atual):
		Global.batalha_atual.alterar_fundo_batalha(Global.id_area_atual)

func _animar_turno_professor(resultado: Dictionary, professor: Professor, vida_antes: int) -> void:
	var posicao_original: Vector2 = professor_sprite_node.position
	var escala_original: Vector2 = professor_sprite_node.scale
	if resultado["heroi_atacou"] and professor.get_vida() < vida_antes:
		atualizar_sprite_professor(ESTADO_TOMANDO_DANO)
		professor_sprite_node.modulate = Color(1.0, 0.62, 0.62)
		var recuo := create_tween()
		recuo.tween_property(professor_sprite_node, "position", posicao_original + Vector2(13, 0), 0.07)
		recuo.tween_property(professor_sprite_node, "position", posicao_original + Vector2(-9, 0), 0.07)
		recuo.tween_property(professor_sprite_node, "position", posicao_original, 0.09)
		await recuo.finished
		professor_sprite_node.modulate = Color.WHITE

	if resultado["professor_atacou"] and professor.esta_vivo():
		atualizar_sprite_professor(ESTADO_ATACANDO)
		professor_sprite_node.scale = escala_original * 1.025
		var investida := create_tween()
		investida.tween_property(professor_sprite_node, "position", posicao_original + Vector2(-48, 0), 0.16).set_trans(Tween.TRANS_QUAD).set_ease(Tween.EASE_IN)
		investida.tween_property(professor_sprite_node, "position", posicao_original + Vector2(8, 0), 0.09).set_trans(Tween.TRANS_BACK).set_ease(Tween.EASE_OUT)
		investida.tween_property(professor_sprite_node, "position", posicao_original, 0.12)
		await investida.finished

	professor_sprite_node.position = posicao_original
	professor_sprite_node.scale = escala_original
	professor_sprite_node.modulate = Color.WHITE
	if professor.esta_vivo():
		atualizar_sprite_professor(ESTADO_PARADO)
	else:
		atualizar_sprite_professor(ESTADO_VITORIA)

func _fala_apos_ataque(nome: String, dano_recebido: int, acertou_o_professor: bool, invulneravel: bool) -> String:
	if invulneravel and not acertou_o_professor:
		return "Minha defesa ainda está de pé. Pense em outra estratégia!"
	if not acertou_o_professor:
		return "Pode tentar de novo; ainda há muito conteúdo pela frente."
	if dano_recebido >= 20:
		return "Esse foi um golpe forte. Quero ver se você aguenta o próximo!"

	var falas: Dictionary = {
		"Substituto": ["Boa tentativa! Agora vamos à próxima questão.", "Essa resposta quase me convenceu!"],
		"Xavier": ["Você está pegando o ritmo. Não pare agora!", "Boa jogada! A regra da cadeia ainda reserva surpresas."],
		"Colina Sombria": ["Um bom golpe. Mas a série ainda não convergiu!", "Você avançou bastante. Mostre o que aprendeu!"],
		"Viktor Bekert": ["Vetor interessante! Quero ver sua próxima operação.", "Boa combinação. Continue calculando!"],
		"Alexandre": ["Seu método está funcionando. Continue assim!", "Boa aproximação! Está chegando à solução."],
		"Flavio": ["Boa energia! Vamos conservar esse ritmo.", "Você está aplicando bem a teoria."],
		"Outro Prof*": ["Foi por pouco! Tente mais uma vez.", "Boa tentativa! Até o giz ficou impressionado."],
		"Ivonete": ["Muito bem! Você encontrou o equilíbrio.", "Boa resposta. Seu momento está crescendo!"],
		"Slide Man": ["Boa apresentação! Ainda tenho mais alguns slides.", "Você está fluindo bem. Continue!"],
		"Rezeck": ["Boa implementação! O código está ficando sólido.", "Resposta aceita. Vamos para o próximo caso de teste!"],
		"Fernando": ["Boa solução. Ainda não chegamos ao fim do projeto.", "Você está quase lá. Não deixe a dívida técnica vencer!"]
	}
	var opcoes: Array = falas.get(nome, ["Boa jogada! Continue tentando."])
	return str(opcoes[randi_range(0, opcoes.size() - 1)])

func _fala_de_vitoria(nome: String) -> String:
	var falas: Dictionary = {
		"Substituto": "Parabéns! Você passou na primeira prova com louvor!",
		"Xavier": "Excelente! A derivada do seu esforço foi uma grande vitória!",
		"Colina Sombria": "Você venceu a série. Seu esforço finalmente convergiu!",
		"Viktor Bekert": "Muito bem! Você encontrou a direção certa para vencer.",
		"Alexandre": "Parabéns! Sua solução foi precisa e estável.",
		"Flavio": "Excelente! Você manteve a energia até o fim.",
		"Outro Prof*": "Muito bem! Você percebeu que essa batalha era só uma encenação.",
		"Ivonete": "Parabéns! Você manteve o equilíbrio e venceu.",
		"Slide Man": "Apresentação impecável! Você concluiu essa etapa.",
		"Rezeck": "Parabéns! Seu código passou em todos os testes.",
		"Fernando": "Você concluiu o projeto! Essa vitória foi muito bem construída."
	}
	return str(falas.get(nome, "Parabéns pela vitória! Você avançou de fase."))

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
		elif professor_sprite_node:
			professor_sprite_node.texture = null

func _desabilitar_botoes() -> void:
	for i in range(3):
		get_node("BarraAcoes/BotoesGolpe/Golpe%d" % i).disabled = true
	$BarraAcoes/BotoesGolpe/BotaoFugir.disabled = true
	$BarraAcoes/BotoesGolpe/BotaoItemEspecial.disabled = true
	$BarraAcoes/BotoesGolpe/BotaoItemChave.disabled = true

func _oferecer_quiz(heroi: Herois) -> void:
	var quiz := GerenciadorQuiz.new()
	var pergunta: Dictionary = quiz.sortear_pergunta(Global.id_area_atual)
	if pergunta.is_empty() or not pergunta.has("enunciado") or not pergunta.has("alternativas") or not pergunta.has("indice_correto"):
		return
	if typeof(pergunta["alternativas"]) != TYPE_ARRAY or typeof(pergunta["enunciado"]) != TYPE_STRING:
		push_error("Pergunta de revisão recebida em formato inválido.")
		return
	var alternativas: Array = pergunta["alternativas"]
	if alternativas.size() != 4 or typeof(pergunta["indice_correto"]) != TYPE_INT or pergunta["indice_correto"] < 0 or pergunta["indice_correto"] >= alternativas.size():
		push_error("A pergunta de revisão precisa ter exatamente quatro alternativas.")
		return

	var janela := Window.new()
	janela.title = "Revisão"
	janela.size = Vector2i(620, 500)
	janela.min_size = Vector2i(620, 500)
	janela.unresizable = true
	janela.borderless = true
	janela.always_on_top = true
	janela.transient = true
	janela.exclusive = true
	var painel := PanelContainer.new()
	var estilo_painel := StyleBoxFlat.new()
	estilo_painel.bg_color = Color("101a2b")
	estilo_painel.border_color = Color("4dd0c8")
	estilo_painel.set_border_width_all(2)
	estilo_painel.set_corner_radius_all(18)
	painel.add_theme_stylebox_override("panel", estilo_painel)
	janela.add_child(painel)
	painel.set_anchors_and_offsets_preset(Control.PRESET_FULL_RECT)
	var margem := MarginContainer.new()
	margem.add_theme_constant_override("margin_left", 28)
	margem.add_theme_constant_override("margin_top", 24)
	margem.add_theme_constant_override("margin_right", 28)
	margem.add_theme_constant_override("margin_bottom", 24)
	painel.add_child(margem)
	var coluna := VBoxContainer.new()
	coluna.add_theme_constant_override("separation", 12)
	margem.add_child(coluna)
	var titulo := Label.new()
	titulo.text = "DESAFIO DE REVISÃO"
	titulo.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	titulo.add_theme_font_size_override("font_size", 24)
	titulo.add_theme_color_override("font_color", Color("70e1d8"))
	coluna.add_child(titulo)
	var enunciado := Label.new()
	enunciado.text = str(pergunta["enunciado"])
	enunciado.autowrap_mode = TextServer.AUTOWRAP_WORD_SMART
	enunciado.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	enunciado.add_theme_font_size_override("font_size", 19)
	coluna.add_child(enunciado)
	var instrucao := Label.new()
	instrucao.text = "Escolha uma das quatro alternativas. Acertar aumenta em 3 o dano do primeiro movimento."
	instrucao.autowrap_mode = TextServer.AUTOWRAP_WORD_SMART
	instrucao.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	instrucao.add_theme_color_override("font_color", Color("b8c5d9"))
	coluna.add_child(instrucao)
	var resposta := [-1]
	var letras := ["A", "B", "C", "D"]
	for i in range(alternativas.size()):
		var botao := Button.new()
		botao.text = "%s.  %s" % [letras[i], str(alternativas[i])]
		botao.custom_minimum_size = Vector2(0, 54)
		botao.alignment = HORIZONTAL_ALIGNMENT_LEFT
		botao.add_theme_font_size_override("font_size", 17)
		botao.add_theme_color_override("font_color", Color("e8f0ff"))
		botao.add_theme_color_override("font_hover_color", Color("ffffff"))
		botao.add_theme_stylebox_override("normal", _criar_estilo_botao(Color("1b2b43"), Color("344b69")))
		botao.add_theme_stylebox_override("hover", _criar_estilo_botao(Color("244b61"), Color("70e1d8")))
		botao.add_theme_stylebox_override("pressed", _criar_estilo_botao(Color("17645f"), Color("70e1d8")))
		botao.pressed.connect(_selecionar_resposta.bind(resposta, janela, i))
		coluna.add_child(botao)
	add_child(janela)
	janela.popup_centered(Vector2i(620, 500))
	while resposta[0] == -1 and janela.visible:
		await get_tree().process_frame
	janela.queue_free()
	if resposta[0] < 0:
		return
	var acertou: bool = quiz.validar_resposta(pergunta, resposta[0])
	quiz.aplicar_resultado(heroi, 0, acertou)
	$CaixaMensagem/LabelMensagem.text += "\n" + ("Resposta certa! Seu primeiro movimento ficou mais forte." if acertou else "Resposta incorreta. Tente novamente na proxima vitoria.")

func _selecionar_resposta(resposta: Array, janela: Window, indice: int) -> void:
	resposta[0] = indice
	janela.hide()

func _criar_estilo_botao(fundo: Color, borda: Color) -> StyleBoxFlat:
	var estilo := StyleBoxFlat.new()
	estilo.bg_color = fundo
	estilo.border_color = borda
	estilo.set_border_width_all(1)
	estilo.set_corner_radius_all(10)
	estilo.content_margin_left = 14
	estilo.content_margin_right = 14
	estilo.content_margin_top = 8
	estilo.content_margin_bottom = 8
	return estilo

func _estilizar_botoes_combate() -> void:
	if _area_estilizada == Global.id_area_atual:
		return
	var moldura: Panel = $BarraAcoes/MolduraMadeira
	var moldura_estilo := StyleBoxFlat.new()
	moldura_estilo.bg_color = Color("29160d")
	moldura_estilo.border_color = Color("d2a05c")
	moldura_estilo.set_border_width_all(3)
	moldura_estilo.set_corner_radius_all(13)
	moldura.add_theme_stylebox_override("panel", moldura_estilo)
	var material_madeira := ShaderMaterial.new()
	material_madeira.shader = SHADER_FUNDO_MADEIRA
	$BarraAcoes/FundoMadeira.material = material_madeira

	var tema: Dictionary = _tema_da_area(Global.id_area_atual)
	for i in range(3):
		var botao: Button = get_node("BarraAcoes/BotoesGolpe/Golpe%d" % i)
		_configurar_botao_combate(botao, tema["normal"], tema["hover"], tema["borda"])
	_configurar_botao_combate($BarraAcoes/BotoesGolpe/BotaoItemEspecial, Color("563e75"), Color("8c63bd"), Color("bca0dc"))
	_configurar_botao_combate($BarraAcoes/BotoesGolpe/BotaoItemChave, Color("6c5528"), Color("c59a3a"), Color("e6c57a"))
	_configurar_botao_combate($BarraAcoes/BotoesGolpe/BotaoFugir, Color("713b45"), Color("bb5965"), Color("dd8a92"))
	_area_estilizada = Global.id_area_atual

func _tema_da_area(area: int) -> Dictionary:
	match area:
		0: return {"simbolo": "∫", "assinatura": "CÁLCULO · TÉCNICA", "normal": Color("365273"), "hover": Color("50769e"), "borda": Color("e6c57a")}
		1: return {"simbolo": "[ ]", "assinatura": "GAAL · MATRIZ / VETOR", "normal": Color("315b60"), "hover": Color("47847f"), "borda": Color("9bd1bd")}
		2: return {"simbolo": "Δ", "assinatura": "FÍSICA · ENERGIA", "normal": Color("3b526b"), "hover": Color("52799c"), "borda": Color("8bc5e8")}
		3: return {"simbolo": ">_", "assinatura": "PDS · FUNÇÃO", "normal": Color("284d43"), "hover": Color("3d7559"), "borda": Color("8ccea2")}
		_: return {"simbolo": "★", "assinatura": "TÉCNICA", "normal": Color("365273"), "hover": Color("50769e"), "borda": Color("e6c57a")}

func _atualizar_rotulos_ataque() -> void:
	if Global.batalha_atual == null:
		return
	var heroi: Herois = Global.batalha_atual.get_heroi()
	var tema: Dictionary = _tema_da_area(Global.id_area_atual)
	for i in range(3):
		var botao: Button = get_node("BarraAcoes/BotoesGolpe/Golpe%d" % i)
		if i < heroi.get_quantidade_movimentos():
			botao.text = "%s  %s\n%s" % [tema["simbolo"], heroi.get_nome_movimento(i), tema["assinatura"]]
			botao.autowrap_mode = TextServer.AUTOWRAP_WORD_SMART

func _estilizar_hud_pokemon() -> void:
	for painel in [$HUDProfessor, $HUDHeroi]:
		var placa := StyleBoxFlat.new()
		placa.bg_color = Color("12233a", 0.9)
		placa.border_color = Color("e2bd69")
		placa.set_border_width_all(2)
		placa.set_corner_radius_all(8)
		painel.add_theme_stylebox_override("panel", placa)
		painel.add_theme_constant_override("margin_left", 16)
		painel.add_theme_constant_override("margin_top", 10)
		painel.add_theme_constant_override("margin_right", 16)
		painel.add_theme_constant_override("margin_bottom", 10)
	for nome_label in [$HUDProfessor/Conteudo/LabelProfessorNome, $HUDHeroi/Conteudo/LabelHeroiNome]:
		nome_label.add_theme_font_size_override("font_size", 19)
		nome_label.add_theme_color_override("font_color", Color("ffe6a6"))
		nome_label.add_theme_color_override("font_outline_color", Color("172338"))
		nome_label.add_theme_constant_override("outline_size", 2)
	for vida_label in [$HUDProfessor/Conteudo/LabelProfessorVida, $HUDHeroi/Conteudo/LabelHeroiVida]:
		vida_label.add_theme_font_size_override("font_size", 15)
		vida_label.add_theme_color_override("font_color", Color("f4f6ff"))
		vida_label.add_theme_color_override("font_outline_color", Color("172338"))
		vida_label.add_theme_constant_override("outline_size", 1)
	var caixa := StyleBoxFlat.new()
	caixa.bg_color = Color("12233a", 0.91)
	caixa.border_color = Color("e2bd69")
	caixa.set_border_width_all(2)
	caixa.set_corner_radius_all(8)
	$CaixaMensagem.add_theme_stylebox_override("panel", caixa)
	$CaixaMensagem.add_theme_constant_override("margin_left", 18)
	$CaixaMensagem.add_theme_constant_override("margin_top", 10)
	$CaixaMensagem.add_theme_constant_override("margin_right", 18)
	$CaixaMensagem.add_theme_constant_override("margin_bottom", 10)
	$CaixaMensagem/LabelMensagem.add_theme_font_size_override("font_size", 16)
	$CaixaMensagem/LabelMensagem.add_theme_color_override("font_color", Color("f4f6ff"))
	$CaixaMensagem/LabelMensagem.add_theme_color_override("font_outline_color", Color("101a2b"))
	$CaixaMensagem/LabelMensagem.add_theme_constant_override("outline_size", 1)
	for barra in [$HUDProfessor/Conteudo/BarraVida, $HUDHeroi/Conteudo/BarraVida]:
		barra.add_theme_stylebox_override("background", _criar_estilo_botao(Color("263446"), Color("111a29")))
		barra.add_theme_stylebox_override("fill", _criar_estilo_botao(Color("63c978"), Color("d2f2a0")))

func _atualizar_cores_barras_vida() -> void:
	for caminho in ["HUDProfessor/Conteudo/BarraVida", "HUDHeroi/Conteudo/BarraVida"]:
		var barra: ProgressBar = get_node(caminho)
		var proporcao: float = barra.value / maxf(barra.max_value, 1.0)
		var cor: Color = Color("63c978") if proporcao > 0.5 else Color("f0c34e") if proporcao > 0.25 else Color("e35b58")
		barra.add_theme_stylebox_override("fill", _criar_estilo_botao(cor, cor.lightened(0.25)))

func _configurar_botao_combate(botao: Button, cor: Color, cor_hover: Color, borda: Color) -> void:
	botao.custom_minimum_size = Vector2(0, 142)
	botao.size_flags_horizontal = Control.SIZE_EXPAND_FILL
	botao.alignment = HORIZONTAL_ALIGNMENT_CENTER
	botao.add_theme_font_size_override("font_size", 16)
	botao.add_theme_color_override("font_color", Color("f3f6ff"))
	botao.add_theme_color_override("font_hover_color", Color("ffffff"))
	botao.add_theme_color_override("font_disabled_color", Color("aeb8c7"))
	botao.add_theme_color_override("font_outline_color", Color("101a24"))
	botao.add_theme_constant_override("outline_size", 1)
	botao.add_theme_stylebox_override("normal", _criar_estilo_botao(cor, borda))
	botao.add_theme_stylebox_override("hover", _criar_estilo_botao(cor_hover, borda.lightened(0.25)))
	botao.add_theme_stylebox_override("pressed", _criar_estilo_botao(cor.darkened(0.25), borda))
	botao.add_theme_stylebox_override("disabled", _criar_estilo_botao(cor.darkened(0.45), borda.darkened(0.4)))

func dropa_item_chave(area: int, indice: int) -> bool:
	var rec := Recompensas.new()
	return rec.dropa_item_chave(area, indice)

func _voltar_ao_menu() -> void:
	_chamar_musica("parar_tudo")
	Global.encerrar_run()
	get_tree().change_scene_to_file(CENA_MENU)

func _chamar_musica(metodo: String) -> void:
	var musica := get_node_or_null("/root/MusicaFundo")
	if musica == null or not musica.has_method(metodo):
		push_warning("Controles de música indisponíveis; continuidade da batalha preservada.")
		return
	musica.call(metodo)
