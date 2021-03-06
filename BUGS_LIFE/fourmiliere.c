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
#include "nourriture.h"
#include "error.h"
#include "constantes.h"
#include "fourmi.h"
#include "graphic.h"
#include "utilitaire.h"
#include "fourmiliere.h"

#define NB_ELEMENTS_FOURMILIERE 6
#define NAISSANCE				1
#define DEAD					1
#define RATIO					0.25
#define MAX_GARDE				10
#define MARGE_DEVIATION         2
#define PROXIMITE               3

struct fourmiliere
{
    double x;
    double y;
    int nbO;
    int nbG;
    int nbF;
    double total_food;
    double rayon;
    FOURMI * p_fourmi_ouvriere;
    FOURMI * p_fourmi_garde;
};

static FOURMILIERE * p_fourmiliere = NULL;
static int nb_fourmiliere;
static char info_glui[MAX_LENGTH];
static int nbF_T = 0, nbO_T = 0, nbG_T = 0, total_food_T = 0;
// tab_dead se met à 1 lorsqu'une fourmilière meurt (outil de comparaison)
static int tab_dead[MAX_FOURMILIERE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 

// lecture du nombre de fourmilière
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
    if (sscanf(tab, "%*[ \t]%lf %lf %d %d %lf %lf", &(p_fourmiliere+i)->x,
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
    if ((strcmp(check, "FIN_LISTE") == ECHEC) && ((p_fourmiliere+i)->nbG == 0))
        return L_FOURMILIERE;
    if(strcmp(check, "FIN_LISTE") == ECHEC) {
        j=0;
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
    if(strcmp(check, "FIN_LISTE") == ECHEC && i==nb_fourmiliere-1) 
		return L_NB_NOURRITURE;
    if(strcmp(check, "FIN_LISTE") == ECHEC) return L_FOURMILIERE;
    if(sscanf(tab, " %lf", &test)==ECHEC) return L_GARDE;
    return L_CONTINUE;
}

int fourmiliere_test_rayon(unsigned num_fourmiliere, int nbF, double total_food,
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

int fourmiliere_test_pos_garde_lecture(unsigned num_fourmiliere, unsigned num_garde,
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

int fourmiliere_test_pos_garde(unsigned num_fourmiliere,
                               double x_garde, double y_garde) {
    int distance = sqrt(pow(x_garde - (p_fourmiliere+num_fourmiliere)->x,2) +
                        pow(y_garde - (p_fourmiliere+num_fourmiliere)->y,2));
    if (distance > (p_fourmiliere+num_fourmiliere)->rayon -
        (RAYON_FOURMI + EPSIL_ZERO)){
        return VRAI;
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

// test superposition en mode LECTURE ou SIMULATION
int fourmiliere_test_superposition(MODE_LS mode){
    unsigned i;
    unsigned j;
    if(nb_fourmiliere <= 1) return FAUX;
    for (i = 0; i < nb_fourmiliere - 1 ; i++){
		if (!(tab_dead[i])){						//vérifie si fourmi morte ou pas
	        for (j = i + 1; j < nb_fourmiliere; j++){
				if (!(tab_dead[j])){
		            double distance = utilitaire_calcul_distance((p_fourmiliere+i)->x,
		                                                         (p_fourmiliere+j)->x,
		                                                         (p_fourmiliere+i)->y,
		                                                         (p_fourmiliere+j)->y);
		            double r1 = (p_fourmiliere+i)->rayon;
		            double r2 = (p_fourmiliere+j)->rayon;
		            if (mode == LECTURE){
			            if (distance - (r1+ r2) <= 0){
			                error_superposition_fourmiliere(i, j);
			                return  VRAI; 
						}
					}
					else {
						if (distance - (r1+ r2) <= EPSIL_ZERO){
							(p_fourmiliere+i)->rayon = ((distance - EPSIL_ZERO)/
													   (r1 + r2)*r1);
							(p_fourmiliere+j)->rayon = ((distance - EPSIL_ZERO)/
													   (r1 + r2)*r2);
						}
					}
		            if ((p_fourmiliere+i)->nbO != 0 || (p_fourmiliere+i)->nbO != 0)
			            if(fourmi_test_superposition_oo(&((p_fourmiliere+i)->p_fourmi_ouvriere),
														&((p_fourmiliere+j)->p_fourmi_ouvriere),
														i, j, mode)){
							if (mode == SIMULATION){
								fourmiliere_diminuer_nbF(i,j, T_OUVRIERE, T_OUVRIERE);
							}	
			                else return VRAI;
						}
			        if ((p_fourmiliere+i)->nbG != 0 || (p_fourmiliere+i)->nbO != 0)
			            if(fourmi_test_superposition_go(&(p_fourmiliere+i)->p_fourmi_garde,
														&(p_fourmiliere+j)->p_fourmi_ouvriere,
														i, j, mode)) {
							if (mode == SIMULATION){
								fourmiliere_diminuer_nbF(i,j, T_GARDE, T_OUVRIERE);
							}	
			                else return VRAI;
						}
			        if ((p_fourmiliere+i)->nbO != 0 || (p_fourmiliere+i)->nbG != 0)
			            if(fourmi_test_superposition_og(&(p_fourmiliere+i)->p_fourmi_ouvriere,
														&(p_fourmiliere+j)->p_fourmi_garde, 
														i, j, mode)){
							if (mode == SIMULATION){
								fourmiliere_diminuer_nbF(i,j, T_OUVRIERE, T_GARDE);
							}
							else return VRAI;
						}
			        if ((p_fourmiliere+i)->nbG != 0 || (p_fourmiliere+i)->nbG != 0)
			            if(fourmi_test_superposition_gg(&(p_fourmiliere+i)->p_fourmi_garde,
														&(p_fourmiliere+j)->p_fourmi_garde,
														i, j, mode)) {
							if (mode == SIMULATION){
								fourmiliere_diminuer_nbF(i,j, T_GARDE, T_GARDE);
							}	
			                else return VRAI;
						}
				}
			}
		}
    }
    return FAUX;
}

// reduit le nombre de fourmi si superposition et mort prématurée de 2 fourmis
void fourmiliere_diminuer_nbF(unsigned i, unsigned j, 
							  TYPE_FOURMI type1, TYPE_FOURMI type2){
	if ((type1 == T_OUVRIERE) && (type2 == T_OUVRIERE)){
		(p_fourmiliere+i)->nbO -= DEAD;
		(p_fourmiliere+j)->nbO -= DEAD;
		(p_fourmiliere+i)->nbF -= DEAD;
		(p_fourmiliere+j)->nbF -= DEAD;
	}
	if ((type1 == T_GARDE) && (type2 == T_OUVRIERE)){
		(p_fourmiliere+i)->nbG -= DEAD;
		(p_fourmiliere+j)->nbO -= DEAD;
		(p_fourmiliere+i)->nbF -= DEAD;
		(p_fourmiliere+j)->nbF -= DEAD;
	}
	if ((type1 == T_OUVRIERE) && (type2 == T_GARDE)){
		(p_fourmiliere+i)->nbO -= DEAD;
		(p_fourmiliere+j)->nbG -= DEAD;
		(p_fourmiliere+i)->nbF -= DEAD;
		(p_fourmiliere+j)->nbF -= DEAD;
	}	
	if ((type1 == T_GARDE) && (type2 == T_GARDE)){
		(p_fourmiliere+i)->nbG -= DEAD;
		(p_fourmiliere+j)->nbG -= DEAD;
		(p_fourmiliere+i)->nbF -= DEAD;
		(p_fourmiliere+j)->nbF -= DEAD;
	}
}

void fourmiliere_dessine(void) {
    unsigned i = 0;
    for(i=0; i<nb_fourmiliere; i=i+1) {
        graphic_find_color (i);
        graphic_draw_circle ((p_fourmiliere+i)->x, (p_fourmiliere+i)->y,
                             (p_fourmiliere+i)->rayon, GRAPHIC_EMPTY);
    if ((p_fourmiliere+i)->nbO != 0)
		fourmi_dessine((p_fourmiliere+i)->p_fourmi_ouvriere, i, (p_fourmiliere+i)->nbO,
						T_OUVRIERE);
	if ((p_fourmiliere+i)->nbG != 0)
		fourmi_dessine((p_fourmiliere+i)->p_fourmi_garde, i, (p_fourmiliere+i)->nbG,
						T_GARDE);
	}
}

// donne les informations de la fourmilière pour le panel
char* fourmiliere_get_info_rollout(unsigned info, unsigned i) {
    char color_name[MAX_FOURMILIERE][MAX_LENGTH] = {"Red", "Green",
        "Blue", "Yellow", "Cyan", "Purple", "Grey", "Orange",
        "Brown", "Khaki"};
    char* color = color_name[i];
    char empty[EMPTY] = "";
    int total_food_int = 0;
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
				total_food_int = (p_fourmiliere+i)->total_food;
                sprintf(info_glui, "%d", total_food_int);
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
        fprintf(f_sortie, " %lf", (p_fourmiliere+i)->total_food);
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

void fourmiliere_update(void) {
    int i = 0;
    fourmiliere_naissance_fourmi(); 
    fourmiliere_consommation();  
    fourmiliere_rayon();
    fourmiliere_test_superposition(SIMULATION);
    for (i = 0; i < nb_fourmiliere; i++){
        if((p_fourmiliere+i)->nbO!=0)
            if (fourmi_ouvriere_update(&(p_fourmiliere+i)->p_fourmi_ouvriere, i,
										nb_fourmiliere)){
				(p_fourmiliere+i)->nbO -= DEAD;
				(p_fourmiliere+i)->nbF -= DEAD;
			}
        if((p_fourmiliere+i)->nbG!=0)
			if (fourmi_garde_update(&(p_fourmiliere+i)->p_fourmi_garde, i, 
										nb_fourmiliere)){
				(p_fourmiliere+i)->nbG -= DEAD;
				(p_fourmiliere+i)->nbF -= DEAD;
			}
    }
}
    
void fourmiliere_naissance_fourmi(void){
	unsigned i;
	double ratio = VRAI;
    double rand_max = RAND_MAX;
	for (i = 0; i < nb_fourmiliere; i++){
		if (!(tab_dead[i])){
			double p = ((p_fourmiliere+i)->total_food) * BIRTH_RATE;
			if (rand()/rand_max <= p) {
				if(p_fourmiliere+i) {
	                if((p_fourmiliere+i)->nbO!=0) ratio = ((p_fourmiliere+i)->nbG/
														  (p_fourmiliere+i)->nbO);
	                if ((ratio <= RATIO) && ((p_fourmiliere+i)->nbG <= MAX_GARDE)){
	                    fourmi_naissance(&(p_fourmiliere+i)->p_fourmi_garde,T_GARDE, 
	                                    (p_fourmiliere+i)->x,(p_fourmiliere+i)->y);
	                    (p_fourmiliere+i)->nbG += NAISSANCE;
	                }
	                else {
	                    fourmi_naissance(&(p_fourmiliere+i)->p_fourmi_ouvriere, 
										T_OUVRIERE, (p_fourmiliere+i)->x,
										(p_fourmiliere+i)->y);
	                    (p_fourmiliere+i)->nbO += NAISSANCE;
	                }
	                (p_fourmiliere+i)->nbF += NAISSANCE;
	            }
			}
		}
	}
}

// diminution de total_food et mort de fourmi si nbF & total_food sont nuls
void fourmiliere_consommation(void){
	unsigned i;
	for (i = 0; i < nb_fourmiliere; i++){
		if (!(tab_dead[i])){
			(p_fourmiliere+i)->total_food -= (p_fourmiliere+i)->nbF*FEED_RATE;
			if ((p_fourmiliere+i)->total_food < VAL_FOOD){
				(p_fourmiliere+i)->total_food = 0;
	
	            if ((p_fourmiliere+i)->nbF == 0)
	                fourmiliere_destruction(i);
			}
		}
	}
}

void fourmiliere_destruction(unsigned i){
	(p_fourmiliere+i)->x = 0.;
	(p_fourmiliere+i)->y = 0.;
    (p_fourmiliere+i)->nbO = 0;
    (p_fourmiliere+i)->nbG = 0;
    (p_fourmiliere+i)->nbF = 0;
    (p_fourmiliere+i)->total_food = 0;
    (p_fourmiliere+i)->rayon = 0.;
	fourmi_free(&(p_fourmiliere+i)->p_fourmi_ouvriere);
	fourmi_free(&(p_fourmiliere+i)->p_fourmi_garde);

	free((p_fourmiliere+i)->p_fourmi_ouvriere);
	free((p_fourmiliere+i)->p_fourmi_garde);
	(p_fourmiliere+i)->p_fourmi_ouvriere = NULL;
	(p_fourmiliere+i)->p_fourmi_garde = NULL;
	tab_dead[i] = DEAD;
}

void fourmiliere_rayon(void){
	unsigned i;
	for (i = 0; i < nb_fourmiliere; i++){
		if (!(tab_dead[i])){
			(p_fourmiliere+i)->rayon = (1 + sqrt((p_fourmiliere+i)->nbF) + 
								   sqrt((p_fourmiliere+i)->total_food))*RAYON_FOURMI;
		}
	}
}

// test de superposition entre fourmiliere et nourriture + appel du test avec fourmi
int fourmiliere_nourriture_test_superposition(double x, double y){
	unsigned i = 0;
    for (i = 0; i < nb_fourmiliere; i++){
		if (!(tab_dead[i])){
			double distance = utilitaire_calcul_distance((p_fourmiliere+i)->x, x,
	                                                     (p_fourmiliere+i)->y, y);
	        double r1 = (p_fourmiliere+i)->rayon;
	        if (distance - (r1 + RAYON_FOOD) <= EPSIL_ZERO)
	            return  VRAI;
	
			if ((p_fourmiliere+i)->nbO != 0)
		        if (fourmi_nourriture_test_superposition_o(
					(p_fourmiliere+i)->p_fourmi_ouvriere, x, y))
					return VRAI;
					
	        if ((p_fourmiliere+i)->nbG != 0)
	            if (fourmi_nourriture_test_superposition_g(
					(p_fourmiliere+i)->p_fourmi_garde, x, y))
	                return VRAI;
		}
	}
	return FAUX;
}

void fourmiliere_new_food(unsigned i) {
    (p_fourmiliere+i)->total_food += NEW_FOOD;
}

// chemin de retour de la fourmi avec deviation de fourmilière sur celui-ci
void fourmiliere_retour_et_deviation(double ouvri_x, double ouvri_y, double *butx,
                                     double *buty, unsigned i, unsigned type) {
    if(type==OUV_CARRY || type==DEFAULT || type==OUV_NO_FOOD){
        *butx = (p_fourmiliere+i)->x;
        *buty = (p_fourmiliere+i)->y;
    }
    if(type==OUV_CARRY || type==OUV_EMPTY){
        unsigned k = 0;
        for (k = 0; k < nb_fourmiliere; k++){
            if(k!=i && tab_dead[k]==0){
                if(fourmiliere_sur_chemin(ouvri_x, ouvri_y, i, *butx, *buty) == VRAI){
                    foumriliere_ouvri_changer_but(butx, (p_fourmiliere+k)->x, buty, 
												 (p_fourmiliere+k)->y, ouvri_x, 
												  ouvri_y, k, i);
                }
            }
        }
    }
}

// test si ouvrière entre dans la fourmilière attaque si oui
void fourmiliere_test_ouvri_intrusion(FOURMI *p_garde, unsigned i) {
    unsigned k = 0;
    for (k = 0; k < nb_fourmiliere; k++){
        if(k!=i && tab_dead[k]==0){
            fourmi_ouvriere_intrusion(p_garde, (p_fourmiliere+k)->p_fourmi_ouvriere,
									  i, (p_fourmiliere+i)->x, (p_fourmiliere+i)->y,
                                      (p_fourmiliere+i)->rayon);
        }
    }
}

// test si d'autres ouvrières sont proches de l'élément que l'ouvrière veut aller 
// chercher
float fourmiliere_test_ouvri_competition(double distance_new, unsigned i, 
										 double nourri_x, double nourri_y){
    unsigned k = 0;
    float risque_mort_new=0;
    for (k = 0; k < nb_fourmiliere; k++){
        if(k!=i && tab_dead[k]==0){
            if(fourmi_test_ouvri_competition(distance_new, 
											(p_fourmiliere+k)->p_fourmi_ouvriere,
                                            nourri_x, nourri_y)==1) 
                risque_mort_new= RISQUE_MORT;
        }
    }
    return risque_mort_new;
}

// test s'il y a des nourritures disponibles pour les ouvrières de la fourmilière
float fourmiliere_test_nourri_dispo(unsigned i, double nourri_x, double nourri_y){
    unsigned dispo = VRAI;
    if(fourmi_test_nourri_dispo((p_fourmiliere+i)->p_fourmi_ouvriere,
                                nourri_x, nourri_y)==0) dispo=0;
    return dispo;
}

// enregistrement du nombre de fourmilières
void fourmiliere_record(FILE *f_record) {
	unsigned i=0;
    for(i=0; i<nb_fourmiliere; i++){
		fprintf(f_record, " %u", (p_fourmiliere+i)->nbF);
	}
	fputs("\n", f_record);
}

// test s'il y a une fourmilière sur le chemin d'une ouvrière
float fourmiliere_sur_chemin(double ouvri_x, double ouvri_y, unsigned i, 
							 double nourri_x, double nourri_y){
    unsigned k = 0;
    nourri_x = nourri_x - ouvri_x;
    nourri_y = nourri_y - ouvri_y;
    double fourmiliere_x;
    double fourmiliere_y;
    float risque_fourmiliere_chemin=0;
    for (k = 0; k < nb_fourmiliere; k++){
        if(k!=i && tab_dead[k]==0){
            fourmiliere_x = (p_fourmiliere+k)->x - ouvri_x;
            fourmiliere_y = (p_fourmiliere+k)->y - ouvri_y;
            double distance_ortho = utilitaire_dist_proj_ortho(nourri_x, 
											fourmiliere_x, nourri_y, fourmiliere_y);
            double distance_ouvri_fourmiliere = utilitaire_calcul_distance(0, 
												fourmiliere_x, 0, fourmiliere_y);
            double distance_ouvri_nourriture = utilitaire_calcul_distance(0, 
												nourri_x, 0, nourri_y);
            if(distance_ortho <= ((p_fourmiliere+k)->rayon + RAYON_FOURMI) &&
               ((fabs(distance_ouvri_fourmiliere - (p_fourmiliere+k)->rayon)) 
														< distance_ouvri_nourriture))
                risque_fourmiliere_chemin = RISQUE_FOURMILIERE;
        }
    }
    return risque_fourmiliere_chemin;
}

// test s'il y a une ouvrière d'une autre fourmilière sur le chemin d'une ouvrière
double fourmiliere_ouvri_sur_chemin(double ouvri_x, double ouvri_y, unsigned i, 
									double nourri_x, double nourri_y){
    unsigned k = 0;
    nourri_x = nourri_x - ouvri_x;
    nourri_y = nourri_y - ouvri_y;
    float risque_ouvriere_chemin=0;
    for (k = 0; k < nb_fourmiliere; k++){
        if(k!=i && tab_dead[k]==0){
            if(fourmi_ouvri_sur_chemin((p_fourmiliere+k)->p_fourmi_ouvriere, ouvri_x,
									   ouvri_y, nourri_x, nourri_y)==1) 
			 risque_ouvriere_chemin=RISQUE_OUVRI;
        }
    }
    return risque_ouvriere_chemin;
}

// ouvrière change de chemin si nourriture se crée qui est plus proche que son but
void foumriliere_ouvri_changer_but(double *butx,double x2,double *buty,double y2, 
								   double ouvri_x,double ouvri_y, unsigned k,
                                   unsigned i) {
    double x1 = *butx - ouvri_x;
    double y1 = *buty - ouvri_y;
    x2 = x2 - ouvri_x;
    y2 = y2 - ouvri_y;
    double proj_parti_1 = x2*x1 + y2*y1;
    double proj_parti_2 = x1*x1 + y1*y1;
    double proj_ortho_x = (proj_parti_1/proj_parti_2)*x1;
    double proj_ortho_y = (proj_parti_1/proj_parti_2)*y1;
    double distance_fc_fpo = utilitaire_calcul_distance(x2, proj_ortho_x, y2,
                                                        proj_ortho_y);
            //distance fourmiliere centre avec la projetée orthogonale
            // du centre de la fourmiliere
    double distance_fc_fpo_x = (proj_ortho_x-x2);
    double distance_fc_fpo_y = (proj_ortho_y-y2);
    double devia_x = ((((p_fourmiliere+k)->rayon + RAYON_FOURMI + EPSIL_ZERO + MARGE_DEVIATION)
                                 *distance_fc_fpo_x)/distance_fc_fpo) + (p_fourmiliere+k)->x;
    double devia_y = ((((p_fourmiliere+k)->rayon + RAYON_FOURMI + EPSIL_ZERO + MARGE_DEVIATION)
                       *distance_fc_fpo_y)/distance_fc_fpo) + (p_fourmiliere+k)->y;
    double distance_devia_pos = utilitaire_calcul_distance(ouvri_x, devia_x, ouvri_y,
                                                           devia_y);
    if(distance_devia_pos > RAYON_FOURMI || distance_devia_pos < DIST_MAX) {
        *butx = devia_x;
        *buty = devia_y;
    }
    else{
        *butx = (p_fourmiliere+i)->x;
        *buty = (p_fourmiliere+i)->y;
    }
}

//Les ouvrières attaquent une autre fourmiliere
int fourmiliere_ouvri_attaque(double *butx, double *buty, unsigned i) {
    int attaque = 0;
    int d = 0;
    int fourmilieres_en_vie = nb_fourmiliere;
    for(d = 0; d<MAX_FOURMILIERE; d++)
        fourmilieres_en_vie -= tab_dead[d];
            //on décremente de 1 pour chaque fourmiliere morte
    if(fourmilieres_en_vie>1) {
        //unsigned nbG_min = (p_fourmiliere+i)->nbO;
            // Si nbG > nbO alors ppas la peine d'attaquer
        unsigned k = 0;
        double distance = DIST_MAX;
        double distance_new = distance;
        for (k = 0; k < nb_fourmiliere; k++){
            if(k!=i && tab_dead[k]==0){
                if((p_fourmiliere+k)->nbG < (p_fourmiliere+i)->nbO) {
                    distance_new = utilitaire_calcul_distance((p_fourmiliere+i)->x,
                                                          (p_fourmiliere+k)->x,
                                                          (p_fourmiliere+i)->y,
                                                          (p_fourmiliere+k)->y);
                    if((nourriture_get_nb()==0 && distance_new < distance) ||
                       distance_new <= (p_fourmiliere+i)->rayon
                       + (p_fourmiliere+k)->rayon + PROXIMITE) {
                        *butx = (p_fourmiliere+k)->x;
                        *buty = (p_fourmiliere+k)->y;
                        attaque = ATTAQUE;
                        distance = distance_new;
                    }
                }
            }
        }
    }
    return attaque;
}

int fourmiliere_food_diminue(double *butx, double *buty, unsigned i) {
    unsigned k = 0;
    for (k = 0; k < nb_fourmiliere; k++){
        if(k!=i && tab_dead[k]==0){
            if((p_fourmiliere+k)->x == *butx && (p_fourmiliere+k)->y == *buty) {
                if((p_fourmiliere+k)->total_food<1) return 0;
                else {
                    (p_fourmiliere+k)->total_food --;
                    return 1;
                }
            }
        }
    }
    return 0;
}

// vérifie si l'objectif était une fourmiliere ou un élément de nourriture
int fourmiliere_ouvri_test_objectif(double posx, double posy,
                                    double butx, double buty, double i) {
    unsigned k = 0;
    double distance = DIST_MAX;
    for (k = 0; k < nb_fourmiliere; k++){
        if(k!=i && tab_dead[k]==0){
            distance = utilitaire_calcul_distance((p_fourmiliere+k)->x, posx,
                                                  (p_fourmiliere+k)->y, posy);
            if(distance <= RAYON_FOURMI)
                return VRAI;
        }
    }
    if(nourriture_ouvri_test_objectif(posx, posy, butx, buty)) return VRAI;
    return FAUX;
}
