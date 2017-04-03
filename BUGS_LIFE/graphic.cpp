#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GLUI/glui.h>
#include <GLUT/glut.h>
#include "graphic.h"
#include <math.h>

extern "C"
{
#include "constantes.h"
}

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

