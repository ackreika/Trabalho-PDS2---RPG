extends Control

const CENA_MENU := "res://CenaPrincipal.tscn"

@onready var painel: PanelContainer = $Painel
@onready var titulo: Label = $Painel/Conteudo/Coluna/LabelTitulo
@onready var label_status: Label = $Painel/Conteudo/Coluna/LabelStatus
@onready var label_passivos: Label = $Painel/Conteudo/Coluna/LabelPassivos
@onready var label_especiais: Label = $Painel/Conteudo/Coluna/LabelEspeciais
@onready var lista_especiais: VBoxContainer = $Painel/Conteudo/Coluna/ScrollEspeciais/ListaEspeciais
@onready var label_mensagem: Label = $Painel/Conteudo/Coluna/LabelMensagem
@onready var botao_voltar: Button = $Painel/Conteudo/Coluna/BotaoVoltar

func _ready() -> void:
	botao_voltar.pressed.connect(_ao_voltar)
	_estilizar_tela()
	atualizar()

func atualizar() -> void:
	var vida: int = 100 + Progresso.nivel_vida * Progresso.BONUS_VIDA_POR_NIVEL
	var dano: int = 20 + Progresso.nivel_dano * Progresso.BONUS_DANO_POR_NIVEL
	label_status.text = "Vida %d     |     Dano %d     |     Pontos %d" % [vida, dano, Progresso.get_pontos()]

	var todos_itens: Array = Progresso.get_todos_itens()
	var passivos: Array = []
	for id in todos_itens:
		if id in Itens.PASSIVOS:
			passivos.append(id)

	var texto := "Itens passivos desbloqueados: %d\n" % passivos.size()
	if passivos.is_empty():
		texto += "Nenhum item passivo desbloqueado ainda."
	else:
		for id in passivos:
			texto += "• %s\n" % Itens.descricao_passivo(id)
	label_passivos.text = texto.strip_edges()

	for filho in lista_especiais.get_children():
		lista_especiais.remove_child(filho)
		filho.queue_free()

	var tem_ativo := false
	for id in todos_itens:
		if not (id in Itens.ESPECIAIS):
			continue
		tem_ativo = true
		var equipado: bool = str(id) == str(Progresso.item_especial_equipado)
		var botao := Button.new()
		botao.text = ("EQUIPADO  ·  " if equipado else "EQUIPAR  ·  ") + Itens.descricao_especial(id)
		botao.disabled = equipado
		botao.custom_minimum_size = Vector2(0, 52)
		botao.add_theme_stylebox_override("normal", _criar_estilo_botao(Color("1b2b43"), Color("344b69")))
		botao.add_theme_stylebox_override("hover", _criar_estilo_botao(Color("244b61"), Color("70e1d8")))
		botao.add_theme_stylebox_override("disabled", _criar_estilo_botao(Color("173c3a"), Color("397b72")))
		botao.add_theme_color_override("font_color", Color("e8f0ff"))
		botao.add_theme_color_override("font_disabled_color", Color("9ce4d9"))
		botao.pressed.connect(_ao_equipar.bind(id))
		lista_especiais.add_child(botao)

	label_especiais.text = "Itens ativos — começam no quarto golpe" if tem_ativo else "Itens ativos — nenhum desbloqueado ainda"

func _ao_equipar(id: String) -> void:
	if Progresso.equipar_item_especial(id):
		label_mensagem.text = "Equipado: %s. Será usado na próxima partida." % Itens.nome_item(id)
	atualizar()

func _estilizar_tela() -> void:
	var estilo_painel := StyleBoxFlat.new()
	estilo_painel.bg_color = Color("101a2b")
	estilo_painel.border_color = Color("355371")
	estilo_painel.set_border_width_all(2)
	estilo_painel.set_corner_radius_all(18)
	painel.add_theme_stylebox_override("panel", estilo_painel)
	titulo.add_theme_font_size_override("font_size", 30)
	titulo.add_theme_color_override("font_color", Color("70e1d8"))
	label_status.add_theme_font_size_override("font_size", 20)
	label_status.add_theme_color_override("font_color", Color("e8f0ff"))
	label_passivos.add_theme_font_size_override("font_size", 17)
	label_passivos.add_theme_color_override("font_color", Color("c5d2e5"))
	label_especiais.add_theme_font_size_override("font_size", 18)
	label_especiais.add_theme_color_override("font_color", Color("dce8f7"))
	label_mensagem.add_theme_color_override("font_color", Color("9ce4d9"))
	botao_voltar.custom_minimum_size = Vector2(0, 50)
	botao_voltar.add_theme_stylebox_override("normal", _criar_estilo_botao(Color("244e73"), Color("397fb0")))
	botao_voltar.add_theme_stylebox_override("hover", _criar_estilo_botao(Color("397fb0"), Color("70e1d8")))
	botao_voltar.add_theme_color_override("font_color", Color("ffffff"))

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
