/*
*  File: memgameview.h
*  Project: Assignment #1 for ENEL323 (Memory Game)
*  Brief: Displays game on screen.
*  Author: Aydin Arik (aar52, 19473818)
*  Last Modified: 21/04/11
* 
*  Additional Notes: 
*   -Acts as the view (MVC) for the program. 
*/

#ifndef VIEW_H
#define VIEW_H 


/********************************************************************************************************
*                        Header Files
********************************************************************************************************/

#include "global.h"
#include "cardchars.h"
#include "memgamecontroller.h"
#include "memgamemodel.h"



/********************************************************************************************************
*                        Global Constants
********************************************************************************************************/

enum windowDimensions{DEFAULT_WIN_WIDTH = 1000, DEFAULT_WIN_HEIGHT = 800}; /* Window is always this size */
enum windowStartPos{DEFAULT_WIN_POSITION_X = 100, DEFAULT_WIN_POSITION_Y = 100}; 


/* Button and Card Dimensions */

enum cardDimensions{CARD_WIDTH = 65, CARD_HEIGHT = 100, CARD_CENTRE_X = DEFAULT_WIN_WIDTH / 2, 
CARD_CENTRE_Y = DEFAULT_WIN_HEIGHT / 2, BORDER = 5, CARD_GAP = 10};

/* BUT_CENTRE_TEXT_Y is required to centre the text (GLUT_BITMAP_HELVETICA_18) in the button */
enum buttonDimensions{BUT_HEIGHT = 75, BUT_WIDTH = 200, BUT_CENTRE_X = DEFAULT_WIN_WIDTH /2,
BUT_CENTRE_Y = DEFAULT_WIN_HEIGHT / 2, BUT_GAP = 10, BUT_CENTRE_TEXT_Y = 9}; 



/********************************************************************************************************
*                        Public Functions
********************************************************************************************************/

/* Display Settings */
void initGlutOpenglSettings(void); /* More GLUT/ openGL initialisations */
void resizeWindow(int windowWidth, int windowHeight); /* Keeps window at a size of 1000x800 */

/* General Display Function (Menus and Game Screen) */
void display(void); 

/* Button Display Function (Menus)  */
void drawButton(button_t *button, char *buttonString);

/* Card Display Function (In-Game) */
int drawCard(card_t *card);



#endif