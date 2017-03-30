#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GLUI/glui.h>
#include <GLUT/glut.h>
#include "graphic.h"

int test = 1;

extern "C"
{
#include "constantes.h"
}

namespace {
    char text[200] = "Salut!";
    GLUI_Checkbox *checkbox1;
    GLUI_Checkbox *checkbox2;
    GLUI_Checkbox *checkbox3;
    GLUI_Spinner *spinner1;
    GLUI_Spinner *spinner2;
    GLUI_EditText *edittext;
    GLUI_RadioGroup *radio;
}

/**************************************** control_cb() *******************/
/* GLUI control callback */

void control_cb(int control)
{
    /******************************************************************** imprimer la valeur du parametre pour indiquer qui a produit l'appel. utiliser les fonctions de type get_int_val() ou get_float_val() plutot que de creer des live variables. ********************************************************************/ printf( "callback: %d\n", control );
    switch (control)
    {
        case (CHECKBOX1_ID):
            printf("checkbox 1: %d\n", checkbox1->get_int_val() );
            break;
        case (CHECKBOX2_ID):
            printf("checkbox 2: %d\n", checkbox2->get_int_val() );
            break;
        case (CHECKBOX3_ID):
            printf("checkbox 3: %d\n", checkbox3->get_int_val() );
            break;
        case (SPINNER1_ID):
            printf("spinner 1: %d\n", spinner1->get_int_val() );
            break;
        case (SPINNER2_ID):
            printf("spinner 2: %f\n", spinner2->get_float_val() );
            break;
        case (EDITTEXT_ID):
            printf("edit text : %s\n", edittext->get_text() );
            break;
        case (RADIOBUTTON_ID):
            printf("radio group: %d\n", radio->get_int_val() );
            break;
        default:
            printf("\n Unknown command\n");
        break;
    }
}

int main_glui()
{
    //glutInit(&argc, argv);
    /****************************************/ /* widgets GLUI */ /****************************************/
    GLUI *glui = GLUI_Master.create_glui((char*) "GLUI" );
    
    // checkbox
    GLUI_Panel *check_panel = glui->add_panel((char*)"Check Buttons", GLUI_PANEL_RAISED);
    checkbox1 = glui->add_checkbox_to_panel(check_panel, (char*)"Option 1",NULL,
                                            CHECKBOX1_ID, control_cb);
    checkbox2 = glui->add_checkbox_to_panel(check_panel,
                                            (char*) "Option 2",NULL,
                                            CHECKBOX2_ID, control_cb);
    checkbox3 = glui->add_checkbox_to_panel(check_panel,
                                            (char*) "Option 3",NULL,
                                            CHECKBOX3_ID, control_cb);
    
    // spinner
    spinner1 = glui->add_spinner((char*)"Segments:", GLUI_SPINNER_INT, NULL,
                                 SPINNER1_ID, control_cb);
    spinner1->set_int_limits( -10, 10 );
    spinner2 = glui->add_spinner((char*)"Segments:", GLUI_SPINNER_FLOAT, NULL,
                                 SPINNER2_ID, control_cb);
    spinner2->set_float_limits( 0.0, 10.0 );
    
    // edit text
    edittext = glui->add_edittext( (char*)"Text:", GLUI_EDITTEXT_TEXT, text, EDITTEXT_ID, control_cb);
    
    // radiobutton
    GLUI_Panel *obj_panel = glui->add_panel((char*) "Object Type" );
    radio = glui->add_radiogroup_to_panel(
                                          obj_panel, NULL, RADIOBUTTON_ID,
                                          control_cb);
    glui->add_radiobutton_to_group( radio,(char*) "Choice 1" );
    glui->add_radiobutton_to_group( radio,(char*) "Choice 2" );
    glui->add_radiobutton_to_group( radio,(char*) "Choice 3" );
    
    // button
    glui->add_button((char*) "Quit", 0,(GLUI_Update_CB)exit );
    
    //Start Glut Main Loop
    //glutMainLoop();
    
    return 0;
}
