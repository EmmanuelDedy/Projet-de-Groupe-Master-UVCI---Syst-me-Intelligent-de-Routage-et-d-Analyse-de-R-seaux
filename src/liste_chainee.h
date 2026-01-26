#ifndef LISTE_CHAINEE_H
#define LISTE_CHAINEE_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Structure pour un paquet
typedef struct Paquet {
  int id;
  int priorite;
  float taille_Mo;
  int source;
  int destination;
  clock_t temps_arrivee;
  struct Paquet *precedent;
  struct Paquet *suivant;
} Paquet;

// Structure pour la file de priorité
typedef struct FileAttente {
  Paquet *tete;
  Paquet *queue;
  int taille_actuelle;
  int capacite_max;

  // Statistiques
  int paquets_total;
  int paquets_defiles;
  int paquets_rejetes;
  double somme_attente;
} FileAttente;

// Déclaration des fonctions

void initialiser_file(FileAttente *file, int capacite_max);
Paquet *creer_paquet(int id, int priorite, float taille, int source,
                     int destination);
int enfiler(FileAttente *file, Paquet *nouveau_p);
Paquet *defiler(FileAttente *file);
Paquet *consulter(FileAttente *file);
void liberer_file(FileAttente *file);
void afficher_stats(FileAttente *file);

#endif // LISTE_CHAINEE_H