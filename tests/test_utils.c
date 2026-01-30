#include "test_utils.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#define FICHIER_LOG "resultats_tests/tests_logs.log"

void init_log_test() {
#ifdef _WIN32
  _mkdir("resultats_tests");
#else
  mkdir("resultats_tests", 0777);
#endif

  FILE *f = fopen(FICHIER_LOG, "a");
  if (f) {
    time_t now = time(NULL);
    fprintf(f, "\n--- SESSION DE TEST : %s", ctime(&now));
    fprintf(f, "---------------------------------------------------\n");
    fclose(f);
  }
}

// Helper interne pour écrire dans le fichier et la console
static void write_log(const char *type, const char *msg, const char *color) {
  FILE *f = fopen(FICHIER_LOG, "a");
  if (f) {
    // Retirer les codes couleur pour le fichier si possible, ou simple écriture
    // Ici on écrit brut sans couleur
    fprintf(f, "[%s] %s\n", type, msg);
    fclose(f);
  }
  // Console avec couleur
  printf("%s[%s] %s%s\n", color, type, msg, COLOR_RESET);
}

// Couleurs ANSI étendues pour un rendu "Premium"
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define RED "\033[38;5;196m"     // Rouge vif
#define GREEN "\033[38;5;46m"    // Vert matrix
#define YELLOW "\033[38;5;226m"  // Jaune électrique
#define BLUE "\033[38;5;39m"     // Bleu ciel
#define MAGENTA "\033[38;5;201m" // Rose néon
#define CYAN "\033[38;5;51m"     // Cyan
#define WHITE "\033[38;5;15m"
#define GRAY "\033[38;5;240m"

#define ICON_CHECK "[OK]"
#define ICON_CROSS "[KO]"
#define ICON_INFO "[INFO]"
#define ICON_WARN "[WARN]"
#define ICON_ROCKET "[TEST]"

void log_section(const char *titre, const char *desc) {
  FILE *f = fopen(FICHIER_LOG, "a");
  if (f) {
    fprintf(f, "\n[SECTION] %s - %s\n", titre, desc);
    fclose(f);
  }
  printf("\n%s%s==============================================================="
         "=================%s\n",
         BOLD, MAGENTA, RESET);
  printf(" %s %s%-60s %s\n", ICON_ROCKET, WHITE, titre, RESET);
  printf(" %s%s%s\n", GRAY, desc, RESET);
  printf("%s%s================================================================="
         "===============%s\n",
         BOLD, MAGENTA, RESET);
}

void log_info(const char *msg) {
  write_log("INFO", msg, BLUE); // Fallback old system
  printf("   %s%s %s\n", BLUE, ICON_INFO, msg);
}

void log_warning(const char *msg) {
  write_log("WARN", msg, YELLOW);
  printf("   %s%s %s\n", YELLOW, ICON_WARN, msg);
}

void log_succes(const char *test_name, const char *details) {
  char buff[512];
  snprintf(buff, sizeof(buff), "%s : %s", test_name, details ? details : "");
  FILE *f = fopen(FICHIER_LOG, "a");
  if (f) {
    fprintf(f, "[SUCCES] %s\n", buff);
    fclose(f);
  }

  printf("   %s%s %s%-30s %s%s\n", GREEN, ICON_CHECK, BOLD, test_name, GRAY,
         details ? details : "");
  printf("%s", RESET);
}

void log_echec(const char *test_name, const char *details) {
  char buff[512];
  snprintf(buff, sizeof(buff), "%s : %s", test_name, details ? details : "");
  FILE *f = fopen(FICHIER_LOG, "a");
  if (f) {
    fprintf(f, "[ECHEC] %s\n", buff);
    fclose(f);
  }

  printf("   %s%s %s%-30s %s%s\n", RED, ICON_CROSS, BOLD, test_name, WHITE,
         details ? details : "");
  printf("%s", RESET);
}

void log_resultat_test(const char *nom_test, int statut, const char *details) {
  if (statut) {
    log_succes(nom_test, details);
  } else {
    log_echec(nom_test, details);
  }
}

int generer_image_depuis_dot(const char *dot_file, const char *png_output) {
  char cmd[1024];
  // On suppose que 'dot' est dans le PATH
  // Commande: dot -Tpng -o resultat.png source.dot
  snprintf(cmd, sizeof(cmd), "dot -Tpng -o \"%s\" \"%s\"", png_output,
           dot_file);

  log_info("Génération image du graphe...");
  int ret = system(cmd);

  if (ret == 0) {
    char msg[256];
    snprintf(msg, sizeof(msg), "Image générée: %s", png_output);
    log_succes("Visualization Mode", msg);
    return 1;
  } else {
    log_warning(
        "Impossible de générer l'image (Graphviz 'dot' non installé ?)");
    return 0;
  }
}

void ajouter_arete_bidirectionnelle(Graphe *g, int u, int v, float cout,
                                    float latence, float bp, int sec) {
  Metriques m1 = {cout, latence, bp, 10.0f, sec};
  graphe_ajouter_arete(g, u, v, m1);

  // Si le graphe est orienté mais qu'on veut une arête bidirectionnelle,
  // il faut ajouter le retour explicitement.
  if (g->est_oriente) {
    graphe_ajouter_arete(g, v, u, m1);
  }
}

// Générateurs

static Graphe *creer_simple(int n) {
  Graphe *g = graphe_creer(n, false); // Non orienté par défaut pour test simple
  for (int i = 0; i < n; i++)
    graphe_ajouter_noeud(g, ROUTEUR, i * 10, i * 10);

  // Chaîne linéaire
  for (int i = 0; i < n - 1; i++) {
    ajouter_arete_bidirectionnelle(g, i, i + 1, 10.0f, 5.0f, 100.0f, 5);
  }
  return g;
}

static Graphe *creer_maillage(int n) {
  Graphe *g = graphe_creer(n, false);
  for (int i = 0; i < n; i++)
    graphe_ajouter_noeud(g, ROUTEUR, i * 10, i * 10);

  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      ajouter_arete_bidirectionnelle(g, i, j, 10.0f, 2.0f, 1000.0f, 8);
    }
  }
  return g;
}

static Graphe *creer_etoile(int n) {
  Graphe *g = graphe_creer(n, false);
  graphe_ajouter_noeud(g, SERVEUR, 50, 50); // Centre
  for (int i = 1; i < n; i++)
    graphe_ajouter_noeud(g, ROUTEUR, i * 10, i * 10);

  for (int i = 1; i < n; i++) {
    ajouter_arete_bidirectionnelle(g, 0, i, 5.0f, 1.0f, 500.0f, 9);
  }
  return g;
}

static Graphe *creer_anneau(int n) {
  Graphe *g = graphe_creer(n, false);
  for (int i = 0; i < n; i++)
    graphe_ajouter_noeud(g, COMMUTATEUR, i * 10, i * 10);

  for (int i = 0; i < n; i++) {
    ajouter_arete_bidirectionnelle(g, i, (i + 1) % n, 15.0f, 10.0f, 100.0f, 3);
  }
  return g;
}

static Graphe *creer_arbre(int n) {
  // Arbre binaire simple
  Graphe *g = graphe_creer(n, false);
  // Noeud 0 est racine
  graphe_ajouter_noeud(g, SERVEUR, 50, 0);

  for (int i = 1; i < n; i++) {
    graphe_ajouter_noeud(g, ROUTEUR, (float)(i % 10) * 10.0f,
                         (float)(i / 10) * 10.0f + 10.0f);
    // Parent de i est (i-1)/2
    int parent = (i - 1) / 2;
    ajouter_arete_bidirectionnelle(g, parent, i, 5.0f, 2.0f, 100.0f, 5);
  }
  return g;
}

static Graphe *creer_grille(int n) {
  // Grille approx sqrt(n) x sqrt(n)
  Graphe *g = graphe_creer(n, false);

  // On fait une grille carrée
  int largeur = (int)sqrt(n);
  if (largeur < 1)
    largeur = 1;
  int hauteur = (n + largeur - 1) / largeur; // Ceiling division

  for (int i = 0; i < n; i++) {
    graphe_ajouter_noeud(g, ROUTEUR, (float)(i % largeur) * 10.0f,
                         (float)(i / largeur) * 10.0f);
  }

  for (int i = 0; i < n; i++) {
    int x = i % largeur;
    int y = i / largeur;

    // Connexion droite
    if (x + 1 < largeur) {
      int droite = i + 1;
      if (droite < n)
        ajouter_arete_bidirectionnelle(g, i, droite, 5.0f, 1.0f, 1000.0f, 5);
    }
    // Connexion bas
    if (y + 1 < hauteur) {
      int bas = i + largeur;
      if (bas < n)
        ajouter_arete_bidirectionnelle(g, i, bas, 5.0f, 1.0f, 1000.0f, 5);
    }
  }
  return g;
}

Graphe *creer_graphe_test(TypeTopologie type, int nb_noeuds) {
  if (nb_noeuds < 2)
    nb_noeuds = 5;

  switch (type) {
  case TOPO_SIMPLE:
    return creer_simple(nb_noeuds);
  case TOPO_MAILLAGE:
    return creer_maillage(nb_noeuds);
  case TOPO_ETOILE:
    return creer_etoile(nb_noeuds);
  case TOPO_ANNEAU:
    return creer_anneau(nb_noeuds);
  case TOPO_ARBRE:
    return creer_arbre(nb_noeuds);
  case TOPO_GRILLE:
    return creer_grille(nb_noeuds);
  default:
    return NULL;
  }
}
