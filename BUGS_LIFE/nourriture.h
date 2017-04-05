/*!
 \file nourriture.c
 \brief Module qui gère les données liées à la nourriture
 \author Diane Remmy & Max Chevron
 \version 1.0
 \date Mars 2017
 */

#include "constantes.h"

typedef struct nourriture NOURRITURE;

int nourriture_nb_nourriture(char tab[MAX_LINE]);
int nourriture_lecture(char tab[MAX_LINE]);
void add_new_food(float pos_x, float pos_y);
