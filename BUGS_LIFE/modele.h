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
    COLOR, NB_FOURMI
} GET_INFO;

int modele_lecture(char mode[], char nom_fichier[]);
int modele_lecture_fichier(char nom_fichier[]);
int modele_update(char *fentree);
int modele_verification_rendu2(void);

void new_food(float pos_x, float pos_y);

void modele_dessine_complet();

char* get_info_rollout(unsigned info, unsigned i, char* data_glui);
void sauvegarde(char *fsortie);
