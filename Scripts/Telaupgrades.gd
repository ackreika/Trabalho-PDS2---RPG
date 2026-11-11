extends Control

const CENA_MENU := "res://cenas/CenaPrincipal.tscn"

@onready var painel: PanelContainer = $Painel
@onready var titulo: Label = $Painel/Conteudo/Coluna/LabelTitulo
@onready var label_pontos: Label = $Painel/Conteudo/Coluna/LabelPontos
@onready var card_vida: PanelContainer = $Painel/Conteudo/Coluna/CardVida
@onready var label_vida: Label = $Painel/Conteudo/Coluna/CardVida/ConteudoVida/LabelVida
@onready var botao_vida: Button = $Painel/Conteudo/Coluna/CardVida/ConteudoVida/BotaoUpgradeVida
@onready var card_dano: PanelContainer = $Painel/Conteudo/Coluna/CardDano
@onready var label_dano: Label = $Painel/Conteudo/Coluna/CardDano/ConteudoDano/LabelDano
@onready var botao_dano: Button = $Painel/Conteudo/Coluna/CardDano/ConteudoDano/BotaoUpgradeDano
@onready var label_mensagem: Label = $Painel/Conteudo/Coluna/LabelMensagem
@onready var botao_voltar: Button = $Painel/Conteudo/Coluna/BotaoVoltar

func _ready() -> void:
	botao_vida.pressed.connect(_ao_comprar_vida)
	botao_dano.pressed.connect(_ao_comprar_dano)
	botao_voltar.pressed.connect(_ao_voltar)
	_estilizar_tela()
	atualizar()

func atualizar() -> void:
	label_pontos.text = "Pontos de progresso: %d" % Progresso.get_pontos()
	label_vida.text = "VIDA\nNível %d  ·  +%d HP permanentes" % [
		Progresso.nivel_vida, Progresso.nivel_vida * Progresso.BONUS_VIDA_POR_NIVEL]
	botao_vida.text = "Aumentar vida (+%d HP)  ·  custo: %d pontos" % [
		Progresso.BONUS_VIDA_POR_NIVEL, Progresso.custo_vida()]
	botao_vida.disabled = Progresso.get_pontos() < Progresso.custo_vida()

	label_dano.text = "DANO\nNível %d  ·  +%d de dano permanentes" % [
		Progresso.nivel_dano, Progresso.nivel_dano * Progresso.BONUS_DANO_POR_NIVEL]
	botao_dano.text = "Aumentar dano (+%d)  ·  custo: %d pontos" % [
		Progresso.BONUS_DANO_POR_NIVEL, Progresso.custo_dano()]
	botao_dano.disabled = Progresso.get_pontos() < Progresso.custo_dano()

func _ao_comprar_vida() -> void:
	if Progresso.comprar_vida():
		label_mensagem.text = "Upgrade de vida comprado. Ele vale na próxima partida."
	else:
		label_mensagem.text = "Pontos insuficientes para comprar vida."
	atualizar()

func _ao_comprar_dano() -> void:
	if Progresso.comprar_dano():
		label_mensagem.text = "Upgrade de dano comprado. Ele vale na próxima partida."
	else:
		label_mensagem.text = "Pontos insuficientes para comprar dano."
	atualizar()

func _estilizar_tela() -> void:
	var estilo_painel := StyleBoxFlat.new()
	estilo_painel.bg_color = Color("101a2b")
	estilo_painel.border_color = Color("355371")
	estilo_painel.set_border_width_all(2)
	estilo_painel.set_corner_radius_all(18)
	painel.add_theme_stylebox_override("panel", estilo_painel)
	var estilo_card := StyleBoxFlat.new()
	estilo_card.bg_color = Color("17263b")
	estilo_card.border_color = Color("2d4662")
	estilo_card.set_border_width_all(1)
	estilo_card.set_corner_radius_all(12)
	estilo_card.content_margin_left = 18
	estilo_card.content_margin_right = 18
	estilo_card.content_margin_top = 14
	estilo_card.content_margin_bottom = 14
	card_vida.add_theme_stylebox_override("panel", estilo_card)
	card_dano.add_theme_stylebox_override("panel", estilo_card)
	titulo.add_theme_font_size_override("font_size", 30)
	titulo.add_theme_color_override("font_color", Color("70e1d8"))
	label_pontos.add_theme_font_size_override("font_size", 20)
	label_pontos.add_theme_color_override("font_color", Color("e8f0ff"))
	label_vida.add_theme_font_size_override("font_size", 19)
	label_dano.add_theme_font_size_override("font_size", 19)
	label_vida.add_theme_color_override("font_color", Color("dce8f7"))
	label_dano.add_theme_color_override("font_color", Color("dce8f7"))
	label_mensagem.add_theme_color_override("font_color", Color("9ce4d9"))
	_estilizar_botao(botao_vida, Color("244e73"), Color("397fb0"))
	_estilizar_botao(botao_dano, Color("244e73"), Color("397fb0"))
	_estilizar_botao(botao_voltar, Color("1b2b43"), Color("344b69"))

func _estilizar_botao(botao: Button, fundo: Color, foco: Color) -> void:
	botao.add_theme_stylebox_override("normal", _criar_estilo_botao(fundo, fundo.darkened(0.3)))
	botao.add_theme_stylebox_override("hover", _criar_estilo_botao(foco, foco.lightened(0.15)))
	botao.add_theme_stylebox_override("disabled", _criar_estilo_botao(Color("263346"), Color("35445b")))
	botao.add_theme_color_override("font_color", Color("f3f6ff"))
	botao.add_theme_color_override("font_disabled_color", Color("8997aa"))
	botao.add_theme_font_size_override("font_size", 16)

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

func _ao_voltar() -> void:
	Global.voltar_para_submenu = true
	get_tree().change_scene_to_file(CENA_MENU)
