#include "../src/routage.h"
#include "test_utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void test_chemin_simple() {
  Graphe *g = creer_graphe_test(TOPO_SIMPLE, 5);
  // 0-1-2-3-4. Cout 10 par arete.

  Chemin *c = routage_dijkstra(g, 0, 4, "cout");

  if (c && c->longueur == 5 && c->cout_total == 40.0f) {
    log_succes("Dijkstra Chemin Simple", "Chemin correct et coût correct (40)");
  } else {
    char buffer[100];
    sprintf(buffer,
            "Echec : Longueur attendue 5, reçue %d. Coût attendu 40, reçu %.2f",
            c ? c->longueur : -1, c ? c->cout_total : -1.0f);
    log_echec("Dijkstra Chemin Simple", buffer);
  }

  chemin_detruire(c);
  graphe_detruire(g);
}

void test_maillage_court() {
  Graphe *g = creer_graphe_test(TOPO_MAILLAGE, 5);
  // Mesh 5: tout connecté à tout. Coût direct 10.
  // 0->4 direct.

  Chemin *c = routage_dijkstra(g, 0, 4, "cout");

  // Le chemin direct 0->4 coûte 10.
  // Via un autre noeud (0->1->4) coûterait 20.

  if (c && c->longueur == 2 && c->cout_total == 10.0f) {
    log_succes("Dijkstra Maillage Direct", "A bien trouvé le lien direct");
  } else {
    log_echec("Dijkstra Maillage Direct", "N'a pas pris le lien direct");
  }

  chemin_detruire(c);
  graphe_detruire(g);
}

void test_direction_anneau() {
  Graphe *g = creer_graphe_test(TOPO_ANNEAU, 10);
  // 0-1-2-...-9-0
  // 0 -> 2. Chemin 0-1-2 (cout 30).
  // 0 -> 9. Chemin 0-9 (cout 15).

  Chemin *c1 = routage_dijkstra(g, 0, 2, "cout");
  Chemin *c2 = routage_dijkstra(g, 0, 9, "cout");

  int ok = 1;
  if (!c1 || c1->cout_total != 30.0f)
    ok = 0;
  if (!c2 || c2->cout_total != 15.0f)
    ok = 0;

  if (ok)
    log_succes("Dijkstra Logique Anneau", "Chemins corrects dans l'anneau");
  else
    log_echec("Dijkstra Logique Anneau", "Erreur de routage anneau");

  chemin_detruire(c1);
  chemin_detruire(c2);
  graphe_detruire(g);
}

int main() {
  init_log_test();
  log_section("TEST DIJKSTRA",
              "Validation de l'algorithme de plus court chemin");

  test_chemin_simple();
  test_maillage_court();
  test_direction_anneau();

  return 0;
}
