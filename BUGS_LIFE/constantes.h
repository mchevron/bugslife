/*!
 \file constantes.h
 \brief Constantes utilisées par le programme
 \author PROG II
 \version 0.9
 \date fevrier 2017
 */

#ifndef CONSTANTES_H
#define CONSTANTES_H

#include "tolerance.h"

#define DELTA_T         0.25
#define BUG_SPEED       4
#define BUG_LIFE        1000
#define DMAX		    20
#define RAYON_FOURMI    1
#define RAYON_FOOD      0.25
#define VAL_FOOD        1
#define FOOD_RATE       0.1
#define FEED_RATE       0.0625
#define BIRTH_RATE      0.01
#define MAX_RENDU1      5 
#define	MAX_FOURMILIERE 10
#define MAX_LINE        120
#define GRAPHIC_EMPTY   0
#define GRAPHIC_FILLED  1
#define AUTOMATIC       0
#define INFO_COLONNES   4
#define INFO            5
#define BLANK           0
#define RUN             1
#define MODE_SIMPLE		1
#define MODE_SPECIFIQUE 3
#define RGB             3
#define ECHEC           0
#define MAX_LENGTH      20
#define EMPTY           1

/********** User IDs pour les fonctions callback ********/
#define FILE_NAME_OPEN  01
#define OPEN            02
#define FILE_NAME_SAVE  03
#define SAVE            04
#define START           05
#define STEP            06
#define EXIT            07
#define RECORD          11
#define AUTO_MAN        12


#endif
