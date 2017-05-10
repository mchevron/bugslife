/*!
 \file fourmi.h
 \brief Module qui gère les données liées aux fourmis
 \author Diane Remmy & Max Chevron
 \version 2.0
 \date Avril 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "error.h"
#include "fourmiliere.h"
#include "graphic.h"
#include "constantes.h"
#include "utilitaire.h"
#include "fourmi.h"


#define NB_ELEMENTS_FOURMILIERE 6
#define NB_ELEMENTS_GARDE		3
#define NB_COORDONNEES			2
#define NB_GARDE_PAR_LIGNE 		2


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
    if (sscanf(tab, "%*[ \t]%u %lf %lf %lf %lf %d", &age, &posx, &posy, &butx, &buty,
               &bool_nourriture) != NB_ELEMENTS_FOURMILIERE) {
        error_lecture_elements_fourmiliere(i, ERR_OUVRIERE, ERR_PAS_ASSEZ);
        return L_EXIT;
    }
    ouvri = fourmi_ajouter(p_fourmi_ouvriere);
    if (ouvri == NULL) return L_EXIT;
    ouvri->ouvriere.age = age;
    ouvri->ouvriere.posx = posx;
    ouvri->ouvriere.posy = posy;
    ouvri->ouvriere.butx = butx;
    ouvri->ouvriere.buty = buty;
    ouvri->ouvriere.bool_nourriture = bool_nourriture;
    ouvri->ouvriere.id = j;
    if(fourmi_test_age(i, j, ouvri->ouvriere.age)) return L_EXIT;
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
        guard = fourmi_ajouter(p_fourmi_garde);
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
        if (fourmi_test_pos_domaine(ERR_GARDE, i, guard->garde.x,
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

int fourmi_test_pos_domaine(ERREUR_ORIG origine, unsigned num_fourmiliere, 
								double x, double y){
	if ((x < -DMAX) || (x > DMAX) || (y < -DMAX) || (y > DMAX)) {
		error_pos_domaine(origine, num_fourmiliere, x, y);
		return VRAI;	
	}
	return FAUX;
}

void fourmi_recoit( FOURMI **p_ouvriere, FOURMI ** p_garde){
    p_fourmi_ouvriere = p_ouvriere;
    p_fourmi_garde = p_garde;
}

FOURMI * fourmi_ajouter ( FOURMI ** p_tete ){
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

int fourmi_test_superposition_oo( FOURMI *p_frm_frml1, FOURMI * p_frm_frml2,
                                 unsigned i, unsigned j ){
    FOURMI * frm1 = p_frm_frml1;
    FOURMI * frm2 = NULL;
    while (frm1 != NULL){
        frm2 = p_frm_frml2;
        while (frm2 != NULL){
            double distance = utilitaire_calcul_distance(frm1->ouvriere.posx,
                                                         frm2->ouvriere.posx,
                                                         frm1->ouvriere.posy,
                                                         frm2->ouvriere.posy);
            if (distance - (RAYON_FOURMI + RAYON_FOURMI) <= 0){
                error_superposition_fourmi(ERR_OUVRIERE, i, frm1->ouvriere.id,
                                           ERR_OUVRIERE, j, frm2->ouvriere.id);
                return  VRAI;
            }
            frm2 = frm2->next;
        }
        frm1 = frm1->next;
    }
    return FAUX;
}

int fourmi_test_superposition_go( FOURMI *p_frm_frml1, FOURMI * p_frm_frml2,
                                 unsigned i, unsigned j ){
    FOURMI * frm1 = p_frm_frml1;
    FOURMI * frm2 = NULL;
    while (frm1 != NULL){
        while (frm2 != NULL){
            double distance = utilitaire_calcul_distance(frm1->garde.x,
                                                         frm2->ouvriere.posx,
                                                         frm1->garde.y,
                                                         frm2->ouvriere.posy);
            if (distance - (RAYON_FOURMI + RAYON_FOURMI) <= 0){
                error_superposition_fourmi(ERR_GARDE, i, frm1->garde.id,
                                           ERR_OUVRIERE, j, frm2->ouvriere.id);
                return  VRAI;
            }
            frm2 = frm2->next;
        }
        frm1 = frm1->next;
    }
    return FAUX;
}
int fourmi_test_superposition_og( FOURMI *p_frm_frml1, FOURMI * p_frm_frml2,
                                 unsigned i, unsigned j ){
    FOURMI * frm1 = p_frm_frml1;
    FOURMI * frm2 = NULL;
    while (frm1 != NULL){
        while (frm2 != NULL){
            double distance = utilitaire_calcul_distance(frm1->ouvriere.posx,
                                                         frm2->garde.x,
                                                         frm1->ouvriere.posy,
                                                         frm2->garde.y);
            if (distance - (RAYON_FOURMI + RAYON_FOURMI) <= 0){
                error_superposition_fourmi(ERR_OUVRIERE, i, frm1->ouvriere.id,
                                           ERR_GARDE, j, frm2->garde.id);
                return  VRAI;				
            }
            frm2 = frm2->next;
        }
        frm1 = frm1->next;
    }
    return FAUX;
}

int fourmi_test_superposition_gg( FOURMI *p_frm_frml1, FOURMI * p_frm_frml2,
                                 unsigned i, unsigned j ){
    FOURMI * frm1 = p_frm_frml1;
    FOURMI * frm2 = NULL;
    while (frm1 != NULL){
        while (frm2 != NULL){
            double distance = utilitaire_calcul_distance(frm1->garde.x, 
                                                         frm2->garde.x,
                                                         frm1->garde.y,
                                                         frm2->garde.y);
            if (distance - (RAYON_FOURMI + RAYON_FOURMI) <= 0){
                error_superposition_fourmi(ERR_GARDE, i, frm1->garde.id,
                                           ERR_GARDE, j, frm2->garde.id);
                return  VRAI;				
            }
            frm2 = frm2->next;
        }
        frm1 = frm1->next;
    }
    return FAUX;
}

void fourmi_dessine(unsigned nb_fourmiliere, FOURMILIERE * p_fourmiliere) {
    int i = 0, j = 0;
    for(i=0; i<nb_fourmiliere; i=i+1) {
        graphic_find_color (i);
        if((p_fourmiliere+i)->nbO != 0) {
            FOURMI * courant_o = (p_fourmiliere+i)->p_fourmi_ouvriere;
            for(j=0; j<(p_fourmiliere+i)->nbO; j=j+1) {
                graphic_draw_circle (courant_o->ouvriere.posx,
                                     courant_o->ouvriere.posy,
                                     RAYON_FOURMI, GRAPHIC_EMPTY);
                courant_o = courant_o->next;
            }
        }
        if((p_fourmiliere+i)->nbG != 0) {
            FOURMI * courant_g = (p_fourmiliere+i)->p_fourmi_garde;
            for(j=0; j<(p_fourmiliere+i)->nbG; j=j+1) {
                graphic_draw_circle (courant_g->garde.x,
                                     courant_g->garde.y,
                                     RAYON_FOURMI, GRAPHIC_FILLED);
                graphic_set_color3f (0., 0., 0.);
                graphic_draw_circle (courant_g->garde.x,
                                     courant_g->garde.y,
                                     RAYON_FOURMI, GRAPHIC_EMPTY);
                courant_g = courant_g->next;
                graphic_find_color (i);
            }
        }
    }
}

void fourmi_save_ouvriere(FILE *f_sortie, FOURMI *p_ouvriere) {
    FOURMI * ouvr = p_ouvriere;
    fputs("\n", f_sortie);
    fputs("    # Ouvrieres\n", f_sortie);
	while (ouvr != NULL){
		fputs("\t", f_sortie);
		fprintf(f_sortie, "%u ", ouvr->ouvriere.age);	
		fprintf(f_sortie, "%lf ", ouvr->ouvriere.posx);		
		fprintf(f_sortie, "%lf", ouvr->ouvriere.posy);
		fprintf(f_sortie, "%lf ", ouvr->ouvriere.butx);	
		fprintf(f_sortie, "%lf ", ouvr->ouvriere.buty);	
		fprintf(f_sortie, "%u", ouvr->ouvriere.bool_nourriture);	
		fputs("\n", f_sortie);		
		ouvr = ouvr->next;
	}
}

void fourmi_save_garde(FILE *f_sortie, FOURMI *p_garde){
    FOURMI * guard = p_garde;
    fputs("\n", f_sortie);
    fputs("    # Gardes\n", f_sortie);
    unsigned i = 0;
	while (guard != NULL){
		fputs("\t", f_sortie);
		fprintf(f_sortie,"%u ", guard->garde.age);	
		fprintf(f_sortie, "%lf ", guard->garde.x);	
		fprintf(f_sortie, "%lf ", guard->garde.y);	
		guard = guard->next;
		i = i + 1;
		if (i == NB_GARDE_PAR_LIGNE){
			fputs("\n", f_sortie);
			i = 0;
		}
	}
}

void fourmi_free ( FOURMI ** p_liste ){
    /* Retire un à un les elements en tete de la liste */
    FOURMI *four;
    while (* p_liste){
        four = * p_liste;
        fourmi_retirer ( p_liste, four );
    }
    etape_lecture = 0;
    j = 0; 
}

void fourmi_retirer ( FOURMI ** p_tete, FOURMI *four ){
    FOURMI* courant= *p_tete;
    FOURMI* precedent= *p_tete;
    while ( courant != four && courant != NULL ){
        precedent=courant;
        courant=courant->next;
    }
    if ( courant != NULL ){
        if ( courant != * p_tete ){
            precedent->next=courant->next;
            free ( courant );
        }
        else {
            * p_tete = courant->next;
            free ( courant );
        }
    }
}

void fourmi_ouvriere_update(void) {
    fourmi_age();
    fourmi_ouvriere_deplacement();
}

void fourmi_age(void) {
    
}

void fourmi_ouvriere_statut(int statut) {
    if(statut==CARRY) {
        graphic_dessine_fourmi_ouvriere(CARRY);
        //ouvri->ouvriere.bool_nourriture = 1
    }
    else {
        graphic_dessine_fourmi_ouvriere(EMPTY);
        //ouvri->ouvriere.bool_nourriture = 0
    }
}

void fourmi_ouvriere_deplacement() {
    //fourmi_algo_bon_choix();
    //fourmi_ouvrière_but(utiliser les positions en pointeurs);
    //Construire vecteur V (pos_x, pos_y, but_x, but_y)
    //Si la norme > rayon_fourmi
        //Normaliser le vecteur (Vn = V / norme)
        //Deplacement : Vd = BUG_SPEED*DELTA_T*Vn
    //Sinon
        //Si ouvrière porteuse
            //Fourmiliere-totalfood += 1
            //Fourmi_ouvriere_statut(vide)
        //Sinon prendre nourriture
            //Fourmi_ouvriere_statut(pleine)
            //Nourriture_retirer(nourriture_recherche(pos_x, pos_y))
}

