/**
 * @file test_securite.c
 * @brief Tests unitaires du module sécurité
 *
 * Tests :
 *  - détection de cycles
 *  - identification de points critiques
 *  - analyse des zones de sécurité (SCC)
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../src/graphe.h"
#include "../src/securite.h"

int main() {

    /* Création des métriques */
    Metriques m = {1.0, 10.0, 100.0, 9.5, 5};

    /* Création d’un graphe orienté */
    Graphe *g = graphe_creer(5, true);
    assert(g != NULL);

    /* Ajout de nœuds */
    int n1 = graphe_ajouter_noeud(g, ROUTEUR, 0.0, 0.0);
    int n2 = graphe_ajouter_noeud(g, SERVEUR, 1.0, 1.0);
    int n3 = graphe_ajouter_noeud(g, ROUTEUR, 2.0, 2.0);

    assert(n1 >= 0 && n2 >= 0 && n3 >= 0);

    /* Ajout d’arêtes (sans cycle) */
    graphe_ajouter_arete(g, n1, n2, m);
    graphe_ajouter_arete(g, n2, n3, m);

    /* Test détection de cycle */
    assert(detecter_cycles(g) == 0);

    /* Création d’un cycle */
    graphe_ajouter_arete(g, n3, n1, m);
    assert(detecter_cycles(g) == 1);

    /* Analyse résilience (ne renvoie rien, juste vérification d’exécution) */
    identifier_points_critiques(g);

    /* Analyse SCC */
    analyser_zones_securite(g);

    /* Nettoyage */
    graphe_detruire(g);

    printf("Tests unitaires sécurité : OK\n");
    return 0;
}
