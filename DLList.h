/*
* Project: IFJ 2021
*
* @file DLList.h
*
* @brief Knihovna pro DLList.c
*
* @author Jan Zdeněk (xzdene01)
* @author Pavel Heřmann (xherma34)
* @author Maxim Plička (xplick04)
*/

#include<stdio.h>
#include<stdlib.h>


typedef struct DLLnode{
	char data;
	struct DLLnode *prev;
	struct DLLnode *next;
} *DLLnodePtr;

typedef struct {
    int length;
	DLLnodePtr firstElement;
	DLLnodePtr lastElement;
} DLList;

/**
 * @brief Funkce na inicializaci seznamu
 * @param Seznam
 */
void DLL_init (DLList *);

/**
 * @brief Funkce na zniceni celeho seznamu
 * @param Seznam
 */
void DLL_dispose(DLList *);

/**
 * @brief Funkce umisti prvek node na posledni pozici seznamu
 * @param Seznam seznam do ktereho vkladam
 * @param Prvek vkladany prvek
 */
int DLL_insertLast(DLList *, char);

/**
 * @brief Funkce smaze prvni prvek v seznamu
 * @param Seznam
 */
void DLL_deleteFirst(DLList *);

/**
 * @brief Funkce smaze posledni prvek v seznamu
 * @param Seznam ze ktereho mazeme
 */
void DLL_deleteLast(DLList *);

/**
 * @brief Funkce vraci CHAR hodnotu z charoveho seznamu
 * @param Seznam
 * @param Char hodnota do ktere vracim
 * @return CHAR hodnotu
 */
void DLL_getFirst(DLList *, char *);

/**
 * @brief Errorove hlaseni
 */
void DLL_error();
