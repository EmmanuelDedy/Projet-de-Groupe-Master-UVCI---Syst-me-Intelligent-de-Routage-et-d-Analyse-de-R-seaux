#include <stdio.h>
#include <stdlib.h>
#include <time.h>
  // Liste Chaînée Double (pour file de priorité) //

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


typedef struct FileAttente {
    Paquet* tete;             
    Paquet* queue;              
    int taille_actuelle;        
    int capacite_max; 
    // pour le calcule des statistiques
     int paquets_total;        
    int paquets_defiles;      
    int paquets_rejetes;      
    double somme_attente;       
} FileAttente;


   //INITIALISATION DE LA FILE//
 
void initialiserFile(FileAttente* f, int capacite_max) {
    f->tete = NULL;             // Aucun paquet en tête
    f->queue = NULL;            // Aucun paquet en queue
    f->taille_actuelle = 0;     // File vide au départ
    f->capacite_max = capacite_max;

     f->paquets_total = 0;
    f->paquets_defiles = 0;
    f->paquets_rejetes = 0;
    f->somme_attente = 0.0;
}


   //CRÉATION D'UN PAQUET //
 
Paquet* creerPaquet(int id, int priorite, float taille,
                    int source, int destination) {

    // Allocation de la mémoire au paquet
    Paquet* Nouveau_p = (Paquet*)malloc(sizeof(Paquet));

    if (Nouveau_p == NULL)
        return NULL;

    // Initialisation des champs
    Nouveau_p->id = id;
    Nouveau_p->priorite = priorite;
    Nouveau_p->taille_Mo = taille;
    Nouveau_p->source = source;
    Nouveau_p->destination = destination;
     Nouveau_p->temps_arrivee = clock();

    Nouveau_p->precedent = NULL;
    Nouveau_p->suivant = NULL;

    return Nouveau_p;
}

  // ENQUEUE //

int enfiler(FileAttente* f, Paquet* Nouveau_p) {
    f->paquets_total++; 

    // On vérifie si la file est pleine
    if (f->capacite_max != -1 &&
        f->taille_actuelle >= f->capacite_max) {
             f->paquets_rejetes++;
        return -1;              // le paquet est rejeté
    }

    // Quand la file est vide
    if (f->tete == NULL) {
        f->tete = Nouveau_p;
        f->queue = Nouveau_p;
    }
    else {
        Paquet* Paquet_actu = f->tete;

        // Insertion du paquet selon la priorité
        while (Paquet_actu != NULL &&
               Paquet_actu->priorite <= Nouveau_p->priorite) {
            Paquet_actu =Paquet_actu->suivant;
        }

        //  Insertion en tête 
        if (Paquet_actu == f->tete) {
            Nouveau_p->suivant = f->tete;
            f->tete->precedent = Nouveau_p;
            f->tete = Nouveau_p;
        }
        // Insertion en queue 
        else if (Paquet_actu == NULL) {
            Nouveau_p->precedent = f->queue;
            f->queue->suivant = Nouveau_p;
            f->queue = Nouveau_p;
        }
        // Insertion au milieu de la file
        else {
            Nouveau_p->suivant = Paquet_actu;
            Nouveau_p->precedent = Paquet_actu->precedent;
            Paquet_actu->precedent->suivant = Nouveau_p;
            Paquet_actu->precedent = Nouveau_p;
        }
    }

    // Mise à jour de la taille de la file
    f->taille_actuelle++;
    return 0;                
}


   //DEFILER//

Paquet* defiler(FileAttente* f) {

    // On vérifie si la file est vide
    if (f->tete == NULL)
        return NULL;

    // Récupération du paquet en tête
    Paquet* Nouveau_p = f->tete;
    f->tete = Nouveau_p->suivant;

    
    if (f->tete != NULL)
        f->tete->precedent = NULL;
    else
        f->queue = NULL;       

  
    Nouveau_p->suivant = NULL;
    Nouveau_p->precedent = NULL;

    //Calcul du temps d'attente 
     clock_t temps_sortie = clock();
    double attente = (double)(temps_sortie - Nouveau_p->temps_arrivee) / CLOCKS_PER_SEC;
    f->somme_attente += attente;


    // Mise à jour de la taille
    f->taille_actuelle--;
     f->paquets_defiles++;

    return Nouveau_p;
}


  //PEEK//
 
Paquet* consulter(FileAttente* f) {
    return f->tete;             // Aucun retrait
}


   //LIBÉRATION DE LA MÉMOIRE//
   
void libererFile(FileAttente* f) {

    Paquet* Nouveau_p = f->tete;

    // Parcours et libération de tous les paquets
    while (Nouveau_p != NULL) {
        Paquet* temp = Nouveau_p;
        Nouveau_p = Nouveau_p->suivant;
        free(temp);
    }

    // Réinitialisation de la file
    f->tete = NULL;
    f->queue = NULL;
    f->taille_actuelle = 0;
     f->taille_actuelle = 0;
    f->paquets_total = 0;
    f->paquets_defiles = 0;
    f->paquets_rejetes = 0;
    f->somme_attente = 0.0;
}
// Affichage des statistiques

void afficherStats(FileAttente* f) {
    printf("\n STATISTIQUES \n");
    printf("Paquets envoyés : %d\n", f->paquets_total);
    printf("Paquets traités : %d\n", f->paquets_defiles);
    printf("Paquets perdus : %d\n", f->paquets_rejetes);

    // Calcul du temps moyen d'attente
    double temps_moyen = (f->paquets_defiles > 0) ? (f->somme_attente / f->paquets_defiles) : 0.0;

    // calcul du taux de pertes 
    double taux_perte = (f->paquets_total > 0) ? ((double)f->paquets_rejetes / f->paquets_total) * 100 : 0.0;

    // Calcul du débit effectif 
    double debit = (f->somme_attente > 0) ? ((double)f->paquets_defiles / f->somme_attente) : 0.0;

    printf("Temps moyen d'attente : %.4f sec\n", temps_moyen);
    printf("Taux de perte : %.2f%%\n", taux_perte);
    printf("Débit effectif : %.2f paquets/s\n", debit);
}


