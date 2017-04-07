
/*!
 \file nourriture.c
 \brief Module qui gère les données liées à la nourriture
 \author Diane Remmy & Max Chevron
 \version 1.0
 \date Mars 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "constantes.h"
#include "nourriture.h"
#include "modele.h"
#include "graphic.h"

#define WORD_LENGTH_COMPARE 	9
#define ZERO_NOURRITURE 		0
#define NB_ELEMENTS_NOURRITURE	2
#define	REUSSI					1
#define ECHEC					0

struct nourriture
{
    double x;
    double y;
    NOURRITURE * next;
};

static NOURRITURE * p_nourriture = NULL;
static unsigned nb_nourriture = 0;
static unsigned l = 0;

int nourriture_nb_nourriture(char tab[MAX_LINE]) {
    unsigned test = 0;
    if(sscanf(tab, " %u", &test) == REUSSI){
        sscanf(tab, " %u", &nb_nourriture);
        if (nb_nourriture == 0) return L_COMPLETE;
        return L_NOURRITURE;
    }
    return L_NB_NOURRITURE;
}

int nourriture_lecture(char tab[MAX_LINE]) {
    NOURRITURE* nourri;
    unsigned k = 0;
    double x = 0., y = 0.;
    double test = 0.;
    char check[MAX_LINE];
    sscanf(tab, " %s", check);
    if((strcmp(check, "FIN_LISTE") == ECHEC)
       && (l < nb_nourriture))
    {
        error_lecture_elements_nourriture(ERR_PAS_ASSEZ);
        return L_EXIT;
    }
    if(strcmp(check, "FIN_LISTE") == ECHEC)
        return L_COMPLETE;
    if(sscanf(tab, " %lf", &test) == ECHEC) return L_NOURRITURE;
    char *deb=tab, *fin=NULL;
    while(sscanf(deb, "%*[ \t]%lf %lf", &x, &y) == NB_ELEMENTS_NOURRITURE) {
        nourri = ajouter_nourriture(&p_nourriture);
        if (nourri == NULL) return L_EXIT;
        nourri->x = x;
        nourri->y = y;
        for (k = 0; k < NB_ELEMENTS_NOURRITURE; k++) {
            strtod(deb,&fin);
            deb = fin;
        }
        l=l+1;
    }
    if (l > nb_nourriture) {
        error_lecture_elements_nourriture(ERR_TROP);
        return L_EXIT;
    }
    return L_NOURRITURE;
}

NOURRITURE * ajouter_nourriture ( NOURRITURE ** p_tete )
{
    NOURRITURE * nour = NULL;
    
    if (!(nour = (NOURRITURE *) malloc (sizeof(NOURRITURE))))
    {
        printf ("Pb d'allocation dans %s\n", __func__);
        return nour;
    }
    
    nour->next = *p_tete;
    *p_tete		= nour;
    
    return nour;
}

void add_new_food(float pos_x, float pos_y) {
    
}

void nourriture_dessine() {
    int i=0;
    graphic_set_color3f (0., 0., 0.);
    for(i=0; i<nb_nourriture; i=i+1) {
        NOURRITURE * courant = p_nourriture;
        graphic_draw_circle (courant->x, courant->y,
                             RAYON_FOOD, GRAPHIC_EMPTY);
        courant = courant->next; /* courant devient nul après le premier
                                  passage. Comme s'il n'y avait pas d'adresse 
                                  stockée dans next ... une idée?  */
    }
}

