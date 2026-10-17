extends GerenciadorInterface


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass



func _on_botao_upgrades_pressed() -> void:
	get_tree().change_scene_to_file("res://TelaUpgrades.tscn")

func _on_botao_jogar_pressed() -> void:
	Global.iniciar_primeira_batalha()
	get_tree().change_scene_to_file("res://Batalha.tscn")
