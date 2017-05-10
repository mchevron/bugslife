/*!
 \file fourmilere.h
 \brief Module qui gère les données liées aux fourmilieres
 \author Diane Remmy & Max Chevron
 \version 2.0
 \date Avril 2017
 */

#include "constantes.h"
#include "error.h"

typedef struct fourmiliere FOURMILIERE;

int fourmiliere_nb_fourmiliere(char tab[MAX_LINE]);
int fourmiliere_lecture(unsigned i, char tab[MAX_LINE]);
int fourmiliere_ouvriere_lecture_precontrol(unsigned i, unsigned j,
											char tab[MAX_LINE]);
int fourmiliere_garde_lecture_precontrol(unsigned i, unsigned j,
										 char tab[MAX_LINE]);
int fourmiliere_test_rayon(unsigned num_fourmiliere, int nbF, int total_food,
							double rayon_fourmiliere);
int fourmiliere_test_lecture_elements(unsigned nb_fourmiliere_fichier);
int fourmiliere_test_pos_domaine(ERREUR_ORIG origine, unsigned num_fourmiliere, 
								double x, double y);
int fourmiliere_test_pos_garde(unsigned num_fourmiliere, unsigned num_garde, 
							   double x_garde, double y_garde);
int fourmiliere_test_superposition(MODE_LS mode);
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
