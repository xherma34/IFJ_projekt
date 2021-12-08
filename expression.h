/*
* Project: IFJ 2021
*
* @file expression.h
*
* @brief Knihovna pro expession.c
*
* @author Pavel Heřmann (xherma34)
* @author Maxim Plička (xplick04)
*/

#include "TStack.h" 

//hodnoty v tabulce
typedef enum
{
	S,	//shift
	R,	//reduce
	E, 	//equal
	X 	//error
} PTaction;

typedef enum
{
	E_ADD_E,  //E -> E + E
	E_SUB_E,  //E -> E - E
	E_MUL_E,  //E -> E * E
	E_DIV_INT_E,  //E -> E / E
	E_DIV_NUM_E,  //E -> E // E
	E_CONCATENATION_E,  // E .. E
	E_LT_E, //E -> E < E
	E_GT_E, //E -> E > E
	E_LET_E,  //E -> E <= E
	E_GET_E,  //E -> E >= E
	E_EQ_E,  //E -> E == E
	E_NEQ_E,  //E -> E != E
	STRLEN_E, //E -> #E
	E_ID, //E -> ID
	E_BRACKETS,  //E -> (E)
	NOT_A_RULE
} PTRule;

/**
 * @brief Funkce ktera vezme Token a pripisuje mu PTindex odpovidajici tabulce
 * @param TNodePtr node->token kteremu pripisujeme PTindex
 * @param SList tabulka symbolu
 */
void getIndex(TNodePtr, SList *);

/**
 * @brief Funkce vraci pozici radku/sloupce v tabulce
 * @param Token index v tabulce
 * @return int cislo sloupce/radku v tabulce
 */
int getIndexValue(Token *);

/**
 * @brief Vraci hodnotu tabulky na indexu [stackTop][listToken]
 * @param Token index sloupce
 * @param Token index radku
 * @return char
 */
PTaction getAction(Token *, Token *);

/**
 * @brief FUnkce ktera vyhodnocuje ktere pravidlo pouzijeme podle nactenych hodnot ze stacku
 * @param int pocet znaku mezi stackTop a stopSign
 * @param Token StackTop
 * @param Token StackTop->next
 * @param Token StackTop->next->next
 * @return Pravidlo pro redukci
 */
PTRule getRule(int, Token *, Token *, Token *);

/**
 * @brief General funkce celeho expression c
 * @param TList list tokenu
 * @param SList tabulka symbolu
 * @param Token_type finalni datovy typ
 * @return 0 -> is an expression, 1 -> isnt an expression, >1 -> error
 */
int Exp(TList *, SList *, Token_type *);

/**
 * @brief funkce pro operaci reduce, podle pravidla
 * @param TStack stack tokenu
 * @param Token_type finalni datovy typ
 * @param SList tabulka symbolu
 * @return 0 -> Probehla spravne redukce, >0 -> error
 */
int reduceByRule(TStack *, Token_type *, SList *);

/**
 * @brief Kontrola semantiky a syntaxe
 * @param PTRule pravidlo redukce
 * @param Token StackTop
 * @param Token StackTop->next
 * @param Token StackTop->next->next
 * @param Token_type finalni datovy typ
 * @param SList tabulka symbolu
 * @return 0 -> vse semanticky/syntakticky v poradku a bez interniho erroru, >0 -> error
 */
int CheckSS(PTRule, Token *, Token *, Token *, Token_type *, SList *);
