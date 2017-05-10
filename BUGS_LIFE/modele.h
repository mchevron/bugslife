/*!
 \file modele.h
 \brief Module qui gère la lecture des données
 \author Diane Remmy & Max Chevron
 \version 2.0
 \date Avril 2017
 */

int modele_lecture(char mode[], char nom_fichier[]);
int modele_lecture_fichier(char nom_fichier[]);
int modele_verification_rendu2(void);
void modele_update(int radiobutton_nourriture);
void modele_cleanup(void);
void modele_new_food(float pos_x, float pos_y);
void modele_dessine_complet(void);
char* modele_get_info_glui(unsigned info, unsigned i);
void modele_sauvegarde(char *fsortie);
