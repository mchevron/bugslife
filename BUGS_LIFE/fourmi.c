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
#include "nourriture.h"
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
    double posx;
    double posy;
    double butx;
    double buty;
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
static FOURMI ** p_fourmi_ouvriere = NULL;
static FOURMI ** p_fourmi_garde = NULL;

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
    /*if(fourmiliere_test_nbO(i, j)==COMPLETE) {
        j=0;
        return L_GARDE;
     }
     */
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
        guard->garde.posx = x;
        guard->garde.posy = y;
        guard->garde.id = j;
        for (k = 0; k < NB_ELEMENTS_GARDE; k++) {
            strtod(deb,&fin);
            deb = fin;
        }
        if((fourmi_test_age(i, j, guard->garde.age))
         || (fourmiliere_test_pos_garde_lecture(i, j, guard->garde.posx, guard->garde.posy)))
            return L_EXIT;
        if (fourmi_test_pos_domaine(ERR_GARDE, i, guard->garde.posx,
                                        guard->garde.posy))
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

int fourmi_test_superposition_oo( FOURMI **p_frm_frml1, FOURMI ** p_frm_frml2,
                                 unsigned i, unsigned j , MODE_LS mode){
    FOURMI * frm1 = * p_frm_frml1;
    FOURMI * frm2 = NULL;
    while (frm1 != NULL){
        frm2 = * p_frm_frml2;
        while (frm2 != NULL){
            double distance = utilitaire_calcul_distance(frm1->ouvriere.posx,
                                                         frm2->ouvriere.posx,
                                                         frm1->ouvriere.posy,
                                                         frm2->ouvriere.posy);
            if (mode == LECTURE){
				if (distance - (RAYON_FOURMI + RAYON_FOURMI) <= 0){
					error_superposition_fourmi(ERR_OUVRIERE, i, frm1->ouvriere.id,
											   ERR_OUVRIERE, j, frm2->ouvriere.id);
					return  VRAI;
				}
			}
			else {
				if (distance - (RAYON_FOURMI + RAYON_FOURMI) <= EPSIL_ZERO){
					if (frm1->ouvriere.bool_nourriture)
						nourriture_ajouter_fixe(frm1->ouvriere.posx, 
												frm1->ouvriere.posy);
	                fourmi_retirer(p_frm_frml1, frm1);
					if (frm2->ouvriere.bool_nourriture)
						nourriture_ajouter_fixe(frm2->ouvriere.posx, 
												frm2->ouvriere.posy);
	                fourmi_retirer(p_frm_frml2, frm2);
					return  VRAI;
				}
			}
            frm2 = frm2->next;
        }
        frm1 = frm1->next;
    }
    return FAUX;
}

int fourmi_test_superposition_go( FOURMI ** p_frm_frml1, FOURMI ** p_frm_frml2,
                                 unsigned i, unsigned j , MODE_LS mode){
    FOURMI * frm1 = * p_frm_frml1;
    FOURMI * frm2 = NULL;
    while (frm1 != NULL){
		frm2 = * p_frm_frml2;
        while (frm2 != NULL){
            double distance = utilitaire_calcul_distance(frm1->garde.posx,
                                                         frm2->ouvriere.posx,
                                                         frm1->garde.posy,
                                                         frm2->ouvriere.posy);
            if (mode == LECTURE){                                             
	            if (distance - (RAYON_FOURMI + RAYON_FOURMI) <= 0){
	                error_superposition_fourmi(ERR_GARDE, i, frm1->garde.id,
	                                           ERR_OUVRIERE, j, frm2->ouvriere.id);
	                return  VRAI;
	            }
			}
			else {
				if (distance - (RAYON_FOURMI + RAYON_FOURMI) <= EPSIL_ZERO){
	                fourmi_retirer(p_frm_frml1, frm1);
					if (frm2->ouvriere.bool_nourriture) 
						nourriture_ajouter_fixe(frm2->ouvriere.posx, 
												frm2->ouvriere.posy);
	                fourmi_retirer(p_frm_frml2, frm2);
	                return  VRAI;
	            }
			}
            frm2 = frm2->next;
        }
        frm1 = frm1->next;
    }
    return FAUX;
}
int fourmi_test_superposition_og( FOURMI ** p_frm_frml1, FOURMI ** p_frm_frml2,
                                 unsigned i, unsigned j , MODE_LS mode){
    FOURMI * frm1 = *p_frm_frml1;
    FOURMI * frm2 = NULL;
    while (frm1 != NULL){
		frm2 = * p_frm_frml2;
        while (frm2 != NULL){
            double distance = utilitaire_calcul_distance(frm1->ouvriere.posx,
                                                         frm2->garde.posx,
                                                         frm1->ouvriere.posy,
                                                         frm2->garde.posy);
            if (mode == LECTURE){                                            
	            if (distance - (RAYON_FOURMI + RAYON_FOURMI) <= 0){
	                error_superposition_fourmi(ERR_OUVRIERE, i, frm1->ouvriere.id,
	                                           ERR_GARDE, j, frm2->garde.id);
	                return  VRAI;				
	            }
			}
			else {
	            if (distance - (RAYON_FOURMI + RAYON_FOURMI) <= EPSIL_ZERO){
					if (frm1->ouvriere.bool_nourriture) 
						nourriture_ajouter_fixe(frm1->ouvriere.posx, 
												frm1->ouvriere.posy);
	                fourmi_retirer(p_frm_frml1, frm1);
	                fourmi_retirer(p_frm_frml2, frm2);
	                return  VRAI;				
	            }
			}			
            frm2 = frm2->next;
        }
        frm1 = frm1->next;
    }
    return FAUX;
}

int fourmi_test_superposition_gg( FOURMI **p_frm_frml1, FOURMI ** p_frm_frml2,
                                 unsigned i, unsigned j , MODE_LS mode ){
    FOURMI * frm1 = * p_frm_frml1;
    FOURMI * frm2 = NULL;
    while (frm1 != NULL){
		frm2 = * p_frm_frml2;
        while (frm2 != NULL){
            double distance = utilitaire_calcul_distance(frm1->garde.posx, 
                                                         frm2->garde.posx,
                                                         frm1->garde.posy,
                                                         frm2->garde.posy);
            if (mode == LECTURE) {                                            
	            if (distance - (RAYON_FOURMI + RAYON_FOURMI) <= 0){
	                error_superposition_fourmi(ERR_OUVRIERE, i, frm1->garde.id,
	                                           ERR_GARDE, j, frm2->garde.id);
	                return  VRAI;				
	            }
			}
			else {
				if (distance - (RAYON_FOURMI + RAYON_FOURMI) <= EPSIL_ZERO){
					fourmi_retirer(p_frm_frml1, frm1);
					fourmi_retirer(p_frm_frml2, frm2);
					return  VRAI;				
				}
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
                graphic_find_color (i);
                graphic_draw_circle (courant_o->ouvriere.posx,
                                     courant_o->ouvriere.posy,
                                     RAYON_FOURMI, GRAPHIC_EMPTY);
                if(courant_o->ouvriere.bool_nourriture==CARRY) {
                    graphic_set_color3f (0., 0., 0.);
                    graphic_draw_circle (courant_o->ouvriere.posx,
                                         courant_o->ouvriere.posy,
                                         RAYON_FOOD, GRAPHIC_EMPTY);
                }
                courant_o = courant_o->next;
            }
        }
        if((p_fourmiliere+i)->nbG != 0) {
            FOURMI * courant_g = (p_fourmiliere+i)->p_fourmi_garde;
            for(j=0; j<(p_fourmiliere+i)->nbG; j=j+1) {
                graphic_find_color (i);
                graphic_draw_circle (courant_g->garde.posx,
                                     courant_g->garde.posy,
                                     RAYON_FOURMI, GRAPHIC_FILLED);
                graphic_set_color3f (0., 0., 0.);
                graphic_draw_circle (courant_g->garde.posx,
                                     courant_g->garde.posy,
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
		fprintf(f_sortie, "%lf ", guard->garde.posx);	
		fprintf(f_sortie, "%lf ", guard->garde.posy);	
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

void fourmi_ouvriere_update(FOURMI **p_ouvriere, int i) {
    FOURMI *p_ouvri = *p_ouvriere;
    while (p_ouvri){
        fourmi_ouvriere_deplacement(p_ouvri, i);
        p_ouvri->ouvriere.age+=1;
        printf("%d\n", p_ouvri->ouvriere.age);
        if(p_ouvri->ouvriere.age==50) {
            fourmi_retirer(p_ouvriere, p_ouvri);
            //fourmiliere_diminuer_NBO
        }
        p_ouvri = p_ouvri->next;
    }
}

void fourmi_garde_update(FOURMI *p_garde, int i, int nb_fourmiliere){
    int j = 0;
    while (p_garde){
        //fourmi_age(p_garde);
        fourmi_garde_deplacement(p_garde, i, nb_fourmiliere);
        p_garde = p_garde->next;
        j+=1;
    }
}

void fourmi_naissance(FOURMI ** p_fourmi, TYPE_FOURMI type, double posx, double posy){
	FOURMI * four = NULL;
	printf("fourmi\n");
	if (type == T_OUVRIERE){
		four = fourmi_ajouter(p_fourmi);
		if (four != NULL){
			four->ouvriere.age = 0;
			four->ouvriere.posx = posx;
			four->ouvriere.posy = posy;
			four->ouvriere.butx = posx;
			four->ouvriere.buty = posy;
			four->ouvriere.bool_nourriture = 0;
			four->ouvriere.id = 0;
		}
	}
	else {
		four = fourmi_ajouter(p_fourmi);
		if (four != NULL){
			four->garde.age = 0;
			four->garde.posx = posx;
			four->garde.posy = posy;
			four->garde.butx = posx;
			four->garde.buty = posy;
			four->ouvriere.id = 0;
		}
	}
}


void fourmi_age(FOURMI *p_fourmi) {
    p_fourmi->ouvriere.age+=1;
    if(p_fourmi->ouvriere.age==BUG_LIFE);
}

int fourmi_nourriture_test_superposition_o(FOURMI *p_four, double x, double y){
	FOURMI * four = p_four;
    while (four != NULL){
		double distance = utilitaire_calcul_distance(four->ouvriere.posx, x,
													 four->ouvriere.posy, y);
		if (distance - (RAYON_FOURMI + RAYON_FOOD) <= EPSIL_ZERO)
			return  VRAI;
		four = four->next;
	}
    return FAUX;
}

int fourmi_nourriture_test_superposition_g(FOURMI *p_four, double x, double y){
	FOURMI * four = p_four;
    while (four != NULL){
		double distance = utilitaire_calcul_distance(four->garde.posx, x,
													 four->garde.posy, y);
		if (distance - (RAYON_FOURMI + RAYON_FOOD) <= EPSIL_ZERO)
			return  VRAI;
		four = four->next;
	}
    return FAUX;
}

void fourmi_ouvriere_deplacement(FOURMI *p_ouvriere, int i) {
    if(p_ouvriere->ouvriere.bool_nourriture==EMPTY) {
        nourriture_choix(&p_ouvriere->ouvriere.posx, &p_ouvriere->ouvriere.posy,
                         &p_ouvriere->ouvriere.butx, &p_ouvriere->ouvriere.buty);
        //cette fonction met à jour le but de la fourmi vide
    }
    else fourmiliere_retour(&p_ouvriere->ouvriere.butx, &p_ouvriere->ouvriere.buty, i);
        //cette fonction adapte le but en ligne droite en fonction des obstacles
    
    double distance = utilitaire_calcul_distance(p_ouvriere->ouvriere.posx,
                                                 p_ouvriere->ouvriere.butx,
                                                 p_ouvriere->ouvriere.posy,
                                                 p_ouvriere->ouvriere.buty);
    if(distance > RAYON_FOURMI) {
        double Vn_x = (p_ouvriere->ouvriere.butx - p_ouvriere->ouvriere.posx) / distance;
        double Vn_y = (p_ouvriere->ouvriere.buty - p_ouvriere->ouvriere.posy) / distance;
        p_ouvriere->ouvriere.posx += BUG_SPEED*DELTA_T*Vn_x;
        p_ouvriere->ouvriere.posy += BUG_SPEED*DELTA_T*Vn_y;
    }
    else {
        if(p_ouvriere->ouvriere.bool_nourriture==1) {
            fourmiliere_new_food(i);
            p_ouvriere->ouvriere.bool_nourriture = 0;;
        }
        else {
            p_ouvriere->ouvriere.bool_nourriture = 1;
            nourriture_cherche_retire(p_ouvriere->ouvriere.butx,
                                      p_ouvriere->ouvriere.buty);
        }
    }
}

void fourmi_garde_deplacement(FOURMI *p_garde, int i, int nb_fourmiliere) {
    fourmiliere_retour(&p_garde->garde.butx, &p_garde->garde.buty, i);
    fourmiliere_test_ouvri_intrustion(p_garde, i);
    double distance = utilitaire_calcul_distance(p_garde->garde.posx,
                                                 p_garde->garde.butx,
                                                 p_garde->garde.posy,
                                                 p_garde->garde.buty);
    if(distance > RAYON_FOURMI) {
        double Vn_x = (p_garde->garde.butx - p_garde->garde.posx) / distance;
        double Vn_y = (p_garde->garde.buty - p_garde->garde.posy) / distance;
        p_garde->garde.posx += BUG_SPEED*DELTA_T*Vn_x;
        p_garde->garde.posy += BUG_SPEED*DELTA_T*Vn_y;
    }
}
    
void fourmi_ouvriere_intrusion(FOURMI *p_garde, FOURMI *p_ouvriere,
                              unsigned i, unsigned c_x, unsigned c_y, unsigned rayon){
    while (p_ouvriere){
        double distance = utilitaire_calcul_distance(c_x,
                                                     p_ouvriere->ouvriere.posx,
                                                     c_y,
                                                     p_ouvriere->ouvriere.posy);
        if (distance <= rayon){
            p_garde->garde.posx = p_ouvriere->ouvriere.posx;
            p_garde->garde.posy = p_ouvriere->ouvriere.posy;
            }
        p_ouvriere = p_ouvriere->next;
    }
}

