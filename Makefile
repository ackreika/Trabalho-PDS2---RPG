CXX = g++
CXXFLAGS = -std=c++17 -Wall -I include/ -fprofile-arcs -ftest-coverage

SRC_DIR = src
TEST_DIR = tests
OBJ_DIR = obj
BIN_DIR = bin

# -------------------------------------------------------------
# TRUQUE PARA GODOT: Para evitar dores de cabeça com dependências,
# vamos compilar APENAS as classes de lógica pura que não tocam 
# no godot_cpp.
# -------------------------------------------------------------
SRC_FILES = $(SRC_DIR)/GerenciadorQuiz.cpp \
            $(SRC_DIR)/GerenciadorRecompensas.cpp \
            $(SRC_DIR)/GerenciadorSemestre.cpp \
            $(SRC_DIR)/ItensConcretos.cpp \
            $(SRC_DIR)/ModificadorStatus.cpp \
            $(SRC_DIR)/Movimento.cpp \
            $(SRC_DIR)/Professor.cpp \
            $(SRC_DIR)/ProfessoresFabrica.cpp \
            $(SRC_DIR)/ProgressoJogador.cpp

# Pega todos os testes, mas depois filtra os que dão problema
TEST_FILES = $(wildcard $(TEST_DIR)/*.cpp)

TEST_FILES_VALIDOS = $(filter-out $(TEST_DIR)/test_herois.cpp \
                                  $(TEST_DIR)/test_Herois.cpp \
                                  $(TEST_DIR)/test_batalha.cpp \
                                  $(TEST_DIR)/test_Batalha.cpp, $(TEST_FILES))

# Gera os nomes dos arquivos .o usando APENAS os testes validos
SRC_OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
TEST_OBJS = $(patsubst $(TEST_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(TEST_FILES_VALIDOS))

EXEC = $(BIN_DIR)/test_suite

# Cria diretórios de objetos e binários se não existirem
$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR))

# Comando padrao do make vai rodar o "test"
all: test

# Regra de compilacao: Codigo fonte
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regra de compilacao: Testes doctest
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Linkagem final para gerar o executavel de testes
test: $(SRC_OBJS) $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $(EXEC)

# Roda os testes no terminal
run_tests: test
	./$(EXEC)

# Roda o gcovr para gerar o relatorio html (Exigência >60%)
coverage: run_tests
	gcovr -r . --html --html-details -o coverage_report.html
	@echo "\n>>> Relatório gerado: coverage_report.html <<<"

# Limpa toda a compilação gerada pelo g++
clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/* *.gcno *.gcda coverage_report*

.PHONY: all test run_tests coverage clean