/*!
 \file fourmilere.c
 \brief Module qui gère les données liées aux fourmilieres
 \author Diane Remmy & Max Chevron
 \version 2.0
 \date Avril 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "error.h"
#include "constantes.h"
#include "fourmi.h"
#include "graphic.h"
#include "utilitaire.h"
#include "fourmiliere.h"

#define NB_ELEMENTS_FOURMILIERE 6
#define EMPTY           		1

struct fourmiliere
{
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
static char info_glui[MAX_LENGTH];
static int nbF_T = 0, nbO_T = 0, nbG_T = 0, total_food_T = 0;

int fourmiliere_nb_fourmiliere(char tab[MAX_LINE]) {
    sscanf(tab, "%d", &nb_fourmiliere);
    if (nb_fourmiliere > MAX_FOURMILIERE) {
        error_nb_fourmiliere(nb_fourmiliere);
        return L_EXIT;
    }
    if (nb_fourmiliere == 0) return L_NB_NOURRITURE;
    return L_FOURMILIERE;
}

int fourmiliere_lecture(unsigned i, char tab[MAX_LINE]){
    if (i >= nb_fourmiliere) return L_NB_NOURRITURE;
    if (!p_fourmiliere){
        if(!(p_fourmiliere = malloc(MAX_FOURMILIERE*sizeof(FOURMILIERE)))){
            printf("No memory for %s\n", __func__);
            return L_EXIT;
        }
    }
    (p_fourmiliere+i)->p_fourmi_ouvriere = NULL;
	(p_fourmiliere+i)->p_fourmi_garde = NULL;
    if (sscanf(tab, "%*[ \t]%lf %lf %d %d %d %lf", &(p_fourmiliere+i)->x,
               &(p_fourmiliere+i)->y,
               &(p_fourmiliere+i)->nbO,
               &(p_fourmiliere+i)->nbG,
               &(p_fourmiliere+i)->total_food,
               &(p_fourmiliere+i)->rayon) != NB_ELEMENTS_FOURMILIERE){
        error_lecture_elements_fourmiliere(i, ERR_FOURMILIERE, ERR_PAS_ASSEZ);
        return L_EXIT;
    }
    fourmi_recoit(&(p_fourmiliere+i)->p_fourmi_ouvriere,
                  &(p_fourmiliere+i)->p_fourmi_garde);
    (p_fourmiliere+i)->nbF = (p_fourmiliere+i)->nbO + (p_fourmiliere+i)->nbG;
    if (fourmiliere_test_rayon(i,(p_fourmiliere+i)->nbF,
                               (p_fourmiliere+i)->total_food,
                               (p_fourmiliere+i)->rayon)) return L_EXIT;
    if (fourmiliere_test_pos_domaine(ERR_FOURMILIERE, i, (p_fourmiliere+i)->x,
                                    (p_fourmiliere+i)->y)) return L_EXIT;
    if((p_fourmiliere+i)->nbO > 0) return L_OUVRIERE;
    if((p_fourmiliere+i)->nbG > 0) return L_GARDE;
    if((p_fourmiliere+i)->nbF == 0) return L_FOURMILIERE;
    else return L_EXIT;
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
							double rayon_fourmiliere){
	double rayon_max = (1 + sqrt(nbF) + sqrt(total_food))*RAYON_FOURMI;
	if (rayon_fourmiliere > (double) rayon_max) {
		error_rayon_fourmiliere(num_fourmiliere);
        return L_EXIT;
	}
    return FAUX;
}

int fourmiliere_test_lecture_elements(unsigned nb_fourmiliere_fichier){
	if (nb_fourmiliere_fichier >= nb_fourmiliere) {
		error_lecture_elements_fourmiliere(nb_fourmiliere,
                                           ERR_FOURMILIERE, ERR_TROP);
		return L_EXIT;
	}
	return FAUX;
}

int fourmiliere_test_pos_domaine(ERREUR_ORIG origine, unsigned num_fourmiliere, 
								double x, double y){
	if ((x < -DMAX) || (x > DMAX) || (y < -DMAX) || (y > DMAX)) {
		error_pos_domaine(origine, num_fourmiliere, x, y);
		return VRAI;	
	}
	return FAUX;
}

int fourmiliere_test_pos_garde(unsigned num_fourmiliere, unsigned num_garde,
							   double x_garde, double y_garde) {
    double distance = sqrt(pow(x_garde - (p_fourmiliere+num_fourmiliere)->x,2) +
                        pow(y_garde - (p_fourmiliere+num_fourmiliere)->y,2));
    if (distance > (p_fourmiliere+num_fourmiliere)->rayon -
        RAYON_FOURMI){
        error_pos_garde(num_fourmiliere, num_garde);
        return VRAI;
    }
    return FAUX;
}

int fourmiliere_test_superposition(void){
    unsigned i = 0;
    unsigned j = 0;
    if(nb_fourmiliere<=1) return FAUX;
    for (i = 0; i < nb_fourmiliere; i++){
        for (j = i+ 1; j < nb_fourmiliere; j++){
            double distance = utilitaire_calcul_distance((p_fourmiliere+i)->x,
                                                         (p_fourmiliere+j)->x,
                                                         (p_fourmiliere+i)->y,
                                                         (p_fourmiliere+j)->y);
            double r1 = (p_fourmiliere+i)->rayon;
            double r2 = (p_fourmiliere+j)->rayon;
            if (distance - (r1+ r2) <= 0){
                error_superposition_fourmiliere(i, j);
                return  VRAI;
            }
			if ((p_fourmiliere+i)->nbO != 0 || (p_fourmiliere+i)->nbO != 0)
	            if(fourmi_test_superposition_oo((p_fourmiliere+i)->p_fourmi_ouvriere,
	                                    (p_fourmiliere+j)->p_fourmi_ouvriere,  i, j))
	                return VRAI;
	        if ((p_fourmiliere+i)->nbG != 0 || (p_fourmiliere+i)->nbO != 0)
	            if(fourmi_test_superposition_go((p_fourmiliere+i)->p_fourmi_garde,
	                                    (p_fourmiliere+j)->p_fourmi_ouvriere, i, j)) 
	                return VRAI;
	        if ((p_fourmiliere+i)->nbO != 0 || (p_fourmiliere+i)->nbG != 0)
	            if(fourmi_test_superposition_og((p_fourmiliere+i)->p_fourmi_ouvriere,
	                                      (p_fourmiliere+j)->p_fourmi_garde, i, j)) 
	                return VRAI;
	        if ((p_fourmiliere+i)->nbG != 0 || (p_fourmiliere+i)->nbG != 0)
	            if(fourmi_test_superposition_gg((p_fourmiliere+i)->p_fourmi_garde,
	                                      (p_fourmiliere+j)->p_fourmi_garde, i, j)) 
	                return VRAI;
        }
    }
    return FAUX;
}

void fourmiliere_dessine(void) {
    unsigned i = 0;
    for(i=0; i<nb_fourmiliere; i=i+1) {
        graphic_find_color (i);
        graphic_draw_circle ((p_fourmiliere+i)->x, (p_fourmiliere+i)->y,
                             (p_fourmiliere+i)->rayon, GRAPHIC_EMPTY);
    }
    fourmi_dessine(nb_fourmiliere, p_fourmiliere);
}

char* fourmiliere_get_info_rollout(unsigned info, unsigned i) {
    char color_name[MAX_FOURMILIERE][MAX_LENGTH] = {"Red", "Green",
        "Blue", "Yellow", "Cyan", "Purple", "Grey", "Orange",
        "Brown", "Khaki"};
    char* color = color_name[i];
    char empty[EMPTY] = "";
    if(i<nb_fourmiliere) {
        switch(info)
        {
            case COLOR:
                sprintf(info_glui, "%s", color);
                break;
            case NB_FOURMI:
                sprintf(info_glui, "%d", (p_fourmiliere+i)->nbF);
                nbF_T = nbF_T + (p_fourmiliere+i)->nbF;
                break;
            case NB_OUVRIERE:
                sprintf(info_glui, "%d", (p_fourmiliere+i)->nbO);
                nbO_T = nbO_T + (p_fourmiliere+i)->nbO;
                break;
            case NB_GARDE:
                sprintf(info_glui, "%d", (p_fourmiliere+i)->nbG);
                nbG_T = nbG_T + (p_fourmiliere+i)->nbG;
                break;
            case NB_NOURRITURE:
                sprintf(info_glui, "%d", (p_fourmiliere+i)->total_food);
                total_food_T = total_food_T + (p_fourmiliere+i)->total_food;
                break;
            case NBT_FOURMI:
                sprintf(info_glui, "%d", nbF_T);
                nbF_T = 0;
                break;
            case NBT_OUVRIERE:
                sprintf(info_glui, "%d", nbO_T);
                nbO_T = 0;
                break;
            case NBT_GARDE:
                sprintf(info_glui, "%d", nbG_T);
                nbG_T = 0;
                break;
            case NBT_NOURRITURE:
                sprintf(info_glui, "%d", total_food_T);
                total_food_T = 0;
                break;
            default:
                sprintf(info_glui, "%s", empty);
        }
    }
    else
        sprintf(info_glui, "%s", empty);
    return info_glui;
}

void fourmiliere_save(FILE *f_sortie) {
	fputs("# Nb fourmilieres\n", f_sortie);
	fprintf(f_sortie, "%u\n", nb_fourmiliere);
	
    unsigned i=0;
    for(i=0; i<nb_fourmiliere; i++){
		fputs("\n", f_sortie);
		fprintf(f_sortie, "  # Fourmiliere[%u]\n", i);
        fprintf(f_sortie, "    %lf", (p_fourmiliere+i)->x);
        fprintf(f_sortie, " %lf", (p_fourmiliere+i)->y);
        fprintf(f_sortie, " %u", (p_fourmiliere+i)->nbO);
        fprintf(f_sortie, " %u", (p_fourmiliere+i)->nbG);
        fprintf(f_sortie, " %u", (p_fourmiliere+i)->total_food);
        fprintf(f_sortie, " %lf\n", (p_fourmiliere+i)->rayon);
        if ((p_fourmiliere+i)->nbO != 0){
			fourmi_save_ouvriere(f_sortie, (p_fourmiliere+i)->p_fourmi_ouvriere);
			fputs("    FIN_LISTE\n", f_sortie);
		}
		if ((p_fourmiliere+i)->nbG != 0){
			fourmi_save_garde(f_sortie, (p_fourmiliere+i)->p_fourmi_garde);
			fputs("    FIN_LISTE\n", f_sortie);
		}
	}
	fputs("\n", f_sortie);
	fputs("FIN_LISTE\n", f_sortie);
	fputs("\n", f_sortie);
}

void fourmiliere_free(void){
    unsigned i;
    for (i = 0; i < nb_fourmiliere; i++){
        fourmi_free(&(p_fourmiliere+i)->p_fourmi_ouvriere);
        fourmi_free(&(p_fourmiliere+i)->p_fourmi_garde);

        free((p_fourmiliere+i)->p_fourmi_ouvriere);
        free((p_fourmiliere+i)->p_fourmi_garde);
        (p_fourmiliere+i)->p_fourmi_ouvriere = NULL;
        (p_fourmiliere+i)->p_fourmi_garde = NULL;
    }
    free(p_fourmiliere);
    p_fourmiliere = NULL;
    nb_fourmiliere = 0;
    char empty[EMPTY] = "";
    sprintf(info_glui, "%s", empty);
}

