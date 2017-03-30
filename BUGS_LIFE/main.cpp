/*!
 \file main.cpp
 \brief Programme Bug's life
 \author Diane Remmy & Max Chevron
 \version 1.0
 \date Mars 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GLUI/glui.h>
#include <GLUT/glut.h>
#include "graphic.h"

extern "C"
{
#include "modele.h"
#include "constantes.h"
}


int main(int argc, char *argv[])
{
    if(modele_lecture(argc, argv)) return EXIT_FAILURE;
    main_glui();
    return EXIT_SUCCESS;
}
