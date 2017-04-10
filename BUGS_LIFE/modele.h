/*!
 \file modele.h
 \brief Module qui gère la lecture des données
 \author Diane Remmy & Max Chevron
 \version 1.0
 \date Mars 2017
 */

typedef enum etape_lecture {
    L_NB_FOURMILIERE, L_FOURMILIERE, L_OUVRIERE, L_GARDE, L_NB_NOURRITURE,
    L_NOURRITURE, L_EXIT, L_COMPLETE, L_CONTINUE,
} ETAPE_LECTURE;

typedef enum get_info {
    COLOR, NB_FOURMI, NB_OUVRIERE, NB_GARDE, NB_NOURRITURE, NBT_FOURMI,
    NBT_OUVRIERE, NBT_GARDE, NBT_NOURRITURE
} GET_INFO;

int modele_lecture(char mode[], char nom_fichier[]);
int modele_lecture_fichier(char nom_fichier[]);
int modele_verification_rendu2(void);
int modele_update(char *fentree);
void cleanup();
void modele_new_food(float pos_x, float pos_y);
void modele_dessine_complet();
char* modele_get_info_glui(unsigned info, unsigned i);
void modele_sauvegarde(char *fsortie);
