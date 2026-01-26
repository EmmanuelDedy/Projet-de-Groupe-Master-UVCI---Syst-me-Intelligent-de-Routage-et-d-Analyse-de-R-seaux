/**
 * @file utils.c
 * @brief Module GSM : Implémentation des utilitaires
 */

#include "utils.h"
#include <stdarg.h>
#include <sys/stat.h>

/*=============================================================================
 * SORTIE JSON STANDARDISÉE
 *===========================================================================*/

void json_success(const char *format, ...) {
  printf("{\"success\": true");
  if (format && format[0] != '\0') {
    printf(", ");
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
  }
  printf("}\n");
  fflush(stdout);
}

void json_error(const char *message) {
  char escaped[512];
  json_escape_string(message, escaped, sizeof(escaped));
  printf("{\"success\": false, \"error\": \"%s\"}\n", escaped);
  fflush(stdout);
}

void json_escape_string(const char *input, char *output, size_t max_len) {
  if (!input || !output || max_len == 0)
    return;

  size_t j = 0;
  for (size_t i = 0; input[i] != '\0' && j < max_len - 1; i++) {
    char c = input[i];
    if (c == '"' || c == '\\') {
      if (j + 2 >= max_len)
        break;
      output[j++] = '\\';
      output[j++] = c;
    } else if (c == '\n') {
      if (j + 2 >= max_len)
        break;
      output[j++] = '\\';
      output[j++] = 'n';
    } else if (c == '\r') {
      if (j + 2 >= max_len)
        break;
      output[j++] = '\\';
      output[j++] = 'r';
    } else if (c == '\t') {
      if (j + 2 >= max_len)
        break;
      output[j++] = '\\';
      output[j++] = 't';
    } else {
      output[j++] = c;
    }
  }
  output[j] = '\0';
}

/*=============================================================================
 * PARSING D'ARGUMENTS
 *===========================================================================*/

int parse_int_arg(const char *arg, int default_val) {
  if (!arg || arg[0] == '\0')
    return default_val;
  char *end;
  long val = strtol(arg, &end, 10);
  if (end == arg)
    return default_val;
  return (int)val;
}

float parse_float_arg(const char *arg, float default_val) {
  if (!arg || arg[0] == '\0')
    return default_val;
  char *end;
  float val = strtof(arg, &end);
  if (end == arg)
    return default_val;
  return val;
}

bool parse_bool_arg(const char *arg, bool default_val) {
  if (!arg || arg[0] == '\0')
    return default_val;
  if (strcmp(arg, "1") == 0 || strcmp(arg, "true") == 0 ||
      strcmp(arg, "yes") == 0 || strcmp(arg, "vrai") == 0 ||
      strcmp(arg, "oui") == 0) {
    return true;
  }
  if (strcmp(arg, "0") == 0 || strcmp(arg, "false") == 0 ||
      strcmp(arg, "no") == 0 || strcmp(arg, "faux") == 0 ||
      strcmp(arg, "non") == 0) {
    return false;
  }
  return default_val;
}

bool check_args(int argc, int min_required, const char *usage) {
  if (argc < min_required) {
    fprintf(stderr, "Usage : %s\n", usage);
    json_error("Arguments insuffisants");
    return false;
  }
  return true;
}

/*=============================================================================
 * UTILITAIRES FICHIERS
 *===========================================================================*/

bool file_exists(const char *path) {
  if (!path)
    return false;
  struct stat st;
  return (stat(path, &st) == 0);
}

bool ensure_directory(const char *filepath) {
  if (!filepath)
    return false;

  // Extraire le répertoire parent
  char dir[512];
  strncpy(dir, filepath, sizeof(dir) - 1);
  dir[sizeof(dir) - 1] = '\0';

  // Trouver le dernier séparateur
  char *last_sep = strrchr(dir, '/');
  if (!last_sep)
    return true; // Pas de répertoire parent

  *last_sep = '\0'; // Tronquer au répertoire parent

  // Créer récursivement (simple, un seul niveau)
  struct stat st;
  if (stat(dir, &st) != 0) {
#ifdef _WIN32
    return mkdir(dir) == 0;
#else
    return mkdir(dir, 0755) == 0;
#endif
  }
  return true;
}
