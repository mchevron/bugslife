#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <GL/glu.h>
//#include <GL/glut.h>
#include <GLUT/glut.h>
#include "graphic.h"
#include <math.h>
#include "constantes.h"

void graphic_draw_circle (float xc,
                          float yc,
                          float r,
                          int   filled)

{ int i;
    const int SIDES = 50;
    
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
    // RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, GREY, ORANGE, DARK_GREEN, PURPLE
    float color[MAX_FOURMILIERE][RGB] = {{255,0,0},
                                         {0,255,0},
                                         {0,0,255},
                                         {255,255,0},
                                         {0,255,255},
                                         {255,0,255},
                                         {128,128,128},
                                         {255,165,0},
                                         {0,100,0},
                                         {128,0,128}};
    graphic_set_color3f(color[i][0], color[i][1], color[i][2]);
}
