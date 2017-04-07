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
    FOURMI * tab_fourmi_ouvriere[MAX_RENDU1];
    FOURMI * tab_fourmi_garde[MAX_RENDU1];
};

static FOURMILIERE tab_fourmiliere[MAX_RENDU1];
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
    /*
    if (i >= nb_fourmiliere) return L_NB_NOURRITURE;
    if (sscanf(tab, " %lf %lf %d %d %d %lf", &tab_fourmiliere[i].x,
                                     &tab_fourmiliere[i].y,
                                     &tab_fourmiliere[i].nbO,
                                     &tab_fourmiliere[i].nbG,
                                     &tab_fourmiliere[i].total_food,
                                     &tab_fourmiliere[i].rayon) 
                                     != NB_ELEMENTS_FOURMILIERE){
		error_lecture_elements_fourmiliere(i, ERR_FOURMILIERE, ERR_PAS_ASSEZ);
		return L_EXIT;
	}
    tab_fourmiliere[i].nbF = tab_fourmiliere[i].nbO + tab_fourmiliere[i].nbG;
    if (fourmiliere_test_rayon(i,tab_fourmiliere[i].nbF,
							   tab_fourmiliere[i].total_food, 
							   tab_fourmiliere[i].rayon)) return L_EXIT;
	if (utilitaire_test_pos_domaine(ERR_FOURMILIERE, i, tab_fourmiliere[i].x,
									tab_fourmiliere[i].y)) return L_EXIT;
    */
    return L_OUVRIERE;
}

int fourmiliere_ouvriere_lecture_precontrol(unsigned i, unsigned j, 
											char tab[MAX_LINE]) {
    /*
    char check[MAX_LINE];
	double test = 0.;
    sscanf(tab, " %s", check);
    if((strcmp(check, "FIN_LISTE") == ECHEC)
		&& (j < tab_fourmiliere[i].nbO)) {
        error_lecture_elements_fourmiliere(i, ERR_OUVRIERE, ERR_PAS_ASSEZ);         
		return L_EXIT;
    }
    //if(strncmp(check, "FIN_LISTE", WORD_LENGTH_COMPARE) == ECHEC) {
    if(strcmp(check, "FIN_LISTE") == ECHEC) {
        return L_GARDE;
    }
    if(j == tab_fourmiliere[i].nbO && tab_fourmiliere[i].nbO > 0) {
        error_lecture_elements_fourmiliere(i, ERR_OUVRIERE, ERR_TROP);
        return L_EXIT;
    }
    if(tab_fourmiliere[i].nbF == 0) return L_FOURMILIERE;
    if(tab_fourmiliere[i].nbO == 0) {
        j = 0;
        return fourmi_garde_lecture(i, tab);
    }
    if(sscanf(tab, " %lf", &test) == ECHEC) return L_OUVRIERE;  
    */
    return L_CONTINUE;
}

int fourmiliere_garde_lecture_precontrol(unsigned i, unsigned j, char tab[MAX_LINE]){
    /*
    char check[MAX_LINE];
	double test = 0.;
    sscanf(tab, " %s", check);
    if((strcmp(check, "FIN_LISTE") == ECHEC)
		&& (j < tab_fourmiliere[i].nbG)) {
        error_lecture_elements_fourmiliere(i, ERR_GARDE, ERR_PAS_ASSEZ);
        return L_EXIT;                                                               
    }
    if (j > tab_fourmiliere[i].nbG) {											
        error_lecture_elements_fourmiliere(i, ERR_GARDE, ERR_TROP);
        return L_EXIT;
    }
    if(strcmp(check, "FIN_LISTE") == ECHEC) return L_FOURMILIERE;
    if(sscanf(tab, " %lf", &test)==ECHEC) return L_GARDE;
     */
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
    int distance = sqrt(pow(x_garde - tab_fourmiliere[num_fourmiliere].x,2) +
                        pow(y_garde - tab_fourmiliere[num_fourmiliere].y,2));
    if (distance > (tab_fourmiliere[num_fourmiliere].rayon - 
				   (RAYON_FOURMI + EPSIL_ZERO))){
        error_pos_garde(num_fourmiliere, num_garde);
        return VRAI;
    }
    return FAUX;
}
