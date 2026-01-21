#include "graphe.h"
#include <string.h>

Graphe *graphe_creer(int max_noeuds, bool est_oriente) {
  Graphe *g = (Graphe *)malloc(sizeof(Graphe));
  if (!g)
    return NULL;

  g->nb_noeuds = 0;
  g->max_noeuds = max_noeuds;
  g->est_oriente = est_oriente;

  g->noeuds = (Noeud *)malloc(max_noeuds * sizeof(Noeud));
  if (!g->noeuds) {
    free(g);
    return NULL;
  }

  g->matrice_adj = (double **)malloc(max_noeuds * sizeof(double *));
  if (!g->matrice_adj) {
    free(g->noeuds);
    free(g);
    return NULL;
  }

  for (int i = 0; i < max_noeuds; i++) {
    g->matrice_adj[i] = (double *)malloc(max_noeuds * sizeof(double));
    for (int j = 0; j < max_noeuds; j++) {
      g->matrice_adj[i][j] = -1.0; // le egal a -1 signifie aucune arête
    }
    g->noeuds[i].id = -1;
    g->noeuds[i].nom[0] = '\0';
    g->noeuds[i].liste_adj = NULL;
    g->noeuds[i].est_actif = false;
  }

  return g;
}

void graphe_detruire(Graphe *g) {
  if (!g)
    return;

  for (int i = 0; i < g->max_noeuds; i++) {
    AreteNoeud *cour = g->noeuds[i].liste_adj;
    while (cour) {
      AreteNoeud *suiv = cour->suivant;
      free(cour);
      cour = suiv;
    }
    free(g->matrice_adj[i]);
  }

  free(g->matrice_adj);
  free(g->noeuds);
  free(g);
}

int graphe_ajouter_noeud(Graphe *g, TypeNoeud type, double x, double y) {
  if (g->nb_noeuds >= g->max_noeuds)
    return -1;

  int id = g->nb_noeuds++;
  g->noeuds[id].id = id;
  sprintf(g->noeuds[id].nom, "Noeud_%d", id);
  g->noeuds[id].type = type;
  g->noeuds[id].x = x;
  g->noeuds[id].y = y;
  g->noeuds[id].est_actif = true;
  g->noeuds[id].liste_adj = NULL;

  return id;
}

/**
 * @brief Variante avec nom personnalisé
 */
int graphe_ajouter_noeud_nomme(Graphe *g, const char *nom, TypeNoeud type,
                               double x, double y) {
  int id = graphe_ajouter_noeud(g, type, x, y);
  if (id != -1 && nom) {
    strncpy(g->noeuds[id].nom, nom, 49);
    g->noeuds[id].nom[49] = '\0';
  }
  return id;
}

bool graphe_ajouter_arete(Graphe *g, int src, int dest, Metriques m) {
  if (src < 0 || src >= g->nb_noeuds || dest < 0 || dest >= g->nb_noeuds)
    return false;

  // Mise à jour de la Matrice d'Adjacence (coût par défaut)
  g->matrice_adj[src][dest] = m.cout;
  if (!g->est_oriente) {
    g->matrice_adj[dest][src] = m.cout;
  }

  // Mise à jour de la Liste d'Adjacence
  AreteNoeud *nouv = (AreteNoeud *)malloc(sizeof(AreteNoeud));
  if (!nouv)
    return false;
  nouv->dest_id = dest;
  nouv->metriques = m;
  nouv->suivant = g->noeuds[src].liste_adj;
  g->noeuds[src].liste_adj = nouv;

  if (!g->est_oriente) {
    AreteNoeud *rev = (AreteNoeud *)malloc(sizeof(AreteNoeud));
    if (!rev)
      return false;
    rev->dest_id = src;
    rev->metriques = m;
    rev->suivant = g->noeuds[dest].liste_adj;
    g->noeuds[dest].liste_adj = rev;
  }

  return true;
}

void graphe_afficher(Graphe *g) {
  printf("Graphe avec %d noeuds :\n", g->nb_noeuds);
  for (int i = 0; i < g->nb_noeuds; i++) {
    printf("Noeud %d (%s, Type %d) a (%.1f, %.1f) :\n", i, g->noeuds[i].nom,
           g->noeuds[i].type, g->noeuds[i].x, g->noeuds[i].y);
    AreteNoeud *cour = g->noeuds[i].liste_adj;
    while (cour) {
      printf("  -> %d (Cout : %.1f, Latence : %.1f ms, BP : %.1f Mbps, Sec : "
             "%d)\n",
             cour->dest_id, cour->metriques.cout, cour->metriques.latence,
             cour->metriques.bande_passante, cour->metriques.securite);
      cour = cour->suivant;
    }
  }
}

bool graphe_supprimer_arete(Graphe *g, int src, int dest) {
  if (src < 0 || src >= g->nb_noeuds || dest < 0 || dest >= g->nb_noeuds)
    return false;

  // Supprimer de la matrice
  g->matrice_adj[src][dest] = -1.0;
  if (!g->est_oriente)
    g->matrice_adj[dest][src] = -1.0;

  // Supprimer de la liste d'adjacence
  AreteNoeud **cour = &(g->noeuds[src].liste_adj);
  while (*cour) {
    if ((*cour)->dest_id == dest) {
      AreteNoeud *a_suppr = *cour;
      *cour = (*cour)->suivant;
      free(a_suppr);
      break;
    }
    cour = &((*cour)->suivant);
  }

  if (!g->est_oriente) {
    cour = &(g->noeuds[dest].liste_adj);
    while (*cour) {
      if ((*cour)->dest_id == src) {
        AreteNoeud *a_suppr = *cour;
        *cour = (*cour)->suivant;
        free(a_suppr);
        break;
      }
      cour = &((*cour)->suivant);
    }
  }
  return true;
}

/**
 * @brief Supprime un nœud du graphe (suppression logique).
 *
 * Cette fonction marque le nœud comme inactif et supprime toutes les arêtes
 * entrantes et sortantes associées à ce nœud.
 *
 * @param g Pointeur vers le graphe
 * @param id Identifiant du nœud à supprimer
 * @return true si la suppression a réussi, false sinon
 */
bool graphe_supprimer_noeud(Graphe *g, int id) {
  if (!g || id < 0 || id >= g->nb_noeuds)
    return false;

  if (!g->noeuds[id].est_actif)
    return false; // Le nœud est déjà supprimé

  // 1. Supprimer toutes les arêtes sortantes du nœud
  AreteNoeud *cour = g->noeuds[id].liste_adj;
  while (cour) {
    AreteNoeud *suiv = cour->suivant;

    // Mettre à jour la matrice d'adjacence
    g->matrice_adj[id][cour->dest_id] = -1.0;
    if (!g->est_oriente) {
      g->matrice_adj[cour->dest_id][id] = -1.0;
    }

    free(cour);
    cour = suiv;
  }
  g->noeuds[id].liste_adj = NULL;

  // 2. Supprimer toutes les arêtes entrantes vers ce nœud (dans les autres
  // listes)
  for (int i = 0; i < g->nb_noeuds; i++) {
    if (i == id || !g->noeuds[i].est_actif)
      continue;

    AreteNoeud **ptr = &(g->noeuds[i].liste_adj);
    while (*ptr) {
      if ((*ptr)->dest_id == id) {
        AreteNoeud *a_suppr = *ptr;
        *ptr = (*ptr)->suivant;
        free(a_suppr);

        // Mettre à jour la matrice
        g->matrice_adj[i][id] = -1.0;
      } else {
        ptr = &((*ptr)->suivant);
      }
    }
  }

  // 3. Marquer le nœud comme inactif
  g->noeuds[id].est_actif = false;

  return true;
}

bool graphe_sauvegarder(Graphe *g, const char *nom_fichier) {
  FILE *f = fopen(nom_fichier, "w");
  if (!f)
    return false;

  fprintf(f, "N %d %d\n", g->nb_noeuds, g->est_oriente ? 1 : 0);
  for (int i = 0; i < g->nb_noeuds; i++) {
    fprintf(f, "V %d %s %d %f %f %d\n", g->noeuds[i].id, g->noeuds[i].nom,
            g->noeuds[i].type, g->noeuds[i].x, g->noeuds[i].y,
            g->noeuds[i].est_actif ? 1 : 0);
  }

  for (int i = 0; i < g->nb_noeuds; i++) {
    AreteNoeud *cour = g->noeuds[i].liste_adj;
    while (cour) {
      // Pour les graphes non-orientés, la sauvegarde se fait qu'une fois chaque
      // arête (id < dest_id)
      if (g->est_oriente || i < cour->dest_id) {
        fprintf(f, "E %d %d %f %f %f %f %d\n", i, cour->dest_id,
                cour->metriques.cout, cour->metriques.latence,
                cour->metriques.bande_passante, cour->metriques.fiabilite,
                cour->metriques.securite);
      }
      cour = cour->suivant;
    }
  }
  fclose(f);
  return true;
}

Graphe *graphe_charger(const char *nom_fichier) {
  FILE *f = fopen(nom_fichier, "r");
  if (!f)
    return NULL;

  char type;
  int nb, oriente;
  if (fscanf(f, " N %d %d", &nb, &oriente) != 2) {
    fclose(f);
    return NULL;
  }

  Graphe *g = graphe_creer(nb + 10, oriente == 1); // Extra marge
  while (fscanf(f, " %c", &type) == 1) {
    if (type == 'V') {
      int id, type_n, actif;
      char nom[50];
      float x, y;
      fscanf(f, " %d %s %d %f %f %d", &id, nom, &type_n, &x, &y, &actif);
      int real_id = graphe_ajouter_noeud_nomme(g, nom, (TypeNoeud)type_n, x, y);
      g->noeuds[real_id].est_actif = (actif == 1);
    } else if (type == 'E') {
      int src, dest, sec;
      float c, l, b, f_val;
      fscanf(f, " %d %d %f %f %f %f %d", &src, &dest, &c, &l, &b, &f_val, &sec);
      Metriques m = {c, l, b, f_val, sec};
      graphe_ajouter_arete(g, src, dest, m);
    }
  }
  fclose(f);
  return g;
}

// fonction pour convertir le graphe en format DOT
void graphe_vers_dot(Graphe *g, const char *nom_fichier) {
  FILE *f = fopen(nom_fichier, "w");
  if (!f)
    return;

  fprintf(f, "%s G {\n", g->est_oriente ? "digraph" : "graph");
  const char *symb = g->est_oriente ? "->" : "--";

  for (int i = 0; i < g->nb_noeuds; i++) {
    fprintf(f, "  %d [label=\"%s\"];\n", i, g->noeuds[i].nom);
  }

  for (int i = 0; i < g->nb_noeuds; i++) {
    AreteNoeud *cour = g->noeuds[i].liste_adj;
    while (cour) {
      if (g->est_oriente || i < cour->dest_id) {
        fprintf(f, "  %d %s %d [label=\"c:%.1f, b:%.1f\"];\n", i, symb,
                cour->dest_id, cour->metriques.cout,
                cour->metriques.bande_passante);
      }
      cour = cour->suivant;
    }
  }
  fprintf(f, "}\n");
  fclose(f);
}

