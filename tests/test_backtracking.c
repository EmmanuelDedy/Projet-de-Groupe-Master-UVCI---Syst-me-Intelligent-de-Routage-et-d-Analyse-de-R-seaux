#include "../src/routage.h"
#include "test_utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void test_backtracking_contrainte_cout() {
  Graphe *g = creer_graphe_test(TOPO_SIMPLE, 5);
  // 0-1-2-3-4. Total cost 40.

  // Test avec max_cout suffisant
  Chemin *c1 = routage_backtracking_contraint(g, 0, 4, 0, 50.0f, NULL, 0);

  // Test avec max_cout insuffisant
  Chemin *c2 = routage_backtracking_contraint(g, 0, 4, 0, 30.0f, NULL, 0);

  int ok = 1;
  if (!c1)
    ok = 0;
  if (c2)
    ok = 0; // Ne devrait pas trouver de chemin

  if (ok)
    log_succes("Backtracking Contrainte Coût",
               "Respecte la contrainte de coût");
  else
    log_echec("Backtracking Contrainte Coût", "Echec contrainte coût");

  if (c1)
    chemin_detruire(c1);
  if (c2)
    chemin_detruire(c2);
  graphe_detruire(g);
}

void test_backtracking_noeud_obligatoire() {
  Graphe *g = creer_graphe_test(TOPO_MAILLAGE, 5);
  // Mesh: 0 peut aller à 4 directement.
  // On force le passage par 2.

  int passage[] = {2};
  Chemin *c = routage_backtracking_contraint(g, 0, 4, 0, 100.0f, passage, 1);

  int passe = 0;
  if (c) {
    for (int i = 0; i < c->longueur; i++) {
      if (c->noeuds[i] == 2)
        passe = 1;
    }
  }

  if (passe)
    log_succes("Backtracking Noeud Obligatoire",
               "A visité le noeud obligatoire 2");
  else
    log_echec("Backtracking Noeud Obligatoire", "N'a pas visité 2");

  if (c)
    chemin_detruire(c);
  graphe_detruire(g);
}

void test_backtracking_min_bande_passante() {
  Graphe *g = graphe_creer(3, false);
  graphe_ajouter_noeud(g, ROUTEUR, 0, 0);
  graphe_ajouter_noeud(g, ROUTEUR, 10, 0);
  graphe_ajouter_noeud(g, ROUTEUR, 20, 0);

  // 0-1 : BP 100
  // 1-2 : BP 50
  // 0-2 : BP 10 (direct mais lent)
  ajouter_arete_bidirectionnelle(g, 0, 1, 10, 1, 100, 5);
  ajouter_arete_bidirectionnelle(g, 1, 2, 10, 1, 50, 5);
  ajouter_arete_bidirectionnelle(g, 0, 2, 50, 1, 10, 5);

  // On veut aller de 0 à 2 avec min BP 40.
  // Le chemin direct 0-2 a BP 10 -> invalide.
  // Le chemin 0-1-2 a BP min 50 -> valide.

  Chemin *c = routage_backtracking_contraint(g, 0, 2, 40.0f, 100.0f, NULL, 0);

  int ok = 0;
  if (c) {
    if (c->longueur == 3 && c->noeuds[1] == 1)
      ok = 1; // Passe par 1
  }

  if (ok)
    log_succes("Backtracking Min Bande Passante",
               "A choisi le chemin à haute BP");
  else
    log_echec("Backtracking Min Bande Passante",
              "A pris le chemin à basse BP ou rien");

  if (c)
    chemin_detruire(c);
  graphe_detruire(g);
}

int main() {
  init_log_test();
  log_section("TEST BACKTRACKING", "Validation du routage avec contraintes");

  test_backtracking_contrainte_cout();
  test_backtracking_noeud_obligatoire();
  test_backtracking_min_bande_passante();

  return 0;
}
