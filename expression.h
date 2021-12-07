#include "TStack.h" //Potrebuji scanner, symstack, parser -> vse linknuto v parseru

//hodnoty v tabulce
typedef enum
{
    S, //shift
    R, //reduce
    E, //equal
    X //error
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
 * @brief Funkce ktera vezme TOken a pripisuje mu PTindex odpovidajici tabulce
 *
 */
void getIndex(TNodePtr, SList *);

/**
 * @brief Pomocna funkce pro getAction
 * @param Token
 * @return int pozici radku/sloupce
 */
int getIndexValue(Token *);

/**
 * @brief Vraci hodnotu tabulky na indexu [stackTop][listToken]
 *
 * @return char
 */
PTaction getAction(Token *, Token *);

/**
 * @brief FUnkce ktera vyhodnocuje ktere pravidlo pouzijeme podle nactenych hodnot ze stacku
 * @param StackTop
 * @param StackTopAfter
 * @param ListActiveToken
 */
PTRule getRule(int, Token *, Token *, Token *);

/**
 * @brief General funkce celeho expression c
 *
 * @return 0 pokud se jedna o expression, jinak 1/9/99
 */
int Exp(TList *, SList *, Token_type *);

/**
 * @brief funkce pro operaci reduce, podle pravidla
 *
 * @return 0
 * @return 2
 * @return 9, pokud nastane deleni nulou
 *
 */
int reduceByRule(TStack *, Token_type *, SList *);

/**
 * @brief Kontrola semantiky a syntaxe
 *
 * @return int
 */
int CheckSS(PTRule, Token *, Token *, Token *, Token_type *, SList *);
