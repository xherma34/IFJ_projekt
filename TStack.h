/*
* Project: IFJ 2021
*
* @file TStack.h
*
* @brief Knihovna pro TStack.c
*
* @author Pavel Heřmann (xherma34)
* @author Maxim Plička (xplick04)
*/

#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK 100

typedef struct
{
    Token *stackToken[MAX_STACK];
    int topIndex;
} TStack;

/**
 * @brief Funkce co inicializuje stack
 *
 */
void TStackInit(TStack *);

/**
 * @brief Funkce pro pushovani na stack
 *
 * @return int, pokud dojde k erroru pri pushovani vraci 1
 */
int TStackPush(TStack *, Token *);

/**
 * @brief Funkce odstrani Top prvek zasobniku
 *
 * @return int
 */
int TStackPop(TStack *);

/**
 * @brief Funkce vraci do promenne
 *
 * @return TStackItem
 */
void TStackTop(TStack *, Token *);

/**
 * @brief Funkce vraci do promenne top, ktery neni E
 *
 * @return TStackItem top zasobniku
 */
void TStackTopNotE(TStack *, Token *);

/**
 * @brief Funkce vraci pocet itemu mezi stackTopem a prvnim Stop SIgnem ve stacku
 *
 * @return int, pokud error => -1
 */
int TStackUntillStopSign(TStack *);

/**
 * @brief Pomocna funkce pro shiftovani -> prida prvek pod top node zasobniku
 *
 */
int TStackPushAfterTop(TStack *, Token *);

/**
 * @brief Funkce pro vlozeni stop symbolu na stack
 *
 */
int TStackPushStop(TStack *);

/**
 * @brief funkce pro uvloneni pameti pro I_E, I_STOP, I_DOLLAR
 *
 */
void freeStackE(TStack *);
