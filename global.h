/*
*  File: global.h
*  Project: Assignment #1 for ENEL323 (Memory Game)
*  Brief: Contains typedefs used by the memgamemodel and memgameview modules.
*  Author: Aydin Arik (aar52, 19473818)
*  Last Modified: 21/04/11
* 
*  Additional Notes: -
*/

#ifndef GLOBAL_H
#define GLOBAL_H 

/********************************************************************************************************
*                        Type Definitions
********************************************************************************************************/
typedef struct
{       
   int cardPositionX;
   int cardPositionY;
   int cardState;
   int cardValue;
   int cardId;
} card_t;

typedef struct
{
   int buttonPositionX;
   int buttonPositionY;
   int buttonState;
   int buttonValue;
} button_t;


#endif
