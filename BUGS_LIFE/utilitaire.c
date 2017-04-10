/*!
 \file utilitaire.c
 \brief Module qui gère des tâches communes à divers modules
 \author Diane Remmy & Max Chevron
 \version 1.0
 \date Mars 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fourmiliere.h"
#include "nourriture.h"
#include "fourmi.h"
#include "constantes.h"
#include "error.h"
#include "utilitaire.h"


int utilitaire_test_pos_domaine(ERREUR_ORIG origine, unsigned num_fourmiliere, 
								double x, double y){
	if ((x < -DMAX) || (x > DMAX) || (y < -DMAX) || (y > DMAX)) {
		error_pos_domaine(origine, num_fourmiliere, x, y);
		return VRAI;	
	}
	return FAUX;
}

double utilitaire_calcul_distance(double x1, double x2, double y1, double y2){
    double distance = sqrt(pow(x1 - x2,2) + pow(y1 - y2,2));
    return distance;
}


