/*!
 \file fourmi.h
 \brief Module qui gère les données liées aux fourmis
 \author Diane Remmy & Max Chevron
 \version 2.0
 \date Avril 2017
 */

#include "constantes.h"
#include "error.h"

typedef struct ouvriere OUVRIERE;
typedef struct garde GARDE;
typedef struct fourmi FOURMI;
typedef struct fourmiliere FOURMILIERE;

int fourmi_ouvriere_lecture(unsigned i, char tab[MAX_LINE]);
int fourmi_garde_lecture(unsigned i, char tab[MAX_LINE]);
int fourmi_test_age(unsigned num_fourmiliere, unsigned num_fourmi,
                    unsigned age);
int fourmi_test_pos_domaine(ERREUR_ORIG origine, unsigned num_fourmiliere, 
								double x, double y);
void fourmi_recoit( FOURMI **p_ouvriere, FOURMI ** p_garde);
FOURMI * fourmi_ajouter ( FOURMI ** p_tete );
int fourmi_test_superposition_oo( FOURMI **p_frm_frml1, FOURMI ** p_frm_frml2,
                                 unsigned i, unsigned j , MODE_LS mode );
int fourmi_test_superposition_go( FOURMI **p_frm_frml1, FOURMI ** p_frm_frml2,
                                 unsigned i, unsigned j, MODE_LS mode );
int fourmi_test_superposition_og( FOURMI **p_frm_frml1, FOURMI ** p_frm_frml2,
                                 unsigned i, unsigned j , MODE_LS mode );
int fourmi_test_superposition_gg( FOURMI **p_frm_frml1, FOURMI ** p_frm_frml2,
                                 unsigned i, unsigned j ,MODE_LS mode );
void fourmi_dessine(FOURMI * p_fourmi, unsigned i, unsigned nbO, unsigned nbG);
void fourmi_save_ouvriere( FILE *f_sortie, FOURMI *p_ouvriere);
void fourmi_save_garde( FILE *f_sortie, FOURMI *p_garde);
void fourmi_free ( FOURMI ** p_liste );
void fourmi_retirer ( FOURMI ** p_tete, FOURMI *four );
void fourmi_naissance(FOURMI ** p_fourmi, TYPE_FOURMI type, double posx, double posy);
int fourmi_nourriture_test_superposition_o(FOURMI *p_four, double x, double y);
int fourmi_nourriture_test_superposition_g(FOURMI *p_four, double x, double y);
int fourmi_ouvriere_update(FOURMI **p_ouvriere, unsigned i, unsigned nb_fourmiliere);
int fourmi_garde_update(FOURMI **p_garde, unsigned i, unsigned nb_fourmiliere);
void fourmi_ouvriere_deplacement(FOURMI *p_ouvriere, unsigned i);
void fourmi_garde_deplacement(FOURMI *p_garde, unsigned i, unsigned nb_fourmiliere);
void fourmi_ouvriere_intrusion(FOURMI *p_garde, FOURMI *p_ouvriere,
                              unsigned i, unsigned c_x, unsigned c_y, unsigned rayon);
double fourmi_test_ouvri_competition(double distance_new, FOURMI *p_ouvriere,
                              double nourri_x, double nourri_y);
double fourmi_test_nourri_dispo(FOURMI *p_ouvriere, double nourri_x, double nourri_y);
double fourmi_ouvri_sur_chemin(FOURMI *p_ouvriere, double ouvri_x, double ouvri_y,
                               double nourri_x, double nourri_y);
