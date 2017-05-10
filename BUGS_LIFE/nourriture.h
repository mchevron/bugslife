/*!
 \file nourriture.c
 \brief Module qui gère les données liées à la nourriture
 \author Diane Remmy & Max Chevron
 \version 2.0
 \date Avril 2017
 */

#include "constantes.h"

typedef struct nourriture NOURRITURE;

typedef enum EtatNour{
	VIDE, PLEIN
} ETAT_NOUR;

void nourriture_dessine(void);
int nourriture_nb_nourriture(char tab[MAX_LINE]);
int nourriture_lecture(char tab[MAX_LINE]);
NOURRITURE * nourriture_ajouter ( NOURRITURE ** p_tete );
void nourriture_clique(float pos_x, float pos_y);
void nourriture_save(FILE *f_sortie);
void nourriture_free(void);
void nourriture_vider ( NOURRITURE ** p_liste );
void nourriture_retirer ( NOURRITURE ** p_tete, NOURRITURE *nour );
unsigned nourriture_get_nb(void);
void nourriture_centre_dessine(ETAT_NOUR etat, double x, double y);
void nourriture_ajouter_fixe(double x, double y);
void nourriture_creation(void);
void nourriture_choix(double *posx, double *posy, double *butx, double *buty);
void nourriture_cherche_retire(double x, double y);
