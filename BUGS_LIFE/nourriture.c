
/*!
 \file nourriture.c
 \brief Module qui gère les données liées à la nourriture
 \author Diane Remmy & Max Chevron
 \version 2.0
 \date Avril 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "constantes.h"
#include "graphic.h"
#include "nourriture.h"

#define WORD_LENGTH_COMPARE 	9
#define ZERO_NOURRITURE 		0
#define NB_ELEMENTS_NOURRITURE	2
#define NB_NOURRITURE_PAR_LIGNE 3

struct nourriture
{
    unsigned id;
    double x;
    double y;
    NOURRITURE * next;
};

static NOURRITURE * p_nourriture = NULL;
static unsigned nb_nourriture = 0;
static unsigned l = 0;

int nourriture_nb_nourriture(char tab[MAX_LINE]) {
    unsigned test = 0;
    if(sscanf(tab, " %u", &test) == VRAI){
        sscanf(tab, " %u", &nb_nourriture);
        printf("%u\n",nb_nourriture);
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
       && (l < nb_nourriture)){
        error_lecture_elements_nourriture(ERR_PAS_ASSEZ);
        return L_EXIT;
    }
    if(strcmp(check, "FIN_LISTE") == ECHEC)
        return L_COMPLETE;
    if(sscanf(tab, " %lf", &test) == ECHEC) return L_NOURRITURE;
    char *deb=tab, *fin=NULL;
    while(sscanf(deb, "%*[ \t]%lf %lf", &x, &y) == NB_ELEMENTS_NOURRITURE) {
        nourri = nourriture_ajouter(&p_nourriture);
        if (nourri == NULL) return L_EXIT;
        nourri->x = x;
        nourri->y = y;
        for (k = 0; k < NB_ELEMENTS_NOURRITURE; k++) {
            strtod(deb,&fin);
            deb = fin;
        }
        l=l+1;
    }
    printf("%u\n",l);
    printf("%u\n",nb_nourriture);
    if (l > nb_nourriture) {
        error_lecture_elements_nourriture(ERR_TROP);
        return L_EXIT;
    }
    return L_NOURRITURE;
}

NOURRITURE * nourriture_ajouter ( NOURRITURE ** p_tete ){
    NOURRITURE * nour = NULL;
    
    if (!(nour = (NOURRITURE *) malloc (sizeof(NOURRITURE)))){
        printf ("Pb d'allocation dans %s\n", __func__);
        return nour;
    }
    
    nour->next = *p_tete;
    *p_tete		= nour;
    
    return nour;
}

void nourriture_clique(float pos_x, float pos_y) {
    NOURRITURE* nourri;
    nourri = nourriture_ajouter(&p_nourriture);
    nourri->x = pos_x;
    nourri->y = pos_y;
    nb_nourriture++;
}

void nourriture_dessine() {
    if(nb_nourriture != 0) {
        int i=0;
        graphic_set_color3f (0., 0., 0.);
        NOURRITURE * courant = p_nourriture;
        for(i=0; i<nb_nourriture; i=i+1) {
            graphic_draw_circle (courant->x, courant->y,
                                 RAYON_FOOD, GRAPHIC_EMPTY);
            courant = courant->next;
        }
    }
}

void nourriture_save(FILE *f_sortie) {
    fputs("# Nb nourriture\n", f_sortie);
	fprintf(f_sortie, "%u\n", nb_nourriture);
	unsigned i = 0;
	NOURRITURE * nour = p_nourriture;
	while (nour != NULL){
		fprintf(f_sortie, "    %lf", nour->x);		
		fprintf(f_sortie, " %lf", nour->y);
		nour = nour->next;
		i = i + 1;
		if (i == NB_NOURRITURE_PAR_LIGNE){
			fputs("\n", f_sortie);
			i = 0;
		}
	}
	if (nb_nourriture != 0)
		fputs("\n", f_sortie);
		fputs("FIN_LISTE\n", f_sortie);
}

void nourriture_free(void){
    nourriture_vider(&p_nourriture);
    free(p_nourriture);
    p_nourriture = NULL;
    nb_nourriture = 0;
    l = 0;
}

void nourriture_vider ( NOURRITURE ** p_liste ){
    /* Retire un à un les elements en tete de la liste */
    NOURRITURE *nour;
    while ( * p_liste != NULL ){
        nour = * p_liste;
        nourriture_retirer ( p_liste, nour );
    }
}

void nourriture_retirer ( NOURRITURE ** p_tete, NOURRITURE *nour ){
    NOURRITURE* courant=* p_tete;
    NOURRITURE* precedent=* p_tete;
    while ( courant != nour && courant != NULL ){
        precedent=courant;
        courant=courant->next;
    }
    if ( courant != NULL ){
        if ( courant != * p_tete ){
            precedent->next=courant->next;
            free ( courant );
        }
        else {
            * p_tete = courant->next;
            free ( courant );
        }
    }
}
