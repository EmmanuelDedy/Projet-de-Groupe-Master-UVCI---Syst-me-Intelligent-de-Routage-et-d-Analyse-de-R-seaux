#ifndef BACKTRACKING_H
#define BACKTRACKING_H

#include "routage.h"

/**
 * @file backtracking.h
 * @brief Exploration exhaustive avec élagage.
 */

Chemin *routage_backtracking_contraint(Graphe *g, int noeud_dep, int noeud_arr,
                                       float min_bp, float max_cout,
                                       int *passage_obligatoire,
                                       int nb_obligatoire);

#endif /* BACKTRACKING_H */
