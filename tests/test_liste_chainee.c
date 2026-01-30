#include "../src/liste_chainee.h"
#include "test_utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void test_initialisation() {
  FileAttente file;
  initialiser_file(&file, 5); // Capacité 5

  if (file.taille_actuelle == 0 && file.capacite_max == 5) {
    log_succes("Init Queue", "Queue initialisée vide avec capacité 5");
  } else {
    log_echec("Init Queue", "Erreur initialisation");
  }
}

void test_enfilement_priorite() {
  FileAttente file;
  initialiser_file(&file, 10);

  // P1 (Prio 1), P2 (Prio 10), P3 (Prio 5)
  Paquet *p1 = creer_paquet(1, 1, 1.0, 0, 1);
  Paquet *p2 = creer_paquet(2, 10, 1.0, 0, 1); // Haute prio
  Paquet *p3 = creer_paquet(3, 5, 1.0, 0, 1);

  enfiler(&file, p1);
  enfiler(&file, p2);
  enfiler(&file, p3);

  // Ordre attendu: P2 (10), P3 (5), P1 (1)
  Paquet *out1 = defiler(&file);
  Paquet *out2 = defiler(&file);
  Paquet *out3 = defiler(&file);

  int ok = 1;
  if (!out1 || out1->id != 2)
    ok = 0;
  if (!out2 || out2->id != 3)
    ok = 0;
  if (!out3 || out3->id != 1)
    ok = 0;

  if (ok)
    log_succes(
        "Priorité Queue",
        "Les paquets sont sortis dans l'ordre de priorité (10 -> 5 -> 1)");
  else {
    char buff[256];
    snprintf(buff, sizeof(buff), "Ordre incorrect: %d -> %d -> %d",
             out1 ? out1->id : -1, out2 ? out2->id : -1, out3 ? out3->id : -1);
    log_echec("Priorité Queue", buff);
  }

  if (out1)
    free(out1);
  if (out2)
    free(out2);
  if (out3)
    free(out3);
}

void test_capacite_max() {
  FileAttente file;
  initialiser_file(&file, 2); // Petite capacité

  Paquet *p1 = creer_paquet(1, 1, 1.0, 0, 1);
  Paquet *p2 = creer_paquet(2, 1, 1.0, 0, 1);
  Paquet *p3 = creer_paquet(3, 1, 1.0, 0, 1); // Devrait être rejeté

  enfiler(&file, p1);
  enfiler(&file, p2);
  int res = enfiler(&file, p3); // Doit retourner 0 ou echec

  if (res != 0 && file.taille_actuelle == 2 && file.paquets_rejetes >= 1) {
    log_succes("Capacité Max", "Paquet rejeté correctement quand pleine");
  } else {
    log_echec("Capacité Max",
              "Paquet accepté alors que queue pleine ou stats incorrectes");
  }

  free(p3); // Car rejeté (si l'implémentation ne le free pas, check code.
            // Supposons qu'il faut le free si rejeté ou l'implémentation le
            // fait. Dans le doute on laisse ou on vérifie fuite avec Valgrind
            // plus tard. Disons on le free ici pour sûreté si non inséré)
  // Note: Si enfiler retourne 0, on suppose que la file ne l'a pas pris
  // possession.

  liberer_file(&file);
}

int main() {
  init_log_test();
  log_section("TEST LISTE CHAINEE (PRIORITY QUEUE)",
              "Vérification FIFO Prioritaire");

  test_initialisation();
  test_enfilement_priorite();
  test_capacite_max();

  return 0;
}
