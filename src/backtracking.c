#include "backtracking.h"
#include <float.h>
#include <string.h>

/**
 * @file backtracking.c
 * @brief Exploration exhaustive sous contraintes (Section 3.2.2.2)
 */

typedef struct {
  Graphe *g;
  int target;
  float min_bp;
  float max_cout;
  int *passage_obligatoire;
  int nb_obligatoire;
  bool *visite;
  int *chemin_courant;
  int indice;
  Chemin *meilleur;
} Context;

static bool contient(int val, int *tab, int nb) {
  for (int i = 0; i < nb; i++)
    if (tab[i] == val)
      return true;
  return false;
}

static void explorer(Context *ctx, int u, float cout_c, float lat_c, float bp_m,
                     int sec_m) {
  if (cout_c > ctx->max_cout)
    return;
  if (ctx->meilleur && cout_c >= ctx->meilleur->cout_total)
    return;

  ctx->visite[u] = true;
  ctx->chemin_courant[ctx->indice++] = u;

  if (u == ctx->target) {
    bool ok = true;
    for (int i = 0; i < ctx->nb_obligatoire; i++) {
      if (!contient(ctx->passage_obligatoire[i], ctx->chemin_courant,
                    ctx->indice)) {
        ok = false;
        break;
      }
    }
    if (ok) {
      if (ctx->meilleur)
        chemin_detruire(ctx->meilleur);
      ctx->meilleur = (Chemin *)malloc(sizeof(Chemin));
      ctx->meilleur->longueur = ctx->indice;
      ctx->meilleur->noeuds = (int *)malloc(ctx->indice * sizeof(int));
      memcpy(ctx->meilleur->noeuds, ctx->chemin_courant,
             ctx->indice * sizeof(int));
      ctx->meilleur->cout_total = cout_c;
      ctx->meilleur->latence_totale = lat_c;
      ctx->meilleur->bande_passante_min = bp_m;
      ctx->meilleur->securite_min = sec_m;
    }
  }

  AreteNoeud *a = ctx->g->noeuds[u].liste_adj;
  while (a) {
    int v = a->dest_id;
    if (!ctx->visite[v] && ctx->g->noeuds[v].est_actif &&
        a->metriques.bande_passante >= ctx->min_bp) {
      float n_bp = (bp_m < a->metriques.bande_passante)
                       ? bp_m
                       : a->metriques.bande_passante;
      int n_sec =
          (sec_m < a->metriques.securite) ? sec_m : a->metriques.securite;
      explorer(ctx, v, cout_c + a->metriques.cout, lat_c + a->metriques.latence,
               n_bp, n_sec);
    }
    a = a->suivant;
  }

  ctx->indice--;
  ctx->visite[u] = false;
}

Chemin *routage_backtracking_contraint(Graphe *g, int dep, int arr,
                                       float min_bp, float max_c, int *req,
                                       int nb_r) {
  if (!g || dep < 0 || dep >= g->max_noeuds)
    return NULL;
  Context ctx = {g,   arr,  min_bp, (max_c > 0) ? max_c : FLT_MAX,
                 req, nb_r, NULL,   NULL,
                 0,   NULL};
  ctx.visite = (bool *)calloc(g->max_noeuds, sizeof(bool));
  ctx.chemin_courant = (int *)malloc(g->max_noeuds * sizeof(int));
  explorer(&ctx, dep, 0, 0, FLT_MAX, 10);
  free(ctx.visite);
  free(ctx.chemin_courant);
  return ctx.meilleur;
}
