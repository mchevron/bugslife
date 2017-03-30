/*!
 \file modele.h
 \brief Module qui gère la lecture des données
 \author Diane Remmy & Max Chevron
 \version 1.0
 \date Mars 2017
 */

typedef enum etape_lecture {
    L_NB_FOURMILIERE, L_FOURMILIERE, L_OUVRIERE, L_GARDE, L_NB_NOURRITURE,
    L_NOURRITURE, L_EXIT, L_COMPLETE, L_CONTINUE,
} ETAPE_LECTURE;

int modele_lecture(int argc, char *argv[]);
