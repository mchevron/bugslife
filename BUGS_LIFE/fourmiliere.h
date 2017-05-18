/*!
 \file fourmilere.h
 \brief Module qui gère les données liées aux fourmilieres
 \author Diane Remmy & Max Chevron
 \version 2.0
 \date Avril 2017
 */

#include "fourmi.h"
#include "constantes.h"
#include "error.h"

typedef struct fourmiliere FOURMILIERE;

int fourmiliere_nb_fourmiliere(char tab[MAX_LINE]);
int fourmiliere_lecture(unsigned i, char tab[MAX_LINE]);
int fourmiliere_ouvriere_lecture_precontrol(unsigned i, unsigned j,
											char tab[MAX_LINE]);
int fourmiliere_garde_lecture_precontrol(unsigned i, unsigned j,
										 char tab[MAX_LINE]);
int fourmiliere_test_rayon(unsigned num_fourmiliere, int nbF, double total_food,
							double rayon_fourmiliere);
int fourmiliere_test_lecture_elements(unsigned nb_fourmiliere_fichier);
int fourmiliere_test_pos_domaine(ERREUR_ORIG origine, unsigned num_fourmiliere, 
								double x, double y);
int fourmiliere_test_pos_garde_lecture(unsigned num_fourmiliere, unsigned num_garde,
                                       double x_garde, double y_garde);
int fourmiliere_test_pos_garde(unsigned num_fourmiliere, double x_garde,
                               double y_garde);
int fourmiliere_test_superposition(MODE_LS mode);
void fourmiliere_diminuer_nbF(unsigned i, unsigned j, 
							  TYPE_FOURMI type1, TYPE_FOURMI type2);
int fourmiliere_test_nbO(int i, int j);
void fourmiliere_dessine(void);
char* fourmiliere_get_info_rollout(unsigned info, unsigned i);
void fourmiliere_save(FILE *f_sortie);
void fourmiliere_free(void);
void fourmiliere_update(void);
void fourmiliere_naissance_fourmi(void);
void fourmiliere_consommation(void);
void fourmiliere_destruction(unsigned i);
void fourmiliere_rayon(void);
int fourmiliere_nourriture_test_superposition(double x, double y);
void fourmiliere_new_food(unsigned i);
void fourmiliere_retour_et_deviation(double ouvri_x, double ouvri_y, double *butx, 
									 double *buty, unsigned i, unsigned type);
void fourmiliere_test_ouvri_intrusion(FOURMI *p_garde, unsigned i);
float fourmiliere_test_ouvri_competition(double distance_new, unsigned i, 
										 double nourri_x, double nourri_y);
float fourmiliere_test_nourri_dispo(unsigned i, double nourri_x, double nourri_y);
void fourmiliere_record(FILE *f_record);
float fourmiliere_sur_chemin(double ouvri_x, double ouvri_y, unsigned i,
                             double nourri_x, double nourri_y);
double fourmiliere_ouvri_sur_chemin(double ouvri_x, double ouvri_y, unsigned i,
                                    double nourri_x, double nourri_y);
void foumriliere_ouvri_changer_but(double *butx,double x2,double *buty,double y2,
                                   double ouvri_x,double ouvri_y, unsigned k, unsigned i);
int fourmiliere_ouvri_attaque(double *butx, double *buty, unsigned i);
int fourmiliere_food_diminue(double *butx, double *buty, unsigned i);
int fourmiliere_ouvri_test_objectif(double posx, double posy,
                                    double butx, double buty, double i);
