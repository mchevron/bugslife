/*!
 \file fourmi.h
 \brief Module qui gère les données liées aux fourmis
 \author Diane Remmy & Max Chevron
 \version 1.0
 \date Mars 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "error.h"
#include "fourmi.h"
#include "fourmiliere.h"
#include "modele.h"

#define NB_ELEMENTS_FOURMILIERE 6
#define NB_ELEMENTS_GARDE		3
#define NB_COORDONNEES			2
#define FAUX					0
#define VRAI					1

struct ouvriere
{
   unsigned age;
   double posx;
   double posy;
   double butx;
   double buty;
   unsigned bool_nourriture;
};

struct garde
{
    unsigned age;
    double x;
    double y;
};

struct fourmi
{
    GARDE garde;
    OUVRIERE ouvriere;
};

struct fourmiliere
{
    double x;
    double y;
    int nbO;
    int nbG;
    int nbF;
    int total_food;
    int rayon;
    FOURMI tab_fourmi_ouvriere[MAX_RENDU1];
    FOURMI tab_fourmi_garde[MAX_RENDU1];
};

static FOURMILIERE tab_fourmiliere[MAX_RENDU1];
static unsigned etape_lecture;
static unsigned j = 0;                          //indice fourmi

int fourmi_ouvriere_lecture(unsigned i, char tab[MAX_LINE]) {
    etape_lecture = fourmiliere_ouvriere_lecture_precontrol(i, j, tab);
    if(etape_lecture != L_CONTINUE && etape_lecture != L_OUVRIERE) j = 0;
    if(etape_lecture != L_CONTINUE) return etape_lecture;
    if (sscanf(tab, "%*[ \t]%u %lf %lf %lf %lf %d",
                &tab_fourmiliere[i].tab_fourmi_ouvriere[j].ouvriere.age,
                &tab_fourmiliere[i].tab_fourmi_ouvriere[j].ouvriere.posx,
                &tab_fourmiliere[i].tab_fourmi_ouvriere[j].ouvriere.posy,
                &tab_fourmiliere[i].tab_fourmi_ouvriere[j].ouvriere.butx,
                &tab_fourmiliere[i].tab_fourmi_ouvriere[j].ouvriere.buty,
                &tab_fourmiliere[i].tab_fourmi_ouvriere[j].ouvriere.bool_nourriture)
                != NB_ELEMENTS_FOURMILIERE) {
        error_lecture_elements_fourmiliere(i, ERR_OUVRIERE, ERR_PAS_ASSEZ);
        return L_EXIT;
    }
    if(fourmi_test_age(i, j,
        tab_fourmiliere[i].tab_fourmi_ouvriere[j].ouvriere.age)) return L_EXIT;
    j = j+1;
    return L_OUVRIERE;
}

int fourmi_garde_lecture(unsigned i, char tab[MAX_LINE]) {
	unsigned k = 0;
    etape_lecture = fourmiliere_garde_lecture_precontrol(i, j, tab);
    if(etape_lecture != L_CONTINUE && etape_lecture != L_GARDE) j=0;
    if(etape_lecture != L_CONTINUE) return etape_lecture;
    char *deb = tab, *fin = NULL;
    while(sscanf(deb, "%*[ \t]%u %lf %lf",
           &tab_fourmiliere[i].tab_fourmi_garde[j].garde.age,
           &tab_fourmiliere[i].tab_fourmi_garde[j].garde.x,
           &tab_fourmiliere[i].tab_fourmi_garde[j].garde.y) == NB_ELEMENTS_GARDE) {
        for (k = 0; k < NB_ELEMENTS_GARDE; k++) {
			strtod(deb,&fin);
			deb = fin;
		}
        if((fourmi_test_age(i, j, tab_fourmiliere[i].tab_fourmi_garde[j].garde.age))
			|| (fourmiliere_test_pos_garde(i, j, 
									tab_fourmiliere[i].tab_fourmi_garde[j].garde.x,
									tab_fourmiliere[i].tab_fourmi_garde[j].garde.y)))
			return L_EXIT;
        if (utilitaire_test_pos_domaine(ERR_FOURMILIERE, i,
								tab_fourmiliere[i].tab_fourmi_ouvriere[j].garde.x,
								tab_fourmiliere[i].tab_fourmi_ouvriere[j].garde.y)) 
			return L_EXIT;
        j=j+1;
    }
    return L_GARDE;                 
}

int fourmi_test_age(unsigned num_fourmiliere, unsigned num_fourmi,
                    unsigned age) {
    if (age >= BUG_LIFE) {
		error_age_fourmi(num_fourmiliere, num_fourmi, age);
        return VRAI;
	}
	return FAUX; 
}

