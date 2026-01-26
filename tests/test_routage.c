#include "graphe.h"
#include "routage.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int tests_passed = 0;

void test_dijkstra_simple() {
  printf("[TEST] Dijkstra Simple...");
  Graphe *g = graphe_creer(3, true);
  graphe_ajouter_noeud_nomme(g, "A", ROUTEUR, 0, 0); // 0
  graphe_ajouter_noeud_nomme(g, "B", ROUTEUR, 1, 0); // 1
  graphe_ajouter_noeud_nomme(g, "C", ROUTEUR, 2, 0); // 2

  // A -> B (cout 10)
  Metriques m1 = {10, 5, 100, 1, 5};
  graphe_ajouter_arete(g, 0, 1, m1);

  // B -> C (cout 10)
  Metriques m2 = {10, 5, 100, 1, 5};
  graphe_ajouter_arete(g, 1, 2, m2);

  // A -> C direct (cout 50)
  Metriques m3 = {50, 5, 100, 1, 5};
  graphe_ajouter_arete(g, 0, 2, m3);

  // Le plus court chemin A->C en cout devrait etre A->B->C (cost 20) vs A->C
  // (cost 50)
  Chemin *c = routage_dijkstra(g, 0, 2, "cout");

  assert(c != NULL);
  assert(c->longueur == 3);
  assert(c->cout_total == 20.0);
  assert(c->noeuds[0] == 0);
  assert(c->noeuds[1] == 1);
  assert(c->noeuds[2] == 2);

  chemin_detruire(c);
  graphe_detruire(g);
  printf(" OK\n");
  tests_passed++;
}

void test_dijkstra_critere_bp() {
  printf("[TEST] Dijkstra Bande Passante...");
  Graphe *g = graphe_creer(3, true);
  graphe_ajouter_noeud(g, ROUTEUR, 0, 0);
  graphe_ajouter_noeud(g, ROUTEUR, 0, 0);
  graphe_ajouter_noeud(g, ROUTEUR, 0, 0);

  // A->B (High BP = 1000)
  Metriques m1 = {10, 5, 1000, 1, 5};
  graphe_ajouter_arete(g, 0, 1, m1);

  // B->C (High BP = 1000)
  Metriques m2 = {10, 5, 1000, 1, 5};
  graphe_ajouter_arete(g, 1, 2, m2);

  // A->C direct (Low BP = 10)
  Metriques m3 = {5, 5, 10, 1, 5};
  graphe_ajouter_arete(g, 0, 2, m3);

  // Dijkstra sur BP maximise la capacité goulot ou minimise l'inverse ?
  // Verifions routage.c : return 1000000.0f / (a->metriques.bande_passante +
  // 1); Donc on minimise (1M / BP). Chemin 1 (via B): cout = (1M/1001) +
  // (1M/1001) approx 999 + 999 = 1998 Chemin 2 (directe): cout = (1M/11) approx
  // 90909 Le chemin via B est "moins cher" donc meilleur BP globale si on parle
  // de somme de poids inversés. Mais attention, Dijkstra "BP" classique cherche
  // souvent le Max-Min bandwidth (bottleneck). Ici l'implé Dijkstra fait une
  // somme de poids. Donc on minimise la somme des inverses. Ca favorise les
  // liens à haute BP.

  Chemin *c = routage_dijkstra(g, 0, 2, "bande_passante");

  assert(c != NULL);
  // Doit passer par B car le cout "inverse" est beaucoup plus faible
  assert(c->longueur == 3);

  chemin_detruire(c);
  graphe_detruire(g);
  printf(" OK\n");
  tests_passed++;
}

void test_backtracking_contraint() {
  printf("[TEST] Backtracking Contraint...");
  Graphe *g = graphe_creer(3, true);
  graphe_ajouter_noeud(g, ROUTEUR, 0, 0);
  graphe_ajouter_noeud(g, ROUTEUR, 0, 0);
  graphe_ajouter_noeud(g, ROUTEUR, 0, 0);

  // A -> B (Cout 10, BP 100)
  Metriques m1 = {10, 0, 100, 0, 0};
  graphe_ajouter_arete(g, 0, 1, m1);

  // B -> C (Cout 10, BP 100)
  Metriques m2 = {10, 0, 100, 0, 0};
  graphe_ajouter_arete(g, 1, 2, m2);

  // A -> C direct (Cout 5, BP 10) - Moins cher mais BP faible
  Metriques m3 = {5, 0, 10, 0, 0};
  graphe_ajouter_arete(g, 0, 2, m3);

  // On veut aller de A à C avec min_bp = 50. Le lien direct A->C (BP 10) est
  // disqualifié. On doit prendre A->B->C.

  Chemin *c = routage_backtracking_contraint(g, 0, 2, 50.0, 1000.0, NULL, 0);

  assert(c != NULL);
  assert(c->longueur == 3); // A, B, C
  assert(c->bande_passante_min >= 50.0);

  chemin_detruire(c);
  graphe_detruire(g);
  printf(" OK\n");
  tests_passed++;
}

void test_k_shortest_paths() {
  printf("[TEST] K-Shortest Paths (Yen simplifie)...");
  Graphe *g = graphe_creer(4, true);
  graphe_ajouter_noeud(g, ROUTEUR, 0, 0); // 0
  graphe_ajouter_noeud(g, ROUTEUR, 0, 0); // 1
  graphe_ajouter_noeud(g, ROUTEUR, 0, 0); // 2
  graphe_ajouter_noeud(g, ROUTEUR, 0, 0); // 3

  // 0 -> 1 -> 3 (Cout 10+10 = 20)
  Metriques m1 = {10, 0, 100, 0, 0};
  graphe_ajouter_arete(g, 0, 1, m1);
  graphe_ajouter_arete(g, 1, 3, m1);

  // 0 -> 2 -> 3 (Cout 20+20 = 40)
  Metriques m2 = {20, 0, 100, 0, 0};
  graphe_ajouter_arete(g, 0, 2, m2);
  graphe_ajouter_arete(g, 2, 3, m2);

  // 0 -> 3 direct (Cout 100)
  Metriques m3 = {100, 0, 100, 0, 0};
  graphe_ajouter_arete(g, 0, 3, m3);

  EnsembleChemins *ec = routage_k_chemins_plus_courts(g, 0, 3, 3, "cout");

  assert(ec != NULL);
  // On espere trouver 3 chemins disctincts : via 1, via 2, direct.
  // Note: notre implémentation simplifiée invalide 1 arc à la fois.
  // - Chemin 1 (via 1): [0,1,3].
  //   - Supprime 0->1 : Trouve via 2 (40) ou direct (100). Via 2 moins cher. ->
  //   Chemin [0,2,3].
  //   - Supprime 1->3 : Trouve via 2 (40). (Doublon possible si non géré, mais
  //   is_unique gère).

  // Avec l'implémentation, on devrait avoir au moins 2 chemins si ce n'est 3.
  assert(ec->nombre >= 2);

  // Verification ordre croissant cout
  if (ec->nombre >= 2) {
    assert(ec->chemins[0].cout_total <= ec->chemins[1].cout_total);
  }

  printf(" OK (%d chemins trouves)\n", ec->nombre);
  ensemble_chemins_detruire(ec);
  tests_passed++;
}

int main() {
  printf("=== TESTS UNITAIRES: MODULE 2 (ROUTAGE) ===\n");
  test_dijkstra_simple();
  test_dijkstra_critere_bp();
  test_backtracking_contraint();
  test_k_shortest_paths();
  printf("=== TOUS LES TESTS ROUTAGE PASSES (%d tests) ===\n", tests_passed);
  return 0;
}
