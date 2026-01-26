#ifndef SECURITE_H
#define SECURITE_H
#include "graphe.h"

/**
 * @file securite.h
 * @brief Module 3 : Détection d'Anomalies et Audit de Résilience
 * @author Équipe Master 1 UVCI
 *
 * Ce module scanne la topologie à la recherche de faiblesses structurelles.
 * Il implémente des algorithmes de parcours (DFS) pour identifier :
 * 1. Les boucles de routage (Cycles).
 * 2. Les points de défaillance uniques (SPOF / Points d'articulation).
 * 3. Les zones isolées (Composantes Fortement Connexes).
 */

/**
 * @brief Détecte la présence de cycles dans le graphe.
 * Utilise un parcours en profondeur (DFS) avec marquage des nœuds "en cours de
 * visite".
 * @return 1 si cycle détecté, 0 sinon.
 */
int detecter_cycles(Graphe *g);

/**
 * @brief Identifie les Points d'Articulation (SPOF).
 *
 * Un nœud est un SPOF si sa suppression augmente le nombre de composantes
 * connexes du graphe. L'algorithme simule la suppression de chaque nœud et
 * vérifie la connectivité.
 *
 * @param g Le graphe à auditer.
 */
void identifier_points_critiques(Graphe *g);

/**
 * @brief Analyse les Zones de Confiance (SCC - Strongly Connected Components).
 *
 * Utilise l'algorithme de **Tarjan** ou **Kosaraju**.
 * Chaque SCC représente une zone où tout nœud peut atteindre tous les autres.
 * Idéal pour segmenter le réseau en sous-zones résilientes.
 */
void analyser_zones_securite(Graphe *g);

/**
 * @brief Rapport complet de vulnérabilité.
 * Exécute tous les tests ci-dessus et affiche un diagnostic console.
 */
void securite_analyser_vulnerabilites(Graphe *g);

#endif