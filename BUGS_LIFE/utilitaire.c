/*!
 \file utilitaire.c
 \brief Module qui gère des tâches communes à divers modules
 \author Diane Remmy & Max Chevron
 \version 2.0
 \date Avril 2017
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

double utilitaire_calcul_distance(double x1, double x2, double y1, double y2){
    double distance = sqrt(pow(x1 - x2,2) + pow(y1 - y2,2));
    return distance;
}

double utilitaire_proba(double p){
    double val = 0;
    double rand_max = RAND_MAX;
    if (rand()/rand_max <= p) val = (rand()/rand_max);
    return val;
}

double utilitaire_dist_proj_ortho(double nourri_x,double nourri_y,double fourmiliere_x,double fourmiliere_y) {
    double proj_parti_1 = fourmiliere_x*nourri_x + fourmiliere_y*nourri_y;
    double proj_parti_2 = nourri_x*nourri_x + nourri_y*nourri_y;
    double proj_ortho_x = (proj_parti_1/proj_parti_2)*nourri_x;
    double proj_ortho_y = (proj_parti_1/proj_parti_2)*nourri_y;
    double distance = utilitaire_calcul_distance(proj_ortho_x,
                                                 fourmiliere_x,
                                                 proj_ortho_y,
                                                 fourmiliere_y);
    return distance;
}
