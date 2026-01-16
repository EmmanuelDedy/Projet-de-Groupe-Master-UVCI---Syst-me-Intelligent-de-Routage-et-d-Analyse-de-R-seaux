# NetFlow Optimizer & Security Analyzer

Projet de groupe – Master Informatique  
UE : Algorithmique et Complexité (ALC2101)  
Université Virtuelle de Côte d’Ivoire (UVCI)

---

## Description
Ce projet consiste à concevoir et implémenter un système en langage C permettant
d’analyser et d’optimiser un réseau informatique modélisé sous forme de graphe pondéré.
Le système intègre des algorithmes de routage, de détection d’anomalies et de gestion
des flux de données.

---

## Structure du projet

src/    : code source (graphes, algorithmes, sécurité)  
data/   : jeux de données de test (réseaux)  
tests/  : tests unitaires  
docs/   : documentation et plan de test  

---

## Compilation

make

---

## Exécution

./netflow data/reseau_simple.txt

---

## Tests

Analyse mémoire (Valgrind) :

valgrind --leak-check=full ./netflow data/reseau_simple.txt

---

## Auteurs
Projet réalisé en groupe dans le cadre du Master Informatique (UVCI).
