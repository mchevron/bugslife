
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
#include "fourmiliere.h"
#include "error.h"
#include "constantes.h"
#include "graphic.h"
#include "utilitaire.h"
#include "nourriture.h"

#define WORD_LENGTH_COMPARE 	9
#define ZERO_NOURRITURE 		0
#define NB_ELEMENTS_NOURRITURE	2
#define NB_NOURRITURE_PAR_LIGNE 3
#define	REUSSI					1

struct nourriture
{
    double x;
    double y;
    NOURRITURE * next;
};

static unsigned nb_nourriture = 0;
static NOURRITURE * p_nourriture = NULL;
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
    if (l > nb_nourriture) {
        error_lecture_elements_nourriture(ERR_TROP);
        return L_EXIT;
    }
    return L_NOURRITURE;
}

NOURRITURE * nourriture_ajouter ( NOURRITURE ** p_tete )
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
		fprintf(f_sortie, "		%lf", nour->x);		
		fprintf(f_sortie, " %lf", nour->y);
		nour = nour->next;
		i = i + 1;
		if (i == NB_NOURRITURE_PAR_LIGNE){
			fputs("\n", f_sortie);
			i = 0;
		}
	}
	if (nb_nourriture != 0)
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
    nb_nourriture -= 1;
}

unsigned nourriture_get_nb(void){
	return nb_nourriture;
}

void nourriture_ajouter_fixe(double x, double y){
	NOURRITURE* nourri = NULL;
    nourri = nourriture_ajouter(&p_nourriture);
	if (nourri != NULL) {
		nourri->x = x;
		nourri->y = y;
	}
}

void nourriture_creation(void){
    double rand_max = RAND_MAX;
	double x = (rand()/rand_max)*(DMAX*2) + DMIN;
	double y = (rand()/rand_max)*(DMAX*2) + DMIN;
	if (rand()/rand_max <= FOOD_RATE){
		while (fourmiliere_nourriture_test_superposition(x,y)){ 
			x = (rand()/rand_max)*(DMAX*2) + DMIN;
			y = (rand()/rand_max)*(DMAX*2) + DMIN;
		}
	nourriture_ajouter_fixe(x, y);
    nb_nourriture += 1;
	}
}

void nourriture_choix(double *posx, double *posy, double *butx, double *buty, int i) {
    //Chercher la nourriture la plus proche
    double distance = utilitaire_calcul_distance(DMAX, DMIN, DMAX, DMIN);
            // distance max entre fourmi et nourriture
    double distance_new = distance;
    float risque_mort = 1;                         // 1 = risque certain
    float risque_mort_new = 1;
    float risque_fourmiliere_chemin = 0;
    float risque_ouvriere_chemin = 0;
    float risque_competition = 0;
    float dispo = 1;
    float dispo_new = dispo;
    NOURRITURE* nourri = p_nourriture;
    while(nourri) {
        distance_new = utilitaire_calcul_distance(*posx, nourri->x, *posy, nourri->y);
        risque_fourmiliere_chemin = fourmiliere_sur_chemin(*posx, *posy, i, nourri->x, nourri->y);
        //risque_ouvriere_chemin = fourmiliere_ouvri_sur_chemin(*posx, *posy, i, nourri->x, nourri->y);
        risque_competition = fourmiliere_test_ouvri_competition(distance_new, i, nourri->x, nourri->y);
        risque_mort_new = (risque_fourmiliere_chemin+risque_competition)/2;
        dispo_new = fourmiliere_test_nourri_dispo(i, nourri->x, nourri->y);
        if((risque_mort_new < risque_mort) || (risque_mort_new <= risque_mort &&
                                               distance_new <= distance && dispo_new <= dispo)) {
                risque_mort = risque_mort_new;
                distance = distance_new;
                *butx = nourri->x;
                *buty = nourri->y;
        }
        nourri = nourri->next;
    }
}

void nourriture_cherche_retire(double x, double y) {
    if(nb_nourriture!=0) {
        NOURRITURE *nourri = p_nourriture;
        while((nourri->x!=x || nourri->y!=y)) {
            nourri = nourri->next;
        }
            nourriture_retirer (&p_nourriture, nourri);
    }
}
