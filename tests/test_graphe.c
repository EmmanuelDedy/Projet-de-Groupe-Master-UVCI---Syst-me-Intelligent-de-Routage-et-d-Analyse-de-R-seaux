/**
 * @file test_graphe.c
 * @brief Tests unitaires du module graphe
 *
 * Ce fichier contient des tests unitaires permettant de valider
 * les fonctionnalités principales du module graphe :
 *  - création du graphe
 *  - ajout et suppression de nœuds
 *  - ajout et suppression d’arêtes
 */
#include <assert.h>
#include <stdio.h>
#include "graphe.h"

int main() {
    /* Définition de métriques de test */
    Metriques m = {1.0, 10.0, 100.0, 9.5, 5};

    /* Test de création du graphe */
    Graphe *g = graphe_creer(5, false);
    assert(g != NULL);

    /* Test d’ajout de nœuds */
    int n1 = graphe_ajouter_noeud(g, ROUTEUR, 0.0, 0.0);
    int n2 = graphe_ajouter_noeud(g, SERVEUR, 1.0, 1.0);
    assert(n1 >= 0 && n2 >= 0);

    /* Test d’ajout et suppression d’arête */
    assert(graphe_ajouter_arete(g, n1, n2, m) == true);
    assert(graphe_supprimer_arete(g, n1, n2) == true);

    /* Test de suppression de nœud */
    assert(graphe_supprimer_noeud(g, n1) == true);

    /* Libération de la mémoire */
    graphe_detruire(g);

    printf("Tests unitaires graphe : OK\n");
    return 0;
}
