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
#include "graphic.h"


#define NB_ELEMENTS_FOURMILIERE 6
#define NB_ELEMENTS_GARDE		3
#define NB_COORDONNEES			2
#define FAUX					0
#define VRAI					1

struct ouvriere
{
    unsigned id;
    unsigned age;
    double posx;
    double posy;
    double butx;
    double buty;
    unsigned bool_nourriture;
};

struct garde
{
    unsigned id;
    unsigned age;
    double x;
    double y;
};

struct fourmi
{
    OUVRIERE ouvriere;
    GARDE garde;
    FOURMI *next;
};

struct fourmiliere
{
    unsigned id;
    double x;
    double y;
    int nbO;
    int nbG;
    int nbF;
    int total_food;
    int rayon;
    FOURMI * p_fourmi_ouvriere;
    FOURMI * p_fourmi_garde;
};

static unsigned etape_lecture;
static unsigned j = 0;                          //indice fourmi
static FOURMI ** p_fourmi_ouvriere;
static FOURMI ** p_fourmi_garde;

int fourmi_ouvriere_lecture(unsigned i, char tab[MAX_LINE]) {
    FOURMI * ouvri;
    unsigned age = 0;
    double posx = 0.;
    double posy = 0.;
    double butx = 0.;
    double buty = 0.;
    unsigned bool_nourriture = 0;
    etape_lecture = fourmiliere_ouvriere_lecture_precontrol(i, j, tab);
    if(etape_lecture != L_CONTINUE && etape_lecture != L_OUVRIERE) j = 0;
    if(etape_lecture != L_CONTINUE) return etape_lecture;
    //printf("%p", (void*)&(p_fourmiliere+i)->p_fourmi_ouvriere);
    if (sscanf(tab, "%*[ \t]%u %lf %lf %lf %lf %d", &age, &posx, &posy, &butx, &buty,
               &bool_nourriture) != NB_ELEMENTS_FOURMILIERE) {
        error_lecture_elements_fourmiliere(i, ERR_OUVRIERE, ERR_PAS_ASSEZ);
        return L_EXIT;
    }
    ouvri = ajouter_fourmi(p_fourmi_ouvriere);
    if (ouvri == NULL) return L_EXIT;
    ouvri->ouvriere.age = age;
    ouvri->ouvriere.posx = posx;
    ouvri->ouvriere.posy = posy;
    ouvri->ouvriere.butx = butx;
    ouvri->ouvriere.buty = buty;
    ouvri->ouvriere.bool_nourriture = bool_nourriture;
    ouvri->ouvriere.id = j;
    if(fourmi_test_age(i, j,
                       ouvri->ouvriere.age)) return L_EXIT;
    j = j+1;
    return L_OUVRIERE;
}

int fourmi_garde_lecture(unsigned i, char tab[MAX_LINE]) {
    unsigned 	k = 0, age = 0;
    double 		x = 0., y = 0.;
    FOURMI * guard;
    etape_lecture = fourmiliere_garde_lecture_precontrol(i, j, tab);
    if(etape_lecture != L_CONTINUE && etape_lecture != L_GARDE) j=0;
    if(etape_lecture != L_CONTINUE) return etape_lecture;
    char *deb = tab, *fin = NULL;
    while(sscanf(deb, "%*[ \t]%u %lf %lf", &age, &x, &y) == NB_ELEMENTS_GARDE) {
        guard = ajouter_fourmi(p_fourmi_garde);
        if (guard == NULL) return L_EXIT;
        guard->garde.age = age;
        guard->garde.x = x;
        guard->garde.y = y;
        guard->garde.id = j;
        for (k = 0; k < NB_ELEMENTS_GARDE; k++) {
            strtod(deb,&fin);
            deb = fin;
        }
        if((fourmi_test_age(i, j, guard->garde.age))
           || (fourmiliere_test_pos_garde(i, j, guard->garde.x, guard->garde.y)))
            return L_EXIT;
        if (utilitaire_test_pos_domaine(ERR_FOURMILIERE, i, guard->garde.x,
                                        guard->garde.y))
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

void fourmi_recoit( FOURMI **p_ouvriere, FOURMI ** p_garde){
    p_fourmi_ouvriere = p_ouvriere;
    p_fourmi_garde = p_garde;
}

FOURMI * ajouter_fourmi ( FOURMI ** p_tete )
{
    FOURMI * four = NULL;
    
    if (!(four = (FOURMI *) malloc (sizeof(FOURMI))))
    {
        printf ("Pb d'allocation dans %s\n", __func__);
        return four;								
    }
    
    four->next = *p_tete;
    *p_tete		= four;
    
    return four;
}

void test_superposition_fourmi() {
    
}

void fourmi_dessine(unsigned nb_fourmiliere, FOURMILIERE * p_fourmiliere) {
    int i = 0, j = 0, k = 0;
    for(i=0; i<nb_fourmiliere; i=i+1) {
        graphic_find_color ((p_fourmiliere+i)->id);
        for(j=0; j<p_fourmiliere->nbO; j=j+1) {
            graphic_draw_circle ((p_fourmiliere+i)->p_fourmi_ouvriere->ouvriere.posx,
                                 (p_fourmiliere+i)->p_fourmi_ouvriere->ouvriere.posy,
                                 RAYON_FOURMI, GRAPHIC_EMPTY);
        }
        k = p_fourmiliere->nbG;
        for(j=0; j<p_fourmiliere->nbG; j=j+1) {
            k = ((p_fourmiliere+i)->p_fourmi_garde+j)->garde.x;
            graphic_draw_circle (((p_fourmiliere+i)->p_fourmi_garde+j)->garde.x,
                                 ((p_fourmiliere+i)->p_fourmi_garde+j)->garde.y,
                                 RAYON_FOURMI, GRAPHIC_FILLED);
            graphic_set_color3f (0., 0., 0.);
            graphic_draw_circle (((p_fourmiliere+i)->p_fourmi_garde+j)->garde.x,
                                 ((p_fourmiliere+i)->p_fourmi_garde+j)->garde.y,
                                 RAYON_FOURMI, GRAPHIC_EMPTY); /* Cette methode ne 
                                fonctionne pas parce que l'adresse de la prochaine garde 
                                n'est pas à garde+j ... une idée? 
                                Il faudra faire la même chose pour nourriture et 
                                ouvrières */
            graphic_find_color ((p_fourmiliere+i)->id);
        }
    }
}
