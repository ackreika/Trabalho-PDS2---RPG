extends Control

const CENA_MENU := "res://CenaPrincipal.tscn"

func _ready() -> void:
	$BotaoVoltar.pressed.connect(_ao_voltar)
	atualizar()

func atualizar() -> void:
	# Status: base do Global.iniciar_primeira_batalha() (vida 100, dano 20) + upgrades
	var vida: int = 100 + Progresso.nivel_vida * Progresso.BONUS_VIDA_POR_NIVEL
	var dano: int = 20 + Progresso.nivel_dano * Progresso.BONUS_DANO_POR_NIVEL
	$LabelStatus.text = "Status: Vida %d | Dano %d | Pontos %d" % [vida, dano, Progresso.pontos]

	# Passivos: quais e quantos
	var passivos: Array = []
	for id in Progresso.itens_desbloqueados:
		if id in Itens.PASSIVOS:
			passivos.append(id)
	var texto := "Itens passivos: %d\n" % passivos.size()
	if passivos.is_empty():
		texto += "(nenhum ainda)"
	for id in passivos:
		texto += "- %s\n" % Itens.descricao_passivo(id)
	$LabelPassivos.text = texto

	# Ativos: um botão por item, o equipado fica desabilitado
	for filho in $ListaEspeciais.get_children():
		$ListaEspeciais.remove_child(filho)
		filho.queue_free()
	var tem_ativo := false
	for id in Progresso.itens_desbloqueados:
		if not (id in Itens.ESPECIAIS):
			continue
		tem_ativo = true
		var equipado: bool = (id == Progresso.item_especial_equipado)
		var botao := Button.new()
		botao.text = ("[EQUIPADO] " if equipado else "") + Itens.descricao_especial(id)
		botao.disabled = equipado
		botao.pressed.connect(_ao_equipar.bind(id))
		$ListaEspeciais.add_child(botao)
	$LabelEspeciais.text = "Item ativo (4o golpe):" if tem_ativo else "Item ativo: nenhum desbloqueado ainda"

func _ao_equipar(id: String) -> void:
	if Progresso.equipar_item_especial(id):
		$LabelMensagem.text = "Equipado: %s (vale na proxima run)" % Itens.nome_item(id)
	atualizar()

func _ao_voltar() -> void:
	get_tree().change_scene_to_file(CENA_MENU)
