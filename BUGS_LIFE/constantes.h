/*!
 \file constantes.h
 \brief Constantes utilisées par le programme
 \author PROG II
 \version 2.0
 \date Avril 2017
 */

#ifndef CONSTANTES_H
#define CONSTANTES_H

#include "tolerance.h"

typedef enum type_fourmi {
	T_OUVRIERE, T_GARDE
} TYPE_FOURMI;


typedef enum ModeLS{
	LECTURE, SIMULATION
} MODE_LS;

typedef enum EtatRecord{
	R_STOP, R_START
} ETAT_RECORD;

typedef enum etape_lecture {
    L_NB_FOURMILIERE, L_FOURMILIERE, L_OUVRIERE, L_GARDE, L_NB_NOURRITURE,
    L_NOURRITURE, L_EXIT, L_COMPLETE, L_CONTINUE
} ETAPE_LECTURE;

typedef enum get_info {
    NB_FOURMI, NB_OUVRIERE, NB_GARDE, NB_NOURRITURE, COLOR, NBT_FOURMI,
    NBT_OUVRIERE, NBT_GARDE, NBT_NOURRITURE
} GET_INFO;


#define DELTA_T                 0.25
#define BUG_SPEED               1
#define BUG_LIFE                1000
#define DMAX                    20
#define DMIN					-20
#define RAYON_FOURMI            1
#define RAYON_FOOD              0.25
#define VAL_FOOD                1
#define FOOD_RATE               0.2
#define FEED_RATE               0.002
#define BIRTH_RATE              0.0005
#define MAX_RENDU1              5
#define MAX_FOURMILIERE         10
#define MAX_LINE                120
#define ECHEC           		0
#define MAX_LENGTH      		20
#define FAUX					0
#define VRAI					1
#define GRAPHIC_EMPTY   		0
#define GRAPHIC_FILLED  		1
#define CARRY                   1
#define EMPTY                   0
#define AUTOMATIC       		0
#define COMPLETE                0
#define INCOMPLETE       		1
#define OUV_CARRY               2
#define OUV_EMPTY               1
#define GAR                     0
#define WAIT                    0
#define GO                      1
#define ATTAQUE                 2



#endif
