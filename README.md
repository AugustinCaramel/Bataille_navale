Projet de Programmation en C : Bataille Navale - Hajer ZEKRI - October 15, 2024

I. Contexte

L'objectif du projet est de programmer un jeu de Bataille Navale en C. Ce jeu classique oppose deux joueurs qui placent secrètement leurs navires sur une grille.
Chaque joueur, à tour de rôle, doit tenter de deviner où sont les navires de l'adversaire et les ”couler” en indiquant des coordonnées sur la grille.
Le but est de couler tous les navires de l'adversaire avant que les siens ne soit détruit.

II. Rendu

La date limite de remise du projet est fixée au 12 décembre 2024, à minuit. Le dossier de soumission devra inclure les éléments suivants :
• Le code source en langage C, bien structuré, documenté et accompagné de commentaires expliquant les parties importantes du programme.

• Un rapport au format PDF, limité à 5 pages, comprenant :
    • Les fonctionnalités implémentées avec une explication du choix des algorithmes et des structures de données utilisées.
    • Des captures d'écran montrant le fonctionnement du programme à différentes étapes (grille avant et après les coups, détection des navires coulés, etc.).
    • Une auto-évaluation décrivant le travail réalisé par chaque étudiant, les défis rencontrés, et les améliorations possibles.

• Une présentation PowerPoint claire, bien organisée et informative pour la soutenance, avec des diapositives structurées autour des points clés du projet. La soutenance comprendra :
    • Une démonstration en direct du programme, illustrant son bon fonctionnement et les principales fonctionnalités implémentées.
    • Un temps de questions-réponses durant lequel chaque membre du binôme devra montrer sa maîtrise du code.

L'évaluation sera basée sur les critères suivants :

• Fonctionnalités : la réalisation complète des fonctionnalités demandées, leur bon fonctionnement, ainsi que toute amélioration ou fonctionnalité supplémentaire pertinente.
• Qualité du code source : lisibilité, organisation, modularité (utilisation appropriée de fonctions) et commentaires.
• Qualité du rapport : clarté, précision des explications, profondeur de l'analyse et réflexion sur le travail accompli.
• Qualité de la présentation : capacité à exposer clairement les différentes parties du projet, à expliquer les choix techniques et à répondre aux questions de manière cohérente et précise.

III. Bataille navale - Intelligent

    A. Version Avancée : Intelligence Artificielle (IA)

• Mode contre l'ordinateur (IA basique) : Dans cette version, l'un des objectifs principaux est de permettre à un joueur humain de jouer contre une IA, représentant l'ordinateur.
L'IA doit être capable de :
    • Placer ses navires aléatoirement sur sa grille, tout en respectant les règles de placement (horizontalement ou verticalement, sans chevauchement ni dépassement de la grille).
    • Effectuer des tirs aléatoires sur la grille du joueur humain, en s'assurant que chaque tir cible une case non attaquée auparavant.

Détails du fonctionnement de l'IA basique :
    • L'IA sélectionne les coordonnées d'une case aléatoire pour chacun de ses tirs.
    • Si le tir touche un navire, l'IA annonce ”Touché”, sinon elle annonce ”Raté”. Les résultats sont affichés sur la grille correspondante.
    • L'IA alterne ses tirs avec ceux du joueur humain, conformément aux règles standards du jeu.

    B. Extensions possibles (Bonus)

Afin de rendre le jeu plus intéressant, plusieurs niveaux de difficulté doivent être implémentés. 
Ces niveaux influencent la stratégie adoptée par l'IA pour le placement des navires et l'exécution des tirs.

• Niveau Facile :
    • L'IA tire de manière totalement aléatoire. Elle ne cherche pas à exploiter les informations obtenues après un tir réussi.
    • Ce niveau est conçu pour les joueurs débutants, ou pour permettre à un joueur de se familiariser avec les règles du jeu.

• Niveau Moyen :
    • Lorsqu'un tir de l'IA touche un navire, l'IA utilise une stratégie simple en ciblant les cases adjacentes (haut, bas, gauche, droite) pour tenter de couler le navire.
    • L'IA conserve une part d'aléatoire pour ses tirs initiaux, mais devient plus stratégique après avoir détecté un navire.

• Niveau Difficile :
    • L'IA adopte une stratégie avancée. Après avoir touché un navire, elle analyse l'environnement immédiat pour couler rapidement le navire en visant de manière ciblée.
    • L'IA optimise ses tirs pour maximiser ses chances de découvrir et couler les navires du joueur humain rapidement, en tenant compte des tirs ratés pour éviter de cibler des zones inutiles.
