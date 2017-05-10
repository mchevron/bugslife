/*!
 \file graphic.h
 \brief Module qui gère la lecture des données
 \author Diane Remmy & Max Chevron
 \version 2.0
 \date Avril 2017
 */


void graphic_draw_circle (float xc,
                          float yc,
                          float r,
                          int   filled);


void graphic_set_color3f(float r,
                         float g,
                         float b);

void graphic_find_color (unsigned i);

void graphic_dessine_fourmi_ouvriere(int statut);
