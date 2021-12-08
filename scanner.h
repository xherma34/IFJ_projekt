/*
* Project: IFJ 2021
*
* @file scanner.h
*
* @brief Knihovna pro scanner.c
*
* @author Jan Zdeněk (xzdene01)
* @author Pavel Heřmann (xherma34)
* @author Maxim Plička (xplick04)
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#include "DLList.h"

typedef enum
{
    //Identifikator
    T_ID,

    //Klicova slova
    T_KW_DO,
    T_KW_ELSE,
    T_KW_END,
    T_KW_FUNCTION,
    T_KW_GLOBAL,
    T_KW_IF,
    T_KW_LOCAL,
    T_KW_NIL,
    T_KW_INTEGER,
    T_KW_NUMBER,
    T_KW_REQUIRE,
    T_KW_RETURN,
    T_KW_STRING,
    T_KW_THEN,
    T_KW_WHILE,

    //Operatory
    T_STRLEN,           //  #
    T_ADD,              //  +
    T_SUB,              //  -
    T_MUL,              //  *
    T_DIV_NUMBER,       //  /   -> NORMALNI DELENI
    T_DIV_INTEGER,      //  //  -> CELOCISELNE DELENI
    T_CONCATENATION,

    //Relacni operatory
    T_LT,               //  <
    T_GT,               //  >
    T_LET,              //  <=
    T_GET,              //  >=
    T_EQ,               //  ==
    T_NEQ,              //  ~=

    //Zbytek
    T_EOF,              //End Of File (konec nacitaneho dokumentu)
    T_EOL,
    T_NUM_INTEGER,
    T_NUM_NUMBER,
    T_BRACKET_RIGHT,
    T_BRACKET_LEFT,
    T_COLON,
    T_STRING,
    T_SETVALUE,         //  = -> prirazeni hodnoty
    T_EMPTY,
    T_COMMA,            //TODO to FSM
} Token_type;

typedef union
{
    int integer;
    double number;
    char *string;
} Token_value;

typedef enum
{
	I_ADD_SUB,
	I_MUL_DIV,
	I_REL_OPERATOR,
	I_CONCATENATION,
	I_STRLEN,
	I_L_BRACKET,
	I_R_BRACKET,
	I_ID,
	I_DOLLAR,
	I_E,
	I_STOP
} PTindex;


typedef struct
{
    Token_type type;
    Token_value value;
    int line;
	PTindex PTindex;
    bool ID;
} Token;

typedef struct TNode
{
    Token  token;
    struct TNode *prev;
    struct TNode *next;
    int index;
} *TNodePtr;

typedef struct
{
    TNodePtr first;
    TNodePtr last;
    TNodePtr active;
} TList;

/**
 * @brief Funkce pro ziskani tokenu ze stdin
 * @param Token do ktereho chceme ulozit typ a hodnotu
 * @return Integer, pokud =0 -> vse probehlo bez chyb, jinak vraci hodnotu nastale chyby
 */
int ScannerGetToken (Token *);


/**
 * @brief Funkce pro identifikaci klicoveho slova nebo identifikatoru
 * @param DLList z ktereho se identifikuje
 * @return Identifikovany token vcetne typu a hodnoty
 */
Token IdentifyKW(DLList *);

/**
 * @brief Funkce pro ziskani stringu ze seznamu
 * @param DLList ktery obsahuje dany string
 * @return Token vcetne typu a hodnoty
 */
Token GetString(DLList *);

/**
 * @brief Funkce pro ziskani integeru ze seznamu
 * @param DLList ktery obsahuje dany integer
 * @return Token vcetne typu a hodnoty
 */
Token GetInteger(DLList *);

/**
 * @brief Funkce pro ziskani doublu ze seznamu
 * @param DLList ktery obsahuje dany double
 * @return Token vcetne typu a hodnoty
 */
Token GetNumber(DLList *);

int TListInit(TList *);

int TListInsert(TList *, Token);

int TListDeleteActive(TList *);

int TListTokenNext(TList *);

int TListTokenPrev(TList *);

int TListDispose(TList *);

int GetTokenList(TList *);


/*-----JINE POMOCNE FUNKCE------*/

/**
 * @brief Funkce pro printovani typu konkretniho tokenu
 * @param Token_type ktery chceme printovat
 */
void PrintToken(Token_type);
