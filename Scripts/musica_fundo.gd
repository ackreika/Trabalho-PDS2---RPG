extends Node

# Dicionário para controlar os estados de áudio
var _primeira_vez := {}

func tocar_menu() -> void:
	if has_node("TocadorMenu") and $TocadorMenu.playing and $TocadorMenu.volume_db >= -1.0:
		return
		
	$TocadorIntro.stop()
	$TocadorBatalha.stop()
	if has_node("TocadorBatalhaBoss"): $TocadorBatalhaBoss.stop()
	if has_node("TocadorSuperBoss"): $TocadorSuperBoss.stop()
	
	if has_node("TocadorMenu"):
		$TocadorMenu.volume_db = -80.0
		_tocar_com_offset_no_loop($TocadorMenu, 0.0)
		var tween = create_tween()
		tween.tween_property($TocadorMenu, "volume_db", 0.0, 0.5)

func tocar_intro() -> void:
	# Se o Super Boss já estiver tocando, bloqueia completamente a intro
	if has_node("TocadorSuperBoss") and $TocadorSuperBoss.playing:
		return
		
	if has_node("TocadorMenu"): $TocadorMenu.stop()
	$TocadorBatalha.stop()
	if has_node("TocadorBatalhaBoss"): $TocadorBatalhaBoss.stop()
	if has_node("TocadorSuperBoss"): $TocadorSuperBoss.stop()
	
	$TocadorIntro.volume_db = 0.0
	_tocar_com_offset_no_loop($TocadorIntro, 0.0)

func iniciar_musica_batalha(tipo_ou_eh_boss = "comum") -> void:
	
	var tocador_alvo = $TocadorBatalha
	
	var tipo_musica := "comum"
	if typeof(tipo_ou_eh_boss) == TYPE_BOOL:
		tipo_musica = "boss" if tipo_ou_eh_boss else "comum"
	else:
		tipo_musica = str(tipo_ou_eh_boss).strip_edges().to_lower()
	
	print("DEBUG - Tocando música do tipo: ", tipo_musica)
	
	if tipo_musica == "super_boss" and has_node("TocadorSuperBoss"):
		tocador_alvo = $TocadorSuperBoss
		print(">> Sucesso: TocadorSuperBoss selecionado!")
	elif tipo_musica == "boss" and has_node("TocadorBatalhaBoss"):
		tocador_alvo = $TocadorBatalhaBoss
		
	if tocador_alvo.playing and tocador_alvo.volume_db >= -1.0:
		return
		
	# Para todas as outras músicas e a intro de uma única vez (limpo e sem duplicações)
	$TocadorIntro.stop()
	if has_node("TocadorMenu"): $TocadorMenu.stop()
	$TocadorBatalha.stop()
	if has_node("TocadorBatalhaBoss"): $TocadorBatalhaBoss.stop()
	if has_node("TocadorSuperBoss"): $TocadorSuperBoss.stop()
	
	tocador_alvo.volume_db = 0.0
	
	var offset_inicial := 0.0
	var offset_loop := 0.0
	
	if tipo_musica == "super_boss":
		offset_inicial = 0.4 # Exemplo: pula 2.5 segundos na PRIMEIRA vez que tocar
		offset_loop = 0.2    # Exemplo: pula 1.0 segundo em todos os LOOPS seguintes
	elif tipo_musica == "boss":
		offset_inicial = 0.0
		offset_loop = 0.0
	else:
		offset_inicial = 0.0
		offset_loop = 0.0
	_tocar_com_offset_no_loop(tocador_alvo, offset_loop)

func _tocar_com_offset_no_loop(tocador: AudioStreamPlayer, tempo_offset: float) -> void:
	_primeira_vez[tocador] = true
	tocador.play(0.0) # Toca do início na primeira execução
	
	# Desconecta conexões anteriores do sinal finished para evitar duplicação
	if tocador.has_signal("finished"):
		for conexao in tocador.get_signal_connection_list("finished"):
			tocador.finished.disconnect(conexao["callable"])

	# Conecta a função lambda limpa para tratar os loops com offset
	tocador.finished.connect(func():
		tocador.play(tempo_offset)
	)

func parar_tudo() -> void:
	$TocadorIntro.stop()
	$TocadorBatalha.stop()
	if has_node("TocadorBatalhaBoss"): $TocadorBatalhaBoss.stop()
	if has_node("TocadorSuperBoss"): $TocadorSuperBoss.stop()
	if has_node("TocadorMenu"): $TocadorMenu.stop()
