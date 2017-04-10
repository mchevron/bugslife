/*!
 \file fourmi.h
 \brief Module qui gère les données liées aux fourmis
 \author Diane Remmy & Max Chevron
 \version 1.0
 \date Mars 2017
 */

#include "constantes.h"
#include "utilitaire.h"

typedef struct ouvriere OUVRIERE;
typedef struct garde GARDE;
typedef struct fourmi FOURMI;
typedef struct fourmiliere FOURMILIERE;

int fourmi_ouvriere_lecture(unsigned i, char tab[MAX_LINE]);
int fourmi_garde_lecture(unsigned i, char tab[MAX_LINE]);
int fourmi_test_age(unsigned num_fourmiliere, unsigned num_fourmi,
                    unsigned age);
void fourmi_recoit( FOURMI **p_ouvriere, FOURMI ** p_garde);
FOURMI * ajouter_fourmi ( FOURMI ** p_tete );
int fourmi_test_superposition_oo( FOURMI *p_frm_frml1, FOURMI * p_frm_frml2,
                                 unsigned i, unsigned j );
int fourmi_test_superposition_go( FOURMI *p_frm_frml1, FOURMI * p_frm_frml2,
                                 unsigned i, unsigned j );
int fourmi_test_superposition_og( FOURMI *p_frm_frml1, FOURMI * p_frm_frml2,
                                 unsigned i, unsigned j );
int fourmi_test_superposition_gg( FOURMI *p_frm_frml1, FOURMI * p_frm_frml2,
                                 unsigned i, unsigned j );
void fourmi_dessine(unsigned nb_fourmiliere, FOURMILIERE * p_fourmiliere);
void fourmi_save_ouvriere( FILE *f_sortie, FOURMI *p_ouvriere);
void fourmi_save_garde( FILE *f_sortie, FOURMI *p_garde);
void fourmi_free ( FOURMI ** p_liste );
void fourmi_retirer ( FOURMI ** p_tete, FOURMI *four );

