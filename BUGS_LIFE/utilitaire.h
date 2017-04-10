/*!
 \file utilitaire.h
 \brief Module qui gère des tâches communes à divers modules
 \author Diane Remmy & Max Chevron
 \version 1.0
 \date Mars 2017
 */

#include "error.h"

int utilitaire_test_pos_domaine(ERREUR_ORIG origine, unsigned num_fourmiliere, 
								double x, double y);
double utilitaire_calcul_distance(double x1, double x2, double y1, double y2);

