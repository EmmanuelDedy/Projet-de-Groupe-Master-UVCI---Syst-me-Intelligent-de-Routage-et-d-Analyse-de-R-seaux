/*
 * PROJET ALGORITHME ET COMPLEXITE - DEV GRAPHES
 * Interface Interactive Principale (CLI)
 *
 * Ce programme charge un graphe et propose un menu interactif pour exploiter
 * les differents modules du projet :
 * 1. Gestion de Graphe (Chargement, Affichage)
 * 2. Algorithmes de Routage (Dijkstra, K-PC, Backtracking)
 * 3. Analyse de Securite (Composantes connexes, points critiques, cycles)
 *
 * Auteurs : Equipe Projet
 * Date : Janvier 2025
 */

#include "graphe.h"
#include "logger.h"
#include "orchestrator.h"
#include "routage.h"
#include "securite.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// Helper pour gérer le chemin (ajoute data/ si nécessaire)
static void formatter_chemin(const char *entree, char *sortie) {
  // Si le chemin commence par . ou / ou data/, on touche pas
  if (entree[0] == '.' || entree[0] == '/' ||
      strncmp(entree, "data/", 5) == 0) {
    strcpy(sortie, entree);
  } else {
    sprintf(sortie, "data/%s", entree);
  }
}

// --- Utilitaires d'Affichage ---

void afficher_menu() {
  printf("\n=== SYSTEME DE GESTION RESEAU (GSM) ===\n");
  printf("1.  Charger un fichier de topologie\n");
  printf("2.  Afficher les informations du graphe\n");
  printf("3.  [Routage] Plus Court Chemin (Dijkstra)\n");
  printf("4.  [Routage] K-Plus Courts Chemins\n");
  printf("5.  [Routage] Chemin Contraint (Backtracking)\n");
  printf("6.  [Securite] Audit de Vulnerabilite (Complet)\n");
  printf("7.  [Securite] Detecter les Cycles\n");
  printf("7.  [Securite] Detecter les Cycles\n");
  printf("8.  Sauvegarder la topologie actuelle\n");
  printf("0.  Quitter\n");
  printf("========================================\n");
  printf("Votre choix : ");
}

void afficher_chemin(Chemin *c, const char *titre) {
  if (!c) {
    printf("   -> %s : Aucun chemin trouve.\n", titre);
    return;
  }
  printf("   -> %s :\n", titre);
  printf("      Cout Total : %.2f | Latence : %.2f ms | BP Min : %.2f Mbps\n",
         c->cout_total, c->latence_totale, c->bande_passante_min);
  printf("      Trajet : [ ");
  for (int i = 0; i < c->longueur; i++) {
    printf("%d ", c->noeuds[i]);
    if (i < c->longueur - 1)
      printf("-> ");
  }
  printf("]\n");
}

// --- Fonctions Principales ---

void menu_dijkstra(Graphe *g) {
  if (!g) {
    printf("Erreur : Aucun graphe charge.\n");
    return;
  }
  int src, dest;
  printf("Source : ");
  if (scanf("%d", &src) != 1)
    return;
  printf("Destination : ");
  if (scanf("%d", &dest) != 1)
    return;

  char critere[20];
  printf("Critere (cout/latence/bande_passante/securite) [defaut: cout] : ");
  scanf("%s", critere);
  // Valeur par défaut si entrée vide ou invalide géré par l'algo (fallback sur
  // cout)

  Chemin *c = routage_dijkstra(g, src, dest, critere);
  afficher_chemin(c, "Resultat Dijkstra");
  chemin_detruire(c);
}

void menu_k_shortest(Graphe *g) {
  if (!g) {
    printf("Erreur : Aucun graphe charge.\n");
    return;
  }
  int src, dest, k;
  printf("Source : ");
  if (scanf("%d", &src) != 1)
    return;
  printf("Destination : ");
  if (scanf("%d", &dest) != 1)
    return;
  printf("Nombre de chemins (K) : ");
  if (scanf("%d", &k) != 1)
    return;

  // On utilise le critère "cout" par défaut pour simplifier l'interaction
  EnsembleChemins *ec = routage_k_chemins_plus_courts(g, src, dest, k, "cout");

  if (ec) {
    printf("\nResultats K-Plus Courts Chemins (K=%d) :\n", ec->nombre);
    for (int i = 0; i < ec->nombre; i++) {
      char label[20];
      sprintf(label, "Chemin #%d", i + 1);
      afficher_chemin(&ec->chemins[i], label);
    }
    ensemble_chemins_detruire(ec); // Nettoyage
  } else {
    printf("Aucun ensemble de chemins retourne.\n");
  }
}

void menu_backtracking(Graphe *g) {
  if (!g) {
    printf("Erreur : Aucun graphe charge.\n");
    return;
  }
  int src, dest;
  float min_bp, max_cout;

  printf("Source : ");
  if (scanf("%d", &src) != 1)
    return;
  printf("Destination : ");
  if (scanf("%d", &dest) != 1)
    return;
  printf("Bande Passante Minimale requise : ");
  if (scanf("%f", &min_bp) != 1)
    return;
  printf("Cout Maximum autorise : ");
  if (scanf("%f", &max_cout) != 1)
    return;

  // Pour cet exemple interactif, on ne demande pas de nœuds obligatoires (NULL)
  Chemin *c =
      routage_backtracking_contraint(g, src, dest, min_bp, max_cout, NULL, 0);
  afficher_chemin(c, "Resultat Backtracking Contraint");
  chemin_detruire(c);
}

// --- Main Loop ---

int main(int argc, char *argv[]) {
  Graphe *g = NULL;
  char filepath[256];

  // Initialiser le système de logging
  logger_init_simple("logs/gsm.log", LOG_LEVEL_INFO);
  LOG_INFO(MOD_MAIN, "Démarrage de l'interface interactive GSM");

  // Si un fichier est passé en argument, on essaie de le charger direct
  if (argc > 1) {
    LOG_INFO(MOD_LOADER, "Chargement depuis argument: %s", argv[1]);
    g = graphe_charger(argv[1]);
    if (g) {
      LOG_INFO(MOD_LOADER, "Graphe chargé avec succès");
      printf("Graphe charge avec succes !\n");
      graphe_afficher(g);
    } else {
      LOG_ERROR(MOD_LOADER, "Échec du chargement: %s", argv[1]);
      printf("Echec du chargement.\n");
    }
  }

  int choix = -1;
  while (choix != 0) {
    afficher_menu();
    if (scanf("%d", &choix) != 1) {
      // Vider le buffer si l'utilisateur rentre n'importe quoi
      while (getchar() != '\n')
        ;
      continue;
    }

    switch (choix) {
    case 1: // Charger
      printf("Nom du fichier (dans data/) ou chemin complet : ");
      scanf("%s", filepath);
      char fullpath[256];
      formatter_chemin(filepath, fullpath);

      if (g)
        graphe_detruire(g); // Liberer l'ancien
      g = graphe_charger(fullpath);
      if (g)
        printf(">>> Graphe charge depuis %s.\n", fullpath);
      else
        printf(">>> Erreur de chargement (%s).\n", fullpath);
      break;

    case 2: // Afficher
      if (g)
        graphe_afficher(g);
      else
        printf(">>> Aucun graphe en memoire.\n");
      break;

    case 3: // Dijkstra
      menu_dijkstra(g);
      break;

    case 4: // K-Shortest
      menu_k_shortest(g);
      break;

    case 5: // Backtracking
      menu_backtracking(g);
      break;

    case 6: // Securite Globale
      if (g)
        securite_analyser_vulnerabilites(g);
      else
        printf(">>> Aucun graphe en memoire.\n");
      break;

    case 7: // Detecter Cycles
      if (g) {
        int has_cycle = detecter_cycles(g);
        if (has_cycle)
          printf(">>> ATTENTION: Le graphe contient des cycles !\n");
        else
          printf(">>> Le graphe est acyclique (DAG).\n");
      } else {
        printf(">>> Aucun graphe en memoire.\n");
      }
      break;

    case 8: // Sauvegarder
      if (!g) {
        printf(">>> Aucun graphe a sauvegarder.\n");
      } else {
        printf("Nom du fichier de sortie (sera dans data/) : ");
        scanf("%s", filepath);
        char outpath[256];
        formatter_chemin(filepath, outpath);

        if (graphe_sauvegarder(g, outpath)) {
          printf(">>> Topologie sauvegardee dans %s\n", outpath);
        } else {
          printf(">>> Erreur lors de la sauvegarde.\n");
        }
      }
      break;

    case 0:
      LOG_INFO(MOD_MAIN, "Fermeture demandée par l'utilisateur");
      printf("Au revoir !\n");
      break;

    default:
      LOG_WARN(MOD_MAIN, "Choix invalide: %d", choix);
      printf("Choix invalide.\n");
    }
  }

  // Nettoyage
  if (g) {
    LOG_DEBUG(MOD_MAIN, "Libération du graphe en mémoire");
    graphe_detruire(g);
  }

  LOG_INFO(MOD_MAIN, "Session terminée");
  logger_close();

  return 0;
}
