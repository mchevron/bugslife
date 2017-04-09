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
    //char* info_rollout = NULL;
    GLUI_EditText *f_entree;
    GLUI_EditText *f_sortie;
    GLUI_Checkbox *record;
    GLUI_RadioGroup *auto_man_radio;
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
        case (OPEN): {
            //remplace la simulation par le contenu du fichier
            printf("modele_update\n one step\n");
            const char *fentree = f_entree->get_text();
            //remplace la simulation par le contenu du fichier
            printf( "text: %s\n", f_entree->get_text());
            printf( "text: %s\n", fentree);
            //if(modele_update(fentree) return EXIT_FAILURE;
            break;
        }
        case (SAVE): {
            const char* fsortie = f_sortie->get_text();
            printf("%c\n", *fsortie);
            //sauvegarde(fsortie);
            break;
        }
        case (AUTO_MAN): {
            printf("radio group: %d\n", auto_man_radio->get_int_val() );
            if (auto_man_radio->get_int_val() == AUTOMATIC) {
                // automatic food creation
            }
            break;
        }
        case (START): {
            // start ! simulation
            printf("start !\n");
            run = RUN;
            break;
        }
        case (STEP): {
            // step simulation
            printf("modele_update\n one step\n");
            run = STEP;
            break;
        }
        case (RECORD): {
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
    
    if (state == GLUT_DOWN && auto_man_radio->get_int_val() != AUTOMATIC) {
        pos_x = -DMAX + ((double) x/width)*(DMAX+DMAX);
        pos_y = -DMAX + ((double) (height -y)/height)*(DMAX+DMAX);
        new_food(pos_x, pos_y);
        glutPostRedisplay();
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
/*
 *panels and rollout
 */
void add_file_panel(GLUI* glui) {
    GLUI_Panel *File_panel = glui->add_panel( "File" );
    f_entree = glui->add_edittext_to_panel(File_panel, "FileName:",
                                            GLUI_EDITTEXT_TEXT, text,
                                            FILE_NAME_OPEN, control_cb);
    glui->add_button_to_panel(File_panel,  "Open", OPEN, control_cb);
    f_sortie = glui->add_edittext_to_panel(File_panel, "FileName:",
                                            GLUI_EDITTEXT_TEXT, text,
                                            FILE_NAME_SAVE, control_cb);
    glui->add_button_to_panel(File_panel,  "Save", SAVE, control_cb);
    
    // Food creation radiobutton
    GLUI_Panel *food_creation_panel = glui->add_panel( "Food Creation" );
    auto_man_radio = glui->add_radiogroup_to_panel(
                                          food_creation_panel, NULL, AUTO_MAN,
                                          control_cb);
    glui->add_radiobutton_to_group( auto_man_radio, "Automatic" );
    glui->add_radiobutton_to_group( auto_man_radio, "Manual" );
}

void add_simulation_panel(GLUI* glui) {
    GLUI_Panel *simulation_panel = glui->add_panel( "File" );
    glui->add_button_to_panel(simulation_panel,  "Start !", START,
                              control_cb);
    glui->add_button_to_panel(simulation_panel,  "Step", STEP,
                              control_cb);
    record = glui->add_checkbox_to_panel(simulation_panel, "Record",NULL,
                                           RECORD, control_cb);
}


void add_rollout(GLUI* glui) {
    GLUI_Panel *info_rollout = glui->add_rollout( "Information");
    //header
    GLUI_Panel *titles = glui->add_panel_to_panel(info_rollout, "", GLUI_PANEL_NONE);
    glui->add_statictext_to_panel(titles, "Couleur");
    glui->add_column_to_panel(titles, BLANK);
    glui->add_statictext_to_panel(titles, "Fourmis total");
    glui->add_column_to_panel(titles, BLANK);
    glui->add_statictext_to_panel(titles, "Ouvrières");
    glui->add_column_to_panel(titles, BLANK);
    glui->add_statictext_to_panel(titles, "Gardes");
    glui->add_column_to_panel(titles, BLANK);
    glui->add_statictext_to_panel(titles, "Nourriture");
    //info
    int i = 0;
    for(i=0; i<10; i=i+1) {
        GLUI_Panel *info = glui->add_panel_to_panel(info_rollout, "", GLUI_PANEL_NONE);
        glui->add_statictext_to_panel(info, modele_get_info_glui(COLOR, i));
        glui->add_column_to_panel(info, BLANK);
        glui->add_statictext_to_panel(info, modele_get_info_glui(NB_FOURMI, i));
        glui->add_column_to_panel(info, BLANK);
        glui->add_statictext_to_panel(info, modele_get_info_glui(NB_OUVRIERE, i));
        glui->add_column_to_panel(info, BLANK);
        glui->add_statictext_to_panel(info, modele_get_info_glui(NB_GARDE, i));
        glui->add_column_to_panel(info, BLANK);
        glui->add_statictext_to_panel(info, modele_get_info_glui(NB_NOURRITURE, i));
    }
    //total
    glui->add_separator_to_panel(info_rollout);
    GLUI_Panel *total = glui->add_panel_to_panel(info_rollout, "", GLUI_PANEL_NONE);
    glui->add_statictext_to_panel(total, "Total");
    glui->add_column_to_panel(total, BLANK);
    glui->add_statictext_to_panel(total, modele_get_info_glui(NBT_FOURMI, BLANK));
    glui->add_column_to_panel(total, BLANK);
    glui->add_statictext_to_panel(total, modele_get_info_glui(NBT_OUVRIERE, BLANK));
    glui->add_column_to_panel(total, BLANK);
    glui->add_statictext_to_panel(total, modele_get_info_glui(NBT_GARDE, BLANK));
    glui->add_column_to_panel(total, BLANK);
    glui->add_statictext_to_panel(total, modele_get_info_glui(NBT_NOURRITURE, BLANK));
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
    GLUI *glui = GLUI_Master.create_glui( "Bug's Life", 0, 600, 50 );
    add_file_panel(glui);
    add_simulation_panel(glui);
    glui->add_button( "Exit", 0, exit);
    glui->add_column();
    add_rollout(glui);

    glui->set_main_gfx_window(main_window);
    
    //Callbacks
    GLUI_Master.set_glutIdleFunc(idle_cb);
    glui->set_main_gfx_window(main_window);
    glutMainLoop();
    
    return EXIT_SUCCESS;
}
