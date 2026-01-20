/**
 * @file test_liste_chainee.c
 * @brief Tests unitaires du module liste chaînée (file de priorité)
 *
 * Ce fichier contient des tests unitaires permettant de valider les fonctionnalités principales du module liste chaînée :
 *  - initialisation de la file
 *  - insertion de paquets selon leur priorité
 *  - retrait des paquets
 *  - consultation de la tête de file
 *  - mise à jour des statistiques
 **/

#include <assert.h>
#include <stdio.h>
#include "../src/liste_chainee.h"

int main() {

    FileAttente f;

    /* Test d'initialisation */
    initialiserFile(&f, 5);
    assert(f.tete == NULL);
    assert(f.queue == NULL);
    assert(f.taille_actuelle == 0);

    /* Création de paquets */
    Paquet* p1 = creerPaquet(1, 2, 1.0, 0, 1);
    Paquet* p2 = creerPaquet(2, 1, 1.0, 0, 1);
    Paquet* p3 = creerPaquet(3, 3, 1.0, 0, 1);

    assert(p1 && p2 && p3);

    /* Insertion des paquets (file de priorité) */
    assert(enfiler(&f, p1) == 0);
    assert(enfiler(&f, p2) == 0);
    assert(enfiler(&f, p3) == 0);

    /* Vérification de l’ordre de priorité */
    assert(f.tete->id == 2);            // priorité la plus élevée (1)
    assert(f.tete->suivant->id == 1);   // priorité intermédiaire (2)
    assert(f.queue->id == 3);           // priorité la plus faible (3)

    /* Test du défilement */
    Paquet* p = defiler(&f);
    assert(p->id == 2);
    free(p);

    assert(f.taille_actuelle == 2);

    /* Test de consultation */
    assert(consulter(&f)->id == 1);

    /* Libération de la file */
    libererFile(&f);
    assert(f.tete == NULL);
    assert(f.queue == NULL);

    printf("Tests unitaires liste chaînée : OK\n");
    return 0;
}
