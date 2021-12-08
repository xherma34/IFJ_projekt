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
//Maximalni pocet prvku ve stacku
#define MAX_STACK 100

//Stack tokenu 
typedef struct
{
    Token *stackToken[MAX_STACK];
    int topIndex;
} TStack;

/**
 * @brief Funkce co inicializuje stack -> rovnou pushuje i dno
 * @param TStack 
 */
void TStackInit(TStack *);

/**
 * @brief Funkce pro pushovani na stack
 * @param TStack
 * @param Token co chci pushovat
 * @return 0 -> uspesny push, >0 -> error
 */
int TStackPush(TStack *, Token *);

/**
 * @brief Funkce odstrani Top prvek zasobniku
 * @param TStack
 * @return 0 -> uspesny pop, >0 -> error
 */
int TStackPop(TStack *);

/**
 * @brief Funkce vraci do promenne top stacku
 * @param TStack
 * @param Token do ktereho ukladam stackTop
 */
void TStackTop(TStack *, Token *);

/**
 * @brief Funkce vraci do promenne top, ktery neni E||stopSign
 * @param TStack
 * @param Token do ktereho ukladam stackTop
 * @return TStackItem top zasobniku
 */
void TStackTopNotE(TStack *, Token *);

/**
 * @brief Funkce vraci pocet prvku mezi stackTopem a prvnim Stop SIgnem ve stacku
 * @param TStack
 * @return Pocet prvku || -1 -> error
 */
int TStackUntillStopSign(TStack *);

/**
 * @brief Pomocna funkce pro shiftovani -> prida prvek pod top node zasobniku
 * @param TStack
 * @param Token co chci pushovat
 * @return 0 -> uspesny push, 1 -> error
 */
int TStackPushAfterTop(TStack *, Token *);

/**
 * @brief Funkce pro vlozeni stop symbolu na stack
 * @param TStack
 * @return 0 -> uspesni push, 1 -> error
 */
int TStackPushStop(TStack *);

/**
 * @brief funkce pro uvloneni pameti pro I_E, I_STOP, I_DOLLAR
 * @param TStack
 */
void freeStackE(TStack *);
