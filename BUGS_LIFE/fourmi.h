/*!
 \file fourmi.h
 \brief Module qui gère les données liées aux fourmis
 \author Diane Remmy & Max Chevron
 \version 1.0
 \date Mars 2017
 */

#include "constantes.h"
#include "utilitaire.h"

typedef struct garde GARDE;
typedef struct ouvriere OUVRIERE;
typedef struct fourmi FOURMI;
typedef struct fourmiliere FOURMILIERE;

int fourmi_test_age(unsigned num_fourmiliere, unsigned num_fourmi,
                    unsigned age);
int fourmi_ouvriere_lecture(unsigned i, char tab[MAX_LINE]);
int fourmi_garde_lecture(unsigned i, char tab[MAX_LINE]);
int fourmi_test_superposition( FOURMI *p_frm_frml1, FOURMI * p_frm_frml2,
                              char type1[], char type2[], unsigned i, unsigned j );
FOURMI * ajouter_fourmi ( FOURMI ** p_tete );
void fourmi_dessine(unsigned nb_fourmiliere, FOURMILIERE * p_fourmiliere);
void fourmi_recoit( FOURMI **p_ouvriere, FOURMI ** p_garde);
void fourmi_save(FILE *f_sortie);
