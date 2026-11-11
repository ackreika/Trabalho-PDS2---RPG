# ==============================================================================
# UNIVERSIDADE FEDERAL DE MINAS GERAIS (UFMG)
# Disciplina: Programação e Desenvolvimento de Software 2 (PDS 2)
# Projeto: RPG Acadêmico em C++ e Godot Engine 4 (GDExtension)
# ==============================================================================
# Este Makefile automatiza o ciclo completo de desenvolvimento:
# 1. Compilação da biblioteca dinâmica (DLL) do backend C++ para a Godot Engine.
# 2. Compilação e execução da suíte de testes unitários com Doctest.
# 3. Geração de relatórios de cobertura de código com Gcov / Gcovr.
# ==============================================================================

# Compilador C++ padrão (GCC / MinGW-w64 no Windows)
CXX = g++

# ------------------------------------------------------------------------------
# DIRETÓRIOS DO PROJETO
# ------------------------------------------------------------------------------
SRC_DIR       = src
INC_DIR       = include
TEST_DIR      = tests
BUILD_DIR     = build
BIN_DIR       = bin
OBJ_DIR       = obj
OBJ_TEST_DIR  = obj_test
COV_DIR       = relatorio
GODOT_CPP_DIR = godot-cpp

# ------------------------------------------------------------------------------
# FLAGS DE COMPILAÇÃO
# ------------------------------------------------------------------------------
# -std=c++17 : Habilita os recursos modernos do padrão C++17
# -Wall -Wextra : Ativa todos os avisos essenciais para qualidade de código
# -g : Inclui símbolos de depuração
# -I$(INC_DIR) : Diretório com os cabeçalhos (.hpp) do jogo
# -isystem ... : Inclui cabeçalhos de terceiros suprimindo avisos internos
CXXFLAGS = -std=c++17 -Wall -Wextra -g \
           -I$(INC_DIR) \
           -isystem third_party \
           -isystem $(GODOT_CPP_DIR)/include \
           -isystem $(GODOT_CPP_DIR)/gen/include \
           -isystem $(GODOT_CPP_DIR)/gdextension

# Flags específicas para execução de testes com instrumentação de cobertura (Gcov)
CXXFLAGS_TEST = $(CXXFLAGS) -O0 --coverage
LDFLAGS_TEST  = --coverage

# Biblioteca estática da GDExtension (Godot C++ bindings)
GODOT_LIB = $(GODOT_CPP_DIR)/bin/libgodot-cpp.windows.template_debug.x86_64.a

# ------------------------------------------------------------------------------
# MAPEAMENTO DE ARQUIVOS-FONTE E OBJETOS
# ------------------------------------------------------------------------------

# 1. Todos os arquivos de implementação do jogo (usados para a DLL)
SRC_FILES_ALL = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES_DLL = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES_ALL))

# 2. Arquivos do domínio de regras de negócio (testáveis sem a GUI do motor)
SRC_FILES_LOGICA = $(SRC_DIR)/Batalha.cpp \
                   $(SRC_DIR)/EstadoMenu.cpp \
                   $(SRC_DIR)/GerenciadorQuiz.cpp \
                   $(SRC_DIR)/GerenciadorRecompensas.cpp \
                   $(SRC_DIR)/GerenciadorSemestre.cpp \
                   $(SRC_DIR)/Herois.cpp \
                   $(SRC_DIR)/ItensConcretos.cpp \
                   $(SRC_DIR)/MochilaItens.cpp \
                   $(SRC_DIR)/ModificadorStatus.cpp \
                   $(SRC_DIR)/Movimento.cpp \
                   $(SRC_DIR)/Professor.cpp \
                   $(SRC_DIR)/ProfessoresFabrica.cpp \
                   $(SRC_DIR)/ProgressoJogador.cpp \
                   $(SRC_DIR)/Recompensas.cpp

OBJ_FILES_TEST_SRC = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_TEST_DIR)/%.o, $(SRC_FILES_LOGICA))

# 3. Arquivos de testes unitários da pasta tests/
TEST_FILES     = $(wildcard $(TEST_DIR)/*.cpp)
OBJ_FILES_TEST = $(patsubst $(TEST_DIR)/%.cpp, $(OBJ_TEST_DIR)/%.o, $(TEST_FILES))

# ------------------------------------------------------------------------------
# ARTEFATOS FINAIS
# ------------------------------------------------------------------------------
# A DLL deve residir em build/ para corresponder à configuração do game.gdextension
TARGET_DLL  = $(BUILD_DIR)/libgame.windows.template_debug.x86_64.dll
TARGET_TEST = $(BIN_DIR)/test_suite.exe

# ==============================================================================
# ALVOS PRINCIPAIS (TARGETS)
# ==============================================================================

# Alvo padrão: compila a DLL e o executável de testes
all: dll test

# Compila a DLL GDExtension para integração com o motor gráfico Godot
dll: directories $(TARGET_DLL)

$(TARGET_DLL): $(OBJ_FILES_DLL)
	@echo [LINKANDO DLL DO JOGO] Gerando biblioteca GDExtension em $@...
	$(CXX) -shared -static -static-libgcc -static-libstdc++ $^ $(GODOT_LIB) -lwinpthread -o $@
	@echo [SUCESSO] DLL gerada com sucesso! O jogo ja pode ser executado no Godot.

# Compila o executável da suíte de testes unitários (Doctest)
test: directories $(TARGET_TEST)

$(TARGET_TEST): $(OBJ_FILES_TEST_SRC) $(OBJ_FILES_TEST)
	@echo [LINKANDO TESTES] Gerando $@...
	$(CXX) $(CXXFLAGS_TEST) $^ $(GODOT_LIB) -o $@ $(LDFLAGS_TEST)
	@echo [SUCESSO] Executavel de testes unitarios criado!

# Executa os testes unitários no terminal
run_tests: test
	@if exist "$(OBJ_TEST_DIR)\*.gcda" del /q "$(OBJ_TEST_DIR)\*.gcda"
	@echo [EXECUTANDO SUITE DE TESTES UNITARIOS - DOCTEST]
	./$(TARGET_TEST)

# Gera relatório visual em HTML da cobertura de código com gcovr
coverage: run_tests
	@echo [GERANDO RELATORIO DE COBERTURA DE TESTES - GCOVR]
	python -m gcovr -r . --object-directory $(OBJ_TEST_DIR) --filter "$(SRC_DIR)/" --html --html-details -o $(COV_DIR)/coverage_report.html
	@echo [SUCESSO] Relatorio gerado em: '$(COV_DIR)/coverage_report.html'

# ------------------------------------------------------------------------------
# REGRAS DE COMPILAÇÃO DE OBJETOS (.cpp -> .o)
# ------------------------------------------------------------------------------

# Objeto padrão para a DLL do jogo
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo [COMPILANDO C++ (DLL)]: $<
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Objeto de lógica de negócio instrumentado para cobertura
$(OBJ_TEST_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo [COMPILANDO C++ (TESTE/COBERTURA)]: $<
	$(CXX) $(CXXFLAGS_TEST) -c $< -o $@

# Objeto do caso de teste
$(OBJ_TEST_DIR)/%.o: $(TEST_DIR)/%.cpp
	@echo [COMPILANDO TESTE UNITARIO]: $<
	$(CXX) $(CXXFLAGS_TEST) -c $< -o $@

# ------------------------------------------------------------------------------
# UTILITÁRIOS DE AMBIENTE E LIMPEZA (COMPATÍVEIS COM WINDOWS / CMD)
# ------------------------------------------------------------------------------

# Garante que as pastas de destino existam antes da compilação
directories:
	@if not exist "$(OBJ_DIR)" mkdir "$(OBJ_DIR)"
	@if not exist "$(OBJ_TEST_DIR)" mkdir "$(OBJ_TEST_DIR)"
	@if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)"
	@if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"
	@if not exist "$(COV_DIR)" mkdir "$(COV_DIR)"

# Limpa artefatos intermediários de compilação preservando a DLL final
clean:
	@echo [LIMPANDO ARQUIVOS INTERMEDIARIOS E TEMPORARIOS]
	@if exist "$(OBJ_DIR)" rmdir /s /q "$(OBJ_DIR)"
	@if exist "$(OBJ_TEST_DIR)" rmdir /s /q "$(OBJ_TEST_DIR)"
	@if exist "$(COV_DIR)" rmdir /s /q "$(COV_DIR)"
	@if exist "$(BIN_DIR)\test_suite.exe" del /q "$(BIN_DIR)\test_suite.exe"
	@echo Limpeza concluida com sucesso!

# Limpeza total, incluindo a DLL final gerada
clean_all: clean
	@if exist "$(TARGET_DLL)" del /q "$(TARGET_DLL)"
	@echo Todos os binarios gerados foram removidos.

.PHONY: all dll test run_tests coverage directories clean clean_all
