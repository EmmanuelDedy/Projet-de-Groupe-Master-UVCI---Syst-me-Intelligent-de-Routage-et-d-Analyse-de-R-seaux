#ifndef SECURITE_H
#define SECURITE_H
#include "graphe.h" 

/*  MODULE 3 : DÉTECTION D'ANOMALIES ET ANALYSE DE SÉCURITÉ */

int detecter_cycles(Graphe* g);
void identifier_points_critiques(Graphe* g);
void analyser_zones_securite(Graphe* g);

#endif
