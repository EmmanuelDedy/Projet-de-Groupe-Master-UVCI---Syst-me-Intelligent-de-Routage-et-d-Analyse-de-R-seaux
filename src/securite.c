#include <stdio.h>
#include <stdlib.h>
#include "securite.h"

/* Macro utilitaire pour l'algorithme de Tarjan */
#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif

//  GESTION DYNAMIQUE DE LA PILE SCC (TARJAN) 
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
    visite[v] = 1; // Gris : en cours de visite
    pile_rec[v] = 1;

    // Utilisation de AreteNoeud et liste_adj (Nouvelle Structure)
    AreteNoeud *courant = g->noeuds[v].liste_adj;
    while (courant != NULL) {
        int voisin = courant->dest_id;

        // Vérification de la robustesse : ignorer si le nœud voisin est inactif
        if (!g->noeuds[voisin].est_actif) {
            courant = courant->suivant;
            continue;
        }

        if (!visite[voisin]) {
            if (dfs_cycle(voisin, visite, pile_rec, g)) return 1;
        } else if (pile_rec[voisin]) {
            return 1; // Arête de retour détectée
        }
        courant = courant->suivant;
    }
    pile_rec[v] = 0;
    return 0;
}

int detecter_cycles(Graphe* g) {
    // Vérification de sécurité mémoire
    if (!g || g->nb_noeuds <= 0) return 0;

    int* visite = (int*)calloc(g->nb_noeuds, sizeof(int));
    int* pile_rec = (int*)calloc(g->nb_noeuds, sizeof(int));
    int trouve = 0;

    for (int i = 0; i < g->nb_noeuds; i++) {
        // On ne lance l'analyse que sur les nœuds actifs
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


// 3.2 POINTS D'ARTICULATION (TARJAN RESILIENCE)

void dfs_points_critiques(int u, int visite[], int disc[], int low[], 
                          int parent[], int est_articul[], Graphe* g, int* temps) {
    int fils = 0;
    visite[u] = 1;
    disc[u] = low[u] = ++(*temps);

    AreteNoeud *courant = g->noeuds[u].liste_adj;
    while (courant != NULL) {
        int voisin = courant->dest_id;

        // Ignorer les nœuds inactifs
        if (!g->noeuds[voisin].est_actif) {
            courant = courant->suivant;
            continue;
        }

        if (!visite[voisin]) {
            fils++;
            parent[voisin] = u;
            dfs_points_critiques(voisin, visite, disc, low, parent, est_articul, g, temps);
            low[u] = MIN(low[u], low[voisin]);

            // Condition pour point d'articulation
            if (parent[u] == -1 && fils > 1) est_articul[u] = 1;
            if (parent[u] != -1 && low[voisin] >= disc[u]) est_articul[u] = 1;
        } else if (voisin != parent[u]) {
            low[u] = MIN(low[u], disc[voisin]);
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

    for (int i = 0; i < g->nb_noeuds; i++) {
        if (est_articul[i]) printf("[ALERTE] Nœud critique (SPOF) détecté : %d\n", i);
    }

    free(disc); free(low); free(parent); free(visite); free(est_articul);
}


// 3.3 ZONES DE SÉCURITÉ (SCC TARJAN)

void dfs_scc(int u, int disc[], int low[], Graphe* g, int* temps) {
    disc[u] = low[u] = ++(*temps);
    push_scc(u);

    AreteNoeud *courant = g->noeuds[u].liste_adj;
    while (courant != NULL) {
        int voisin = courant->dest_id;

        if (!g->noeuds[voisin].est_actif) {
            courant = courant->suivant;
            continue;
        }

        if (disc[voisin] == -1) {
            dfs_scc(voisin, disc, low, g, temps);
            low[u] = MIN(low[u], low[voisin]);
        } else if (present_dans_pile[voisin]) {
            low[u] = MIN(low[u], disc[voisin]);
        }
        courant = courant->suivant;
    }

    if (low[u] == disc[u]) {
        printf("Zone SCC identifiée : ");
        while (1) {
            int v = pop_scc();
            printf("%d ", v);
            if (u == v) break;
        }
        printf("\n");
    }
}

void analyser_zones_securite(Graphe* g) {
    if (!g || g->nb_noeuds <= 0) return;

    // Initialisation dynamique de la pile pour éviter les dépassements (MAX_NODES)
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

    // Libération de toute la mémoire dynamique
    free(disc); 
    free(low);
    free(pile_scc);
    free(present_dans_pile);
    pile_scc = NULL; 
    present_dans_pile = NULL;
}
