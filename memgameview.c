/*
*  File: memgameview.c
*  Project: Assignment #1 for ENEL323 (Memory Game)
*  Brief: Displays game on screen.
*  Author: Aydin Arik (aar52, 19473818)
*  Last Modified: 21/04/11
* 
*  Additional Notes: 
*   -Acts as the view (MVC) for the program. 
*/


/********************************************************************************************************
*                        Header Files
********************************************************************************************************/

#include <stdio.h>
#include <GL/glut.h>
#include "memgameview.h"


/********************************************************************************************************
*                        Private Constants
********************************************************************************************************/

#define STRLEN_SCORES 13 /* length of string required to display player scores. This includes NULL character */

/* Background Colour */
#define BKGD_R 0
#define BKGD_G 0
#define BKGD_B 0

/* Card Border Colour */
#define BORDER_COLOUR_R 0.5
#define BORDER_COLOUR_G 0.5
#define BORDER_COLOUR_B 0.5

/* Card Colour */
#define CARD_OUT_R 1.0
#define CARD_OUT_G 0
#define CARD_OUT_B 0
#define CARD_IN_R 1.0
#define CARD_IN_G 1.0
#define CARD_IN_B 1.0
#define CARD_REMOVE_R 0
#define CARD_REMOVE_G 0
#define CARD_REMOVE_B 0
#define CARD_DIM 0.5

/* Card Character Bitmap Text Height*/
#define CARD_CHAR_HEIGHT 24

/* Card Character Colour */
#define CARD_TEXT_R 0
#define CARD_TEXT_G 0
#define CARD_TEXT_B 0

/* Button Colour */
#define BUT_OUT_R 1.0
#define BUT_OUT_G 0
#define BUT_OUT_B 0
#define BUT_DIM 0.5

/* Button Text Colour */
#define BUT_TEXT_R 1.0
#define BUT_TEXT_G 1.0
#define BUT_TEXT_B 1.0

/* Default Displayed Text Colour (used for screen messages and displaying player scores */
#define TEXT_R 1.0
#define TEXT_G 1.0 
#define TEXT_B 1.0 

/* Text Highlight Colour (used to show current players turn) */
#define TEXT_HIGHLIGHT_R 0.0
#define TEXT_HIGHLIGHT_G 0.0 
#define TEXT_HIGHLIGHT_B 1.0 

/* Messages */
#define GAME_NAME "MEMORY GAME"
#define AUTHOR "by Aydin Arik"
#define GAME_MENU_MSG "How many cards would you like to play with?"
#define GAME_END_MSG "Would you like to play again?"
#define GAME_WINNER_P1_MSG "Player 1 Wins!"
#define GAME_WINNER_P2_MSG "Player 2 Wins!"
#define GAME_DRAW_MSG "Draw."
#define PLAYER_NUM_1 "Player 1"
#define PLAYER_NUM_2 "Player 2"


/* "Memory Game" Screen Position */
#define GAME_NAME_MSG_X 400
#define GAME_NAME_MSG_Y 200

/* "by Aydin Arik" Screen Position */
#define AUTHOR_MSG_X 430
#define AUTHOR_MSG_Y 220

/* Menu and End Game Message Positions */
#define GAME_MENU_MSG_X 315
#define GAME_END_MSG_X 380
#define GAME_MSG_Y 350 

/* "Player 1 Wins!"/ "Player 2 Wins!" Screen Position */
#define GAME_WINNER_MSG_X 425
#define GAME_WINNER_MSG_Y 250

/* "Draw." Screen Position */ 
#define GAME_DRAW_MSG_X 475
#define GAME_DRAW_MSG_Y 250

/* Screen Position of Players Scores */
#define P1_SCORE_X 300
#define P2_SCORE_X 600
#define P_SCORE32_Y 150
#define P_SCORE72_Y 50




/********************************************************************************************************
*                        Private Functions
********************************************************************************************************/

/* Display Functions (Menus and Game Screen) */
static void drawPlayerScores(void);


/* Text Rendering Functions */

/* renderText is used to draw general bitmap messages to the screen */
static void renderText(int stringPosX, int stringPosY, void *font, char *string);

/* renderButtonText draws a bitmap text to the screen depending on the position of a button */  
static void renderButtonText(void *font, button_t *button, char *buttonString); 

/* renderCardChar draws a bitmap character to the screen depending on the position of a card */ 
static void renderCardChar(void *font, card_t *card);




/********************************************************************************************************
*                        Public Functions
********************************************************************************************************/

/* Display Settings */
void initGlutOpenglSettings(void);
void resizeWindow(int windowWidth, int windowHeight);

/*  General Display Function (Menus and In-Game Screen) */
void display(void);

/* Button Display Function (Menus)  */
void drawButton(button_t *button, char *buttonString);

/* Card Display Function (In-Game) */
int drawCard(card_t *card);





/********************************************************************************************************
 * 
 *                        Display Settings  
 * 
 * ******************************************************************************************************/

/* More GLUT/ openGL initialisations */
void initGlutOpenglSettings(void)
{
    /* Setting background colour */
    glClearColor(BKGD_R, BKGD_G, BKGD_B, 0); 
    
    /* Setting the origin of the viewing area at top left corner. Order is (Left,Right,Bottom,Top). */
    gluOrtho2D(0, DEFAULT_WIN_WIDTH, DEFAULT_WIN_HEIGHT, 0); 
}


/* GLUT call-back function. Keeps window at the same size throughout the programs life */
 void resizeWindow(int windowWidth, int windowHeight)
 {
     glutReshapeWindow(DEFAULT_WIN_WIDTH, DEFAULT_WIN_HEIGHT);
 }





/********************************************************************************************************
 * 
 *                        General Display Function (Menus and In-Game Screen)
 * 
 * ******************************************************************************************************/

/* GLUT call-back function. Used to display menus and in-game content */
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    /* What game mode are we in? */
    if ((getGameMode() == NUM_CARDS_32) || (getGameMode() == NUM_CARDS_72)) /* In-game */
    {
        drawPlayerScores();
        createCards();
    }
    else if (getGameMode() == GAME_MENU) /* Main menu */
    {
        glPushMatrix();
        glColor3f(BUT_TEXT_R, BUT_TEXT_G, BUT_TEXT_B);
        renderText(GAME_NAME_MSG_X, GAME_NAME_MSG_Y, GLUT_BITMAP_TIMES_ROMAN_24, GAME_NAME); /* Draw the games name */
        renderText(AUTHOR_MSG_X, AUTHOR_MSG_Y, GLUT_BITMAP_9_BY_15, AUTHOR); /* Draw the author of the game */
        renderText(GAME_MENU_MSG_X, GAME_MSG_Y, GLUT_BITMAP_HELVETICA_18, GAME_MENU_MSG); /* Draw button related message */
        glPopMatrix();
        
        createButtons(); 
    }
    else if (getGameMode() == GAME_END) /* End-game menu */
    {
        glPushMatrix();
        glColor3f(BUT_TEXT_R, BUT_TEXT_G, BUT_TEXT_B);
        
        /* Decide and display game winner */
        if (getPlayer1Score() > getPlayer2Score()) 
            renderText(GAME_WINNER_MSG_X, GAME_WINNER_MSG_Y, GLUT_BITMAP_TIMES_ROMAN_24, GAME_WINNER_P1_MSG);
        else if (getPlayer1Score() < getPlayer2Score())
            renderText(GAME_WINNER_MSG_X, GAME_WINNER_MSG_Y, GLUT_BITMAP_TIMES_ROMAN_24, GAME_WINNER_P2_MSG);
        else /* Draw */
            renderText(GAME_DRAW_MSG_X, GAME_DRAW_MSG_Y, GLUT_BITMAP_TIMES_ROMAN_24, GAME_DRAW_MSG);
        
        
        renderText(GAME_END_MSG_X, GAME_MSG_Y, GLUT_BITMAP_HELVETICA_18, GAME_END_MSG); /* Draw button related message */
        
        glPopMatrix();
        
        drawPlayerScores();
        createButtons(); 
    }
    
    glutSwapBuffers();
}




/********************************************************************************************************
 * 
 *                        Display Functions (Menus and Game Screen)
 * 
 * ******************************************************************************************************/

/* Function which displays the players scores and current players turn (text is highlighted) */ 
static void drawPlayerScores(void)
{
    /* Player Score Strings */
    char player1String[STRLEN_SCORES];
    char player2String[STRLEN_SCORES];
    
    /* Attaching player scores to their strings (for score display purposes) */
    sprintf(player1String, PLAYER_NUM_1": %d",getPlayer1Score());
    sprintf(player2String, PLAYER_NUM_2": %d",getPlayer2Score());
    
    
    /* Required to correctly display player score strings on screen */ 
    int p1StrScorePosX = P1_SCORE_X;
    int p1StrScorePosY;
    int p2StrScorePosX = P2_SCORE_X;
    int p2StrScorePosY;

    switch (getGameMode())
    {
        case NUM_CARDS_32:
            p1StrScorePosY = P_SCORE32_Y;
            p2StrScorePosY = P_SCORE32_Y; 
            break;
        case NUM_CARDS_72:
            p1StrScorePosY = P_SCORE72_Y;
            p2StrScorePosY = P_SCORE72_Y;
            break;
    }
    
    glPushMatrix();
    
    /* Display current player scores and highlight current players turn */
    if ((getCurrentPlayer() == PLAYER_1) && (getGameMode() != GAME_END)) /* Player 1 is playing and the game hasn't finished */
    {
        glColor3f(TEXT_HIGHLIGHT_R, TEXT_HIGHLIGHT_G, TEXT_HIGHLIGHT_B);
        renderText(p1StrScorePosX, p1StrScorePosY,GLUT_BITMAP_HELVETICA_18,player1String); /* Player 1's Score Highlighted */
        glColor3f(TEXT_R, TEXT_G, TEXT_B);
        renderText(p2StrScorePosX,p2StrScorePosY,GLUT_BITMAP_HELVETICA_18,player2String); /* Player 2's Normal Text Color */
    }
    else if ((getCurrentPlayer() == PLAYER_2) && (getGameMode() != GAME_END)) /* Player 2 is playing and the game hasn't finished */
    {
        glColor3f(TEXT_R, TEXT_G, TEXT_B);
        renderText(p1StrScorePosX,p1StrScorePosY,GLUT_BITMAP_HELVETICA_18,player1String); /* Player 1's Score Normal Text Color */
        glColor3f(TEXT_HIGHLIGHT_R, TEXT_HIGHLIGHT_G, TEXT_HIGHLIGHT_B);
        renderText(p2StrScorePosX,p2StrScorePosY,GLUT_BITMAP_HELVETICA_18,player2String); /* Player 2's Score Highlighted*/
    }
    else if (getGameMode() == GAME_END) /* No ones turn. Game has finished. Display score as normal without highlights */
    {
        glColor3f(TEXT_R, TEXT_G, TEXT_B);
        renderText(P1_SCORE_X,P_SCORE32_Y,GLUT_BITMAP_HELVETICA_18,player1String); /* Player 1's Score */
        renderText(P2_SCORE_X,P_SCORE32_Y,GLUT_BITMAP_HELVETICA_18,player2String); /* Player 2's Score */
    }
    glPopMatrix();
}





/********************************************************************************************************
 * 
 *                        Button Display Function (Menus)  
 * 
 * ******************************************************************************************************/

void drawButton(button_t *button, char *buttonString)
{
    /* Determine the edges of the button */
    int leftButtonEdge = getLeftButtonEdge(button), rightButtonEdge = getRightButtonEdge(button),
        topButtonEdge = getTopButtonEdge(button), bottomButtonEdge = getBottomButtonEdge(button);
    
    int buttonState = getButtonState(button);
    
    
    /* What state is the button in? (Deciding how to colour button) */
    switch(buttonState)
    {
       case BUT_OVER:
       case BUT_PRESSED:
           glColor3f(BORDER_COLOUR_R * CARD_DIM, BORDER_COLOUR_G * CARD_DIM, BORDER_COLOUR_B * CARD_DIM);
           break;
       case BUT_OUT:
           glColor3f(BORDER_COLOUR_R, BORDER_COLOUR_G, BORDER_COLOUR_B); 
           break;
    }
    
    /* Draw border of button */
    glBegin(GL_QUADS);
    glVertex2f(leftButtonEdge, topButtonEdge); /* Top left corner */
    glVertex2f(rightButtonEdge, topButtonEdge); /* Top right corner */
    glVertex2f(rightButtonEdge, bottomButtonEdge); /* Bottom right corner */
    glVertex2f(leftButtonEdge, bottomButtonEdge); /* Bottom left corner */
    glEnd();
    
    /* What state is the button in? (Deciding how to colour button) */
    switch (buttonState)
    {
       case BUT_OUT:
           glColor3f(BUT_OUT_R, BUT_OUT_G, BUT_OUT_B);
           break;
       case BUT_OVER:
           glColor3f(BUT_OUT_R, BUT_OUT_G, BUT_OUT_B);
           break;
       case BUT_PRESSED:
           glColor3f(BUT_OUT_R * BUT_DIM, BUT_OUT_G * BUT_DIM, BUT_OUT_B * BUT_DIM);
           break;
    }
    
    /* Draw the main part of the button */
    glBegin(GL_QUADS);
    glVertex2f(leftButtonEdge + BORDER, topButtonEdge + BORDER); /* Top left corner */
    glVertex2f(rightButtonEdge - BORDER, topButtonEdge + BORDER); /* Top right corner */
    glVertex2f(rightButtonEdge - BORDER, bottomButtonEdge - BORDER); /* Bottom right corner */
    glVertex2f(leftButtonEdge + BORDER, bottomButtonEdge - BORDER); /* Bottom left corner */
    glEnd();

    renderButtonText(GLUT_BITMAP_HELVETICA_18, button, buttonString);
              
}




/********************************************************************************************************
 * 
 *                        Card Display Function (In-Game)  
 * 
 * ******************************************************************************************************/

int drawCard(card_t *card)
{
    /* Determine the edges of the card */
    int leftCardEdge = getLeftCardEdge(card), rightCardEdge = getRightCardEdge(card),
    topCardEdge = getTopCardEdge(card), bottomCardEdge = getBottomCardEdge(card);
    int cardState = getCardState(card); 
    
    int cardFlipped = 0; /* Used to determine if this current card has been flipped */
    
    
    /* What state is the card in? (Deciding how to colour card) */
    switch(cardState)
    {
        case CARD_OVER:
        case CARD_PRESSED:
            glColor3f(BORDER_COLOUR_R * CARD_DIM, BORDER_COLOUR_G * CARD_DIM, BORDER_COLOUR_B * CARD_DIM);
            break;
        case CARD_IN:
            glColor3f(CARD_IN_R, CARD_IN_G, CARD_IN_B);
            break;
        case CARD_OUT:
            glColor3f(BORDER_COLOUR_R, BORDER_COLOUR_G, BORDER_COLOUR_B); 
            break;
        case CARD_REMOVE:
            glColor3f(CARD_REMOVE_R, CARD_REMOVE_G, CARD_REMOVE_B);
    }
    
    /* Drawing the edges of the card */
    glBegin(GL_QUADS);
    glVertex2f(leftCardEdge, topCardEdge); /* Top left corner */
    glVertex2f(rightCardEdge, topCardEdge); /* Top right corner */
    glVertex2f(rightCardEdge, bottomCardEdge); /* Bottom right corner */
    glVertex2f(leftCardEdge, bottomCardEdge); /* Bottom left corner */
    glEnd();
    
    /* What state is the card in? (Deciding how to colour card) */
    switch (cardState)
    {
        case CARD_OUT:
            glColor3f(CARD_OUT_R, CARD_OUT_G, CARD_OUT_B);
            break;
        case CARD_OVER:
            glColor3f(CARD_OUT_R, CARD_OUT_G, CARD_OUT_B);
            break;
        case CARD_PRESSED:
            glColor3f(CARD_OUT_R * CARD_DIM, CARD_OUT_G * CARD_DIM, CARD_OUT_B * CARD_DIM);
            break;
        case CARD_IN:
            glColor3f(CARD_IN_R, CARD_IN_G, CARD_IN_B);
            break;
        case CARD_REMOVE:
            glColor3f(CARD_REMOVE_R, CARD_REMOVE_G, CARD_REMOVE_B); /* Same colour has background. card disappears */
    }
    
    /* Drawing the main part of the card */
    glBegin(GL_QUADS);
    glVertex2f(leftCardEdge + BORDER, topCardEdge + BORDER); /* Top left corner */
    glVertex2f(rightCardEdge - BORDER, topCardEdge + BORDER); /* Top right corner */
    glVertex2f(rightCardEdge - BORDER, bottomCardEdge - BORDER); /* Bottom right corner */
    glVertex2f(leftCardEdge + BORDER, bottomCardEdge - BORDER); /* Bottom left corner */
    glEnd();
    
    if (cardState == CARD_IN)
    {
        renderCardChar(GLUT_BITMAP_TIMES_ROMAN_24, card); 
        cardFlipped = 1; /* This card has been flipped */
    }
    
    return (cardFlipped);
}




/********************************************************************************************************
 * 
 *                        Text Rendering Functions   
 * 
 * ******************************************************************************************************/

/* Draws messages and player scores when required */ 
static void renderText(int stringPosX, int stringPosY, void *font, char *string)
{   
    char *stringChar; /* Used as a pointer to a character in string */    
    
    glRasterPos2i(stringPosX, stringPosY); /* Positioning text */
    for (stringChar = string; *stringChar != '\0'; stringChar++) /* Drawing a character at a time */
    {
        glutBitmapCharacter(font, *stringChar);
    }
}



static void renderButtonText(void *font, button_t *button, char *buttonString)
{    
    char *butStringChar; /* Used as a pointer to a character in buttonString */ 
    
    /* Figuring out where to position button text on screen */
    int stringPosX = getButtonPositionX(button) - (glutBitmapLength(font, (const unsigned char *) buttonString) / 2);
    int stringPosY = getButtonPositionY(button) + BUT_CENTRE_TEXT_Y;
    
    glColor3f(BUT_TEXT_R, BUT_TEXT_G, BUT_TEXT_B); /* Button character colour */
    glRasterPos2i(stringPosX, stringPosY); /* Positioning button text */
    
    for (butStringChar = buttonString; *butStringChar != '\0'; butStringChar++) /* Drawing a character at a time */
    {
        glutBitmapCharacter(font, *butStringChar);
    }
}




static void renderCardChar(void *font, card_t *card)
{    
    char cardChar = cardChars[getCardValue(card)]; /* Getting a card character to display */
    
    /* Figuring out where to position text on screen */
    int cardCharPosX = getCardPositionX(card) - glutBitmapWidth(font, cardChar) / 2;
    int cardCharPosY = getCardPositionY(card) + CARD_CHAR_HEIGHT / 2;
    
    
    glColor3f(CARD_TEXT_R, CARD_TEXT_G, CARD_TEXT_B); /* Card character colour */
    glRasterPos2i(cardCharPosX, cardCharPosY); /* Positioning card character */
    glutBitmapCharacter(font, cardChar); /* Draw card character */
}

