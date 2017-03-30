/*!
 \file modele.c
 \brief Module qui gère la lecture des données
 \author Diane Remmy & Max Chevron
 \version 1.0
 \date Mars 2017
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

#define DEBUT	0
#define COMPLETE	0
#define EXIT		1

int modele_lecture(int argc, char *argv[])
{
    char tab[MAX_LINE];
    unsigned i=0;     			// indice fourmiliere
    unsigned etape_lecture=0;
    FILE *fentree;
    
    if (argc != 2) {
        error_fichier_inexistant();
        return EXIT_FAILURE;
    }
                                         
    fentree = fopen(argv[1], "r");
    if (fentree==NULL) {
        error_fichier_inexistant();
        return EXIT_FAILURE;
    }
    
    while(fgets(tab, MAX_LINE, fentree) != NULL|
          etape_lecture == L_COMPLETE|
          etape_lecture == L_EXIT)
    {
        if((tab[DEBUT]=='#')||(tab[DEBUT]=='\n')||(tab[DEBUT]=='\r'))
            continue;
        switch(etape_lecture)
        {
            case L_NB_FOURMILIERE:
                etape_lecture = fourmiliere_nb_fourmiliere(tab);
                break;
            case L_FOURMILIERE:
                etape_lecture = fourmiliere_lecture(i, tab);
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
                error_success();
                fclose(fentree);
                return COMPLETE;
        }
    }
    error_fichier_incomplet();
    fclose(fentree);
    return EXIT;
}
