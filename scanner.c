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
    S_NUMBER_DOT_NUMBER,
    S_NUMBER_E,
    S_NUMBER_E_NUMBER,

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
    S_COMMENT_START_SECOND,
    S_COMMENT_START_BLOCK_FIRST,
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
    S_COLON,

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

    S_SETVALUE,

    S_BRACKET_LEFT,
    S_BRACKET_RIGHT,


} FSM_STATE;


int ScannerGetToken (Token *currentToken)
{
    currentToken->type = T_EMPTY;
    FSM_STATE state = S_START;

    char loadChar;

    DLList list;
    DLL_init(&list);

    while(1)
    {
        loadChar = getc(stdin);

        switch(state)
        {
            case(S_START):
                if(isspace(loadChar))
                {
                    state = S_START;
                }
                else if (loadChar == '#')
                {
                    currentToken->type = T_STRLEN;
                    return 0;
                }
                else if(loadChar == '*')
                {
                    currentToken->type = T_MUL;
                    return 0;
                }
                else if(loadChar == '/')
                {
                    state = S_DIV_NUMBER;
                }
                else if (loadChar == '+')
                {
                    currentToken->type = T_ADD;
                    return 0;
                }
                else if(loadChar == '-')
                {
                    state = S_SUB;
                }
                else if(loadChar == ':')
                {
                    currentToken->type = T_COLON;
                    return 0;
                }
                else if(loadChar == '.')
                {
                    state = S_CONCATENATION_FIRST;
                }
                else if(loadChar == '<')
                {
                    state = S_LT;
                }
                else if(loadChar == '>')
                {
                    state = S_GT;
                }
                else if(loadChar == '=')
                {
                    state = S_SETVALUE;
                }
                else if(loadChar == '~')
                {
                    state = S_NEQ_FIRST;
                }
                else if(loadChar == '(')
                {
                    state = S_BRACKET_LEFT;
                }
                else if(loadChar == ')')
                {
                    state = S_BRACKET_RIGHT;
                }
                else if(isalpha(loadChar) || loadChar == '_')
                {
                    DLL_insertLast(&list, loadChar);
                    state = S_ID;
                }
                else if(isdigit(loadChar))
                {
                    DLL_insertLast(&list, loadChar);
                    state = S_INTEGER;
                }
                else if(loadChar == '"')
                {
                    state = S_STRING_START;
                }
                else if(loadChar == EOF)
                {
                    currentToken->type = T_EOF;
                    return 0;
                }
                break;

            case(S_DIV_NUMBER):
                if(loadChar == '/')     //Nacetl jsem '//', coz urcuje celociselne deleni
                {
                    currentToken->type = T_DIV_INTEGER;     //predavam token
                    return 0;
                }
                else if(loadChar != '/')        //nacetl jsem '/', coz urcuje necelociselne deleni
                {
                    currentToken->type = T_DIV_NUMBER;      //predavam token
                    ungetc(loadChar, stdin);       //musim vratit nacteny znak
                    return 0;
                }
                break;

            case(S_SUB):
                if(loadChar == '-')         //Nacetl jsem znaky '--'
                {
                    state = S_COMMENT_START_SECOND;     //Vim ze budu nacitat komentar
                }
                else if(loadChar != '-')         //Pokud je za - cokoliv jineho, predavam token
                {
                    currentToken->type = T_SUB;
                    ungetc(loadChar, stdin);
                    return 0;
                }
                break;

            case(S_COMMENT_START_SECOND):       //Nacetl jsem dva znaky '--'
                if(loadChar == '[')     //Pokud je nasledujici znak '['
                {
                    state = S_COMMENT_START_BLOCK_FIRST;       //Jdu do nasledujiciho stavu
                }
                else if(loadChar == '\n')       //Pokud je nasledujici znak EOL -> znaci mi konec radkoveho komentare, jdu do stavu start
                {
                    state = S_START;
                }
                else        //Pokud jsem nenacetl jako dalsi znak [ || EOL, dumpuji vnitrek komentare
                {
                    state = S_COMMENT_START_SECOND;
                }
                break;

            case(S_COMMENT_START_BLOCK_FIRST):      //Nacetl jsem znaky '--['
                if(loadChar == '[')     //POkud nasledujici znak je [, tak mi to znaci comment block
                {
                    state = S_COMMENT_BLOCK;
                }
                else if (loadChar != '[')       //pokud jsem po '--[' nenacetl [, jedna se o radkovy komentar
                {
                    state = S_COMMENT_START_SECOND;
                }
                break;

            case(S_COMMENT_BLOCK):
                if(loadChar == ']')     //Nacitam vnitrek komentare a kontroluji zda neprijde znak ']'
                {
                    state = S_COMMENT_BLOCK_END;        //Pokud prisel, jdu do stavu comment block end
                }
                else if(loadChar != ']')        //pokud neprisel, opet dumpuji vnitrek komentare a cekam na nacteni znaku ']'
                {
                    state = S_COMMENT_BLOCK;
                }
                break;

            case(S_COMMENT_BLOCK_END):
                if(loadChar == ']')     //nacetl jsem ']]', vracim se zpet na start stav
                {
                    state = S_START;
                }
                else if(loadChar != ']')        //Nacetl jsem pouze ']' a za nim neco jineho nez ']', vracim se do comment block a dumpuji
                {
                    state = S_COMMENT_BLOCK;
                }
                break;

            case(S_CONCATENATION_FIRST):
                if(loadChar == '.')     //nacetl jsem '..' -> korektni stav FSM prechazim do koncoveho stavu
                {
                    state = S_CONCATENATION_SECOND;
                }
                else
                {
                    return 1;       //Spatne nacteny znak
                }
                break;

            case(S_CONCATENATION_SECOND):
                ungetc(loadChar, stdin);
                currentToken->type = T_CONCATENATION;
                return 0;
                break;

            case(S_GT):
                if(loadChar != '=')
                {
                    ungetc(loadChar, stdin);
                    currentToken->type = T_GT;
                    return 0;
                }
                else if(loadChar == '=')
                {
                    state = S_GET;
                }
                break;

            case(S_GET):
                ungetc(loadChar, stdin);
                currentToken->type = T_GET;
                return 0;
                break;

            case(S_LT):
                if(loadChar != '=')
                {
                    ungetc(loadChar, stdin);
                    currentToken->type = T_LET;
                    return 0;
                }
                else if(loadChar == '=')
                {
                    state = S_LET;
                }
                break;

            case(S_LET):
                ungetc(loadChar, stdin);
                currentToken->type = T_LET;
                return 0;
                break;

            case(S_SETVALUE):
                if(loadChar != '=')
                {
                    ungetc(loadChar, stdin);
                    currentToken->type = T_SETVALUE;
                    return 0;
                }
                else if(loadChar == '=')
                {
                    state = S_EQ;
                }
                break;

            case(S_EQ):
                ungetc(loadChar, stdin);
                currentToken->type = T_EQ;
                return 0;
                break;

            case(S_NEQ_FIRST):
                if(loadChar != '=')
                {
                    return 1;
                }
                else if(loadChar == '=')
                {
                    state = S_NEQ_SECOND;
                }
                break;

            case(S_NEQ_SECOND):
                ungetc(loadChar, stdin);
                currentToken->type = T_NEQ;
                return 0;
                break;

            case(S_BRACKET_LEFT):
                ungetc(loadChar, stdin);
                currentToken->type = S_BRACKET_LEFT;
                return 0;
                break;

            case(S_BRACKET_RIGHT):
                ungetc(loadChar, stdin);
                currentToken->type = S_BRACKET_RIGHT;
                return 0;
                break;

            case(S_INTEGER):
                if(isdigit(loadChar))
                {
                    DLL_insertLast(&list, loadChar);
                    state = S_INTEGER;
                }
                else if(loadChar == '.')
                {
                    DLL_insertLast(&list, loadChar);
                    state = S_NUMBER_DOT;
                }
                else if(loadChar == 'e' || loadChar == 'E')
                {
                    DLL_insertLast(&list, loadChar);
                    state = S_NUMBER_E;
                }
                else
                {
                    ungetc(loadChar, stdin);
                    currentToken->type = T_NUM_INTEGER;
                    currentToken->value.integer = 10;       //DOPLNIT FUNKCI CO UDELA Z DLLISTU INTEGER
                    return 0;
                }
                break;

            case(S_NUMBER_DOT):
                if(isdigit(loadChar))
                {
                    DLL_insertLast(&list, loadChar);
                    state = S_NUMBER_DOT_NUMBER;
                }
                else
                {
                    return 1;
                }
                break;

            case(S_NUMBER_DOT_NUMBER):
                if(isdigit(loadChar))
                {
                    DLL_insertLast(&list, loadChar);
                    state = S_NUMBER_DOT_NUMBER;
                }
                else if(loadChar == 'e' || loadChar == 'E')
                {
                    DLL_insertLast(&list, loadChar);
                    state = S_NUMBER_E;
                }
                else
                {
                    currentToken->type = T_NUM_NUMBER;
                    currentToken->value.number = 10;        // -||-
                    return 0;
                }
                break;

            case(S_NUMBER_E):
                if(isdigit(loadChar))
                {
                    DLL_insertLast(&list, loadChar);
                    state = S_NUMBER_E_NUMBER;
                }
                else
                {
                    return 1;
                }
                break;

            case(S_NUMBER_E_NUMBER):
                if(isdigit(loadChar))
                {
                    DLL_insertLast(&list, loadChar);
                    state = S_NUMBER_E_NUMBER;
                }
                else
                {
                    currentToken->type = T_NUM_NUMBER;
                    currentToken->value.number = 10;
                    return 0;
                }
                break;

            case(S_STRING_START):
                if(loadChar == '\\')        //  '\\' v tomto pripade znamena hodnotu jednoho backslashe
                {
                    state = S_STRING_BACKSLASH;
                }
                else if(loadChar > 31 && loadChar != 34)
                {
                    DLL_insertLast(&list, loadChar);
                    state = S_STRING_START;
                }
                else if(loadChar == '"')        //Znaci konec stringu
                {
                    currentToken->type = T_STRING;
                    currentToken->value.string = "hovno";
                    return 0;
                }
                else
                {
                    return 1;
                }
                break;

            case(S_STRING_BACKSLASH):
                if(loadChar == 'n')
                {
                    DLL_insertLast(&list, '\n');
                    state = S_STRING_START;
                }
                else if(loadChar == '"')
                {
                    DLL_insertLast(&list, '\"');
                    state = S_STRING_START;
                }
                else if(loadChar == '\\')
                {
                    DLL_insertLast(&list, '\\');
                    state = S_STRING_START;
                }
                else if(loadChar == 't')
                {
                    DLL_insertLast(&list, '\t');
                    state = S_STRING_START;
                }
                else if(loadChar == '0')
                {
                    DLL_insertLast(&list, loadChar);
                    state = S_STRING_BACKSLASH_ZERO;
                }
                else if(loadChar == '1')
                {
                    DLL_insertLast(&list, loadChar);
                    state = S_STRING_BACKSLASH_ONE;
                }
                else if(loadChar == '2')
                {
                    DLL_insertLast(&list, loadChar);
                    state = S_STRING_BACKSLASH_TWO;
                }
                else
                {
                    return 1;
                }
                break;

            case(S_STRING_BACKSLASH_ZERO):
                if(loadChar == '0')
                {
                    DLL_insertLast(&list, loadChar);
                    state = S_STRING_BACKSLASH_ZERO_ZERO;
                }
                else if(loadChar >= '1' && loadChar <= '9')
                {
                    DLL_insertLast(&list, loadChar);
                    state = S_STRING_BACKSLASH_ZEROorONE_ONEtoNINE;
                }
                else
                {
                    return 1;
                }
                break;

            case(S_STRING_BACKSLASH_ZERO_ZERO):
                if(loadChar >= '1' && loadChar <= '9')
                {
                    DLL_insertLast(&list, loadChar);
                    state = S_STRING_START;
                }
                else
                {
                    return 1;
                }
                break;

            case(S_STRING_BACKSLASH_ZEROorONE_ONEtoNINE):
                if(loadChar >= '0' && loadChar <= '9')
                {
                    DLL_insertLast(&list, loadChar);
                    state = S_STRING_START;
                }
                else
                {
                    return 1;
                }
                break;

            case(S_STRING_BACKSLASH_ONE):
                if(loadChar >= '0' && loadChar <= '9')
                {
                    DLL_insertLast(&list, loadChar);
                    state = S_STRING_BACKSLASH_ZEROorONE_ONEtoNINE;
                }
                else
                {
                    return 1;
                }
                break;

            case(S_STRING_BACKSLASH_TWO):
                if(loadChar >= '0' && loadChar <= '5')
                {
                    DLL_insertLast(&list, loadChar);
                    state = S_STRING_BACKSLASH_TWO_ONEtoFIVE;
                }
                else
                {
                    return 1;
                }
                break;

            case(S_STRING_BACKSLASH_TWO_ONEtoFIVE):
                if(loadChar >= '0' && loadChar <= '5')
                {
                    DLL_insertLast(&list, loadChar);
                    state = S_STRING_START;
                }
                else
                {
                    return 1;
                }
                break;

            case(S_ID):
                if(isalpha(loadChar) || isdigit(loadChar) || loadChar == '_')
                {
                    DLL_insertLast(&list, loadChar);
                    state = S_ID;
                }
                else
                {
                    currentToken->type = T_ID_KW;
                    return 0;
                }
                break;

            default:
                return 1;
        }
    }
    return 0;
}
