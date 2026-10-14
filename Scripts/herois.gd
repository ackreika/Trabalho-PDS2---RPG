extends Herois


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	print("O herói C++ foi iniciado na cena!")
	
	\
	curar(20)
	print("Função curar(20) chamada com sucesso a partir do GDScript.")


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass
