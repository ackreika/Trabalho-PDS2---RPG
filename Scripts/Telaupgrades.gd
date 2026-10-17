extends Control
# Anexe ao nó raiz (Control) da cena TelaUpgrades.tscn.
#
# Nós esperados (nomes exatos, todos filhos diretos do Control):
#   LabelPontos, LabelVida, BotaoUpgradeVida, LabelDano, BotaoUpgradeDano,
#   LabelMensagem, BotaoVoltar

# Cena pra onde o botão "Voltar" leva — ajuste pro caminho real do seu menu.
const CENA_MENU := "res://CenaPrincipal.tscn"

func _ready() -> void:
	$BotaoUpgradeVida.pressed.connect(_ao_comprar_vida)
	$BotaoUpgradeDano.pressed.connect(_ao_comprar_dano)
	$BotaoVoltar.pressed.connect(_ao_voltar)
	atualizar()

func atualizar() -> void:
	$LabelPontos.text = "Pontos: %d" % Progresso.pontos

	$LabelVida.text = "Vida - nivel %d (+%d HP)" % [
		Progresso.nivel_vida, Progresso.nivel_vida * Progresso.BONUS_VIDA_POR_NIVEL]
	$BotaoUpgradeVida.text = "Aumentar vida (+%d)  |  custo: %d" % [
		Progresso.BONUS_VIDA_POR_NIVEL, Progresso.custo_vida()]
	$BotaoUpgradeVida.disabled = Progresso.pontos < Progresso.custo_vida()

	$LabelDano.text = "Dano - nivel %d (+%d de dano)" % [
		Progresso.nivel_dano, Progresso.nivel_dano * Progresso.BONUS_DANO_POR_NIVEL]
	$BotaoUpgradeDano.text = "Aumentar dano (+%d)  |  custo: %d" % [
		Progresso.BONUS_DANO_POR_NIVEL, Progresso.custo_dano()]
	$BotaoUpgradeDano.disabled = Progresso.pontos < Progresso.custo_dano()

func _ao_comprar_vida() -> void:
	if Progresso.comprar_vida():
		$LabelMensagem.text = "Vida aumentada! Vale na proxima run."
	else:
		$LabelMensagem.text = "Pontos insuficientes."
	atualizar()

func _ao_comprar_dano() -> void:
	if Progresso.comprar_dano():
		$LabelMensagem.text = "Dano aumentado! Vale na proxima run."
	else:
		$LabelMensagem.text = "Pontos insuficientes."
	atualizar()

func _ao_voltar() -> void:
	get_tree().change_scene_to_file(CENA_MENU)
