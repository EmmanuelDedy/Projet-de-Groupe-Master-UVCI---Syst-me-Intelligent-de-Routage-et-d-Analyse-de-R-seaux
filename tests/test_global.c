#include "graphe.h"
#include "routage.h"
#include "securite.h"
#include <assert.h>
#include <stdio.h>

int main() {
  printf("=== TEST INTEGRATION GLOBAL ===\n");

  // 1. Creation d'une topologie reseau type "Entreprise"
  printf("1. Construction du reseau...");
  Graphe *g = graphe_creer(10, true);

  // Core (0,1)
  graphe_ajouter_noeud_nomme(g, "Core1", ROUTEUR, 0, 0);
  graphe_ajouter_noeud_nomme(g, "Core2", ROUTEUR, 10, 0);

  // Distribution (2,3)
  graphe_ajouter_noeud_nomme(g, "Dist1", COMMUTATEUR, 0, 10);
  graphe_ajouter_noeud_nomme(g, "Dist2", COMMUTATEUR, 10, 10);

  // Access (4,5)
  graphe_ajouter_noeud_nomme(g, "Access1", COMMUTATEUR, 0, 20);
  graphe_ajouter_noeud_nomme(g, "Access2", COMMUTATEUR, 10, 20);

  // Servers (6)
  graphe_ajouter_noeud_nomme(g, "ServerFarm", SERVEUR, 5, -10);

  // Liens Core-Core (Haute dispo)
  Metriques m_10g = {1, 1, 10000, 10, 10};
  graphe_ajouter_arete(g, 0, 1, m_10g);
  graphe_ajouter_arete(g, 1, 0, m_10g);

  // Liens Core-Dist
  Metriques m_1g = {10, 2, 1000, 8, 8};
  graphe_ajouter_arete(g, 0, 2, m_1g);
  graphe_ajouter_arete(g, 1, 3, m_1g);
  // Redondance croisee
  graphe_ajouter_arete(g, 0, 3, m_1g);
  graphe_ajouter_arete(g, 1, 2, m_1g);

  // Liens Dist-Access
  graphe_ajouter_arete(g, 2, 4, m_1g);
  graphe_ajouter_arete(g, 3, 5, m_1g);

  // Acces vers Core (Retour pour le routage bi-directionnel simplifie)
  graphe_ajouter_arete(g, 4, 2, m_1g);
  graphe_ajouter_arete(g, 5, 3, m_1g);
  graphe_ajouter_arete(g, 2, 0, m_1g);
  graphe_ajouter_arete(g, 3, 1, m_1g);

  printf(" OK\n");

  // 2. Verification Sante Reseau
  printf("2. Audit Securite...");
  int cycles = detecter_cycles(g);
  if (cycles)
    printf(" [Info] Redondance (cycles) detectee, normal pour ce design.");
  // Ce test passe toujours, c'est juste informatif ici

  // 3. Test Routage Access -> Core -> Server (Hypothetique)
  // Ajoutons le serveur connecte au Core1
  graphe_ajouter_arete(g, 0, 6, m_10g);
  graphe_ajouter_arete(g, 6, 0, m_10g);

  printf("\n3. Calcul Routage (Access1 -> ServerFarm)...");
  // Access1(4) -> Dist1(2) -> Core1(0) -> ServerFarm(6)
  Chemin *c = routage_dijkstra(g, 4, 6, "latence");

  assert(c != NULL);
  printf(" Chemin trouve ! Longueur: %d sauts\n", c->longueur);
  assert(c->noeuds[0] == 4);
  assert(c->noeuds[c->longueur - 1] == 6);

  chemin_detruire(c);

  // 4. Test Panne (Suppression lien Dist1->Core1) et Reroutage
  printf("4. Simulation Panne (Dist1->Core1 HS)...");
  graphe_supprimer_arete(g, 2, 0); // Coupe le lien principal

  // Access1(4) -> Dist1(2) -> Core2(1) -> Core1(0) -> ServerFarm(6)
  // ou via la redondance croisee si implementee correctement (Dist1 a lien vers
  // Core2 ?) J'ai mis: graphe_ajouter_arete(g, 1, 2, m_1g); (Core2 -> Dist1).
  // Mais Dist1 -> Core2 ? Non, je n'ai pas mis Dist1 -> Core2 explicitement
  // dans la liste ci-dessus ("Redondance croisee" commentaire mais code a
  // verifier) Core1->Dist1, Core1->Dist2, Core2->Dist2, Core2->Dist1 (lignes
  // 35-39). Ah les liens montants (Dist->Core) sont lignes 43-46. 2->0
  // (Dist1->Core1) et 3->1 (Dist2->Core2). Pas de lien croise montant 2->1 dans
  // mon setup initial. Ajoutons le pour la robustesse du test.
  graphe_ajouter_arete(g, 2, 1, m_1g); // Dist1 -> Core2

  // Recalcul
  Chemin *c_failover = routage_dijkstra(g, 4, 6, "cout");
  assert(c_failover != NULL);
  printf(" Failover reussi via chemin alternatif !\n");
  // On s'attend a passer par 4 -> 2 -> 1 -> 0 -> 6
  if (c_failover->longueur > 0) {
    printf("Trajet secours: ");
    for (int i = 0; i < c_failover->longueur; i++)
      printf("%d ", c_failover->noeuds[i]);
    printf("\n");
  }
  chemin_detruire(c_failover);

  graphe_detruire(g);
  printf("\n=== TEST INTEGRATION TERMINE AVEC SUCCES ===\n");
  return 0;
}
