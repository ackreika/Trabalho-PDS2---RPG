extends GerenciadorInterface

const CENA_BATALHA := "res://cenas/Batalha.tscn"

## Índice = id da área no Global (0=Cálculo, 1=GAAL, 2=Física, 3=PDS).
const NOMES_AREAS := ["Cálculo", "GAAL / Análise Numérica", "Física", "PDS", "Luciana (secreta)"]


func _ready() -> void:
	var btn_reset = find_child("BotaoResetarProgresso", true, false)
	if btn_reset and not btn_reset.is_connected("pressed", _on_botao_resetar_progresso_pressed):
		btn_reset.pressed.connect(_on_botao_resetar_progresso_pressed)
	var musica := get_node_or_null("/root/MusicaFundo")
	if musica and musica.has_method("tocar_menu"):
		musica.tocar_menu()
	var slider_vol: HSlider = find_child("SliderVolume", true, false)
	if slider_vol:
		slider_vol.value = 50.0  # Garante que o visual do slider vá para o meio
		_on_slider_volume_value_changed(50.0) # Aplica a curva de 50% no AudioServer
	if Global.voltar_para_submenu:
		Global.voltar_para_submenu = false
		clicar_jogar()
	var caixa_res = find_child("CaixaResolucao", true, false)
	if caixa_res:
		if not caixa_res.is_connected("item_selected", _on_caixa_resolucao_item_selected):
			caixa_res.item_selected.connect(_on_caixa_resolucao_item_selected)
# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass


## Atualiza quais áreas estão liberadas e mostra o painel criado no C++.
func _abrir_painel_areas() -> void:
	for id_area in range(NOMES_AREAS.size()):
		var liberada: bool = Global.area_desbloqueada(id_area)
		atualizar_estado_area(id_area, liberada)
	abrir_painel_areas()


func _fechar_painel_areas() -> void:
	fechar_painel_areas()


func _on_area_escolhida(id_area: int) -> void:
	if Global.iniciar_primeira_batalha(id_area):
		get_tree().change_scene_to_file(CENA_BATALHA)


func _on_botao_upgrades_pressed() -> void:
	get_tree().change_scene_to_file("res://cenas/TelaUpgrades.tscn")

func _on_botao_mundo_pressed() -> void:
	_abrir_painel_areas()


func _on_botao_itens_pressed() -> void:
	get_tree().change_scene_to_file("res://cenas/TelaItens.tscn")


func _on_botao_config_pressed() -> void:
	clicar_configuracoes() 

func _on_botao_voltar_pressed() -> void:
	clicar_voltar()

func _on_slider_volume_value_changed(valor: float) -> void:
	mudar_volume(valor) # Mantém o seu código C++ salvo
	
	var bus_index = AudioServer.get_bus_index("Master")
	
	if valor <= 0.0:
		AudioServer.set_bus_mute(bus_index, true)
	else:
		AudioServer.set_bus_mute(bus_index, false)
		# Aplica a curva perceptual ao quadrado: o valor 50% real agora soa como metade do volume
		var volume_perceptual = pow(valor / 100.0, 2.0)
		var volume_db = linear_to_db(volume_perceptual)
		AudioServer.set_bus_volume_db(bus_index, volume_db)

func _on_caixa_resolucao_item_selected(indice: int) -> void:
	mudar_resolucao(indice)
	
func _on_botao_sair_pressed() -> void:
	clicar_sair()

func _on_botao_jogar_pressed() -> void:
	clicar_jogar()

# --- FUNÇÕES DO NOVO SUBMENU (CONTAINER JOGAR) ---

func _on_botao_voltar_jogar_pressed() -> void:
	# Puxa o voltar do C++ para esconder o submenu e voltar pro menu principal
	clicar_voltar()
func _on_botao_resetar_progresso_pressed() -> void:
	var dialog := ConfirmationDialog.new()
	dialog.title = "Confirmação"
	dialog.dialog_text = "Tem certeza que deseja apagar todo o seu progresso?\nUpgrades, pontos e conquistas serão perdidos!"
	dialog.ok_button_text = "Sim, apagar tudo"
	dialog.cancel_button_text = "Cancelar"
	
	dialog.confirmed.connect(func():
		Progresso.resetar_progresso()
		get_tree().reload_current_scene()
	)
	
	add_child(dialog)
	dialog.popup_centered()
