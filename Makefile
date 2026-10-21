CXX = g++
CXXFLAGS = -std=c++17 -Wall -I include/ -I godot-cpp/gdextension -I godot-cpp/include -I godot-cpp/gen/include -fprofile-arcs -ftest-coverage

SRC_DIR = src
TEST_DIR = tests
OBJ_DIR = obj
BIN_DIR = bin
DLL_DIR = build

# ARQUIVO DE BIBLIOTECA GERADO PELO SCONS
GODOT_LIB = godot-cpp/bin/libgodot-cpp.windows.template_debug.x86_64.a

# NOME EXATO DA DLL QUE O GODOT ESPERA
DLL_OUT = $(DLL_DIR)/libgame.windows.template_debug.x86_64.dll


# 1. ARQUIVOS PARA O GODOT
GAME_SRC = $(wildcard $(SRC_DIR)/*.cpp)
GAME_OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(GAME_SRC))

# 2. ARQUIVOS PARA OS TESTES DE TERMINAL (Lógica Pura)
SRC_FILES_LOGICA = $(SRC_DIR)/GerenciadorQuiz.cpp \
            $(SRC_DIR)/GerenciadorRecompensas.cpp \
            $(SRC_DIR)/GerenciadorSemestre.cpp \
            $(SRC_DIR)/ItensConcretos.cpp \
            $(SRC_DIR)/ModificadorStatus.cpp \
            $(SRC_DIR)/Movimento.cpp \
            $(SRC_DIR)/Professor.cpp \
            $(SRC_DIR)/ProfessoresFabrica.cpp \
            $(SRC_DIR)/ProgressoJogador.cpp

TEST_FILES = $(wildcard $(TEST_DIR)/*.cpp)

TEST_FILES_VALIDOS = $(filter-out $(TEST_DIR)/test_herois.cpp \
                                  $(TEST_DIR)/test_Herois.cpp \
                                  $(TEST_DIR)/test_batalha.cpp \
                                  $(TEST_DIR)/test_Batalha.cpp \
                                  $(TEST_DIR)/test_Professor.cpp \
                                  $(TEST_DIR)/test_Movimento.cpp, $(TEST_FILES))

SRC_OBJS_LOGICA = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES_LOGICA))
TEST_OBJS = $(patsubst $(TEST_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(TEST_FILES_VALIDOS))

EXEC_TESTE = $(BIN_DIR)/test_suite

# Cria diretórios nativamente no Windows
$(shell if not exist $(OBJ_DIR) mkdir $(OBJ_DIR))
$(shell if not exist $(BIN_DIR) mkdir $(BIN_DIR))
$(shell if not exist $(DLL_DIR) mkdir $(DLL_DIR))

# Comando padrao do make vai rodar a compilação do jogo
all: dll

# Regras de compilacao
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 1. GERA A DLL PARA O GODOT
dll: $(GAME_OBJ)
	$(CXX) $(CXXFLAGS) -shared -static -static-libgcc -static-libstdc++ -o $(DLL_OUT) $(GAME_OBJ) $(GODOT_LIB)
	@echo "\n>>> DLL do jogo gerada com sucesso na pasta 'build'! Abra o Godot. <<<"

# 2. COMANDOS DE TESTE E COBERTURA
test: $(SRC_OBJS_LOGICA) $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) $^ $(GODOT_LIB) -o $(EXEC_TESTE)

run_tests: test
	./$(EXEC_TESTE)

coverage: run_tests
	if not exist relatorio mkdir relatorio
	python -m gcovr -r . --html --html-details -o relatorio/coverage_report.html
	@echo "\n>>> Relatórios gerados com sucesso na pasta 'relatorio' <<<"

# Limpa toda a compilação e apaga a pasta de relatórios
clean:
	-del /q /s obj\* bin\* build\* *.gcno *.gcda coverage_report* 2>nul
	-rmdir /q /s relatorio 2>nul

.PHONY: all dll test run_tests coverage clean