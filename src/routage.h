#ifndef ROUTING_H
#define ROUTING_H

#include "graphe.h"

/**
 * @file routing.h
 * @brief Module 2 : Algorithmes de Routage Optimal
 */

/**
 * @struct Chemin
 * @brief Résultat d'une recherche d'itinéraire.
 */
typedef struct {
  int *noeuds;
  int longueur;
  float cout_total;
  float latence_totale;
  float bande_passante_min;
  int securite_min;
} Chemin;

/**
 * @struct EnsembleChemins
 * @brief Liste de chemins alternatifs (K-Plus Courts).
 */
typedef struct {
  Chemin *chemins;
  int nombre;
} EnsembleChemins;

/*--- Déstockage mémoire ---*/
void chemin_detruire(Chemin *c);
void ensemble_chemins_detruire(EnsembleChemins *ec);

/*--- Algorithmes (Prototypes snake_case) ---*/

/**
 * @brief Algorithme de Dijkstra (Section 3.2.2.1)
 */
Chemin *routage_dijkstra(Graphe *g, int source, int destination,
                         const char *critere);

/**
 * @brief Algorithme de Bellman-Ford (Section 3.2.2.1)
 */
Chemin *routage_bellman_ford(Graphe *g, int source, int destination);

/**
 * @brief Recherche par Backtracking (Section 3.2.2.2)
 */
Chemin *routage_backtracking_contraint(Graphe *g, int noeud_dep, int noeud_arr,
                                       float min_bp, float max_cout,
                                       int *passage_obligatoire,
                                       int nb_obligatoire);

/**
 * @brief K-Plus Courts Chemins (Section 3.2.2.3)
 */
EnsembleChemins *routage_k_chemins_plus_courts(Graphe *g, int source,
                                               int destination, int k,
                                               const char *critere);

#endif /* ROUTING_H */
