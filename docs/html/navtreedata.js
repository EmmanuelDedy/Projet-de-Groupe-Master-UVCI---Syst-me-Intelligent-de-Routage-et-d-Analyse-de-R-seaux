/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "Système Intelligent de Routage (UVCI)", "index.html", [
    [ "Système Intelligent de Routage &amp; Analyse de Réseaux", "index.html", "index" ],
    [ "📄 Rapport Technique", "rapport_projet.html", [
      [ "2. Introduction", "rapport_projet.html#rap_intro", [
        [ "Contexte et Motivation", "rapport_projet.html#rap_context", null ],
        [ "Objectifs du Projet", "rapport_projet.html#rap_objectifs", null ],
        [ "Organisation du Rapport", "rapport_projet.html#rap_org", null ]
      ] ],
      [ "3. État de l'Art", "rapport_projet.html#rap_state_art", [
        [ "Algorithmes de Routage Existants", "rapport_projet.html#rap_algo_exist", null ],
        [ "Comparaison des Approches", "rapport_projet.html#rap_comparaison", null ],
        [ "Justification des Choix", "rapport_projet.html#rap_choix", null ]
      ] ],
      [ "4. Conception", "rapport_projet.html#rap_conception", [
        [ "4.1 Architecture Globale", "rapport_projet.html#rap_arch", null ],
        [ "4.2 Structures de Données Détaillées", "rapport_projet.html#rap_data_struct", [
          [ "A. Représentation du Graphe (Hybride)", "rapport_projet.html#autotoc_md12", null ],
          [ "B. Métriques Multidimensionnelles", "rapport_projet.html#autotoc_md13", null ],
          [ "C. File à Priorité (Simulation QoS)", "rapport_projet.html#autotoc_md14", null ]
        ] ],
        [ "4.3 Justification des Choix de Conception", "rapport_projet.html#rap_choix_conc", [
          [ "1. Langage C vs Python/Java", "rapport_projet.html#autotoc_md16", null ],
          [ "2. Tableau vs Tas Binaire (Binary Heap) pour Dijkstra", "rapport_projet.html#autotoc_md17", null ],
          [ "3. Modélisation Hybride (Matrice + Liste)", "rapport_projet.html#autotoc_md18", null ]
        ] ],
        [ "4.4 Algorithmes Implémentés (Détails)", "rapport_projet.html#rap_algo_struct", [
          [ "1. Module Routage : Dijkstra (<span class=\"tt\">routage_dijkstra</span>)", "rapport_projet.html#autotoc_md20", null ],
          [ "2. Module Routage : Backtracking (<span class=\"tt\">routage_backtracking</span>)", "rapport_projet.html#autotoc_md21", null ],
          [ "3. Module Routage : K-Plus Courts Chemins (<span class=\"tt\">routage_k_chemins</span>)", "rapport_projet.html#autotoc_md22", null ],
          [ "4. Module Sécurité : Détection de Cycle (<span class=\"tt\">detecter_cycles</span>)", "rapport_projet.html#autotoc_md23", null ],
          [ "5. Module Sécurité : Points d'Articulation (<span class=\"tt\">identifier_points_critiques</span>)", "rapport_projet.html#autotoc_md24", null ],
          [ "6. Module Graphe : Ajout d'Arête (<span class=\"tt\">graphe_ajouter_arete</span>)", "rapport_projet.html#autotoc_md25", null ],
          [ "7. Module Simulation : File à Priorité (<span class=\"tt\">enfiler</span>)", "rapport_projet.html#autotoc_md26", null ]
        ] ],
        [ "4.5 Format des Fichiers de Données", "rapport_projet.html#rap_file_format", [
          [ "A. Structure du Fichier", "rapport_projet.html#autotoc_md30", null ],
          [ "B. Légende des Préfixes", "rapport_projet.html#autotoc_md31", null ],
          [ "C. Exemple Concret", "rapport_projet.html#autotoc_md32", null ]
        ] ]
      ] ],
      [ "5. Analyse Théorique de Complexité (CRUCIAL)", "rapport_projet.html#rap_complexity", [
        [ "5.1 Algorithme de Dijkstra", "rapport_projet.html#rap_proof_dijkstra", null ],
        [ "5.2 Algorithme de Backtracking", "rapport_projet.html#rap_proof_backtracking", null ],
        [ "5.3 K-Plus Courts Chemins (Yen Simplifié)", "rapport_projet.html#rap_proof_yen", null ],
        [ "5.4 Algorithmes de Sécurité (DFS/Tarjan)", "rapport_projet.html#rap_proof_secu", null ],
        [ "Tableau Comparatif Théorique", "rapport_projet.html#rap_summary", null ]
      ] ],
      [ "6. Implémentation", "rapport_projet.html#rap_impl", [
        [ "Choix Techniques (Langage C)", "rapport_projet.html#rap_tech_choice", null ],
        [ "Difficultés &amp; Solutions", "rapport_projet.html#rap_difficulties", null ]
      ] ],
      [ "7. Résultats Expérimentaux (CRUCIAL)", "rapport_projet.html#rap_res", [
        [ "7.1 Protocole de Test", "rapport_projet.html#rap_proto", null ],
        [ "7.2 Déroulement et Validation Fonctionnelle", "rapport_projet.html#rap_valid_func", [
          [ "A. Stratégie de Test", "rapport_projet.html#autotoc_md43", null ],
          [ "B. Journal d'Exécution Complet", "rapport_projet.html#autotoc_md44", null ],
          [ "C. Matrice de Couverture", "rapport_projet.html#autotoc_md45", null ]
        ] ],
        [ "7.3 Résultats du Test", "rapport_projet.html#rap_res_test", [
          [ "A. Données Générées (Persistance)", "rapport_projet.html#autotoc_md47", null ],
          [ "B. Galerie de Validation (Visuel)", "rapport_projet.html#autotoc_md48", [
            [ "1. Cas Unitaires (Tests de base)", "rapport_projet.html#autotoc_md49", null ],
            [ "2. Topologies Réseau Classiques", "rapport_projet.html#autotoc_md50", null ],
            [ "3. Scénario d'Intégration (Simulation Complète)", "rapport_projet.html#autotoc_md51", null ],
            [ "4. Stress Test et Chaos (Random 100)", "rapport_projet.html#autotoc_md52", null ]
          ] ],
          [ "C. Interopérabilité (.DOT)", "rapport_projet.html#autotoc_md53", null ]
        ] ],
        [ "7.4 Mesures de Performance Comparatives", "rapport_projet.html#rap_perf", [
          [ "A. Comparaison Algorithmique : Dijkstra vs Bellman-Ford", "rapport_projet.html#autotoc_md55", null ],
          [ "B. Impact de l'Élagage sur le Backtracking", "rapport_projet.html#autotoc_md56", null ]
        ] ],
        [ "7.5 Comparaison Structurelle : Matrice vs Liste d'Adjacence", "rapport_projet.html#rap_comp_struct", null ],
        [ "7.6 Validation de la Complexité Théorique", "rapport_projet.html#rap_val_theory", null ],
        [ "7.7 Synthèse Globale des Performances", "rapport_projet.html#rap_synth", [
          [ "A. Répartition de la Charge", "rapport_projet.html#autotoc_md61", null ],
          [ "B. Limites de Scalabilité (Heatmap)", "rapport_projet.html#autotoc_md62", null ],
          [ "C. Performance I/O et Résilience", "rapport_projet.html#autotoc_md63", null ]
        ] ]
      ] ],
      [ "8. Innovation : Conception d'un Logiciel d'Analyse Graphique", "rapport_projet.html#rap_innov", [
        [ "8.1 Présentation du Logiciel \"S.I.R.A.R\"", "rapport_projet.html#rap_pres_soft", [
          [ "A. Interface Utilisateur (Captures d'écran)", "rapport_projet.html#autotoc_md67", null ],
          [ "B. Module de Visualisation Graphique", "rapport_projet.html#autotoc_md68", null ]
        ] ],
        [ "8.2 Architecture Hybride et Modulaire", "rapport_projet.html#rap_ext", [
          [ "1. Le Coeur Algorithmique (C Natif)", "rapport_projet.html#autotoc_md70", null ],
          [ "2. Module GSM (Global System Management)", "rapport_projet.html#autotoc_md71", null ],
          [ "3. La Couche d'Orchestration (Python Core)", "rapport_projet.html#autotoc_md72", null ],
          [ "3. Interface Graphique (PyQt6)", "rapport_projet.html#autotoc_md73", null ],
          [ "4. Services Transverses", "rapport_projet.html#autotoc_md74", null ]
        ] ],
        [ "8.3 Apport par rapport à l'existant", "rapport_projet.html#rap_apport", null ],
        [ "8.4 Perspectives d'Amélioration", "rapport_projet.html#rap_perspectives", null ]
      ] ],
      [ "9. Conclusion Générale", "rapport_projet.html#rap_conc", null ],
      [ "10. Bibliographie et Références", "rapport_projet.html#rap_biblio", null ]
    ] ],
    [ "Structures de données", "annotated.html", [
      [ "Structures de données", "annotated.html", "annotated_dup" ],
      [ "Index des structures de données", "classes.html", null ],
      [ "Champs de donnée", "functions.html", [
        [ "Tout", "functions.html", null ],
        [ "Variables", "functions_vars.html", null ]
      ] ]
    ] ],
    [ "Fichiers", "files.html", [
      [ "Liste des fichiers", "files.html", "files_dup" ],
      [ "Variables globale", "globals.html", [
        [ "Tout", "globals.html", null ],
        [ "Fonctions", "globals_func.html", null ],
        [ "Variables", "globals_vars.html", null ],
        [ "Énumérations", "globals_enum.html", null ],
        [ "Valeurs énumérées", "globals_eval.html", null ],
        [ "Macros", "globals_defs.html", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"annotated.html",
"securite_8h.html#ad543fa19b5a48eadcb131545d552aa5d"
];

var SYNCONMSG = 'cliquez pour désactiver la synchronisation du panel';
var SYNCOFFMSG = 'cliquez pour activer la synchronisation du panel';
var LISTOFALLMEMBERS = 'Liste de tous les membres';