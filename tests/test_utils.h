#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include "../src/graphe.h"
#include <stdio.h>

// Types de topologies
typedef enum {
  TOPO_SIMPLE,   // 5-10 noeuds
  TOPO_MAILLAGE, // Maillage complet (Mesh)
  TOPO_ETOILE,   // Etoile (Star)
  TOPO_ANNEAU,   // Anneau (Ring)
  TOPO_ARBRE,    // Arbre (Tree)
  TOPO_GRILLE    // Grille (Grid)
} TypeTopologie;

// Couleurs pour le terminal
#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN "\033[36m"
#define COLOR_BOLD "\033[1m"

/**
 * @brief Crée un graphe selon une topologie donnée
 * @param type Type de topologie
 * @param nb_noeuds Nombre de noeuds (approximatif pour certaines topologies)
 * @return Pointeur vers le Graphe créé
 */
Graphe *creer_graphe_test(TypeTopologie type, int nb_noeuds);

/**
 * @brief Initialise le système de log (crée le dossier/fichier)
 */
void init_log_test();

// --- Nouveau Système de Log ---

void log_section(const char *titrem, const char *desc);
void log_info(const char *msg);
void log_succes(const char *test_name, const char *details);
void log_echec(const char *test_name, const char *details);
void log_warning(const char *msg);

// --- Visualisation ---

/**
 * @brief Convertit un fichier .dot en image .png via la commande 'dot'
 * (Graphviz)
 * @param dot_file Chemin du fichier source .dot
 * @param png_output Chemin du fichier de sortie .png
 * @return 1 si succès, 0 sinon
 */
int generer_image_depuis_dot(const char *dot_file, const char *png_output);

// Deprecated but kept for compatibility during refactor
void log_resultat_test(const char *nom_test, int statut, const char *details);

/**
 * @brief Helper : Ajoute une arête bidirectionnelle avec métriques par défaut
 */
void ajouter_arete_bidirectionnelle(Graphe *g, int u, int v, float cout,
                                    float latence, float bp, int sec);

#endif
