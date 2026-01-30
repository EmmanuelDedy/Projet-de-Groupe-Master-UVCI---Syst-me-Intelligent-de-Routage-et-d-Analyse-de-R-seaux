#ifndef LISTE_CHAINEE_H
#define LISTE_CHAINEE_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * @file liste_chainee.h
 * @brief Module 4 : Simulation et Gestion de Flux (QoS)
 * @author Équipe Master 1 UVCI
 *
 * Gestion d'une **File d'Attente à Priorité** (Priority Queue).
 * Contrairement à une file FIFO standard, les éléments sont insérés
 * en fonction de leur importance (Priorité 0-10).
 *
 * @struct Paquet
 * @brief Unité de données transitant sur le réseau.
 */
typedef struct Paquet {
  int id;                   /**< ID unique du paquet */
  int priorite;             /**< 0 (Fond) à 10 (Temps Réel) */
  float taille_Mo;          /**< Taille pour calcul de la bande passante */
  int source;               /**< ID source */
  int destination;          /**< ID destination */
  clock_t temps_arrivee;    /**< Timestamp pour calcul de la gigue (Jitter) */
  struct Paquet *precedent; /**< Liste doublement chaînée */
  struct Paquet *suivant;
} Paquet;

/**
 * @struct FileAttente
 * @brief Gestionnaire de file prioritaire.
 */
typedef struct FileAttente {
  Paquet *tete;        /**< Premier élément (Plus haute priorité) */
  Paquet *queue;       /**< Dernier élément (Plus basse priorité) */
  int taille_actuelle; /**< Nombre de paquets en attente */
  int capacite_max;    /**< Buffer overflow si dépassé */

  // Statistiques
  int paquets_total;    /**< Total traités depuis le début */
  int paquets_defiles;  /**< Total servis */
  int paquets_rejetes;  /**< Total perdus (Drop Tail) */
  double somme_attente; /**< Pour calcul moyenne attente */
} FileAttente;

// Déclaration des fonctions

/**
 * @brief Initialise une file vide.
 * @param capacite_max Taille du buffer (en nombre de paquets).
 */
void initialiser_file(FileAttente *file, int capacite_max);

/**
 * @brief Fabrique un nouveau paquet (Factory).
 */
Paquet *creer_paquet(int id, int priorite, float taille, int source,
                     int destination);

/**
 * @brief Insère un paquet selon sa priorité (Ordre Décroissant).
 * Complexité : O(N) dans le pire des cas (Insertion fin).
 * @return 1 si succès, 0 si file pleine (Drop).
 */
int enfiler(FileAttente *file, Paquet *nouveau_p);

/**
 * @brief Extrait le paquet le plus prioritaire (Tête de file).
 * Complexité : O(1).
 */
Paquet *defiler(FileAttente *file);

/**
 * @brief Regarde le prochain paquet sans l'extraire (Peek).
 */
Paquet *consulter(FileAttente *file);

void liberer_file(FileAttente *file);
void afficher_stats(FileAttente *file);

#endif // LISTE_CHAINEE_H