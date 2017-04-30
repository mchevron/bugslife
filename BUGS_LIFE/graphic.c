/*!
 \file graphic.c
 \brief Programme Bug's life
 \author Diane Remmy & Max Chevron
 \version 2.0
 \date Avril 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include <GL/glu.h>
//#include <GL/glut.h>
#include <GLUT/GLUT.h>
#include "constantes.h"
#include "graphic.h"

#define SIDES_COUNT				50
#define RGB             		3

void graphic_draw_circle (float xc,
                          float yc,
                          float r,
                          int   filled)

{ int i;
    const int SIDES = SIDES_COUNT;
    
    if (filled == GRAPHIC_FILLED)
        glBegin (GL_POLYGON);
    else
        glBegin (GL_LINE_LOOP);
    
    for (i=0; i < SIDES; i++)
    {
        float alpha = i * 2. * M_PI / SIDES;
        
        glVertex2f (xc + r*cos(alpha), yc + r*sin(alpha));
        
    }
    
    glEnd ();
}

void graphic_set_color3f(float r,
                         float g,
                         float b)
{
    glColor3f(r, g, b);
}

void graphic_find_color (unsigned i) {
    // Color code
    // RED, GREEN, BLUE, YELLOW, CYAN, PURPLE, GREY, ORANGE, BROWN, KHAKI
    float color[MAX_FOURMILIERE][RGB] = {{1. ,0. ,0. },
                                         {0. ,1. ,0. },
                                         {0. ,0. ,1. },
                                         {1. ,1. ,0. },
                                         {0. ,1. ,1. },
                                         {1. ,0. ,1. },
                                         {0.5,0.5,0.5},
                                         {1.0,0.5,0. },
                                         {0.5,0.1,0.3},
                                         {0.6,0.7,0.3}};
    graphic_set_color3f(color[i][0], color[i][1], color[i][2]);
}
