/**
 * @file utils.h
 * @brief Module GSM : Utilitaires pour sortie JSON et parsing CLI
 *
 * Fournit des fonctions d'aide pour la communication entre
 * les binaires C et l'interface Desktop Python.
 */

#ifndef GSM_UTILS_H
#define GSM_UTILS_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*=============================================================================
 * SORTIE JSON STANDARDISÉE
 *===========================================================================*/

/**
 * @brief Écrit une réponse JSON de succès sur stdout
 * @param format Format printf pour le contenu JSON (sans les accolades
 * externes)
 */
void json_success(const char *format, ...);

/**
 * @brief Écrit une réponse JSON d'erreur sur stdout
 * @param message Message d'erreur
 */
void json_error(const char *message);

/**
 * @brief Écape une chaîne pour inclusion dans du JSON
 * @param input Chaîne à échapper
 * @param output Buffer de sortie (doit être au moins 2x input)
 * @param max_len Taille max du buffer de sortie
 */
void json_escape_string(const char *input, char *output, size_t max_len);

/*=============================================================================
 * PARSING D'ARGUMENTS
 *===========================================================================*/

/**
 * @brief Parse un argument comme entier avec valeur par défaut
 */
int parse_int_arg(const char *arg, int default_val);

/**
 * @brief Parse un argument comme flottant avec valeur par défaut
 */
float parse_float_arg(const char *arg, float default_val);

/**
 * @brief Parse un argument comme booléen (0/1, true/false)
 */
bool parse_bool_arg(const char *arg, bool default_val);

/**
 * @brief Vérifie le nombre minimum d'arguments
 * @return true si suffisant, false sinon (affiche usage)
 */
bool check_args(int argc, int min_required, const char *usage);

/*=============================================================================
 * UTILITAIRES FICHIERS
 *===========================================================================*/

/**
 * @brief Vérifie si un fichier existe
 */
bool file_exists(const char *path);

/**
 * @brief Crée le répertoire parent si nécessaire
 */
bool ensure_directory(const char *filepath);

#endif /* GSM_UTILS_H */
