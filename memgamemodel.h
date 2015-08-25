/*
*  File: memgamemodel.h
*  Project: Assignment #1 for ENEL323 (Memory Game)
*  Brief: Framework for the game.
*  Author: Aydin Arik (aar52, 19473818)
*  Last Modified: 21/04/11
* 
*  Additional Notes: 
*   -Acts as the model (MVC) for the program.   
*/

#ifndef MODEL_H
#define MODEL_H 


/********************************************************************************************************
*                        Header Files
********************************************************************************************************/

#include "global.h"
#include "memgamecontroller.h"
#include "memgameview.h"



/********************************************************************************************************
*                        Global Constants
********************************************************************************************************/

enum gameModes{QUIT = -1, GAME_MENU = 0, GAME_END = 1, MOUSE_DISABLE = 2, NUM_CARDS_32 = 32, NUM_CARDS_72 = 72};
enum cardStates{CARD_OUT = 0, CARD_OVER, CARD_PRESSED, CARD_IN, CARD_REMOVE};
enum buttonStates{BUT_OUT = 0, BUT_OVER, BUT_PRESSED}; 
enum playerNumber{PLAYER_1 = 1, PLAYER_2};



/********************************************************************************************************
*                        Public Functions
********************************************************************************************************/

/* Game Menu Functions */
void createButtons(void);

/* In-Game Functions */
void createCards(void);

/* Mouse Callback Functions */
void mouseButtonFunc(int mouseButton, int mouseButtonState, int mousePositionX, int mousePositionY);
void mouseMotionFunc(int mousePositionX, int mousePositionY); 
void passiveMotionFunc(int mousePositionX, int mousePositionY);



/* Get and Change Functions */

/* Get/ Change Button ... */
void changeButtonState(button_t *button, int newButtonState);
int getButtonState(button_t *button);
int getButtonPositionX(button_t *button);
int getButtonPositionY(button_t *button);

/* Get/ Change Card ... */
void changeCardState(card_t *card, int newCardState);
int getCardState(card_t *card);
int getCardValue(card_t *card);
int getCardPositionX(card_t *card);
int getCardPositionY(card_t *card);

/* Get Player ... */
int getPlayer1Score(void);
int getPlayer2Score(void);
int getCurrentPlayer(void);

int getGameModeChosen(button_t *button);

/* Get Button Edges */
int getLeftButtonEdge(button_t *button);
int getRightButtonEdge(button_t *button);
int getTopButtonEdge(button_t *button);
int getBottomButtonEdge(button_t *button);
int getLeftCardEdge(card_t *card);

/* Get Card Edges */
int getRightCardEdge(card_t *card);
int getTopCardEdge(card_t *card);
int getBottomCardEdge(card_t *card);

/* Get Game Mode */
int getGameMode(void);



#endif
