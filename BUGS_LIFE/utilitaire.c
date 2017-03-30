/*!
 \file utilitaire.c
 \brief Module qui gère des tâches communes à divers modules
 \author Diane Remmy & Max Chevron
 \version 1.0
 \date Mars 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include "fourmiliere.h"
#include "nourriture.h"
#include "fourmi.h"
#include "utilitaire.h"
#include "constantes.h"
#include "error.h"

#define FAUX		0
#define VRAI		1

int utilitaire_test_pos_domaine(ERREUR_ORIG origine, unsigned num_fourmiliere, 
								double x, double y){
	if ((x < -DMAX) || (x > DMAX) || (y < -DMAX) || (y > DMAX)) {
		error_pos_domaine(origine, num_fourmiliere, x, y);
		return VRAI;	
	}
	return FAUX;
}

