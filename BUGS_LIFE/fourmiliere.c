/*!
 \file fourmilere.c
 \brief Module qui gère les données liées aux fourmilieres
 \author Diane Remmy & Max Chevron
 \version 1.0
 \date Mars 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "error.h"
#include "constantes.h"
#include "fourmi.h"
#include "fourmiliere.h"
#include "modele.h"
#include "graphic.h"

#define NB_ELEMENTS_FOURMILIERE 6
#define WORD_LENGTH_COMPARE 	9
#define FAUX					0
#define VRAI					1
#define ECHEC					0

struct fourmiliere
{
    unsigned id;
    double x;
    double y;
    int nbO;
    int nbG;
    int nbF;
    int total_food;
    double rayon;
    FOURMI * p_fourmi_ouvriere;
    FOURMI * p_fourmi_garde;
};

static FOURMILIERE * p_fourmiliere = NULL;
static int nb_fourmiliere;

int fourmiliere_nb_fourmiliere(char tab[MAX_LINE]) {
    sscanf(tab, "%d", &nb_fourmiliere);
    if (nb_fourmiliere > MAX_FOURMILIERE) {
        error_nb_fourmiliere(nb_fourmiliere);
        return L_EXIT;
    }
    if (nb_fourmiliere == 0) return L_NB_NOURRITURE;
    return L_FOURMILIERE;
}

int fourmiliere_lecture(unsigned i, char tab[MAX_LINE])
{
    
    if (i >= nb_fourmiliere) return L_NB_NOURRITURE;
    if (!(p_fourmiliere = malloc(MAX_FOURMILIERE*sizeof(FOURMILIERE))))
    {
        printf("No memory for %s\n", __func__);
        return L_EXIT;
    }
    if (sscanf(tab, "%*[ \t]%lf %lf %d %d %d %lf", &(p_fourmiliere+i)->x,
               &(p_fourmiliere+i)->y,
               &(p_fourmiliere+i)->nbO,
               &(p_fourmiliere+i)->nbG,
               &(p_fourmiliere+i)->total_food,
               &(p_fourmiliere+i)->rayon) != NB_ELEMENTS_FOURMILIERE){
        error_lecture_elements_fourmiliere(i, ERR_FOURMILIERE, ERR_PAS_ASSEZ);
        return L_EXIT;
    }
    graphic_set_color3f (1., 0., 0.);
    graphic_draw_circle (0. ,0. ,1.5, GRAPHIC_EMPTY);
    //graphic_draw_circle ((p_fourmiliere+i)->x, (p_fourmiliere+i)->y,
    //                     (p_fourmiliere+i)->rayon, GRAPHIC_EMPTY);
    printf("%lf %lf %d %d %d %lf\n",(p_fourmiliere+i)->x,
           (p_fourmiliere+i)->y,
           (p_fourmiliere+i)->nbO,
           (p_fourmiliere+i)->nbG,
           (p_fourmiliere+i)->total_food,
           (p_fourmiliere+i)->rayon);
    (p_fourmiliere+i)->id = i;
    (p_fourmiliere+i)->nbF = (p_fourmiliere+i)->nbO + (p_fourmiliere+i)->nbG;
    if (fourmiliere_test_rayon(i,(p_fourmiliere+i)->nbF,
                               (p_fourmiliere+i)->total_food,
                               (p_fourmiliere+i)->rayon)) return L_EXIT;
    if (utilitaire_test_pos_domaine(ERR_FOURMILIERE, i, (p_fourmiliere+i)->x,
                                    (p_fourmiliere+i)->y)) return L_EXIT;
    return L_OUVRIERE;
}

int fourmiliere_ouvriere_lecture_precontrol(unsigned i, unsigned j, 
											char tab[MAX_LINE]) {
    char check[MAX_LINE];
    double test = 0.;
    sscanf(tab, " %s", check);
    if((strcmp(check, "FIN_LISTE") == ECHEC)
       && (j < (p_fourmiliere+i)->nbO)) {
        error_lecture_elements_fourmiliere(i, ERR_OUVRIERE, ERR_PAS_ASSEZ);
        return L_EXIT;
    }
    if ((strcmp(check, "FIN_LISTE") == ECHEC) && ((p_fourmiliere+i)->nbF == 0))
        return L_COMPLETE;
    if(strcmp(check, "FIN_LISTE") == ECHEC) {
        return L_GARDE;
    }
    if(j == (p_fourmiliere+i)->nbO && (p_fourmiliere+i)->nbO > 0) {
        error_lecture_elements_fourmiliere(i, ERR_OUVRIERE, ERR_TROP);
        return L_EXIT;
    }
    if((p_fourmiliere+i)->nbF == 0) return L_FOURMILIERE;
    if((p_fourmiliere+i)->nbO == 0) {
        j = 0;
        return fourmi_garde_lecture(i, tab);
    }
    if(sscanf(tab, " %lf", &test) == ECHEC) return L_OUVRIERE;
    return L_CONTINUE;
}

int fourmiliere_garde_lecture_precontrol(unsigned i, unsigned j, char tab[MAX_LINE]){
    char check[MAX_LINE];
    double test = 0.;
    sscanf(tab, " %s", check);
    if((strcmp(check, "FIN_LISTE") == ECHEC)
       && (j < (p_fourmiliere+i)->nbG)) {
        error_lecture_elements_fourmiliere(i, ERR_GARDE, ERR_PAS_ASSEZ);
        return L_EXIT;
    }
    if (j > (p_fourmiliere+i)->nbG) {
        error_lecture_elements_fourmiliere(i, ERR_GARDE, ERR_TROP);
        return L_EXIT;
    }
    if(strcmp(check, "FIN_LISTE") == ECHEC) return L_FOURMILIERE;
    if(sscanf(tab, " %lf", &test)==ECHEC) return L_GARDE;
    return L_CONTINUE;
}

int fourmiliere_test_rayon(unsigned num_fourmiliere, int nbF, int total_food,
							double rayon_fourmiliere)
{
	double rayon_max = (1 + sqrt(nbF) + sqrt(total_food))*RAYON_FOURMI;
	if (rayon_fourmiliere > (int) rayon_max) {
		error_rayon_fourmiliere(num_fourmiliere);
        return L_EXIT;
	}
    return FAUX;
}

int fourmiliere_test_lecture_elements(unsigned nb_fourmiliere_fichier) 
{
	if (nb_fourmiliere_fichier >= nb_fourmiliere) {
		error_lecture_elements_fourmiliere(nb_fourmiliere,
                                           ERR_FOURMILIERE, ERR_TROP);
		return L_EXIT;
	}
	return FAUX;
}

int fourmiliere_test_pos_garde(unsigned num_fourmiliere, unsigned num_garde,
							   double x_garde, double y_garde) {
    int distance = sqrt(pow(x_garde - (p_fourmiliere+num_fourmiliere)->x,2) +
                        pow(y_garde - (p_fourmiliere+num_fourmiliere)->y,2));
    if (distance > (p_fourmiliere+num_fourmiliere)->rayon -
        (RAYON_FOURMI + EPSIL_ZERO)){
        error_pos_garde(num_fourmiliere, num_garde);
        return VRAI;
    }
    return FAUX;
}

int fourmiliere_test_superposition(){
    unsigned i = 0;
    unsigned j = 0;
    while ((p_fourmiliere + i) != NULL){
        printf("%lf %lf %lf",(p_fourmiliere+i)->x,
               (p_fourmiliere+i)->y,
               (p_fourmiliere+i)->rayon);
        j = i + 1;
        while ((p_fourmiliere + j) != NULL){
            printf("%lf %lf %lf",(p_fourmiliere+j)->x,
                   (p_fourmiliere+j)->y,
                   (p_fourmiliere+j)->rayon);
            double dist1 = pow((p_fourmiliere+i)->x - (p_fourmiliere+j)->x,2);
            double dist2 = pow((p_fourmiliere+i)->y - (p_fourmiliere+j)->y,2);
            double distance = sqrt(dist1 + dist2);
            printf("%lf\n", dist1);
            printf("%lf\n", dist2);
            printf("%lf\n", distance);
            double r1 = (p_fourmiliere+i)->rayon;
            double r2 = (p_fourmiliere+j)->rayon;
            if (distance - (r1+ r2) <= 0){
                error_superposition_fourmiliere(i, j);
                return  VRAI;
            }
            j = j + 1;
        }
        i = i + 1;
    }
    return FAUX;
}
