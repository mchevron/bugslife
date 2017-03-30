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

#define WORD_LENGTH_COMPARE 	9
#define ZERO_NOURRITURE 		0
#define NB_ELEMENTS_NOURRITURE	2
#define	REUSSI					1
#define ECHEC					0

struct nourriture
{
	double x;
	double y;  
};

static NOURRITURE tab_nourriture[MAX_RENDU1];
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
	unsigned k = 0;
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
    while(sscanf(deb, "%*[ \t]%lf %lf", &tab_nourriture[l].x, 
		  &tab_nourriture[l].y) == NB_ELEMENTS_NOURRITURE) {
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
