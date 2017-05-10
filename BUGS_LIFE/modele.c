/*!
 \file modele.c
 \brief Module qui gère la lecture des données
 \author Diane Remmy & Max Chevron
 \version 2.0
 \date Avril 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fourmiliere.h"
#include "nourriture.h"
#include "fourmi.h"
#include "constantes.h"
#include "error.h"
#include "modele.h"

#define DEBUT		0
#define COMPLETE	0
#define EXIT		1

int modele_lecture(char mode[], char nom_fichier[]){
    if (strcmp(mode, "Error") == 0){
        if (modele_lecture_fichier(nom_fichier)) return EXIT;
        else {
            error_success();
            return COMPLETE;
        }
    }
    else if ((modele_lecture_fichier(nom_fichier)) || (modele_verification_rendu2()))
        return EXIT;
    return COMPLETE;
}

int modele_lecture_fichier(char nom_fichier[]){
    char tab[MAX_LINE];
    unsigned i=0;     			// indice fourmiliere
    unsigned etape_lecture=0;
    char test;
    FILE *fentree;
    
    fentree = fopen(nom_fichier, "r");
    if (fentree==NULL) {
        error_fichier_inexistant();
        return EXIT_FAILURE;
    }
    
    while((fgets(tab, MAX_LINE, fentree) != NULL)||
          (etape_lecture == L_COMPLETE)||
          (etape_lecture == L_EXIT)) {
        if((tab[DEBUT]=='#')||(tab[DEBUT]=='\n')||(tab[DEBUT]=='\r'))
            continue;
        if (sscanf(tab, " %c", &test), test == '#')
            continue;
        switch(etape_lecture){
            case L_NB_FOURMILIERE:
                etape_lecture = fourmiliere_nb_fourmiliere(tab);
                break;
            case L_FOURMILIERE:
                etape_lecture = fourmiliere_lecture(i, tab);
                if(etape_lecture == L_FOURMILIERE) i=i+1;
                break;
            case L_OUVRIERE:
                etape_lecture = fourmi_ouvriere_lecture(i, tab);
                if(etape_lecture == L_FOURMILIERE) i=i+1;
                break;
            case L_GARDE:
                etape_lecture = fourmi_garde_lecture(i, tab);
                if(etape_lecture == L_FOURMILIERE) i=i+1;
                break;
            case L_NB_NOURRITURE:
                etape_lecture = nourriture_nb_nourriture(tab);
                break;
            case L_NOURRITURE:
                etape_lecture = nourriture_lecture(tab);
                break;
            case L_EXIT:
                return EXIT;
            case L_COMPLETE:
                fclose(fentree);
                return COMPLETE;
        }
    }
    error_fichier_incomplet();
    fclose(fentree);
    return EXIT;
}

int modele_verification_rendu2(void) {
    if (fourmiliere_test_superposition()) return EXIT;
    return COMPLETE;
}

void modele_update(void){
    printf("modele_update\n");
    fourmiliere_update();
}

void modele_cleanup(void){
	fourmiliere_free();
	nourriture_free();
}

void modele_new_food(float pos_x, float pos_y) {
    nourriture_clique(pos_x, pos_y);
}

void modele_dessine_complet(void) {
    fourmiliere_dessine();
    nourriture_dessine();
}

char* modele_get_info_glui(unsigned info, unsigned i) {
    return fourmiliere_get_info_rollout(info, i);
}

void modele_sauvegarde(char *fsortie) {
    FILE *f_sortie = fopen(fsortie, "w");
    fputs("# Fichier sauvegarde Bug's life\n", f_sortie);
    fputs("\n", f_sortie);
    fourmiliere_save(f_sortie);
    nourriture_save(f_sortie);
    fclose(f_sortie);
}
