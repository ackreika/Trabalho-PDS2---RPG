extends GerenciadorInterface

const CENA_BATALHA := "res://Batalha.tscn"

## Índice = id da área no Global (0=Cálculo, 1=GAAL, 2=Física, 3=PDS).
const NOMES_AREAS := ["Cálculo", "GAAL / Análise Numérica", "Física", "PDS"]

var _painel_areas: Control
var _botoes_areas: Array[Button] = []


func _ready() -> void:
	_criar_painel_areas()
	if Global.voltar_para_submenu:
		Global.voltar_para_submenu = false 
		clicar_jogar()
	var caixa_res = $ContainerConfig/CaixaResolucao
	if caixa_res.item_count == 6:
		caixa_res.add_item("Tela Cheia (Exclusive)")
		caixa_res.add_item("Tela Cheia (Borderless)")
# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass


## Monta, por código, o painel de seleção de área (começa escondido).
func _criar_painel_areas() -> void:
	_painel_areas = Control.new()
	_painel_areas.visible = false
	add_child(_painel_areas)
	_painel_areas.set_anchors_and_offsets_preset(Control.PRESET_FULL_RECT)

	# Fundo escuro próprio, para o painel ficar legível sobre qualquer fundo
	var fundo := ColorRect.new()
	fundo.color = Color(0, 0, 0, 0.85)
	_painel_areas.add_child(fundo)
	fundo.set_anchors_and_offsets_preset(Control.PRESET_FULL_RECT)

	var centro := CenterContainer.new()
	_painel_areas.add_child(centro)
	centro.set_anchors_and_offsets_preset(Control.PRESET_FULL_RECT)

	var caixa := VBoxContainer.new()
	caixa.add_theme_constant_override("separation", 20)
	centro.add_child(caixa)

	var titulo := Label.new()
	titulo.text = "Escolha a área"
	titulo.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	titulo.add_theme_font_size_override("font_size", 48)
	caixa.add_child(titulo)

	for id_area in range(NOMES_AREAS.size()):
		var botao := Button.new()
		botao.add_theme_font_size_override("font_size", 40)
		botao.pressed.connect(_on_area_escolhida.bind(id_area))
		caixa.add_child(botao)
		_botoes_areas.append(botao)

	var voltar := Button.new()
	voltar.text = "Voltar"
	voltar.add_theme_font_size_override("font_size", 40)
	voltar.pressed.connect(_fechar_painel_areas)
	caixa.add_child(voltar)


## Atualiza quais áreas estão liberadas e mostra o painel.
func _abrir_painel_areas() -> void:
	for id_area in range(_botoes_areas.size()):
		var liberada: bool = Global.area_desbloqueada(id_area)
		var botao: Button = _botoes_areas[id_area]
		botao.disabled = not liberada
		if liberada:
			botao.text = NOMES_AREAS[id_area]
		else:
			botao.text = "%s (bloqueada)" % NOMES_AREAS[id_area]
	_painel_areas.visible = true


func _fechar_painel_areas() -> void:
	_painel_areas.visible = false


func _on_area_escolhida(id_area: int) -> void:
	Global.iniciar_primeira_batalha(id_area)
	get_tree().change_scene_to_file(CENA_BATALHA)


func _on_botao_upgrades_pressed() -> void:
	get_tree().change_scene_to_file("res://TelaUpgrades.tscn")

func _on_botao_mundo_pressed() -> void:
	_abrir_painel_areas()


func _on_botao_itens_pressed() -> void:
	get_tree().change_scene_to_file("res://TelaItens.tscn")


func _on_botao_config_pressed() -> void:
	clicar_configuracoes() 

func _on_botao_voltar_pressed() -> void:
	
	clicar_voltar()

func _on_slider_volume_value_changed(valor: float) -> void:
	mudar_volume(valor) # Mantém o seu código C++ salvo
	
	# Se o valor for 0, fica mudo. Se for 100, fica no volume normal.
	var volume_db = linear_to_db(valor / 100.0)
	AudioServer.set_bus_volume_db(AudioServer.get_bus_index("Master"), volume_db)

func _on_caixa_resolucao_item_selected(indice: int) -> void:
	
	mudar_resolucao(indice)
	
func _on_botao_sair_pressed() -> void:
	clicar_sair()

func _on_botao_jogar_pressed() -> void:
	var nova_textura = load("res://imagens/maxpayne.jpg")
	if nova_textura:
		$FundoJogo.texture = nova_textura
	clicar_jogar() 

# --- FUNÇÕES DO NOVO SUBMENU (CONTAINER JOGAR) ---

func _on_botao_voltar_jogar_pressed() -> void:
	# Puxa o voltar do C++ para esconder o submenu e voltar pro menu principal
	clicar_voltar()



	
