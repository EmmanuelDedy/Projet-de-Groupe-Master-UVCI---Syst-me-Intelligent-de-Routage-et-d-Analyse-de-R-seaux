/**
 * @file test_liste_chainee.c
 * @brief Tests unitaires du module liste chaînée (file de priorité)
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "../src/liste_chainee.h"

int main() {

    FileAttente f;

    /* Test d'initialisation */
    initialiser_file(&f, 5);
    assert(f.tete == NULL);
    assert(f.queue == NULL);
    assert(f.taille_actuelle == 0);

    /* Création de paquets */
    Paquet* p1 = creer_paquet(1, 2, 1.0, 0, 1);
    Paquet* p2 = creer_paquet(2, 1, 1.0, 0, 1);
    Paquet* p3 = creer_paquet(3, 3, 1.0, 0, 1);

    assert(p1 && p2 && p3);

    /* Insertion des paquets */
    assert(enfiler(&f, p1) == 0);
    assert(enfiler(&f, p2) == 0);
    assert(enfiler(&f, p3) == 0);

    /* Vérification de l’ordre de priorité */
    assert(f.tete->id == 2);
    assert(f.tete->suivant->id == 1);
    assert(f.queue->id == 3);

    /* Test du défilement */
    Paquet* p = defiler(&f);
    assert(p->id == 2);
    free(p);

    assert(f.taille_actuelle == 2);

    /* Test de consultation */
    assert(consulter(&f)->id == 1);

    /* Libération de la file */
    liberer_file(&f);
    assert(f.tete == NULL);
    assert(f.queue == NULL);

    printf("Tests unitaires liste chaînée : OK\n");
    return 0;
}