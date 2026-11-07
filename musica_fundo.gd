extends Node

func tocar_intro() -> void:
	# Garante que a música de batalha esteja muda e parada
	$TocadorBatalha.volume_db = -80.0
	$TocadorBatalha.stop()
	
	# Toca a intro no volume normal
	$TocadorIntro.volume_db = 0.0
	$TocadorIntro.play(0.3)

func iniciar_musica_batalha() -> void:
	# Se a batalha já estiver tocando, não faz nada
	if $TocadorBatalha.playing and $TocadorBatalha.volume_db >= -1.0:
		return
		
	$TocadorBatalha.play()
	
	# Faz o fade entre as duas músicas
	var tween = create_tween()
	tween.tween_property($TocadorIntro, "volume_db", -80.0, -0.5)
	tween.parallel().tween_property($TocadorBatalha, "volume_db", 0.0, 0)

func parar_tudo() -> void:
	$TocadorIntro.stop()
	$TocadorBatalha.stop()
