extends GerenciadorInterface

const CENA_BATALHA := "res://Batalha.tscn"

## Índice = id da área no Global (0=Cálculo, 1=GAAL, 2=Física, 3=PDS).
const NOMES_AREAS := ["Cálculo", "GAAL / Análise Numérica", "Física", "PDS", "Luciana (secreta)"]


func _ready() -> void:
	if Global.voltar_para_submenu:
		Global.voltar_para_submenu = false
		clicar_jogar()
	var caixa_res = find_child("CaixaResolucao", true, false)
	if caixa_res and caixa_res.item_count == 6:
		caixa_res.add_item("Tela Cheia (Exclusive)")
		caixa_res.add_item("Tela Cheia (Borderless)")

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
	clicar_jogar()

# --- FUNÇÕES DO NOVO SUBMENU (CONTAINER JOGAR) ---

func _on_botao_voltar_jogar_pressed() -> void:
	# Puxa o voltar do C++ para esconder o submenu e voltar pro menu principal
	clicar_voltar()
