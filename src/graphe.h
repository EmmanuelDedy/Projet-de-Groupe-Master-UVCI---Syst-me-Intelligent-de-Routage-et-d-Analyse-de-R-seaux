#ifndef GRAPHE_H
#define GRAPHE_H

/**
 * @file graphe.h
 * @brief Module 1 : Modélisation et Structures de Données (Cœur du Système)
 * @author Équipe Master 1 UVCI
 * @date 2026-01-26
 *
 * Ce module définit les structures fondamentales pour la représentation du
 * réseau. Nous utilisons une **Modélisation Hybride** pour optimiser à la fois
 * le temps d'accès et l'occupation mémoire.
 *
 * @see ProjetAlgo2026.pdf (Section 3.1)
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Types d'équipements réseaux supportés
 * Utilisé pour la visualisation et les règles de sécurité (ex: un SWITCH ne
 * peut pas router).
 */
typedef enum {
  ROUTEUR,     /**< Équipement de couche 3 (Routage IP) */
  COMMUTATEUR, /**< Équipement de couche 2 (Switching) */
  SERVEUR,     /**< Point de terminaison (End system) */
  PARE_FEU     /**< Équipement de sécurité */
} TypeNoeud;

/**
 * @struct Metriques
 * @brief Vecteur de poids multidimensionnel pour une arête.
 *
 * Contrairement à un graphe simple où poids = distance, ici nous gérons 5
 * dimensions :
 * - **Cout** : Opex/Capex ou distance physique.
 * - **Latence** : Délai de propagation (ms).
 * - **Bande Passante** : Capacité du lien (Mbps).
 * - **Fiabilité** : Probabilité de panne (0.0 - 1.0).
 * - **Sécurité** : Niveau de confiance (int).
 */
typedef struct {
  float cout;           /**< Coût financier ou distance (km) */
  float latence;        /**< Latence (ms) - Critique pour VoIP */
  float bande_passante; /**< Bande passante (Mbps) - Critique pour Streaming */
  float fiabilite;      /**< Indice de fiabilité (0-10) ou probabilité */
  int securite;         /**< Niveau de sécurité (1-5) */
} Metriques;

/**
 * @struct Noeud
 * @brief Représentation d'un sommet du graphe (Équipement).
 */
typedef struct Noeud {
  int id;         /**< Identifiant unique (Index dans le tableau) */
  char nom[50];   /**< Nom mnémonique aka Hostname */
  TypeNoeud type; /**< Rôle de l'équipement */
  double x, y;    /**< Coordonnées géographiques (Visualisation) */
  bool est_actif; /**< État opérationnel (True=UP, False=DOWN) */
  struct AreteNoeud *liste_adj; /**< Tête de la liste d'adjacence (Voisins) */
} Noeud;

/**
 * @struct AreteNoeud
 * @brief Élément de liste chaînée représentant un arc sortant.
 *
 * Structure optimisée pour les graphes creux (Sparse Graphs) où |E| << |V|^2.
 * Chaque nœud possède une liste de ces structures.
 */
typedef struct AreteNoeud {
  int dest_id;                /**< ID du nœud de destination */
  Metriques metriques;        /**< Poids de l'arc */
  struct AreteNoeud *suivant; /**< Pointeur vers le prochain voisin */
} AreteNoeud;

/**
 * @struct Graphe
 * @brief Structure principale encapsulant la topologie réseau.
 *
 * Combine deux représentations pour la performance :
 * 1. **Tableau de Nœuds** : Accès O(1) aux métadonnées des sommets.
 * 2. **Matrice d'Adjacence** (Optionnelle) : Pour les graphes denses.
 * 3. **Listes d'Adjacence** : Pour le parcours efficace des voisins.
 */
typedef struct {
  int nb_noeuds;        /**< Nombre actuel de sommets */
  int max_noeuds;       /**< Capacité maximale allouée */
  Noeud *noeuds;        /**< Tableau dynamique des sommets */
  double **matrice_adj; /**< Matrice d'adjacence (Poids simple ou NULL) */
  bool est_oriente;     /**< Type de graphe (Orienté ou Non) */
} Graphe;

/* --- API PUBLIQUE --- */

/**
 * @brief Alloue et initialise un graphe vide.
 * @param max_noeuds Capacité initiale pré-allouée.
 * @param est_oriente true pour un graphe orienté, false pour non-orienté.
 * @return Graphe* Pointeur vers la structure allouée ou NULL en cas d'erreur.
 */
Graphe *graphe_creer(int max_noeuds, bool est_oriente);

/**
 * @brief Libère toute la mémoire associée au graphe (Nœuds, Arêtes, Matrice).
 * @param g Pointeur vers le graphe.
 */
void graphe_detruire(Graphe *g);

/* Gestion des Nœuds et Arêtes */
int graphe_ajouter_noeud(Graphe *g, TypeNoeud type, double x, double y);
int graphe_ajouter_noeud_nomme(Graphe *g, const char *nom, TypeNoeud type,
                               double x, double y);

/**
 * @brief Ajoute un arc pondéré entre deux nœuds.
 * @param g Le graphe.
 * @param src ID source.
 * @param dest ID destination.
 * @param m Métriques complètes de l'arc.
 * @return true si succès, false si nœuds invalides.
 */
bool graphe_ajouter_arete(Graphe *g, int src, int dest, Metriques m);

bool graphe_supprimer_arete(Graphe *g, int src, int dest);
bool graphe_supprimer_noeud(Graphe *g, int id);

/* Utilitaires */
void graphe_afficher(Graphe *g);

/**
 * @brief Exporte la topologie au format Graphviz (.dot).
 * Permet la visualisation graphique via l'outil `dot` ou `neato`.
 * @param g Le graphe.
 * @param nom_fichier Chemin de sortie.
 */
void graphe_vers_dot(Graphe *g, const char *nom_fichier);

/**
 * @brief Persistance : Sauvegarde la topologie sur disque.
 * Format texte custom : V (Vertices) / E (Edges).
 */
bool graphe_sauvegarder(Graphe *g, const char *nom_fichier);

/**
 * @brief Persistance : Charge une topologie depuis le disque.
 * @return Graphe* Nouveau graphe instancié avec les données du fichier.
 */
Graphe *graphe_charger(const char *nom_fichier);

#endif /* GRAPHE_H */
