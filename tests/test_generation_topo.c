#include "test_utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void test_topo_simple() {
  Graphe *g = creer_graphe_test(TOPO_SIMPLE, 50);
  int ok = 1;
  if (!g)
    ok = 0;
  if (g->nb_noeuds != 50)
    ok = 0;

  if (g->nb_noeuds != 50)
    ok = 0;

  if (ok)
    log_succes("GenTopo Simple", "Création de 50 noeuds OK");
  else
    log_echec("GenTopo Simple", "Echec création");

  if (g)
    graphe_detruire(g);
}

void test_topo_arbre() {
  int n = 15;
  Graphe *g = creer_graphe_test(TOPO_ARBRE, n);
  int ok = 1;
  if (g->nb_noeuds != n)
    ok = 0;

  // Verif racine a des enfants
  AreteNoeud *a = g->noeuds[0].liste_adj;
  int enfants = 0;
  while (a) {
    enfants++;
    a = a->suivant;
  }

  if (enfants < 1)
    ok = 0;

  if (ok)
    log_succes("GenTopo Arbre", "Arbre créé avec racine connectée");
  else
    log_echec("GenTopo Arbre", "Structure Arbre suspecte");

  if (g)
    graphe_detruire(g);
}

void test_topo_grille() {
  int n = 16;
  Graphe *g = creer_graphe_test(TOPO_GRILLE, n);
  // 4x4 Grille.

  int ok = 1;
  if (g->nb_noeuds != n)
    ok = 0;

  AreteNoeud *a0 = g->noeuds[0].liste_adj;
  int d0 = 0;
  while (a0) {
    d0++;
    a0 = a0->suivant;
  }

  if (d0 != 2) {
    char buff[100];
    sprintf(buff, "Degré coin %d attendu 2", d0);
    log_echec("GenTopo Grille", buff);
  } else {
    log_succes("GenTopo Grille", "Degré coin correct");
  }

  if (g)
    graphe_detruire(g);
}

int main() {
  init_log_test();
  log_section("TEST GENERATION TOPOLOGIE",
              "Vérification de la création de graphes");
  test_topo_simple();
  test_topo_arbre();
  test_topo_grille();
  return 0;
}
