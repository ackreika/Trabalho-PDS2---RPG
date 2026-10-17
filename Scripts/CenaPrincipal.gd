extends Control

# Chamado quando o nó entra na árvore da cena
func _ready() -> void:
	pass

# Função que é chamada quando o botão Jogar é pressionado
func _on_botao_jogar_pressed() -> void:
	# 1. Inicializa os dados da primeira batalha no Singleton/Autoload Global
	Global.iniciar_primeira_batalha()
	
	# 2. Muda para a cena de batalha
	get_tree().change_scene_to_file("res://Batalha.tscn")
