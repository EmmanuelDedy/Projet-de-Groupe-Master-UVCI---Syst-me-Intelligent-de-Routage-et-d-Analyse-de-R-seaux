#include <stdio.h>
#include <stdlib.h>
#include "securite.h"

/* Macro utilitaire pour les calculs de Tarjan */
#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif

//  GESTION DYNAMIQUE DE LA PILE POUR LES SCC 
static int *pile_scc = NULL;
static int sommet_pile = -1;
static int *present_dans_pile = NULL;

void push_scc(int v) {
    pile_scc[++sommet_pile] = v;
    present_dans_pile[v] = 1;
}

int pop_scc() {
    int v = pile_scc[sommet_pile--];
    present_dans_pile[v] = 0;
    return v;
}


// 3.1 DÉTECTION DE CYCLES (DFS TRICOLORE)

int dfs_cycle(int v, int visite[], int pile_rec[], Graphe* g) {
    visite[v] = 1; // Nœud en cours de visite (Gris)
    pile_rec[v] = 1;

    // Utilisation de AreteNoeud et liste_adj selon les nouvelles exigences
    AreteNoeud* courant = g->noeuds[v].liste_adj; //
    while (courant != NULL) {
        int voisin = courant->dest_id; //

        // Vérification de robustesse : ignorer les nœuds inactifs
        if (g->noeuds[voisin].est_actif) {
            if (!visite[voisin]) {
                if (dfs_cycle(voisin, visite, pile_rec, g)) return 1;
            } else if (pile_rec[voisin]) {
                return 1; // Cycle détecté
            }
        }
        courant = courant->suivant;
    }
    pile_rec[v] = 0;
    return 0;
}

int detecter_cycles(Graphe* g) {
    if (!g || g->nb_noeuds <= 0) return 0; // Sécurité mémoire

    int* visite = (int*)calloc(g->nb_noeuds, sizeof(int));
    int* pile_rec = (int*)calloc(g->nb_noeuds, sizeof(int));
    int trouve = 0;

    for (int i = 0; i < g->nb_noeuds; i++) {
        if (g->noeuds[i].est_actif && !visite[i]) {
            if (dfs_cycle(i, visite, pile_rec, g)) {
                trouve = 1;
                break;
            }
        }
    }
    free(visite);
    free(pile_rec);
    return trouve;
}


// 3.2 POINTS D'ARTICULATION (RÉSILIENCE DU RÉSEAU)

void dfs_points_critiques(int u, int visite[], int disc[], int low[], 
                          int parent[], int est_articul[], Graphe* g, int* temps) {
    int fils = 0;
    visite[u] = 1;
    disc[u] = low[u] = ++(*temps);

    AreteNoeud* courant = g->noeuds[u].liste_adj; //
    while (courant != NULL) {
        int v = courant->dest_id; //

        if (!g->noeuds[v].est_actif) {
            courant = courant->suivant;
            continue;
        }

        if (!visite[v]) {
            fils++;
            parent[v] = u;
            dfs_points_critiques(v, visite, disc, low, parent, est_articul, g, temps);
            low[u] = MIN(low[u], low[v]);

            // Cas racine et cas général
            if (parent[u] == -1 && fils > 1) est_articul[u] = 1;
            if (parent[u] != -1 && low[v] >= disc[u]) est_articul[u] = 1;
        } else if (v != parent[u]) {
            low[u] = MIN(low[u], disc[v]);
        }
        courant = courant->suivant;
    }
}

void identifier_points_critiques(Graphe* g) {
    if (!g || g->nb_noeuds <= 0) return;

    int* disc = (int*)malloc(g->nb_noeuds * sizeof(int));
    int* low = (int*)malloc(g->nb_noeuds * sizeof(int));
    int* parent = (int*)malloc(g->nb_noeuds * sizeof(int));
    int* visite = (int*)calloc(g->nb_noeuds, sizeof(int));
    int* est_articul = (int*)calloc(g->nb_noeuds, sizeof(int));
    int temps = 0;

    for (int i = 0; i < g->nb_noeuds; i++) parent[i] = -1;

    printf("\n--- ANALYSE DE RÉSILIENCE ---\n");
    for (int i = 0; i < g->nb_noeuds; i++) {
        if (g->noeuds[i].est_actif && !visite[i]) {
            dfs_points_critiques(i, visite, disc, low, parent, est_articul, g, &temps);
        }
    }

    int nb_critiques = 0;
    for (int i = 0; i < g->nb_noeuds; i++) {
        if (est_articul[i]) {
            printf("[VULNÉRABILITÉ] Le nœud %d est un point critique (SPOF).\n", i);
            nb_critiques++;
        }
    }
    if (nb_critiques == 0) printf("Aucun point critique structurel détecté.\n");

    free(disc); free(low); free(parent); free(visite); free(est_articul);
}

// 3.3 COMPOSANTES FORTEMENT CONNEXES (ZONAGE SÉCURITÉ)

void dfs_scc(int u, int disc[], int low[], Graphe* g, int* temps) {
    disc[u] = low[u] = ++(*temps);
    push_scc(u);

    AreteNoeud* courant = g->noeuds[u].liste_adj; //
    while (courant != NULL) {
        int v = courant->dest_id; //

        if (!g->noeuds[v].est_actif) {
            courant = courant->suivant;
            continue;
        }

        if (disc[v] == -1) {
            dfs_scc(v, disc, low, g, temps);
            low[u] = MIN(low[u], low[v]);
        } else if (present_dans_pile[v]) {
            low[u] = MIN(low[u], disc[v]);
        }
        courant = courant->suivant;
    }

    if (low[u] == disc[u]) {
        printf("Zone de confiance (SCC) détectée : ");
        while (1) {
            int node = pop_scc();
            printf("%d ", node);
            if (u == node) break;
        }
        printf("\n");
    }
}

void analyser_zones_securite(Graphe* g) {
    if (!g || g->nb_noeuds <= 0) return;

    // Allocation dynamique de la pile pour plus de flexibilité
    pile_scc = (int*)malloc(g->nb_noeuds * sizeof(int));
    present_dans_pile = (int*)calloc(g->nb_noeuds, sizeof(int));
    sommet_pile = -1;

    int* disc = (int*)malloc(g->nb_noeuds * sizeof(int));
    int* low = (int*)malloc(g->nb_noeuds * sizeof(int));
    int temps = 0;

    for (int i = 0; i < g->nb_noeuds; i++) {
        disc[i] = -1;
        low[i] = -1;
    }

    printf("\n--- ANALYSE DU ZONAGE RÉSEAU (SCC) ---\n");
    for (int i = 0; i < g->nb_noeuds; i++) {
        if (g->noeuds[i].est_actif && disc[i] == -1) {
            dfs_scc(i, disc, low, g, &temps);
        }
    }

    free(disc); free(low);
    free(pile_scc); free(present_dans_pile);
    pile_scc = NULL; present_dans_pile = NULL;
}
