#include "routage.h"
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/**
 * @file routage.c
 * @brief Module 2 : Algorithmes de Routage Optimal
 */

/*=============================================================================
 * UTILITAIRES CHEMINS
 *===========================================================================*/

void chemin_detruire(Chemin *c) {
  if (c) {
    if (c->noeuds)
      free(c->noeuds);
    free(c);
  }
}

void ensemble_chemins_detruire(EnsembleChemins *ec) {
  if (!ec)
    return;
  for (int i = 0; i < ec->nombre; i++) {
    if (ec->chemins[i].noeuds)
      free(ec->chemins[i].noeuds);
  }
  free(ec->chemins);
  free(ec);
}

/*=============================================================================
 * DIJKSTRA (Section 3.2.2.1)
 *===========================================================================*/

static float obtenir_poids(AreteNoeud *a, const char *critere) {
  if (strcmp(critere, "latence") == 0)
    return a->metriques.latence;
  if (strcmp(critere, "securite") == 0)
    return 11.0f - (float)a->metriques.securite;
  if (strcmp(critere, "bande_passante") == 0)
    return 1000000.0f / (a->metriques.bande_passante + 1);
  return a->metriques.cout;
}

Chemin *routage_dijkstra(Graphe *g, int source, int destination,
                         const char *critere) {
  if (!g || source < 0 || source >= g->max_noeuds)
    return NULL;

  int n = g->max_noeuds;
  float *dist = (float *)malloc(n * sizeof(float));
  int *pred = (int *)malloc(n * sizeof(int));
  bool *visite = (bool *)calloc(n, sizeof(bool));

  for (int i = 0; i < n; i++) {
    dist[i] = FLT_MAX;
    pred[i] = -1;
  }
  dist[source] = 0.0f;

  for (int i = 0; i < n; i++) {
    int u = -1;
    float min_d = FLT_MAX;
    for (int v = 0; v < n; v++) {
      if (!visite[v] && dist[v] < min_d) {
        min_d = dist[v];
        u = v;
      }
    }
    if (u == -1 || u == destination)
      break;
    visite[u] = true;

    AreteNoeud *a = g->noeuds[u].liste_adj;
    while (a) {
      int v = a->dest_id;
      if (!visite[v] && g->noeuds[v].est_actif) {
        float p = obtenir_poids(a, critere);
        if (dist[u] + p < dist[v]) {
          dist[v] = dist[u] + p;
          pred[v] = u;
        }
      }
      a = a->suivant;
    }
  }

  Chemin *res = NULL;
  if (destination >= 0 && destination < n && dist[destination] != FLT_MAX) {
    res = (Chemin *)malloc(sizeof(Chemin));
    int curr = destination, len = 0;
    while (curr != -1) {
      len++;
      curr = pred[curr];
    }
    res->longueur = len;
    res->noeuds = (int *)malloc(len * sizeof(int));
    curr = destination;
    for (int i = len - 1; i >= 0; i--) {
      res->noeuds[i] = curr;
      curr = pred[curr];
    }

    // Recalcul des métriques réelles du trajet
    res->cout_total = 0;
    res->latence_totale = 0;
    res->bande_passante_min = FLT_MAX;
    res->securite_min = 10;
    for (int i = 0; i < len - 1; i++) {
      AreteNoeud *a = g->noeuds[res->noeuds[i]].liste_adj;
      while (a && a->dest_id != res->noeuds[i + 1])
        a = a->suivant;
      if (a) {
        res->cout_total += a->metriques.cout;
        res->latence_totale += a->metriques.latence;
        if (a->metriques.bande_passante < res->bande_passante_min)
          res->bande_passante_min = a->metriques.bande_passante;
        if (a->metriques.securite < res->securite_min)
          res->securite_min = a->metriques.securite;
      }
    }
  }
  free(dist);
  free(pred);
  free(visite);
  return res;
}

/*=============================================================================
 * BELLMAN-FORD (Section 3.2.2.1)
 *===========================================================================*/

Chemin *routage_bellman_ford(Graphe *g, int source, int destination) {
  (void)g;
  (void)source;
  (void)destination;
  printf("Bellman-Ford non implemente (Non requis)\n");
  return NULL;
}

/*=============================================================================
 * K-PLUS COURTS CHEMINS (Section 3.2.2.3)
 *===========================================================================*/

// Helper pour verifier unicite
static bool chemin_existe_deja(EnsembleChemins *ec, Chemin *c) {
  for (int i = 0; i < ec->nombre; i++) {
    if (ec->chemins[i].longueur != c->longueur)
      continue;
    bool t = true;
    for (int j = 0; j < c->longueur; j++) {
      if (ec->chemins[i].noeuds[j] != c->noeuds[j]) {
        t = false;
        break;
      }
    }
    if (t && fabs(ec->chemins[i].cout_total - c->cout_total) < 0.001)
      return true;
  }
  return false;
}

EnsembleChemins *routage_k_chemins_plus_courts(Graphe *g, int source,
                                               int destination, int k,
                                               const char *critere) {
  if (!g || k <= 0)
    return NULL;

  EnsembleChemins *ec = (EnsembleChemins *)malloc(sizeof(EnsembleChemins));
  // Allocation max large, realloc plus tard ou statique
  ec->chemins = (Chemin *)malloc(k * sizeof(Chemin));
  ec->nombre = 0;

  // 1. Premier plus court chemin (Dijkstra standard)
  Chemin *start = routage_dijkstra(g, source, destination, critere);
  if (!start)
    return ec;

  ec->chemins[ec->nombre++] =
      *start; // Copie structure shallow, mais chemin_detruire devra faire gaffe
              // ou on fait deep copy
  // Dijkstra retourne un malloc. ec stocke la struct Chemin, pas un pointeur.
  // start->noeuds est alloué. On transfère la propriété à ec->chemins[0].
  free(start);

  // Algorithme itératif simple (Approche suppression d'arcs)
  // Pour K-1 itérations : cherche une alternative en supprimant temporairement
  // chaque arc du chemin precedent Note: C'est une simplification tre brutale
  // de Yen. Yen utilise "Deviations". Ici on va essayer de trouver K chemins
  // differents en penalisant les arcs deja utilisés ? Ou juste simple
  // suppression.

  // Pour simplifier et ne pas complexifier le code avec des heaps de candidats
  // (Yen complet), on fait une boucle qui essaie de trouver un chemin
  // différent. Approche naive : Pour chaque chemin trouvé, on essaie de
  // l'éviter. Mais Dijkstra déterministe donnera toujours le même. Il faut
  // modifier le graphe temporairement.

  // On va implémenter un "Yen simplifié" :
  // On prend le dernier chemin trouvé.
  // Pour chaque arête de ce chemin :
  //   On la desactive.
  //   On lance Dijkstra.
  //   On stocke le résultat s'il est valide et nouveau.
  //   On réactive.
  // On prend le meilleur des candidats trouvés.

  while (ec->nombre < k) {
    Chemin *candidat_choisi = NULL;

    // Chercher des candidats à partir des chemins DEJA trouvés
    // (En théorie Yen ne regarde que les déviations du dermier, mais
    // simplifions en regardant suppression d'arêtes sur le dernier chemin
    // ajouté)

    Chemin *dernier = &ec->chemins[ec->nombre - 1];

    for (int i = 0; i < dernier->longueur - 1; i++) {
      int u = dernier->noeuds[i];
      int v = dernier->noeuds[i + 1];

      // Trouver l'arete et la desactiver
      AreteNoeud *a = g->noeuds[u].liste_adj;
      float save_cout = 0;
      while (a) {
        if (a->dest_id == v) {
          // Hack: on change le cout temporairement à FLT_MAX pour "désactiver"
          // sans structure complexe Attention: Dijkstra utilise
          // "obtenir_poids". Si on modifie a->metriques.cout, cela affecte tout
          // le monde. Comme c'est monothread, OK.
          save_cout = a->metriques.cout;
          // On veut le rendre inutilisable. On met un cout enorme.
          // Mais si critere est BP, on met 0 ? Si latence, MAX.
          // On va utiliser un flag "actif" sur l'arete si possible, mais struct
          // AreteNoeud n'a pas de flag actif. On suppose que le graphe a des
          // couts positifs. On met cout = 1e9.
          a->metriques.cout = 1e9;
          // Si critere autre que cout, il faudrait aussi modifier l'autre
          // metrique. Simplification: on affecte TOUTES les metriques
          // penalisantes.
          float save_lat = a->metriques.latence;
          a->metriques.latence = 1e9;
          float save_bp = a->metriques.bande_passante;
          a->metriques.bande_passante =
              0.001; // BP quasi nulle -> Poids enorme (1M/BP)

          // Lance Dijkstra
          Chemin *alt = routage_dijkstra(g, source, destination, critere);

          // Rétablir
          a->metriques.cout = save_cout;
          a->metriques.latence = save_lat;
          a->metriques.bande_passante = save_bp;

          if (alt) {
            if (!chemin_existe_deja(ec, alt)) {
              if (candidat_choisi == NULL ||
                  alt->cout_total < candidat_choisi->cout_total) {
                if (candidat_choisi)
                  chemin_detruire(candidat_choisi);
                candidat_choisi = alt;
              } else {
                chemin_detruire(alt);
              }
            } else {
              chemin_detruire(alt);
            }
          }
          break; // Arc trouvé et traité
        }
        a = a->suivant;
      }
    }

    if (candidat_choisi) {
      ec->chemins[ec->nombre++] = *candidat_choisi;
      free(candidat_choisi); // Libère la struct, garde les arrays (copie
                             // shallow struct dans array)
    } else {
      break; // Plus de chemin alternatif trouvé par cette méthode simple
    }
  }

  return ec;
}
