#include "graphe.h"
#include "routage.h"
#include "securite.h"
#include "test_utils.h"
#include <assert.h>
#include <stdio.h>

void test_simulation_complete() {
  log_section(
      "TEST GLOBAL INTÉGRATION",
      "Simulation complète : Design -> Audit -> Routage -> Panne -> Visu");

  // 1. Generation Topologie (Grille 5x5 = 25 noeuds pour avoir un truc
  // consistant)
  log_info("Phase 1: Génération de la Topologie (Grille 5x5)...");
  Graphe *g = creer_graphe_test(TOPO_GRILLE, 25);

  if (!g) {
    log_echec("Creation Graphe", "Echec allocation");
    return;
  }
  log_succes("Creation Graphe", "Graphe Grille 25 noeuds généré");

  // 2. Sauvegarde et Rechargement (Persistence)
  const char *txt_file = "resultats_tests/simulation_topo.txt";
  log_info("Phase 2: Test Persistence (I/O)...");
  if (graphe_sauvegarder(g, txt_file)) {
    log_succes("Sauvegarde", txt_file);
  } else {
    log_echec("Sauvegarde", "Erreur écriture fichier");
  }

  graphe_detruire(g); // On détruit pour forcer le rechargement propre
  g = graphe_charger(txt_file);
  if (g && g->nb_noeuds == 25) {
    log_succes("Chargement", "Graphe rechargé avec succès (25 noeuds)");
  } else {
    log_echec("Chargement", "Echec rechargement graphe");
    return;
  }

  // 3. Audit Sécurité
  log_info("Phase 3: Audit de Sécurité...");
  // Sur une grille parfaite, il y a beaucoup de redondance, donc peu de points
  // critiques normalement. Sauf les coins si on isole ?
  int cycles = detecter_cycles(g);
  if (cycles)
    log_succes("Détection Cycles", "Cycles détectés (Attendu pour une grille)");

  identifier_points_critiques(g); // Affiche dans la console pour l'instant
  // TODO: Capturer le résultat si la fonction retournait une structure.

  // 4. Routage Normal
  // Coin haut gauche (0) -> Coin bas droite (24)
  log_info("Phase 4: Calcul Itinéraire Nominal (0 -> 24)...");
  Chemin *c = routage_dijkstra(g, 0, 24, "cout");
  if (c) {
    char buff[100];
    snprintf(buff, sizeof(buff), "Chemin trouvé, Longueur: %d, Coût: %.1f",
             c->longueur, c->cout_total);
    log_succes("Routage Dijkstra", buff);
    chemin_detruire(c);
  } else {
    log_echec("Routage Dijkstra", "Aucun chemin trouvé");
  }

  // 5. Simulation Panne
  log_info("Phase 5: Simulation de Panne Majeure...");
  // On coupe un lien critique ou central.
  // Dans une grille 5x5 (width 5). 0 est connecté à 1 et 5.
  // Coupons 0-1 et 0-5 ? Isolons partiellement.
  // Coupons juste le lien 0->1.
  graphe_supprimer_arete(g, 0, 1);
  graphe_supprimer_arete(g, 1, 0); // Bidirectionnel
  log_warning("Lien 0 <-> 1 coupé !");

  // 6. Reroutage
  log_info("Phase 6: Recalcul Itinéraire (Failover)...");
  Chemin *c2 = routage_dijkstra(g, 0, 24, "cout");
  if (c2) {
    char buff[100];
    snprintf(buff, sizeof(buff), "Chemin alternatif trouvé, Longueur: %d",
             c2->longueur);
    log_succes("Routage Secours", buff);

    // Verifions qu'il ne passe pas par 1 (sauf si 1 est dispo autrement mais
    // lien 0-1 coupé)
    int passe_par_1 = 0;
    // On sait que 0-1 est coupé. Donc il doit passer par 5 (le bas).
    if (c2->longueur > 1 && c2->noeuds[1] == 1)
      passe_par_1 = 1;

    if (!passe_par_1)
      log_succes("Vérification Route", "Le chemin contourne le lien coupé");
    else
      log_echec("Vérification Route",
                "Le chemin passe toujours par le lien coupé ! (Bug?)");

    chemin_detruire(c2);
  } else {
    log_echec("Routage Secours", "Isolement du noeud source ?");
  }

  // 7. Visualisation
  log_info("Phase 7: Génération des Artefacts Visuels...");
  const char *dot_file = "resultats_tests/simulation_visu.dot";
  const char *img_file = "resultats_tests/simulation_visu.png";

  graphe_vers_dot(g, dot_file);
  log_succes("Export DOT", dot_file);

  // Essai conversion PNG
  generer_image_depuis_dot(dot_file, img_file);

  graphe_detruire(g);
  log_section(
      "FIN SIMULATION",
      "Le système a démontré ses capacités de résilience et d'analyse.");
}

int main() {
  init_log_test();
  test_simulation_complete();
  return 0;
}
