#include "liste_chainee.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// INITIALISATION DE LA FILE//

void initialiser_file(FileAttente *file, int capacite_max) {
  if (!file)
    return;
  file->tete = NULL;         // Aucun paquet en tête
  file->queue = NULL;        // Aucun paquet en queue
  file->taille_actuelle = 0; // File vide au départ
  file->capacite_max = capacite_max;

  file->paquets_total = 0;
  file->paquets_defiles = 0;
  file->paquets_rejetes = 0;
  file->somme_attente = 0.0;
}

// CRÉATION D'UN PAQUET //

Paquet *creer_paquet(int id, int priorite, float taille, int source,
                     int destination) {

  // Allocation de la mémoire au paquet
  Paquet *nouveau_p = (Paquet *)malloc(sizeof(Paquet));

  if (nouveau_p == NULL)
    return NULL;

  // Initialisation des champs
  nouveau_p->id = id;
  nouveau_p->priorite = priorite;
  nouveau_p->taille_Mo = taille;
  nouveau_p->source = source;
  nouveau_p->destination = destination;
  nouveau_p->temps_arrivee = clock();

  nouveau_p->precedent = NULL;
  nouveau_p->suivant = NULL;

  return nouveau_p;
}

// ENQUEUE //

int enfiler(FileAttente *file, Paquet *nouveau_p) {
  if (!file || !nouveau_p)
    return -1;
  file->paquets_total++;

  // On vérifie si la file est pleine
  if (file->capacite_max != -1 && file->taille_actuelle >= file->capacite_max) {
    file->paquets_rejetes++;
    return -1; // le paquet est rejeté
  }

  // Quand la file est vide
  if (file->tete == NULL) {
    file->tete = nouveau_p;
    file->queue = nouveau_p;
  } else {
    Paquet *paquet_actu = file->tete;

    // Insertion du paquet selon la priorité (Décroissante: Plus grand = Plus
    // prioritaire)
    while (paquet_actu != NULL &&
           paquet_actu->priorite >= nouveau_p->priorite) {
      paquet_actu = paquet_actu->suivant;
    }

    //  Insertion en tête
    if (paquet_actu == file->tete) {
      nouveau_p->suivant = file->tete;
      file->tete->precedent = nouveau_p;
      file->tete = nouveau_p;
    }
    // Insertion en queue
    else if (paquet_actu == NULL) {
      nouveau_p->precedent = file->queue;
      file->queue->suivant = nouveau_p;
      file->queue = nouveau_p;
    }
    // Insertion au milieu de la file
    else {
      nouveau_p->suivant = paquet_actu;
      nouveau_p->precedent = paquet_actu->precedent;
      paquet_actu->precedent->suivant = nouveau_p;
      paquet_actu->precedent = nouveau_p;
    }
  }

  // Mise à jour de la taille de la file
  file->taille_actuelle++;
  return 0;
}

// DEFILER//

Paquet *defiler(FileAttente *file) {
  if (!file)
    return NULL;

  // On vérifie si la file est vide
  if (file->tete == NULL)
    return NULL;

  // Récupération du paquet en tête
  Paquet *nouveau_p = file->tete;
  file->tete = nouveau_p->suivant;

  if (file->tete != NULL)
    file->tete->precedent = NULL;
  else
    file->queue = NULL;

  nouveau_p->suivant = NULL;
  nouveau_p->precedent = NULL;

  // Calcul du temps d'attente
  clock_t temps_sortie = clock();
  double attente =
      (double)(temps_sortie - nouveau_p->temps_arrivee) / CLOCKS_PER_SEC;
  file->somme_attente += attente;

  // Mise à jour de la taille
  file->taille_actuelle--;
  file->paquets_defiles++;

  return nouveau_p;
}

// PEEK//

Paquet *consulter(FileAttente *file) {
  if (!file)
    return NULL;
  return file->tete; // Aucun retrait
}

// LIBÉRATION DE LA MÉMOIRE//

void liberer_file(FileAttente *file) {
  if (!file)
    return;

  Paquet *nouveau_p = file->tete;

  // Parcours et libération de tous les paquets
  while (nouveau_p != NULL) {
    Paquet *temp = nouveau_p;
    nouveau_p = nouveau_p->suivant;
    free(temp);
  }

  // Réinitialisation de la file
  file->tete = NULL;
  file->queue = NULL;
  file->taille_actuelle = 0;
  file->paquets_total = 0;
  file->paquets_defiles = 0;
  file->paquets_rejetes = 0;
  file->somme_attente = 0.0;
}
// Affichage des statistiques

void afficher_stats(FileAttente *file) {
  if (!file)
    return;
  printf("\n STATISTIQUES \n");
  printf("Paquets envoyés : %d\n", file->paquets_total);
  printf("Paquets traités : %d\n", file->paquets_defiles);
  printf("Paquets perdus : %d\n", file->paquets_rejetes);

  // Calcul du temps moyen d'attente
  double temps_moyen = (file->paquets_defiles > 0)
                           ? (file->somme_attente / file->paquets_defiles)
                           : 0.0;

  // calcul du taux de pertes
  double taux_perte =
      (file->paquets_total > 0)
          ? ((double)file->paquets_rejetes / file->paquets_total) * 100
          : 0.0;

  // Calcul du débit effectif
  double debit = (file->somme_attente > 0)
                     ? ((double)file->paquets_defiles / file->somme_attente)
                     : 0.0;

  printf("Temps moyen d'attente : %.4f sec\n", temps_moyen);
  printf("Taux de perte : %.2f%%\n", taux_perte);
  printf("Débit effectif : %.2f paquets/s\n", debit);
}
