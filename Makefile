CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -Isrc -std=c11 -g
LDFLAGS = 

# Dossiers sources
SRC_DIR = src
BIN_DIR = bin

# Chemins des Modules (Structure Plate)
MOD1_DIR = $(SRC_DIR)
MOD2_DIR = $(SRC_DIR)
MOD3_DIR = $(SRC_DIR)
MOD4_DIR = $(SRC_DIR)
MOD_GSM_DIR = $(SRC_DIR)

# Sources Noyau (Core)
CORE_SRC = $(MOD1_DIR)/graphe.c $(MOD4_DIR)/liste_chainee.c $(MOD_GSM_DIR)/utils.c
LOGGER_SRC = $(MOD_GSM_DIR)/logger.c  # Note: logger.c n'etait pas dans ls src, verifier s'il existe
ORCH_SRC = $(MOD_GSM_DIR)/orchestrator.c # Pareil
ROUTAGE_SRC = $(MOD2_DIR)/routage.c $(MOD2_DIR)/backtracking.c
SECURITY_SRC = $(MOD3_DIR)/securite.c
SIMULATOR_SRC = $(MOD_GSM_DIR)/simulator.c # Pareil

# Tests Utils
TEST_UTILS = tests/test_utils.c

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
TEST_SRC = tests
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

test_global: $(TEST_SRC)/test_global.c $(TEST_UTILS) $(CORE_SRC) $(ROUTAGE_SRC) $(SECURITY_SRC) | $(TEST_BIN)
	$(CC) $(CFLAGS) -o $(TEST_BIN)/test_global $^ $(LDFLAGS)
	./$(TEST_BIN)/test_global



test_generator: $(TEST_SRC)/test_generator.c $(MOD_GSM_DIR)/generator.c $(CORE_SRC) | $(TEST_BIN)
	$(CC) $(CFLAGS) -o $(TEST_BIN)/test_generator $^ $(LDFLAGS)
	./$(TEST_BIN)/test_generator

test_sim: $(TEST_SRC)/test_sim.c $(MOD_GSM_DIR)/simulator.c $(MOD_GSM_DIR)/utils.c | $(TEST_BIN)
	$(CC) $(CFLAGS) -DTEST_MODE -o $(TEST_BIN)/test_sim $^ $(LDFLAGS)
	./$(TEST_BIN)/test_sim

# New Tests

test_dijsktra: tests/test_dijsktra.c $(TEST_UTILS) $(CORE_SRC) $(ROUTAGE_SRC) | $(TEST_BIN)
	$(CC) $(CFLAGS) -o $(TEST_BIN)/test_dijsktra $^ $(LDFLAGS)
	./$(TEST_BIN)/test_dijsktra

test_backtracking: tests/test_backtracking.c $(TEST_UTILS) $(CORE_SRC) $(ROUTAGE_SRC) | $(TEST_BIN)
	$(CC) $(CFLAGS) -o $(TEST_BIN)/test_backtracking $^ $(LDFLAGS)
	./$(TEST_BIN)/test_backtracking

test_generation_topo: tests/test_generation_topo.c $(TEST_UTILS) $(CORE_SRC) | $(TEST_BIN)
	$(CC) $(CFLAGS) -o $(TEST_BIN)/test_generation_topo $^ $(LDFLAGS)
	./$(TEST_BIN)/test_generation_topo

test_liste: tests/test_liste_chainee.c $(TEST_UTILS) $(CORE_SRC) | $(TEST_BIN)
	$(CC) $(CFLAGS) -o $(TEST_BIN)/test_liste $^ $(LDFLAGS)
	./$(TEST_BIN)/test_liste

test_donnees: tests/test_donnees_reelles.c $(TEST_UTILS) $(CORE_SRC) $(ROUTAGE_SRC) | $(TEST_BIN)
	$(CC) $(CFLAGS) -o $(TEST_BIN)/test_donnees $^ $(LDFLAGS)
	./$(TEST_BIN)/test_donnees

tests: test_graphe test_routage test_securite test_global test_dijsktra test_backtracking test_generation_topo test_liste test_donnees
	@echo "-----------------------------------"
	@echo ">>> TOUS LES TESTS SONT PASSES <<<"
	@echo "-----------------------------------"

clean:
	rm -rf $(BIN_DIR)
	rm -f resultats_tests/*.log resultats_tests/*.txt resultats_tests/*.dot resultats_tests/*.png resultats_tests/*.tmp

.PHONY: all clean directories tests test_graphe test_routage test_securite test_global
