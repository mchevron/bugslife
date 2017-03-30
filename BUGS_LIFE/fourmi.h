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

int fourmi_test_age(unsigned num_fourmiliere, unsigned num_fourmi,
					 unsigned age);
int fourmi_ouvriere_lecture(unsigned i, char tab[MAX_LINE]);
int fourmi_garde_lecture(unsigned i, char tab[MAX_LINE]);
