#ifndef FILE_PRIORITE_H   
#define FILE_PRIORITE_H

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
    struct Paquet* precedent;   
    struct Paquet* suivant; 
} Paquet;

// Structure pour la file de priorité
typedef struct FileAttente {
    Paquet* tete;             
    Paquet* queue;              
    int taille_actuelle;        
    int capacite_max; 

    // Statistiques
    int paquets_total;        
    int paquets_defiles;      
    int paquets_rejetes;      
    double somme_attente;       
} FileAttente;

// Déclaration des fonctions

void initialiserFile(FileAttente* f, int capacite_max);
Paquet* creerPaquet(int id, int priorite, float taille, int source, int destination);
int enfiler(FileAttente* f, Paquet* Nouveau_p);
Paquet* defiler(FileAttente* f);
Paquet* consulter(FileAttente* f);
void libererFile(FileAttente* f);
void afficherStats(FileAttente* f);

#endif
