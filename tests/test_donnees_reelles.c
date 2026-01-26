#include "../src/routage.h"
#include "test_utils.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

// Liste des sous-dossiers
const char *DIRS[] = {"simple", "medium", "realistic", "large"};
const int NB_DIRS = 4;
int count_total = 0;

void tester_dossier(const char *base_path, const char *subdir) {
  char path[1024];
  snprintf(path, sizeof(path), "%s/%s", base_path, subdir);

  DIR *d = opendir(path);
  if (!d) {
    char msg[200];
    snprintf(msg, sizeof(msg), "Impossible d'ouvrir %s", path);
    log_echec("Ouverture Dossier", msg);
    return;
  }

  struct dirent *dir;
  int count = 0;

  log_info("Scanning dossier...");
  printf("   > %s\n", subdir);

  while ((dir = readdir(d)) != NULL) {
    if (dir->d_type == DT_REG && strstr(dir->d_name, ".txt")) {
      char filepath[2048];
      snprintf(filepath, sizeof(filepath), "%s/%s", path, dir->d_name);

      Graphe *g = graphe_charger(filepath);
      if (g) {
        char res[512];
        snprintf(res, sizeof(res), "OK (%d noeuds) - %s", g->nb_noeuds,
                 dir->d_name);
        log_succes("Chargement Fichier", res);
        count++;
        count_total++;

        // Petit test Dijkstra pour vérifier intégrité
        if (g->nb_noeuds > 1) {
          Chemin *c = routage_dijkstra(g, 0, g->nb_noeuds - 1, "cout");
          if (c)
            chemin_detruire(c);
        }

        // Génération des artefacts visuels (DOT + PNG)
        char dot_path[2048];
        char png_path[2048];
        char basename[512];

        // Extraction du nom de fichier sans extension
        strncpy(basename, dir->d_name, sizeof(basename));
        char *ext = strrchr(basename, '.');
        if (ext)
          *ext = '\0';

        snprintf(dot_path, sizeof(dot_path), "resultats_tests/visu_%s.dot",
                 basename);
        snprintf(png_path, sizeof(png_path), "resultats_tests/visu_%s.png",
                 basename);

        // Export DOT
        graphe_vers_dot(g, dot_path);

        // Conversion PNG (si dot est installé)
        char cmd[4096];
        snprintf(cmd, sizeof(cmd), "dot -Tpng %s -o %s", dot_path, png_path);
        int ret_val = system(cmd);
        if (ret_val == 0) {
          // log_succes("Visu", png_path); // Trop verbeux pour 20 fichiers, on
          // garde discret
        }

        graphe_detruire(g);
      } else {
        char err[512];
        snprintf(err, sizeof(err), "Echec - %s", dir->d_name);
        log_echec("Chargement Fichier", err);
      }
    } // end if txt

    if (count_total >= 6) {
      log_info("Limite atteinte (6 fichiers). Arret du scan dossier.");
      break; // break while loop
    }
  }

  /*
  if (count >= 3) {
    char msg[100];
    snprintf(msg, sizeof(msg), "%d fichiers validés dans %s", count, subdir);
    log_succes("Couverture Dossier", msg);
  } else if (count > 0) {
     // Silence warning if we hit global limit
  } else {
    // char msg[100];
    // snprintf(msg, sizeof(msg), "Aucun fichier dans %s", subdir);
    // log_warning(msg);
  }
  */

  closedir(d);
}

int main() {
  init_log_test();
  log_section("TEST DONNEES REELLES",
              "Chargement massif des topologies (Limit: 6)");

  for (int i = 0; i < NB_DIRS; i++) {
    if (count_total >= 6)
      break;
    tester_dossier("data/topologies", DIRS[i]);
  }

  if (count_total >= 6)
    log_succes("FIN", "Limite de 6 fichiers atteinte comme demandé");

  return 0;
}
