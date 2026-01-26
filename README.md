<div align="center">

# 🌍 Système Intelligent de Routage & Analyse de Réseaux
### **Projet de Fin de Semestre : Master 1 UVCI**
### *Algorithmique Avancée et Complexité*

![Language](https://img.shields.io/badge/Language-C11-00599C?style=for-the-badge&logo=c&logoColor=white)
![Standard](https://img.shields.io/badge/Standard-CDC_Respect-orange?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-blue?style=for-the-badge)

</div>

---

## 📑 Sommaire : Respect du Cahier des Charges

Le projet suit rigoureusement le découpage modulaire imposé par le **Cahier des Charges (CDC) 2026**.

1. [Introduction et Contexte Académique](#1-introduction-et-contexte-académique)
2. [Module 1 : Modélisation et Structures de Données](#2-module-1--modélisation-et-structures-de-données)
3. [Module 2 : Algorithmes de Routage Optimal](#3-module-2--algorithmes-de-routage-optimal)
4. [Module 3 : Sécurité et Résilience](#4-module-3--sécurité-et-résilience)
5. [Module 4 : Simulation et Gestion de Flux](#5-module-4--simulation-et-gestion-de-flux)
6. [Phase de Tests et Validation (Livrables)](#6-phase-de-tests-et-validation-livrables)
7. [Installation et Démarrage](#7-installation-et-démarrage)
8. [Conclusion et Perspectives](#8-conclusion-et-perspectives)

---

## 1. Introduction et Contexte Académique

Ce projet vise à répondre à la problématique de la gestion des réseaux à grande échelle. Il met en application les concepts théoriques vus en cours : **Graphes, Complexité, et Structures de Données Avancées**.

### Objectifs du CDC
*   Implémenter un système modulaire en **Langage C**.
*   Respecter les contraintes de complexité temporelle et spatiale.
*   Fournir une simulation réaliste d'un réseau de routeurs.

---

## 2. Module 1 : Modélisation et Structures de Données

Ce module constitue le cœur du système (Kernel). Il définit comment le réseau est représenté en mémoire.

### 2.1 Choix de la Structure de Graphe
Conformément au CDC, nous avons opté pour une structure hybride capable de gérer :
*   **Les Nœuds** : Routeurs, Switchs, Serveurs (typés).
*   **Les Arcs (Liens)** : Pondérés par 4 métriques (Coût, Latence, Bande Passante, Fiabilité).

```c
// Structure Graphe (src/graphe.h)
typedef struct {
    int nb_noeuds;
    Noeud* noeuds;          // Tableau dynamique
    double** matrice_adj;   // Optimisation pour graphes denses (O(1))
    bool est_oriente;
} Graphe;
```

### 2.2 Persistance des Données
Le module assure la sauvegarde et le chargement des topologies depuis des fichiers textes formatés, permettant la réutilisabilité des jeux de tests (`data/topologies/`).

---

## 3. Module 2 : Algorithmes de Routage Optimal

Ce module implémente la logique décisionnelle du réseau.

### 3.1 Algorithme de Dijkstra (Plus Court Chemin)
Utilisé pour le routage OSPF standard.
*   **Complexité** : $O(E \log V)$
*   **Critère** : Minimisation du coût ou de la latence.
*   **Implémentation** : Version relaxée avec gestion optimisée de la mémoire.

### 3.2 Algorithme de Backtracking (Routage Avancé)
Répond aux exigences de **Qualité de Service (QoS)** et de contraintes strictes.
*   **Scénario** : "Trouver un chemin de A à B avec moins de 50ms de latence et plus de 100Mbps de bande passante".
*   **Technique** : Exploration DFS avec élagage (Pruning) pour éviter l'explosion combinatoire.

### 3.3 Algorithme de Yen (K-Plus Courts Chemins)
Assure la redondance. En cas de panne du chemin principal, le système bascule sur le k-ième chemin alternatif pré-calculé.

---

## 4. Module 3 : Sécurité et Résilience

Ce module audit le réseau pour détecter les vulnérabilités structurelles avant qu'elles ne causent des pannes.

### 4.1 Détection des Composantes Fortement Connexes (SCC)
Utilisation de l'algorithme de **Tarjan/Kosaraju**.
*   **But** : Identifier les sous-réseaux isolés ou les "îlots" de communication qui pourraient se former en cas de coupure.
*   **Complexité** : Linéaire $O(V+E)$.

### 4.2 Points d'Articulation (SPOF)
Identification des nœuds critiques (Single Point of Failure). Si un tel nœud tombe, le réseau est partitionné.
*   **Action** : Le système alerte l'administrateur pour renforcer ces nœuds.

---

## 5. Module 4 : Simulation et Gestion de Flux

Simulation du comportement dynamique du réseau sous charge.

### 5.1 Gestion des Priorités (Priority Queue)
Implémentation d'une file d'attente prioritaire pour les paquets.
*   **Structure** : Liste chaînée triée à l'insertion (`src/liste_chainee.c`).
*   **Règle** : Les paquets "VoIP" (Priorité 10) passent avant les paquets "Email" (Priorité 1).

### 5.2 Simulation de Pannes
Le moteur permet de couper dynamiquement un lien ou d'éteindre un nœud pour tester la réaction des algorithmes de routage (Reroutage dynamique).

---

## 6. Phase de Tests et Validation (Livrables)

Le projet intègre une suite de tests complète validant chaque module du CDC.

| Module Testé | Fichier de Test | Résultat Attendu |
| :--- | :--- | :--- |
| **Graphes** | `test_graphe.c` | Création correcte de 500 nœuds |
| **Routage** | `test_dijsktra.c` | Chemin optimal validé mathématiquement |
| **Données** | `test_donnees_reelles.c` | Chargement de 6 fichiers complexes |
| **Global** | `test_global.c` | Scénario complet (Design -> Panne -> Fix) |

### Visualisation des Résultats
Les tests génèrent des rapports visuels dans `resultats_tests/`.
![Grid Topology](resultats_tests/visu_grid_25.png)

---

## 7. Installation et Démarrage

### Prérequis
*   GCC (GNU Compiler Collection)
*   Make

### Commandes
```bash
make        # Compiler le projet
make tests  # Lancer la validation CDC
make clean  # Nettoyer l'environnement
```

---

## 8. Conclusion et Perspectives

Ce projet remplit l'ensemble des critères du Cahier des Charges 2026. Il offre une base solide et extensible pour l'étude des réseaux.
Les perspectives d'évolution incluent l'ajout de l'algorithme A* pour les très grands graphes et une interface graphique (GUI) complète.

---
**Université Virtuelle de Côte d'Ivoire - 2026**
