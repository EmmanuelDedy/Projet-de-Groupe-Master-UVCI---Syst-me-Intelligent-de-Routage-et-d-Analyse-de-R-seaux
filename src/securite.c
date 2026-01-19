#include <stdio.h>
#include <stdlib.h>
#include "securite.h"

/* Macro utilitaire pour l'algorithme de Tarjan */
#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif

// --- STRUCTURES INTERNES POUR LES SCC (PILE STATIQUE) ---
#define MAX_NODES 1000
int pile_scc[MAX_NODES];
int sommet_pile = -1;
int present_dans_pile[MAX_NODES];

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
    visite[v] = 1; // Gris : en cours
    pile_rec[v] = 1;

    Arete* courant = g->noeuds[v].aretes;
    while (courant != NULL) {
        int voisin = courant->destination;
        if (!visite[voisin]) {
            if (dfs_cycle(voisin, visite, pile_rec, g)) return 1;
        } else if (pile_rec[voisin]) {
            return 1; // Arête de retour détectée = Cycle
        }
        courant = courant->suivant;
    }
    pile_rec[v] = 0;
    return 0;
}

int detecter_cycles(Graphe* g) {
    int* visite = (int*)calloc(g->nb_noeuds, sizeof(int));
    int* pile_rec = (int*)calloc(g->nb_noeuds, sizeof(int));
    int trouve = 0;

    for (int i = 0; i < g->nb_noeuds; i++) {
        if (!visite[i]) {
            if (dfs_cycle(i, visite, pile_rec, g)) {
                trouve = 1;
                break;
            }
        }
    }
    free(visite); free(pile_rec);
    return trouve;
}


// 3.2 POINTS D'ARTICULATION (TARJAN RESILIENCE)

void dfs_points_critiques(int u, int visite[], int disc[], int low[], 
                          int parent[], int est_articul[], Graphe* g, int* temps) {
    int fils = 0;
    visite[u] = 1;
    disc[u] = low[u] = ++(*temps);

    Arete* courant = g->noeuds[u].aretes;
    while (courant != NULL) {
        int v = courant->destination;
        if (!visite[v]) {
            fils++;
            parent[v] = u;
            dfs_points_critiques(v, visite, disc, low, parent, est_articul, g, temps);
            low[u] = MIN(low[u], low[v]);

            if (parent[u] == -1 && fils > 1) est_articul[u] = 1;
            if (parent[u] != -1 && low[v] >= disc[u]) est_articul[u] = 1;
        } else if (v != parent[u]) {
            low[u] = MIN(low[u], disc[v]);
        }
        courant = courant->suivant;
    }
}

void identifier_points_critiques(Graphe* g) {
    int* disc = (int*)malloc(g->nb_noeuds * sizeof(int));
    int* low = (int*)malloc(g->nb_noeuds * sizeof(int));
    int* parent = (int*)malloc(g->nb_noeuds * sizeof(int));
    int* visite = (int*)calloc(g->nb_noeuds, sizeof(int));
    int* est_articul = (int*)calloc(g->nb_noeuds, sizeof(int));
    int temps = 0;

    for (int i = 0; i < g->nb_noeuds; i++) parent[i] = -1;

    printf("\n--- ANALYSE DE RÉSILIENCE ---\n");
    for (int i = 0; i < g->nb_noeuds; i++) {
        if (!visite[i]) dfs_points_critiques(i, visite, disc, low, parent, est_articul, g, &temps);
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

    Arete* courant = g->noeuds[u].aretes;
    while (courant != NULL) {
        int v = courant->destination;
        if (disc[v] == -1) {
            dfs_scc(v, disc, low, g, temps);
            low[u] = MIN(low[u], low[v]);
        } else if (present_dans_pile[v]) {
            low[u] = MIN(low[u], disc[v]);
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
    int* disc = (int*)malloc(g->nb_noeuds * sizeof(int));
    int* low = (int*)malloc(g->nb_noeuds * sizeof(int));
    int temps = 0;
    for (int i = 0; i < g->nb_noeuds; i++) { disc[i] = -1; present_dans_pile[i] = 0; }

    printf("\n--- ANALYSE DU ZONAGE RÉSEAU ---\n");
    for (int i = 0; i < g->nb_noeuds; i++) {
        if (disc[i] == -1) dfs_scc(i, disc, low, g, &temps);
    }
    free(disc); free(low);
}
