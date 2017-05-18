/*!
 \file main.cpp
 \brief Programme Bug's life
 \author Diane Remmy & Max Chevron
 \version 2.0
 \date Avril 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __APPLE__
#  include <GLUT/GLUT.h>
#  include <GLUI/GLUI.h>
#else
#  include <GL/glu.h>
#  include <GL/glut.h>
#  include <GL/glui.h>
#endif

extern "C"
{
#include "modele.h"
#include "constantes.h"
}

#define R_T_LINE				12
#define R_T_COL                 4
#define TOTAL_ROLLOUT           11
#define ENUM_INCREMENT          5
#define MODE_SIMPLE				1		
#define MODE_SPECIFIQUE 		3
#define INFO_COLONNES   		4
#define INFO            		5
#define BLANK           		0
#define RUN             		1
#define STOP             		0
/********** User IDs pour les fonctions callback ********/
#define FILE_NAME_OPEN  		01
#define OPEN           			02
#define FILE_NAME_SAVE  		03
#define SAVE            		04
#define START           		05
#define STEP            		06
#define RECORD         		 	11
#define AUTO_MAN        		12

void main_rollout_update(void);

namespace {
    //GLUT
    int main_window;
    int width, height;			/* Taille de la fenetre (en pixels) */
    GLfloat aspect_ratio;
    
    //GLUI
    int run = 0;
    GLUI_EditText* entree;
    GLUI_EditText* sortie;
    GLUI_Checkbox* record;
    GLUI_RadioGroup* auto_man_radio;
    GLUI_StaticText* rollout[R_T_LINE][R_T_COL];
    GLUI_Button* start_stop;
}

void control_cb(int control){
    char mode[] = "Final";
    switch (control){
        case (OPEN): 
            printf( "Open file: %s\n", entree->get_text());
            modele_cleanup();
            if(modele_lecture(mode, (char*)entree->get_text())) modele_cleanup();
            glutPostRedisplay();
            break;
        case (SAVE):
            printf("Save file: %s\n", sortie->get_text());
            modele_sauvegarde((char*)sortie->get_text());
            break;
        case (AUTO_MAN):
            printf("radio group: %d\n", auto_man_radio->get_int_val() );
            break;
        case (START):
            if(run==0||run==STEP){
                printf("start !\n");
                run = RUN;
                start_stop->set_name("Stop");
            }
            else{
                printf("Stop\n");
                run = STOP;
                start_stop->set_name("Start !");
            }
            break;
        case (STEP):
            // step simulation
            printf("modele_update\n one step\n");
            if (glutGetWindow() != main_window) glutSetWindow( main_window );
            modele_update(auto_man_radio->get_int_val());
            modele_record(record->get_int_val());
            glutPostRedisplay();
            run = STEP;
            start_stop->set_name("Start !");
            break;
        case (RECORD):
            // record simulation
            modele_record(record->get_int_val());
            break;
        default:
            printf("\n Unknown command\n");
            break;
    }
}

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
    main_rollout_update();
    glutSwapBuffers();
}

void  reshape_cb ( int  x,  int  y){
    width = x;
    height = y;
    glViewport( 0, 0, width, height);
    aspect_ratio = (GLfloat) width / (GLfloat) height;
    glutPostRedisplay();
}

// s'occupe de la convertion OPENGL/GLUT de l'endroit cliquer par la souris
void processMouse(int button, int state, int x, int y){
    float pos_x, pos_y;
    if (state == GLUT_DOWN && auto_man_radio->get_int_val() != AUTOMATIC) {
        float dmax_x = DMAX;
        float dmax_y = DMAX;
        if(width>height) dmax_x = DMAX*aspect_ratio;
        if(width<height) dmax_y = DMAX/aspect_ratio;
        pos_x = -dmax_x + ((double) x/width)*(dmax_x+dmax_x);
        pos_y = -dmax_y + ((double) (height -y)/height)*(dmax_y+dmax_y);
        if(width>height && (pos_x < -dmax_x + ((double)
                           (width-height)/(2*width))*(dmax_x+dmax_x)
                            || pos_x > -dmax_x + ((double)
                               (width+height)/(2*width))*(dmax_x+dmax_x)))
            printf("Cannot add food out of the map\n");
        else if(width<height && (pos_y < -dmax_y + ((double)
                            (height-width)/(2*height))*(dmax_y+dmax_y)
                            || pos_y > -dmax_y + ((double)
                               (height+width)/(2*height))*(dmax_y+dmax_y)))
            printf("Cannot add food out of the map\n");
        else {
            modele_new_food(pos_x, pos_y);
            glutPostRedisplay();
        }
    }
}

void idle_cb(){
    if (run == RUN){
        if (glutGetWindow() != main_window) glutSetWindow(main_window);
        modele_update(auto_man_radio->get_int_val());
        modele_record(record->get_int_val());
        glutPostRedisplay();
    }
}

// ajoute des fonctionnalitées au panel
void add_file_panel(GLUI* glui) {
    GLUI_Panel *File_panel = glui->add_panel( "File" );
    entree = glui->add_edittext_to_panel(File_panel, "FileName:");
    glui->add_button_to_panel(File_panel,  "Open", OPEN, control_cb);
    sortie = glui->add_edittext_to_panel(File_panel, "FileName:");
    glui->add_button_to_panel(File_panel, "Save", SAVE, control_cb);
    
    // Food creation radiobutton
    GLUI_Panel *food_creation_panel = glui->add_panel( "Food Creation" );
    auto_man_radio = glui->add_radiogroup_to_panel(
                                          food_creation_panel, NULL, AUTO_MAN,
                                          control_cb);
    glui->add_radiobutton_to_group( auto_man_radio, "Automatic" );
    glui->add_radiobutton_to_group( auto_man_radio, "Manual" );
}

// ajoute des fonctionnalitées au panel pour la simulaiton
void add_simulation_panel(GLUI* glui) {
    GLUI_Panel *simulation_panel = glui->add_panel( "File" );
    start_stop = glui->add_button_to_panel(simulation_panel,  "Start !", START,
                              control_cb);
    glui->add_button_to_panel(simulation_panel,  "Step", STEP,
                              control_cb);
    record = glui->add_checkbox_to_panel(simulation_panel, "Record", NULL,
                                           RECORD, control_cb);
}


// s'occupe de remplir le tableau du panel
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
    for(i=0; i<MAX_FOURMILIERE; i=i+1) {
        GLUI_Panel *info = glui->add_panel_to_panel(info_rollout, "",
                                                    GLUI_PANEL_NONE);
        glui->add_statictext_to_panel(info, modele_get_info_glui(COLOR, i));
        glui->add_column_to_panel(info, BLANK);
        rollout[i][NB_FOURMI] = glui->add_statictext_to_panel(info, "");
        glui->add_column_to_panel(info, BLANK);
        rollout[i][NB_OUVRIERE] = glui->add_statictext_to_panel(info, "");
        glui->add_column_to_panel(info, BLANK);
        rollout[i][NB_GARDE] = glui->add_statictext_to_panel(info, "");
        glui->add_column_to_panel(info, BLANK);
        rollout[i][NB_NOURRITURE] = glui->add_statictext_to_panel(info, "");
    }
    //total
    glui->add_separator_to_panel(info_rollout);
    GLUI_Panel *total = glui->add_panel_to_panel(info_rollout, "", GLUI_PANEL_NONE);
    glui->add_statictext_to_panel(total, "Total");
    glui->add_column_to_panel(total, BLANK);
    rollout[TOTAL_ROLLOUT][NB_FOURMI] = glui->add_statictext_to_panel(total, "");
    glui->add_column_to_panel(total, BLANK);
    rollout[TOTAL_ROLLOUT][NB_OUVRIERE] = glui->add_statictext_to_panel(total, "");
    glui->add_column_to_panel(total, BLANK);
    rollout[TOTAL_ROLLOUT][NB_GARDE] = glui->add_statictext_to_panel(total, "");
    glui->add_column_to_panel(total, BLANK);
    rollout[TOTAL_ROLLOUT][NB_NOURRITURE] = glui->add_statictext_to_panel(total, "");
}

// s'occupe d'actualiser le tableau du panel avec les nouvelles valeurs
void main_rollout_update(void) {
    int i, c;								// indices des lignes i et colonnes c
    for(i=0; i<MAX_FOURMILIERE; i=i+1) {
        for(c=0; c<R_T_COL; c=c+1) {
            rollout[i][c]->set_text(modele_get_info_glui(c, i));
        }
    }
    for(c=0; c<R_T_COL; c=c+1) {
        rollout[TOTAL_ROLLOUT][c]->set_text(modele_get_info_glui(c+ENUM_INCREMENT, 
																 BLANK));
    }
}


int main(int argc, char *argv[]){
    switch (argc) {
        case MODE_SIMPLE:          
			system("./demo.x");
			return EXIT_SUCCESS;
        case MODE_SPECIFIQUE:
            if ((strcmp(argv[1], "Error") == 0) ||
                (strcmp(argv[1] , "Verification") == 0)) {
                if (modele_lecture(argv[1], argv[2])) return EXIT_FAILURE;
            }
            if (strcmp(argv[1], "Final") == 0) {
                if (modele_lecture(argv[1], argv[2])) modele_cleanup();
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
    GLUI_Master.set_glutIdleFunc(idle_cb);
    glui->set_main_gfx_window(main_window);

    glutMainLoop();
    
    return EXIT_SUCCESS;
}
