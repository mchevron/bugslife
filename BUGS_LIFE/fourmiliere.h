/*!
 \file fourmilere.h
 \brief Module qui gère les données liées aux fourmilieres
 \author Diane Remmy & Max Chevron
 \version 1.0
 \date Mars 2017
 */

#include "constantes.h"
#include "error.h"
#include "utilitaire.h"

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
int fourmiliere_test_pos_garde(unsigned num_fourmiliere, unsigned num_garde, 
							   double x_garde, double y_garde);
int fourmiliere_test_superposition(void);
void fourmiliere_dessine(void);
char* fourmiliere_get_info_rollout(unsigned info, unsigned i);
void fourmiliere_save(FILE *f_sortie);
void fourmiliere_free(void);

