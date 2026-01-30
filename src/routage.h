#ifndef ROUTING_H
#define ROUTING_H

#include "graphe.h"

/**
 * @file routage.h
 * @brief Module 2 : Algorithmes de Routage Optimal & Intelligent
 * @author Équipe Master 1 UVCI
 *
 * Ce module implémente la logique de décision du plan de contrôle.
 * Il fournit des solutions pour :
 * - Le plus court chemin (Dijkstra)
 * - Le routage sous contraintes (Backtracking CSP)
 * - La redondance (K-Shortest Paths)
 *
 * @section complexity_sec Complexité
 * - Dijkstra : \f$ O(E \log V) \f$
 * - Backtracking : \f$ O(b^d) \f$ (Exponentielle)
 * - Yen (KSP) : \f$ O(K \cdot N \cdot (E + N \log N)) \f$
 */

/**
 * @struct Chemin
 * @brief Représente un itinéraire complet calculé.
 * Contient la suite de nœuds et les métriques agrégées.
 */
typedef struct {
  int *noeuds;  /**< Tableau ordonné des IDs de nœuds (Source -> ... -> Dest) */
  int longueur; /**< Nombre de nœuds dans le chemin */
  float cout_total;         /**< Somme des coûts des arcs */
  float latence_totale;     /**< Somme des latences (ms) */
  float bande_passante_min; /**< Goulot d'étranglement (Bottleneck) */
  int securite_min; /**< Niveau de sécurité le plus faible sur le trajet */
} Chemin;

/**
 * @struct EnsembleChemins
 * @brief Conteneur pour les résultats de recherche multiple (ex: K-chemins).
 */
typedef struct {
  Chemin *chemins; /**< Tableau dynamique de chemins */
  int nombre;      /**< Nombre de chemins trouvés */
} EnsembleChemins;

/*--- Gestion Mémoire ---*/
void chemin_detruire(Chemin *c);
void ensemble_chemins_detruire(EnsembleChemins *ec);

/*--- Algorithmes ---*/

/**
 * @brief Algorithme de Dijkstra (Routage Optimal).
 *
 * Trouve le chemin minimisant le critère donné (cout ou latence).
 * Utilise une stratégie gloutonne par relaxation.
 *
 * **Pseudo-code :**
 * @code
 *  Pour v dans V: dist[v] = INF, pred[v] = NULL
 *  dist[source] = 0
 *  Q = PriorityQueue(V)
 *  Tant que Q non vide:
 *      u = ExtractMin(Q)
 *      Pour chaque voisin v de u:
 *          alt = dist[u] + poids(u,v)
 *          Si alt < dist[v]:
 *              dist[v] = alt
 *              pred[v] = u
 *              Update(Q, v, alt)
 * @endcode
 *
 * @param g Le graphe
 * @param source ID nœud départ
 * @param destination ID nœud arrivée
 * @param critere "cout" ou "latence"
 * @return Chemin* Structure allouée ou NULL si inaccessible.
 */
Chemin *routage_dijkstra(Graphe *g, int source, int destination,
                         const char *critere);

/**
 * @brief Algorithme de Bellman-Ford.
 * Supporte les poids négatifs (non utilisé par défaut).
 */
Chemin *routage_bellman_ford(Graphe *g, int source, int destination);

/**
 * @brief Recherche par Backtracking avec Élagage (Pruning).
 *
 * Explore l'arbre des possibilités pour trouver un chemin satisfaisant des
 * contraintes multiples (Multi-Constrained Path Problem).
 *
 * **Contraintes traitées :**
 * - Bande passante >= min_bp
 * - Coût total <= max_cout
 * - Passage obligatoire par une liste de nœuds (Waypoints)
 *
 * @return Chemin* Le meilleur chemin valide trouvé.
 */
Chemin *routage_backtracking_contraint(Graphe *g, int noeud_dep, int noeud_arr,
                                       float min_bp, float max_cout,
                                       int *passage_obligatoire,
                                       int nb_obligatoire);

/**
 * @brief Algorithme de Yen (K-Plus Courts Chemins).
 *
 * Génère K chemins alternatifs pour la redondance (Failover).
 * Utilise Dijkstra de manière itérative en "supprimant" virtuellement des
 * liens.
 *
 * @param k Nombre de chemins demandés.
 */
EnsembleChemins *routage_k_chemins_plus_courts(Graphe *g, int source,
                                               int destination, int k,
                                               const char *critere);

#endif /* ROUTING_H */
