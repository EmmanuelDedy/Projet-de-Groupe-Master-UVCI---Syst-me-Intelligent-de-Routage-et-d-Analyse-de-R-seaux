CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -std=c11 -g
LDFLAGS = 

# Dossiers sources
SRC_DIR = src
BIN_DIR = bin

# Chemins des Modules
MOD1_DIR = $(SRC_DIR)/Module1_Graphe
MOD2_DIR = $(SRC_DIR)/Module2_Routage
MOD3_DIR = $(SRC_DIR)/Module3_Securite
MOD4_DIR = $(SRC_DIR)/Module4_Files
MOD_GSM_DIR = $(SRC_DIR)/Module_GSM

# Sources Noyau (Core)
CORE_SRC = $(MOD1_DIR)/graphe.c $(MOD4_DIR)/liste_chainee.c $(MOD_GSM_DIR)/utils.c
LOGGER_SRC = $(MOD_GSM_DIR)/logger.c
ORCH_SRC = $(MOD_GSM_DIR)/orchestrator.c
ROUTAGE_SRC = $(MOD2_DIR)/routage.c $(MOD2_DIR)/backtracking.c
SECURITY_SRC = $(MOD3_DIR)/securite.c
SIMULATOR_SRC = $(MOD_GSM_DIR)/simulator.c

# Cibles de compilation
TARGETS = $(BIN_DIR)/main $(BIN_DIR)/dijkstra \
          $(BIN_DIR)/k_shortest $(BIN_DIR)/backtracking $(BIN_DIR)/scc_analyzer \
          $(BIN_DIR)/topo_generator $(BIN_DIR)/packet_sim

all: $(TARGETS)

$(TARGETS): | directories

directories:
	@mkdir -p $(BIN_DIR)

# Exécutable Principal (Menu Interactif avec Logger)
$(BIN_DIR)/main: $(SRC_DIR)/main.c $(CORE_SRC) $(LOGGER_SRC) $(ORCH_SRC) $(ROUTAGE_SRC) $(SECURITY_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Binaires de Routage (CLI avec Logger)
$(BIN_DIR)/dijkstra: $(MOD_GSM_DIR)/dijkstra_cli.c $(CORE_SRC) $(LOGGER_SRC) $(ROUTAGE_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BIN_DIR)/k_shortest: $(MOD_GSM_DIR)/k_shortest_cli.c $(CORE_SRC) $(LOGGER_SRC) $(ROUTAGE_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BIN_DIR)/backtracking: $(MOD_GSM_DIR)/backtracking_cli.c $(CORE_SRC) $(LOGGER_SRC) $(ROUTAGE_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Binaires de Sécurité (CLI avec Logger)
$(BIN_DIR)/scc_analyzer: $(MOD_GSM_DIR)/scc_analyzer_cli.c $(CORE_SRC) $(LOGGER_SRC) $(SECURITY_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)


# Binaire de Simulation
$(BIN_DIR)/packet_sim: $(MOD_GSM_DIR)/simulator.c $(CORE_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Binaire Générateur de Topologie
$(BIN_DIR)/topo_generator: $(MOD_GSM_DIR)/topo_generator_cli.c $(MOD_GSM_DIR)/generator.c $(CORE_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)


# Tests
TEST_SRC = $(SRC_DIR)/tests
TEST_BIN = $(BIN_DIR)/tests

$(TEST_BIN):
	@mkdir -p $(TEST_BIN)

test_graphe: $(TEST_SRC)/test_graphe.c $(CORE_SRC) | $(TEST_BIN)
	$(CC) $(CFLAGS) -o $(TEST_BIN)/test_graphe $^ $(LDFLAGS)
	./$(TEST_BIN)/test_graphe

test_routage: $(TEST_SRC)/test_routage.c $(CORE_SRC) $(ROUTAGE_SRC) | $(TEST_BIN)
	$(CC) $(CFLAGS) -o $(TEST_BIN)/test_routage $^ $(LDFLAGS)
	./$(TEST_BIN)/test_routage

test_securite: $(TEST_SRC)/test_securite.c $(CORE_SRC) $(SECURITY_SRC) | $(TEST_BIN)
	$(CC) $(CFLAGS) -o $(TEST_BIN)/test_securite $^ $(LDFLAGS)
	./$(TEST_BIN)/test_securite

test_global: $(TEST_SRC)/test_global.c $(CORE_SRC) $(ROUTAGE_SRC) $(SECURITY_SRC) | $(TEST_BIN)
	$(CC) $(CFLAGS) -o $(TEST_BIN)/test_global $^ $(LDFLAGS)
	./$(TEST_BIN)/test_global

test_liste: $(TEST_SRC)/test_liste.c src/Module4_Files/liste_chainee.c | $(TEST_BIN)
	$(CC) $(CFLAGS) -o $(TEST_BIN)/test_liste $^ $(LDFLAGS)
	./$(TEST_BIN)/test_liste

test_generator: $(TEST_SRC)/test_generator.c $(MOD_GSM_DIR)/generator.c $(CORE_SRC) | $(TEST_BIN)
	$(CC) $(CFLAGS) -o $(TEST_BIN)/test_generator $^ $(LDFLAGS)
	./$(TEST_BIN)/test_generator

test_sim: $(TEST_SRC)/test_sim.c $(MOD_GSM_DIR)/simulator.c $(MOD_GSM_DIR)/utils.c | $(TEST_BIN)
	$(CC) $(CFLAGS) -DTEST_MODE -o $(TEST_BIN)/test_sim $^ $(LDFLAGS)
	./$(TEST_BIN)/test_sim

tests: test_graphe test_routage test_securite test_global test_liste test_generator test_sim
	@echo "-----------------------------------"
	@echo ">>> TOUS LES TESTS SONT PASSES <<<"
	@echo "-----------------------------------"

clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean directories tests test_graphe test_routage test_securite test_global
