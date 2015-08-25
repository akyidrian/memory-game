/*
*  File: memgamemodel.c
*  Project: Assignment #1 for ENEL323 (Memory Game)
*  Brief: Framework for the game.
*  Author: Aydin Arik (aar52, 19473818)
*  Last Modified: 21/04/11
* 
*  Additional Notes: 
*   -Acts as the model (MVC) for the program.   
*/

/********************************************************************************************************
*                        Header Files
********************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>
#include "memgamemodel.h"



/********************************************************************************************************
*                        Private Constants
********************************************************************************************************/

enum boolean{FALSE = 0, TRUE};
enum endGameButtons{NO = 0, YES};
enum initialPlayerScores{STARTING_P1_SCORE = 0, STARTING_P2_SCORE = 0};
enum cardPositioning{MAX_SHIFT_LEFT_32 = -3, MAX_SHIFT_RIGHT_32 = 4, MAX_SHIFT_UP_32 = -1, 
MAX_SHIFT_DOWN_32 = 2, MAX_SHIFT_LEFT_72 = -5, MAX_SHIFT_RIGHT_72 = 6, MAX_SHIFT_UP_72 = -2, 
MAX_SHIFT_DOWN_72 = 3};

#define NUM_BUTTONS 2
#define MOUSE_NOT_IN_BUTTON -1
#define NUM_CARDS_IN_A_PAIR 2
#define MOUSE_NOT_IN_CARD -1

/* Wait times after two cards are flipped */
#define CARD_SUCCESS_TIME_WAIT 1000
#define CARD_FAILURE_TIME_WAIT 2000

/* Button Texts Used */
#define GAME_MODE_32 "32 Cards"
#define GAME_MODE_72 "72 Cards"
#define GAME_END_OPTION_Y "YES"
#define GAME_END_OPTION_N "No, I want to quit."



/********************************************************************************************************
*                        Private Type Definitions
********************************************************************************************************/

typedef struct 
{
    int CardValue;
    int CardId;
} flippedCard_t;

typedef struct
{
    int currentPlayer;
    int player1Score;
    int player2Score;
} players_t;



/********************************************************************************************************
*                        Private Functions
********************************************************************************************************/

/* Game Menu Functions */
static void createMainMenuButtonSet(void);
static void createEndGameButtonSet(void);

/* Setting A Game Up... */
static void beginGame(void); 
static card_t *createCardSet(void);
static card_t createCardAttributes(int centreX, int centreY, int moveCardX, int moveCardY, int cardIndex); 
static int generateCardValue(int currentCardValue, int currentCardIndex);
static int findRandomCardValue(void);
static players_t *createPlayersGameData(void);

/* In-Game Functions */
static void updateGame(flippedCard_t *flippedCards, int numCardsFlipped);
static void getFlippedCardsAttributes(flippedCard_t *flippedCards);
static void correctCardDelayFunc(int currentGameMode);
static void increaseCurrentPlayersScore(void);
static void wrongCardDelayFunc(int currentGameMode);
static void changeCurrentPlayersTurn(void);

/* End-Game Functions */
static int noCardsRemaining(void);

/* Mouse Related Functions */
static void mouseClickOnButtons(int mouseButton, int mouseButtonState, int mousePositionX, int mousePositionY);
static void mouseClickOnCards(int mouseButton, int mouseButtonState, int mousePositionX, int mousePositionY);
static void mouseMotionWithButtons(int mousePositionX, int mousePositionY);
static void mouseMotionWithCards(int mousePositionX, int mousePositionY);
static int mouseInAnyButton(int mousePositionX, int mousePositionY);
static int mouseInButton(int mousePositionX, int mousePositionY, button_t *button);
static int mouseInAnyCard(int mousePositionX, int mousePositionY);
static int mouseInCard(int mousePositionX, int mousePositionY, card_t *card);



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




/********************************************************************************************************
*                        Global to File Variables
********************************************************************************************************/

/* cardMatrix is NULL before the game starts (in main menu). Once gameMode is set to a 'go into game' value, 
 * cardMatrix points to the first card structure in the array of card structures; assuming a successful malloc 
 * call. The number of card structures created in memory depends on the game mode (gameMode) chosen. */
static card_t *cardMatrix = NULL;

/* gameMode steers the program code and stores the number of cards chosen in the main menu. 
 * Possible states are GAME_MENU, GAME_END, MOUSE_DISABLE, NUM_CARDS_32, NUM_CARDS_72. */
static int gameMode = GAME_MENU;

/* playersGameData is NULL before the game starts (in main menu). playersGameData points to a structure when successfully 
 * allocated in memory (malloc). The structure contains information on the current players turn and players scores. */
static players_t *playersGameData = NULL;

/* Used to implementing the main menu and end-game buttons. These buttons are required when user input is needed to decide
 * the course of the program/ game. */
static button_t buttonMatrix[NUM_BUTTONS]; 




/********************************************************************************************************
 * 
 *                       Game Menu Functions
 * 
 * ******************************************************************************************************/

void createButtons(void)
{
    int buttonIndex; 
    char *buttonString[NUM_BUTTONS];
    
    /* Setting up buttons attributes and button strings depending on current gameMode */
    if (gameMode == GAME_MENU)
    {
        buttonString[0] = GAME_MODE_32;
        buttonString[1] = GAME_MODE_72;
        createMainMenuButtonSet();      
    }
    else if (gameMode == GAME_END)
    {
        buttonString[0] = GAME_END_OPTION_Y;
        buttonString[1] = GAME_END_OPTION_N;
        createEndGameButtonSet();   
    }
        
    /* Draw all buttons */
    for (buttonIndex = 0; buttonIndex < NUM_BUTTONS; buttonIndex++) 
    {  
       drawButton(&buttonMatrix[buttonIndex], buttonString[buttonIndex]);
    }
}

/* buttonMatrix is only initialised once for current gameMode */
static void createMainMenuButtonSet(void)
{    
    if (buttonMatrix[0].buttonValue != NUM_CARDS_32) 
    {
        buttonMatrix[0].buttonPositionX = BUT_CENTRE_X - (BUT_WIDTH + BUT_GAP) / 2;
        buttonMatrix[0].buttonPositionY = BUT_CENTRE_Y;
        buttonMatrix[0].buttonState = BUT_OUT;
        buttonMatrix[0].buttonValue = NUM_CARDS_32;
    }
    
    if (buttonMatrix[1].buttonValue != NUM_CARDS_72)
    {
        buttonMatrix[1].buttonPositionX = BUT_CENTRE_X + (BUT_WIDTH + BUT_GAP) / 2;
        buttonMatrix[1].buttonPositionY = BUT_CENTRE_Y;
        buttonMatrix[1].buttonState = BUT_OUT;
        buttonMatrix[1].buttonValue = NUM_CARDS_72;
    }
}


/* buttonMatrix is only initialised once for current gameMode */
static void createEndGameButtonSet(void)
{
    if (buttonMatrix[0].buttonValue != YES) 
    {
        buttonMatrix[0].buttonPositionX = BUT_CENTRE_X - (BUT_WIDTH + BUT_GAP) / 2;
        buttonMatrix[0].buttonPositionY = BUT_CENTRE_Y;
        buttonMatrix[0].buttonState = BUT_OUT;
        buttonMatrix[0].buttonValue = YES;
    }
    
    if (buttonMatrix[1].buttonValue != NO)
    {
        buttonMatrix[1].buttonPositionX = BUT_CENTRE_X + (BUT_WIDTH + BUT_GAP) / 2;
        buttonMatrix[1].buttonPositionY = BUT_CENTRE_Y;
        buttonMatrix[1].buttonState = BUT_OUT;
        buttonMatrix[1].buttonValue = NO;
    }
}





/********************************************************************************************************
 * 
 *                       Setting A Game Up...
 * 
 * ******************************************************************************************************/

/* Executed only once in any one game */
static void beginGame(void)
{
    if ((createCardSet() == NULL) || (createPlayersGameData() == NULL)) /* Making sure a card set and player data is placed in memory */
    {
        fprintf (stderr, "Memory allocation error. Unable to run game.");
        exit(EXIT_FAILURE); 
    }
}


static card_t *createCardSet(void) 
{
    int cardIndex = 0;
    int moveCardX, moveCardY; /* Aids in positioning cards. Provides a magnitude and direction to a shift */
    int numCards = gameMode;
    int maxCardShiftLeft, maxCardShiftRight, maxCardShiftUp, maxCardShiftDown; /* Depend on gameMode. Whether we're playing with 32 or 72 cards */
    
    srand((unsigned)time(NULL)); /* Setting system clock as the seed. Used to 'shuffle' the cards in the set */
    
    /* Deciding on maximum permissable shifts in the x and y directions from the centre of the screen */
    switch (gameMode) 
    {
        case NUM_CARDS_32:
            maxCardShiftLeft = MAX_SHIFT_LEFT_32;
            maxCardShiftRight = MAX_SHIFT_RIGHT_32;
            maxCardShiftUp = MAX_SHIFT_UP_32;
            maxCardShiftDown = MAX_SHIFT_DOWN_32;
            break;
        case NUM_CARDS_72:
            maxCardShiftLeft = MAX_SHIFT_LEFT_72;
            maxCardShiftRight = MAX_SHIFT_RIGHT_72;
            maxCardShiftUp = MAX_SHIFT_UP_72;
            maxCardShiftDown = MAX_SHIFT_DOWN_72;
            
            break;
    }
            
    cardMatrix = (card_t *) malloc (numCards * sizeof(card_t)); /* Allocating memory to the heap for the cards */
    
    
    /* Creating cards */
    if (cardMatrix != NULL) /* If successfull in making space for cards in memory. Give cards meaning; position, state, value, id */
    {
        for (moveCardY = maxCardShiftUp; moveCardY <= maxCardShiftDown; moveCardY++) /* Positioning a row of cards */
        {
            for (moveCardX = maxCardShiftLeft; moveCardX <= maxCardShiftRight; moveCardX++) /* Positioning cards along a row */
            {
                cardMatrix[cardIndex] = createCardAttributes(CARD_CENTRE_X, CARD_CENTRE_Y, moveCardX, moveCardY, cardIndex);
               
                cardIndex++; /* Next card to be made */
                
            }
            moveCardX = maxCardShiftLeft; /* Resetting moveCardX for the first card in the next row */
        }
    }
    return cardMatrix;
}


static card_t createCardAttributes(int centreX, int centreY, int moveCardX, int moveCardY, int cardIndex) 
{
   card_t card;
    
   /* Deciding on the cards position */
   card.cardPositionX = centreX + moveCardX * (CARD_WIDTH + CARD_GAP) - ((CARD_WIDTH + CARD_GAP) / 2);
   card.cardPositionY = centreY + moveCardY * (CARD_HEIGHT + CARD_GAP) - ((CARD_HEIGHT + CARD_GAP) / 2);
   
   card.cardState = CARD_OUT; 
   card.cardValue = generateCardValue(card.cardValue, cardIndex); /* Generated value is peudo-random */
   card.cardId = cardIndex; /* Required for comparing flipped cards later on */

   return card; 
}


static int generateCardValue(int currentCardValue, int currentCardIndex)
{
    int prevCardIndex; /* Required to look at already assigned cardValues */
    int cardPair = FALSE; /* Initialised to FALSE to correctly check if a card pair already existed */
    int redoCheck = FALSE; /* Ensures only pairs of cards are created and displayed */

    currentCardValue = findRandomCardValue(); /* Find a random value */
    
    /* Required to ensure cards exist in pairs only */
    do 
    {
        redoCheck = FALSE; /* Resets value in the event random generator fails to provide a valid random number */
        
        for (prevCardIndex = (currentCardIndex - 1); prevCardIndex > -1; prevCardIndex--) /* Decrements through card which have already been assigned values */
        {
            if ((currentCardValue == cardMatrix[prevCardIndex].cardValue) && (cardPair == FALSE)) /* Another card with same value and no card pair found yet */
            {
                cardPair = TRUE;
            }
            else if ((currentCardValue == cardMatrix[prevCardIndex].cardValue) && (cardPair == TRUE)) /* Another card with same value and card pair found */
            {
                currentCardValue = findRandomCardValue(); /* Get another random value */
                cardPair = FALSE; /* Reset to no card pair found */
                redoCheck = TRUE; /* Need to check if new value matches at most another cards value */
                break;
            }
        }
    }while (redoCheck == TRUE); /* Retry if card pairs already exist for a given cardValue */
    
    return currentCardValue;
}


static int findRandomCardValue(void)
{
    int randomNumber;
    
    randomNumber = (int) ((float) (gameMode / 2) * (rand() / (RAND_MAX + 1.0))); /* Generating pseudo-random number */
    
    return randomNumber;
}


/* Function sets up the required in-game player related data */
static players_t *createPlayersGameData(void)
{
    playersGameData = (players_t *) malloc (sizeof(players_t));
    
    playersGameData->currentPlayer = PLAYER_1; /* First turn is assigned to Player 1 */
    
    /* Initialising player scores to zero */
    playersGameData->player1Score = STARTING_P1_SCORE;
    playersGameData->player2Score = STARTING_P2_SCORE;
    
    return playersGameData;
}





/********************************************************************************************************
 * 
 *                   In-Game Functions
 * 
 * ******************************************************************************************************/

void createCards(void)
{
    int cardIndex;
    int numCards = gameMode;
    int cardFlipped = FALSE; /* Initialised to FALSE so that first card can be correctly checked to be flipped or not */
    int numCardsFlipped = 0; 
    flippedCard_t *flippedCards = (flippedCard_t *) malloc (NUM_CARDS_IN_A_PAIR * sizeof(flippedCard_t));
    
    /* Draw all cards */
    for (cardIndex = 0; cardIndex < numCards; cardIndex++)
    {
      cardFlipped = drawCard(&cardMatrix[cardIndex]);
      
      if (cardFlipped == TRUE) 
          numCardsFlipped++; 
      
      cardFlipped = FALSE; /* Reset cardFlipped so the next card can be checked */
    }
    
    updateGame(flippedCards, numCardsFlipped); /* Updates game after two card are flipped */

    free(flippedCards);
}


static void updateGame(flippedCard_t *flippedCards, int numCardsFlipped)
{
    int currentGameMode = gameMode;
    
    switch (numCardsFlipped)
    {
        case 0:
        case 1: 
            break; 
        case 2:
            getFlippedCardsAttributes(flippedCards);
            
            if (flippedCards[0].CardValue == flippedCards[1].CardValue) /* Are the two flipped cards the same value? */
            { 
                cardMatrix[flippedCards[0].CardId].cardState = CARD_REMOVE; 
                cardMatrix[flippedCards[1].CardId].cardState = CARD_REMOVE;
                gameMode = MOUSE_DISABLE; 
                glutTimerFunc(CARD_SUCCESS_TIME_WAIT, correctCardDelayFunc, currentGameMode); /* Freezes game for a second */ 
            }
            else /* Failure to choose the same card */
            {                     
                cardMatrix[flippedCards[0].CardId].cardState = CARD_OUT;
                cardMatrix[flippedCards[1].CardId].cardState = CARD_OUT;
                gameMode = MOUSE_DISABLE;
                glutTimerFunc(CARD_FAILURE_TIME_WAIT, wrongCardDelayFunc, currentGameMode); /* Freezes game for two second */ 
            }
            break;
    }
}

static void getFlippedCardsAttributes(flippedCard_t *flippedCards)
{
    int flippedIndex = 0; /* used in junction with the flippedCards struct so that a flipped card cardValues and cardIds can be stored correctly in the struct */
    int cardIndex = 0;
    int numCards = gameMode;
    
    while (cardIndex < numCards)
    {
        if (cardMatrix[cardIndex].cardState == CARD_IN)
        {
            flippedCards[flippedIndex].CardValue = cardMatrix[cardIndex].cardValue;
            flippedCards[flippedIndex].CardId = cardMatrix[cardIndex].cardId;
            flippedIndex++;
        }
        cardIndex++;
    }
}

static void correctCardDelayFunc(int currentGameMode)
{ 
    /* Score changes and current player remains unchanged */
    increaseCurrentPlayersScore();
   
   gameMode = currentGameMode;
   
   if (noCardsRemaining() == TRUE)
       gameMode = GAME_END;
   
   refreshDisplay();
}

static void increaseCurrentPlayersScore(void)
{
    if (getCurrentPlayer() == PLAYER_1) 
   {
       (playersGameData->player1Score)++;
   }
   else /* it's Player 2 that choose correctly */
   {
       (playersGameData->player2Score)++;
   }
}


void wrongCardDelayFunc(int currentGameMode)
{ 
    /* Players changes but scores stay the same */
    changeCurrentPlayersTurn();

   gameMode = currentGameMode;
   refreshDisplay();
}

void changeCurrentPlayersTurn(void)
{
    if (getCurrentPlayer() == PLAYER_1)
   {
       playersGameData->currentPlayer = PLAYER_2;
   }
   else /* it WAS Player 2's turn, now it isn't */
   {
       playersGameData->currentPlayer = PLAYER_1;
   }    
}




/********************************************************************************************************
 * 
 *                   End-Game Functions
 * 
 * ******************************************************************************************************/

int noCardsRemaining(void)
{
    int numCardsRemoved = 0;
    int cardIndex = 0;
    
    while (cardIndex < gameMode)
    {
        if (cardMatrix[cardIndex].cardState == CARD_REMOVE)
        {
            numCardsRemoved++;
        }
        else /* A card is not removed, so not all cards are removed */
        {
            break;
        }
        cardIndex++;
    }
    return (numCardsRemoved == gameMode); /* If 1: all cards have been removed. If 0: some cards are still on screen */
}





/********************************************************************************************************
 * 
 *                       Mouse Callback Functions
 * 
 * ******************************************************************************************************/

/* GLUT call-back function which handles only mouse clicks */
void mouseButtonFunc(int mouseButton, int mouseButtonState, int mousePositionX, int mousePositionY) 
{
    
    if ((gameMode == GAME_MENU) || (gameMode == GAME_END))
    {
        mouseClickOnButtons(mouseButton, mouseButtonState, mousePositionX, mousePositionY);
        
    }
    else if ((gameMode == NUM_CARDS_32) || (gameMode == NUM_CARDS_72))
    {
        mouseClickOnCards(mouseButton, mouseButtonState, mousePositionX, mousePositionY);
    }
    else if (gameMode == MOUSE_DISABLE)
    {
        /* Do nothing...*/
    }
}

/* GLUT call-back function which handles both mouse movements and mouse clicks together */
void mouseMotionFunc(int mousePositionX, int mousePositionY) 
{
    if ((gameMode == GAME_MENU) || (gameMode == GAME_END))
    {
        mouseMotionWithButtons(mousePositionX, mousePositionY);
    }
    else if ((gameMode == NUM_CARDS_32) || (gameMode == NUM_CARDS_72))
    {
        mouseMotionWithCards(mousePositionX, mousePositionY);
    }
    else if (gameMode == MOUSE_DISABLE)
    {
        /* Do nothing...*/
    }
}

/* GLUT call-back function which handles only mouse movements */
void passiveMotionFunc(int mousePositionX, int mousePositionY)
/* Handles situations where a mouse button(s) is not pressed before entering the area of a card. Mouse cursor is just dragged on card */
{
    mouseMotionFunc(mousePositionX, mousePositionY); 
}





/********************************************************************************************************
 * 
 *                       Mouse Related Functions
 * 
 * ******************************************************************************************************/

static void mouseClickOnButtons(int mouseButton, int mouseButtonState, int mousePositionX, int mousePositionY)
{
    int buttonIndex;
    button_t *button;
    
    /* Is mouse click with left mouse button and within a displayed button? */
    if ( ((buttonIndex = mouseInAnyButton(mousePositionX, mousePositionY)) > MOUSE_NOT_IN_BUTTON) && (mouseButton == GLUT_LEFT_BUTTON) ) 
    {
        button = &buttonMatrix[buttonIndex]; 
        
        /* Change the buttonState of the 'physical' button that was clicked on */ 
        switch (getButtonState(button))
        {
            case BUT_OUT:
            case BUT_OVER:
                if (mouseButtonState == GLUT_DOWN) /* Mouse button is held down */
                {
                    changeButtonState(button, BUT_PRESSED);                       
                    refreshDisplay(); 
                }
                break;
            case BUT_PRESSED:
                if (mouseButtonState == GLUT_UP) /* Mouse button has been released */
                {
                    changeButtonState(button, BUT_OUT); 
                    
                    if (getGameModeChosen(button) == NUM_CARDS_32)
                    {
                        gameMode = NUM_CARDS_32;
                        beginGame(); /* Creates cards and in-game player related data */
                    }
                    else if (getGameModeChosen(button) == NUM_CARDS_72)
                    {
                        gameMode = NUM_CARDS_72;
                        beginGame(); /* Creates cards and players data */
                    }
                    else if (getGameModeChosen(button)  == YES)
                    {
                        gameMode = GAME_MENU;
                        free(cardMatrix);
                        free(playersGameData);
                    }
                    else if (getGameModeChosen(button)  == NO)
                    {
                        exit(EXIT_SUCCESS);
                    }
                    
                    refreshDisplay(); 
                }
                break;
        }
    }
}

static void mouseClickOnCards(int mouseButton, int mouseButtonState, int mousePositionX, int mousePositionY)
{
    int cardIndex;
    card_t *card;
    
    /* Is mouse click with left mouse button and within a displayed card? */
    if ( ((cardIndex = mouseInAnyCard(mousePositionX, mousePositionY)) > MOUSE_NOT_IN_CARD) && (mouseButton == GLUT_LEFT_BUTTON) )
    {
        card = &cardMatrix[cardIndex];
        
        /* Change the cardState of the 'physical' card that was clicked on */ 
        switch (getCardState(card))
        {
            case CARD_OUT:
            case CARD_OVER:
                if (mouseButtonState == GLUT_DOWN) /* Mouse button is held down */
                {
                    changeCardState(card, CARD_PRESSED);
                    refreshDisplay(); 
                }
                break;
            case CARD_PRESSED:
                if (mouseButtonState == GLUT_UP) /* Mouse button has been released */
                {
                    changeCardState(card, CARD_IN);
                    refreshDisplay(); 
                }
                break;
        }
    }
}

/* Functions for mouse motion actions acting on 'physical' buttons */
static void mouseMotionWithButtons(int mousePositionX, int mousePositionY)
{
    int buttonIndex;
    button_t *button;
    int resetButtonIndex; /* Variable required to reset all previously changed button buttonStates back to the default BUT_OUT */
    
    /* Is mouse cursor within a button? */
    if ((buttonIndex = mouseInAnyButton(mousePositionX, mousePositionY)) > MOUSE_NOT_IN_BUTTON)
    {
        button = &buttonMatrix[buttonIndex];
        switch (getButtonState(button))
        {
            case BUT_OUT:
                changeButtonState(button, BUT_OVER);
                refreshDisplay();    
                break;
        }
        
        for (resetButtonIndex = 0; resetButtonIndex < NUM_BUTTONS; resetButtonIndex++) /* Removes a multiple button highlight glitch which occur with rapid cursor movements */
                if (((buttonMatrix[resetButtonIndex].buttonState == BUT_OVER) || (buttonMatrix[resetButtonIndex].buttonState == BUT_PRESSED)) 
                    && (resetButtonIndex != buttonIndex))
                {
                    changeButtonState(&buttonMatrix[resetButtonIndex], BUT_OUT);
                    refreshDisplay();    
                }           
                
    }
    else  /* Mouse cursor outside all buttons. We must ensure all buttons have state BUT_OUT */
        { 
            for (buttonIndex = 0; buttonIndex < NUM_BUTTONS; buttonIndex++)
                if ((buttonMatrix[buttonIndex].buttonState == BUT_OVER) || (buttonMatrix[buttonIndex].buttonState == BUT_PRESSED))
                {
                    changeButtonState(&buttonMatrix[buttonIndex], BUT_OUT);
                    refreshDisplay();    
                }
        }
}

/* Functions for mouse motion actions acting on cards */
static void mouseMotionWithCards(int mousePositionX, int mousePositionY)
{
    int cardIndex;
    int numCards = gameMode;
    card_t *card;
    int resetCardIndex; /* Variable required to reset all previously changed card cardStates back to the default CARD_OUT */
    
    /* Is mouse cursor within a card? */
    if ((cardIndex = mouseInAnyCard(mousePositionX, mousePositionY)) > MOUSE_NOT_IN_CARD)
    {
        card = &cardMatrix[cardIndex];
        switch (getCardState(card))
        {
            case CARD_OUT:
                changeCardState(card, CARD_OVER);
                refreshDisplay();    
                break;
        }
        
        for (resetCardIndex = 0; resetCardIndex < gameMode; resetCardIndex++) /* Removes a multiple card highlight glitch which occur with rapid cursor movements */
                if (((cardMatrix[resetCardIndex].cardState == CARD_OVER) || (cardMatrix[resetCardIndex].cardState == CARD_PRESSED)) 
                    && (resetCardIndex != cardIndex))
                {
                    changeCardState(&cardMatrix[resetCardIndex], CARD_OUT);
                    refreshDisplay();    
                }         
                
    }
    else  /* Mouse cursor outside all cards. We must ensure all cards have state CARDS_OUT */
        { 
            for (cardIndex = 0; cardIndex < numCards; cardIndex++)
                if ((cardMatrix[cardIndex].cardState == CARD_OVER) || (cardMatrix[cardIndex].cardState == CARD_PRESSED))
                {
                    changeCardState(&cardMatrix[cardIndex], CARD_OUT);
                    refreshDisplay();    
                }
        }
}

/* Determines if any buttons had an left mouse button action act upon it */
static int mouseInAnyButton(int mousePositionX, int mousePositionY)
{
   int buttonIndex;
   
   for (buttonIndex = 0; buttonIndex < NUM_BUTTONS; buttonIndex++)
   {
      if (mouseInButton(mousePositionX, mousePositionY, &buttonMatrix[buttonIndex]))
         return buttonIndex;
   }
   return -1;
}

/* Checking if left mouse button action occurs within a particular button */
static int mouseInButton(int mousePositionX, int mousePositionY, button_t *button)
{
   int leftButtonEdge = getLeftButtonEdge(button), rightButtonEdge = getRightButtonEdge(button),
       topButtonEdge = getTopButtonEdge(button), bottomButtonEdge = getBottomButtonEdge(button);
       
   return (mousePositionX >= leftButtonEdge && mousePositionX <= rightButtonEdge
        && mousePositionY >= topButtonEdge  && mousePositionY <= bottomButtonEdge);
}

/* Determines if any cards had an left mouse button action act upon it */
static int mouseInAnyCard(int mousePositionX, int mousePositionY)
{
   int cardIndex;
   int numCards = gameMode;
   
   for (cardIndex = 0; cardIndex < numCards; cardIndex++)
   {
      if (mouseInCard(mousePositionX, mousePositionY, &cardMatrix[cardIndex]))
         return cardIndex;
   }
   return -1;
}

/* Checking if left mouse button action occurs within a particular card */
static int mouseInCard(int mousePositionX, int mousePositionY, card_t *card)
{
   int leftCardEdge = getLeftCardEdge(card), rightCardEdge = getRightCardEdge(card),
       topCardEdge = getTopCardEdge(card), bottomCardEdge = getBottomCardEdge(card);
       
   return (mousePositionX >= leftCardEdge && mousePositionX <= rightCardEdge
        && mousePositionY >= topCardEdge  && mousePositionY <= bottomCardEdge);
}






/********************************************************************************************************
 * 
 *                       Get and Change Functions
 * 
 * ******************************************************************************************************/

void changeButtonState(button_t *button, int newButtonState)
{
   button->buttonState = newButtonState;
}



int getButtonState(button_t *button)
{
   return button->buttonState;
}



int getButtonPositionX(button_t *button)
{
    return button->buttonPositionX;
}



int getButtonPositionY(button_t *button)
{
    return button->buttonPositionY;
}



void changeCardState(card_t *card, int newCardState)
{
   card->cardState = newCardState;
}



int getCardState(card_t *card)
{
   return card->cardState;
}



int getCardValue(card_t *card)
{
   return card->cardValue;
}



int getCardPositionX(card_t *card)
{
   return card->cardPositionX;
}



int getCardPositionY(card_t *card)
{
   return card->cardPositionY;
}



int getPlayer1Score(void)
{
    return playersGameData->player1Score;
}



int getPlayer2Score(void)
{
    return playersGameData->player2Score;
}



int getCurrentPlayer(void)
{
    return playersGameData->currentPlayer;
}



int getGameModeChosen(button_t *button)
{
    return button->buttonValue;
}



int getLeftButtonEdge(button_t *button)
{
    return (button->buttonPositionX - BUT_WIDTH / 2);
}



int getRightButtonEdge(button_t *button)
{
    return (button->buttonPositionX + BUT_WIDTH / 2);
}



int getTopButtonEdge(button_t *button)
{
    return (button->buttonPositionY - BUT_HEIGHT / 2);
}



int getBottomButtonEdge(button_t *button)
{
    return (button->buttonPositionY + BUT_HEIGHT / 2);
}



int getLeftCardEdge(card_t *card)
{
    return (card->cardPositionX - CARD_WIDTH / 2);
}



int getRightCardEdge(card_t *card)
{
    return (card->cardPositionX + CARD_WIDTH / 2);
}



int getTopCardEdge(card_t *card)
{
    return (card->cardPositionY - CARD_HEIGHT / 2);
}



int getBottomCardEdge(card_t *card)
{
    return (card->cardPositionY + CARD_HEIGHT / 2);
}

int getGameMode(void)
{
    return gameMode;
}
        
