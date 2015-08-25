/*
*  File: memgamecontroller.h
*  Project: Assignment #1 for ENEL323 (Memory Game)
*  Brief: Provides gui functionality (GLUT/ openGL) and screen refresh ability.
*  Author: Aydin Arik (aar52, 19473818)
*  Last Modified: 21/04/11
* 
*  Additional Notes: 
*   -Acts as the controller (MVC) for the program.
*/

/* Header Files */
#include <GL/glut.h>
#include "memgamecontroller.h"


void refreshDisplay(void)
{
    glutPostRedisplay();
}


int main(int argc, char **argv)
{
    //GLuint tex_2d;
    /* GLUT/openGL Initialisations for Window */
    glutInit(&argc, argv);
    glutInitWindowSize(DEFAULT_WIN_WIDTH, DEFAULT_WIN_HEIGHT); 
    glutInitWindowPosition(DEFAULT_WIN_POSITION_X, DEFAULT_WIN_POSITION_Y);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutCreateWindow("Memory Game");
    initGlutOpenglSettings(); 
    
    /*
    tex_2d = SOIL_load_OGL_texture
    (
        "img.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );
*/
    /* Registering Callback Functions */
    glutDisplayFunc(display);
    glutMouseFunc(mouseButtonFunc);
    glutMotionFunc(mouseMotionFunc);
    glutPassiveMotionFunc(passiveMotionFunc);
    glutReshapeFunc(resizeWindow); /* Removes the distortion caused by rescaling the window */    
    
    /* Entering the GLUT event processing loop */
    glutMainLoop();
    
    return 0;
    
}