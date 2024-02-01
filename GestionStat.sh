#!/bin/bash

# Variables pour stocker les statistiques

totalPointsJoueur=0
totalPointsRobot=0
parties=0

defaitesJoueur=0
defaitesRobot=0
totalPointsGagnesJoueur=0
totalPointsGagnesRobot=0

# Fonction pour afficher les statistiques
afficherStatistiques() {
    echo "Points moyens par parties : $((totalPointsJoueur + totalPointsRobot))"
    echo "Parties gagnées : $parties"
    echo "Défaites du joueur : $defaitesJoueur"
    echo "Défaites du robot : $defaitesRobot"
    echo "Total de points gagnés par le joueur : $totalPointsGagnesJoueur"
    echo "Total de points gagnés par le robot : $totalPointsGagnesRobot"
}

# Fonction pour sauvegarder les parties dans un fichier de log
sauvegarderLog() {
    cat "$1" | awk '/Manche/{print; getline; print}' > parties.log
}

# Fonction pour générer un document PDF
genererPDF() {
cat > statistiques.tex <<EOL
\documentclass{article}
\usepackage{datetime}
\begin{document}
\title{ Jeu "6 qui prend"}
\author{DJAWOU DETAIL}
\maketitle

\section*{Statistiques de jeu}

\subsection*{Points moyens par parties}
Le nombre total de points par parties est : $((totalPointsJoueur + totalPointsRobot)).

\subsection*{nombre total de Manche}
Le nombre totale de manche jouées  : $parties.
\subsection*{Défaites des joueurs }
Le nombre de Défaites des joueurs est  : $defaitesJoueur.
\subsection*{Défaites des robots }
Le nombre de Défaites des robots est  : $defaitesRobot.
\subsection*{Total de points gagnés par le joueur}
Le nombre points Total gagnés par le joueur : $totalPointsGagnesJoueur.
\subsection*{Total de points gagnés par le robot}
Le nombre points Total gagnés par le robot : $totalPointsGagnesRobot.
\end{document}
EOL

pdflatex statistiques.tex

}

# Parcours du fichier
while read -r ligne; do
       if [[ $ligne == *Manche* ]]; then
        # Nouvelle manche, incrémenter le nombre de parties
        ((parties++))
    elif [[ $ligne == *Joueur* && $ligne == *perdu* ]]; then
        # Ligne contenant la défaite du joueur
        ((defaitesJoueur++))
    elif [[ $ligne == *Robots* && $ligne == *perdu* ]]; then
        # Ligne contenant la défaite du robot
        ((defaitesRobot++))
    elif [[ $ligne == *Joueur* ]]; then
        # Ligne contenant les points du joueur
        points=${ligne##*=> }
        ((totalPointsJoueur += points))
        ((totalPointsGagnesJoueur += points))
    elif [[ $ligne == *Robots* ]]; then
        # Ligne contenant les points du robot
        points=${ligne##*=> }
        ((totalPointsRobot += points))
        ((totalPointsGagnesRobot += points))
    fi

done < logfileStat.txt

# Calcul du nombre de parties perdues
partiesPerdues=$((partiesGagnees - 1))

# Affichage des statistiques
afficherStatistiques

# Sauvegarde des parties dans un fichier de log
sauvegarderLog logfileStat.txt

# Génération du document PDF
genererPDF
