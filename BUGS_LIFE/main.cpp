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

namespace {
    //GLUT
    int main_window;
    int width, height;			/* Taille de la fenetre (en pixels) */
    
    //GLUI
    char text[200] = "Fileinput";
    //char color[MAX_FOURMILIERE][200];
    GLUI_EditText *edittext1;
    GLUI_EditText *edittext2;
    GLUI_Checkbox *checkbox;
    GLUI_RadioGroup *radio;
    GLUI_Checkbox *checkbox1;
    GLUI_Checkbox *checkbox2;
    GLUI_Checkbox *checkbox3;
    GLUI_Spinner *spinner1;
    GLUI_Spinner *spinner2;

}

/*------------------------------------------------------------------*/
/*
 * GLUI control callback
 */
void control_cb(int control)
{
    printf( "callback: %d\n", control );
    switch (control)
    {
        case (BUTTON1_ID):
            fopen("E01.txt", "r");
            break;
        case (BUTTON2_ID):
            // save source to destination
            break;
        case (RADIOBUTTON_ID):
            printf("radio group: %d\n", radio->get_int_val() );
            if (radio->get_int_val() == AUTOMATIC) {
                // automatic food creation
            }
            else {
                // manual food creation
            }
            break;
        case (BUTTON3_ID):
            // start ! simulation
            break;
        case (BUTTON4_ID):
            // stop simulation
            break;
        case (CHECKBOX_ID):
            // record simulation
            break;
        default:
            printf("\n Unknown command\n");
            break;
    }
}

/*------------------------------------------------------------------*/
/*
 * widget display callback.
 */
void display_cb(){
    /*Efface le contenu de la fenetre*/
    glClearColor (1.,1.,1.,1.); // spécifie la couleur
    glClear (GL_COLOR_BUFFER_BIT);
    /*Defini le domaine*/
    glLoadIdentity ();
    glOrtho (-20., 20., -20., 20., -1, 1);
    //boucle pour chaque fourmillère
        graphic_set_color3f (0.8, 0.8, 0.8);
        graphic_draw_circle (0., 0., 5, GRAPHIC_EMPTY);
        //boucle pour chaque ouvrière
            graphic_draw_circle (0., 0., 2, GRAPHIC_FILLED);
        //boucle pour chaque garde
            graphic_draw_circle (0., 0., 2, GRAPHIC_FILLED);
    //boucle pour chaque nourriture
    
    glutSwapBuffers();
}

/*------------------------------------------------------------------*/
/*
 * widget reshape callback.
 */
void  reshape_cb ( int  x,  int  y)
{
    width = x;
    height = y;
    glViewport( 0, 0, width, height);
}

/*------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    //if(modele_lecture(argc, argv)) return EXIT_FAILURE;
    
    /**********************/ /* GLUT */ /**********************/
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowPosition( 50, 50 );
    glutInitWindowSize( 400, 400 );
    
    main_window = glutCreateWindow(  "Serie 17-19"  );
    glutDisplayFunc( display_cb );
    glutReshapeFunc( reshape_cb );
    
    /**********************/ /* GLUI */ /**********************/
    GLUI *glui = GLUI_Master.create_glui((char*) "Bug's Life", 0, 500, 50 );
    
    // file panel
    GLUI_Panel *File_panel = glui->add_panel((char*) "File" );
    edittext1 = glui->add_edittext_to_panel(File_panel, (char*)"FileName:",
                                            GLUI_EDITTEXT_TEXT, text,
                                            EDITTEXT_ID, control_cb);
    glui->add_button_to_panel(File_panel, (char*) "Open", BUTTON1_ID, control_cb);
    edittext2 = glui->add_edittext_to_panel(File_panel, (char*)"FileName:",
                                            GLUI_EDITTEXT_TEXT, text,
                                            EDITTEXT_ID, control_cb);
    glui->add_button_to_panel(File_panel, (char*) "Save", BUTTON2_ID, control_cb);
    
    // Food creation radiobutton
    GLUI_Panel *food_creation_panel = glui->add_panel((char*) "Food Creation" );
    radio = glui->add_radiogroup_to_panel(
                                          food_creation_panel, NULL, RADIOBUTTON_ID,
                                          control_cb);
    glui->add_radiobutton_to_group( radio,(char*) "Automatic" );
    glui->add_radiobutton_to_group( radio,(char*) "Manual" );
    
    // simulation panel
    GLUI_Panel *simulation_panel = glui->add_panel((char*) "File" );
    glui->add_button_to_panel(simulation_panel, (char*) "Start !", BUTTON3_ID,
                              control_cb);
    glui->add_button_to_panel(simulation_panel, (char*) "Stop", BUTTON4_ID,
                              control_cb);
    checkbox = glui->add_checkbox_to_panel(simulation_panel, (char*)"Record",NULL,
                                            CHECKBOX_ID, control_cb);
    
    glui->add_button((char*) "Exit", 0, exit);
    
    // Information rollout
    glui->add_column();
    GLUI_Panel *information_rollout = glui->add_rollout((char*) "Information");
    glui->add_statictext_to_panel(information_rollout, (char*) "Couleur");
    //for(i=0; i<nb_fourmiliere; i+=1) {
    //glui->add_statictext_to_panel(information_rollout, (char*) color[i]);
    //}
    glui->add_column_to_panel(information_rollout);
    glui->add_statictext_to_panel(information_rollout, (char*) "Fourmis total");
    glui->add_column_to_panel(information_rollout);
    glui->add_statictext_to_panel(information_rollout, (char*) "Ouvrières");
    glui->add_column_to_panel(information_rollout);
    glui->add_statictext_to_panel(information_rollout, (char*) "Gardes");
    glui->add_column_to_panel(information_rollout);
    glui->add_statictext_to_panel(information_rollout, (char*) "Nourriture");
    
    //Start Glut Main Loop
    glutMainLoop();

    return EXIT_SUCCESS;
}
