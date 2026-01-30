#ifndef GRAPHE_H
#define GRAPHE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Types de nœuds dans le réseau
 */
typedef enum { ROUTEUR, COMMUTATEUR, SERVEUR, PARE_FEU } TypeNoeud;

/**
 * @brief Métriques 4D pour une arête
 */
typedef struct {
  float cout;           /**< Coût de transmission */
  float latence;        /**< Latence en ms */
  float bande_passante; /**< Bande passante en Mbps */
  float fiabilite;      /**< Indice de fiabilité (0-10) */
  int securite;         /**< Niveau de sécurité (requis par le PDF) */
} Metriques;

/**
 * @brief Informations sur un nœud
 */
typedef struct Noeud {
  int id;
  char nom[50]; /**< Nom du nœud (requis par le PDF) */
  TypeNoeud type;
  double x, y; /**< Position géographique/logique */
  bool est_actif;
  struct AreteNoeud *liste_adj; /**< Liste d'adjacence pour les graphes creux */
} Noeud;

/**
 * @brief Élément de la liste d'adjacence
 */
typedef struct AreteNoeud {
  int dest_id;
  Metriques metriques;
  struct AreteNoeud *suivant;
} AreteNoeud;

/**
 * @brief Structure du graphe représentant le réseau
 */
typedef struct {
  int nb_noeuds;
  int max_noeuds;
  Noeud *noeuds;
  double **matrice_adj; /**< Matrice d'adjacence pour les graphes denses */
  bool est_oriente;
} Graphe;

/* Constructeur et Destructeur */
Graphe *graphe_creer(int max_noeuds, bool est_oriente);
void graphe_detruire(Graphe *g);

/* Gestion des Nœuds et Arêtes */
int graphe_ajouter_noeud(Graphe *g, TypeNoeud type, double x, double y);
int graphe_ajouter_noeud_nomme(Graphe *g, const char *nom, TypeNoeud type,
                               double x, double y);
bool graphe_ajouter_arete(Graphe *g, int src, int dest, Metriques m);
bool graphe_supprimer_arete(Graphe *g, int src, int dest);
bool graphe_supprimer_noeud(Graphe *g, int id);

/* Utilitaires */
void graphe_afficher(Graphe *g);
void graphe_vers_dot(Graphe *g, const char *nom_fichier);

/**
 * @brief Sauvegarde la topologie du réseau dans un fichier texte.
 */
bool graphe_sauvegarder(Graphe *g, const char *nom_fichier);

/**
 * @brief Charge la topologie du réseau depuis un fichier texte.
 */
Graphe *graphe_charger(const char *nom_fichier);

#endif /* GRAPHE_H */