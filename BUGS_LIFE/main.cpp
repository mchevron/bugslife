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


extern "C"
{
#include "modele.h"
#include "constantes.h"
#include "graphic.h"
}

namespace {
    //GLUT
    int main_window;
    int width, height;			/* Taille de la fenetre (en pixels) */
    GLfloat aspect_ratio;
    
    //GLUI
    char text[200] = "Fileinput";
    int run = 0;
    GLUI_EditText *edittext1;
    GLUI_EditText *edittext2;
    GLUI_Checkbox *checkbox;
    GLUI_RadioGroup *radio;
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
        case (BUTTON1_ID): {
            //remplace la simulation par le contenu du fichier
            printf("modele_update\n one step\n");
            const char *fentree = edittext1->get_text();
            //remplace la simulation par le contenu du fichier
            printf("%s\n", fentree);
            //if(modele_update(fentree) return EXIT_FAILURE;
            break;
        }
        case (BUTTON2_ID): {
            const char* fsortie = edittext2->get_text();
            printf("%c\n", *fsortie);
            //sauvegarde(fsortie);
            break;
        }
        case (RADIOBUTTON_ID): {
            printf("radio group: %d\n", radio->get_int_val() );
            if (radio->get_int_val() == AUTOMATIC) {
                // automatic food creation
            }
            break;
        }
        case (BUTTON3_ID): {
            // start ! simulation
            printf("start !\n");
            run = RUN;
            break;
        }
        case (BUTTON4_ID): {
            // step simulation
            printf("modele_update\n one step\n");
            run = STEP;
            break;
        }
        case (CHECKBOX_ID): {
            // record simulation
            break;
        }
        default: {
            printf("\n Unknown command\n");
            break;
        }
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
    glOrtho (-1, 1, -1, 1, -1, 1);
    
    if (aspect_ratio <= 1.)
        glOrtho(-DMAX, DMAX, -DMAX/aspect_ratio, DMAX/aspect_ratio, -1.0, 1.0);
    else
        glOrtho(-DMAX*aspect_ratio, DMAX*aspect_ratio, -DMAX, DMAX, -1.0, 1.0);
    
    modele_dessine_complet();

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
    aspect_ratio = (GLfloat) width / (GLfloat) height;
    glutPostRedisplay();
}

/*------------------------------------------------------------------*/
/*
 * Clique souris pour nourriture en mode manuel
 */
void processMouse(int button, int state, int x, int y)
{
    float pos_x, pos_y;
    
    if (state == GLUT_DOWN && radio->get_int_val() != AUTOMATIC) {
        pos_x = -DMAX + ((double) x/width)*(DMAX+DMAX);
        pos_y = -DMAX + ((double) (height -y)/height)*(DMAX+DMAX);
        new_food(pos_x, pos_y);
    }
}

/*------------------------------------------------------------------*/
/*
 *idle
 */

void idle_cb()
{
    if (run == RUN){
        if (glutGetWindow() != main_window) glutSetWindow( main_window);
        printf("Modele update\n");
        glutPostRedisplay();
    }
}

/*------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    switch (argc) {
        case MODE_SIMPLE:
            
            break;
        case MODE_SPECIFIQUE:
            if ((strcmp(argv[1], "Error") == 0) ||
                (strcmp(argv[1] , "Verification") == 0)
                || (strcmp(argv[1], "Graphic") == 0)
                || (strcmp(argv[1], "Final") == 0)){
                if (modele_lecture(argv[1], argv[2])) return EXIT_FAILURE;
            }
            break;
        default:
            printf("usage : \"%s mode_test nom_fichier\" ou \"%s\"\n"
                   ,argv[0],argv[0]);
            return EXIT_FAILURE;
    }
    /**********************/ /* GLUT */ /**********************/
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA );
    glutInitWindowPosition( 50, 50 );
    glutInitWindowSize( 500, 500 );
    
    main_window = glutCreateWindow("Bug's Life");
    glutDisplayFunc(display_cb);
    glutReshapeFunc(reshape_cb);
    glutMouseFunc(processMouse);
    
    /**********************/ /* GLUI */ /**********************/
    GLUI *glui = GLUI_Master.create_glui((char*) "Bug's Life", 0, 600, 50 );
    
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
    glui->add_button_to_panel(simulation_panel, (char*) "Step", BUTTON4_ID,
                              control_cb);
    checkbox = glui->add_checkbox_to_panel(simulation_panel, (char*)"Record",NULL,
                                            CHECKBOX_ID, control_cb);
    
    glui->add_button((char*) "Exit", 0, exit);
    
    // Information rollout
    glui->add_column();
    GLUI_Panel *information_rollout = glui->add_rollout((char*) "Information");
    GLUI_Panel *titles = glui->add_panel_to_panel(information_rollout, "", GLUI_PANEL_NONE);
    glui->add_statictext_to_panel(titles, (char*) "Couleur");
    glui->add_column_to_panel(titles);
    glui->add_statictext_to_panel(titles, (char*) "Fourmis total");
    glui->add_column_to_panel(titles);
    glui->add_statictext_to_panel(titles, (char*) "Ouvrières");
    glui->add_column_to_panel(titles);
    glui->add_statictext_to_panel(titles, (char*) "Gardes");
    glui->add_column_to_panel(titles);
    glui->add_statictext_to_panel(titles, (char*) "Nourriture");
    
    int i = 0;
    GLUI_Panel *info[MAX_FOURMILIERE];
    for(i=0; i<10; i=i+1) {
        info[i] = glui->add_panel_to_panel(information_rollout, "", GLUI_PANEL_NONE);
        glui->add_statictext_to_panel(info[i], "RED");
        glui->add_column_to_panel(info[i]);
        glui->add_statictext_to_panel(info[i],  get_info_rollout(NB_FOURMI));
        glui->add_column_to_panel(info[i]);
        glui->add_statictext_to_panel(info[i], (char*) "Ouvrières");
        glui->add_column_to_panel(info[i]);
        glui->add_statictext_to_panel(info[i], (char*) "Gardes");
        glui->add_column_to_panel(info[i]);
        glui->add_statictext_to_panel(info[i], (char*) "Nourriture");
    }
    glui->add_separator_to_panel(information_rollout );
    
    glui->set_main_gfx_window(main_window);
    
    //Callbacks
    GLUI_Master.set_glutIdleFunc(idle_cb);
    glui->set_main_gfx_window(main_window);
    glutMainLoop();
    
    return EXIT_SUCCESS;
}
