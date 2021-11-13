/*
* Obsah souboru: Lexikalni analyzator
*   Autor:  Pavel Hermann(xherma34)
*           Jan Zdenek(xzdene00)
*           Maxim Plicka(xplick01)
*/

#include "scanner.h"

typedef enum
{
    //Start stav
    S_START,

    //Typy
    S_ID,
    
    //Number
    S_INTEGER,    
    S_NUMBER_DOT,
    S_NUMBER_E,
    S_NUMBER_DOT_E,

    //String
    S_STRING_START,
    S_STRING_BACKSLASH,      //Escape sekvence
    S_STRING_BACKSLASH_ZERO,
    S_STRING_BACKSLASH_ZERO_ZERO,
    S_STRING_BACKSLASH_ZEROorONE_ONEtoNINE,
    S_STRING_BACKSLASH_ONE,
    S_STRING_BACKSLASH_TWO,
    S_STRING_BACKSLASH_TWO_ONEtoFIVE,
    S_STRING_END,

    //Komentare
    S_COMMENT_START_FIRST,
    S_COMMENT_START_SECOND,
    S_COMMENT_START_BLOCK_FIRST,
    S_COMMENT_START_BLOCK_SECOND,
    S_COMMENT_BLOCK,
    S_COMMENT_BLOCK_END,
    S_COMMENT_LINE,

    //Operatory
    S_STRLEN,       //#
    S_ADD,
    S_SUB,
    S_MUL,
    S_DIV_NUMBER,
    S_DIV_INTEGER,

    //Relacni operatory
    S_CONCATENATION_FIRST,        //Konkatenace
    S_CONCATENATION_SECOND,       //Konkatenace
    S_EQ,
    S_NEQ_FIRST,
    S_NEQ_SECOND,
    S_GT,
    S_LT,
    S_GET,
    S_LET,
   
    S_BRACKET_LEFT,
    S_BRACKET_RIGHT, 
   

} FSM_STATE;


int ScannerGetToken (Token *currentToken)
{
    currentToken->type = T_EMPTY;
    FSM_STATE state = S_START;

    char loadChar;

    while(1)
    {
        loadChar = getc(stdin);

        switch(state)
        {
            case(S_START):

        }
    }
}