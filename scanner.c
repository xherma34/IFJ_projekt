/*
* Project: IFJ 2021
*
* @file scanner.c
*
* @brief Implementace lexikálního analyzátoru
*
* @author Jan Zdeněk (xzdene01)
* @author Pavel Heřmann (xherma34)
* @author Maxim Plička (xplick04)
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

    S_COMMA,


} FSM_STATE;

int ScannerGetToken (Token *currentToken)
{
    int error;
    Token tmp;
    currentToken->type = T_EMPTY;
    FSM_STATE state = S_START;

    char loadChar;
    //char backslashChar;
    int backslashLen = 0;

    DLList list;
    DLL_init(&list);

    while(1)
    {
        loadChar = getc(stdin);

        switch(state)
        {
            case(S_START):
                if(loadChar == '\n')
                {
                    currentToken->type = T_EOL;
                    return 0;
                }
                else if(isspace(loadChar))
                {
                    state = S_START;
                }
                else if(loadChar == ',')
                {
                    state = S_COMMA;
                }
                else if (loadChar == '#')
                {
                    state = S_STRLEN;
                }
                else if(loadChar == '*')
                {
                    state = S_MUL;
                }
                else if(loadChar == '/')
                {
                    state = S_DIV_NUMBER;
                }
                else if (loadChar == '+')
                {
                    state = S_ADD;
                }
                else if(loadChar == '-')
                {
                    state = S_SUB;
                }
                else if(loadChar == ':')
                {
                    state = S_COLON;
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
                    error = DLL_insertLast(&list, loadChar);
                    if(error != 0)
                    {
                        return error;
                    }
                    list.length = list.length + 1;
                    state = S_ID;
                }
                else if(isdigit(loadChar))
                {
                    error = DLL_insertLast(&list, loadChar);
                    if(error != 0)
                    {
                        return error;
                    }
                    list.length = list.length + 1;
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
                else
                {
                    return 1;
                }
                break;

            case(S_COMMA):
                ungetc(loadChar, stdin);
                currentToken->type = T_COMMA;
                return 0;

            case(S_STRLEN):
                ungetc(loadChar, stdin);
                currentToken->type = T_STRLEN;
                return 0;
            case(S_MUL):
                ungetc(loadChar, stdin);
                currentToken->type = T_MUL;
                return 0;

            case(S_DIV_NUMBER):
                if(loadChar == '/')     //Nacetl jsem '//', coz urcuje celociselne deleni
                {
                    currentToken->type = T_DIV_INTEGER;     //predavam token
                    return 0;
                }
                else        //nacetl jsem '/', coz urcuje necelociselne deleni
                {
                    currentToken->type = T_DIV_NUMBER;      //predavam token
                    ungetc(loadChar, stdin);       //musim vratit nacteny znak
                    return 0;
                }

            case(S_ADD):
                ungetc(loadChar, stdin);
                currentToken->type = T_ADD;
                return 0;

            case(S_SUB):
                if(loadChar == '-')         //Nacetl jsem znaky '--'
                {
                    state = S_COMMENT_START_SECOND;     //Vim ze budu nacitat komentar
                }
                else         //Pokud je za - cokoliv jineho, predavam token
                {
                    currentToken->type = T_SUB;
                    ungetc(loadChar, stdin);
                    return 0;
                }
                break;

            case(S_COLON):
                ungetc(loadChar, stdin);
                currentToken->type = T_COLON;
                return 0;

            case(S_COMMENT_START_SECOND):       //Nacetl jsem dva znaky '--'
                if(loadChar == '[')     //Pokud je nasledujici znak '['
                {
                    state = S_COMMENT_START_BLOCK_FIRST;       //Jdu do nasledujiciho stavu
                }
                else if(loadChar == '\n')       //Pokud je nasledujici znak EOL -> znaci mi konec radkoveho komentare, jdu do stavu start
                {
                    ungetc(loadChar, stdin);
                    state = S_START;
                }
                else if(loadChar == EOF)
                {
                    currentToken->type = T_EOF;
                    return 0;
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
                else      //pokud jsem po '--[' nenacetl [, jedna se o radkovy komentar
                {
                    state = S_COMMENT_START_SECOND;
                }
                break;

            case(S_COMMENT_BLOCK):
                if(loadChar == ']')     //Nacitam vnitrek komentare a kontroluji zda neprijde znak ']'
                {
                    state = S_COMMENT_BLOCK_END;        //Pokud prisel, jdu do stavu comment block end
                }
                else if(loadChar == EOF)
                {
                    return 1;
                }
                else        //pokud neprisel, opet dumpuji vnitrek komentare a cekam na nacteni znaku ']'
                {
                    state = S_COMMENT_BLOCK;
                }
                break;

            case(S_COMMENT_BLOCK_END):
                if(loadChar == ']')     //nacetl jsem ']]', vracim se zpet na start stav
                {
                    state = S_START;
                }
                else        //Nacetl jsem pouze ']' a za nim neco jineho nez ']', vracim se do comment block a dumpuji
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
                else
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
                    currentToken->type = T_LT;
                    return 0;
                }
                else
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
                else
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
                else
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
                currentToken->type = T_BRACKET_LEFT;
                return 0;
                break;

            case(S_BRACKET_RIGHT):
                ungetc(loadChar, stdin);
                currentToken->type = T_BRACKET_RIGHT;
                return 0;
                break;

            case(S_INTEGER):
                if(isdigit(loadChar))
                {
                    error = DLL_insertLast(&list, loadChar);
                    if(error != 0)
                    {
                        return error;
                    }
                    list.length = list.length + 1;
                    state = S_INTEGER;
                }
                else if(loadChar == '.')
                {
                    error = DLL_insertLast(&list, loadChar);
                    if(error != 0)
                    {
                        return error;
                    }
                    list.length = list.length + 1;
                    state = S_NUMBER_DOT;
                }
                else if(loadChar == 'e' || loadChar == 'E')
                {
                    error = DLL_insertLast(&list, loadChar);
                    if(error != 0)
                    {
                        return error;
                    }
                    list.length = list.length + 1;
                    state = S_NUMBER_E;
                }
                else
                {
                    ungetc(loadChar, stdin);
                    tmp = GetInteger(&list);
                    currentToken->type = tmp.type;
                    currentToken->value.integer = tmp.value.integer;
                    return 0;
                }
                break;

            case(S_NUMBER_DOT):
                if(isdigit(loadChar))
                {
                    error = DLL_insertLast(&list, loadChar);
                    if(error != 0)
                    {
                        return error;
                    }
                    list.length = list.length + 1;
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
                    error = DLL_insertLast(&list, loadChar);
                    if(error != 0)
                    {
                        return error;
                    }
                    list.length = list.length + 1;
                    state = S_NUMBER_DOT_NUMBER;
                }
                else if(loadChar == 'e' || loadChar == 'E')
                {
                    error = DLL_insertLast(&list, loadChar);
                    if(error != 0)
                    {
                        return error;
                    }
                    list.length = list.length + 1;
                    state = S_NUMBER_E;
                }
                else
                {
                    tmp = GetNumber(&list);
                    currentToken->type = tmp.type;
                    currentToken->value = tmp.value;
                    ungetc(loadChar, stdin);
                    return 0;
                }
                break;

            case(S_NUMBER_E):
                if(isdigit(loadChar))
                {
                    error = DLL_insertLast(&list, loadChar);
                    if(error != 0)
                    {
                        return error;
                    }
                    list.length = list.length + 1;
                    state = S_NUMBER_E_NUMBER;
                }
                else if(loadChar == '+' || loadChar == '-')
                {
                  error = DLL_insertLast(&list, loadChar);
                  if(error != 0)
                  {
                      return error;
                  }
                  list.length = list.length + 1;
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
                    error = DLL_insertLast(&list, loadChar);
                    if(error != 0)
                    {
                        return error;
                    }
                    list.length = list.length + 1;
                    state = S_NUMBER_E_NUMBER;
                }
                else
                {
                    tmp = GetNumber(&list);
                    currentToken->type = tmp.type;
                    currentToken->value = tmp.value;
                    ungetc(loadChar, stdin);
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
                    error = DLL_insertLast(&list, loadChar);
                    if(error != 0)
                    {
                        return error;
                    }
                    list.length = list.length + 1;
                    state = S_STRING_START;
                }
                else if(loadChar == '"')        //Znaci konec stringu
                {
                    tmp = GetString(&list);
                    currentToken->type = tmp.type;
                    currentToken->value.string = tmp.value.string;
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
                    error = DLL_insertLast(&list, '\n');
                    if(error != 0)
                    {
                        return error;
                    }
                    list.length = list.length + 1;
                    state = S_STRING_START;
                }
                else if(loadChar == '"')
                {
                    error = DLL_insertLast(&list, '\"');
                    if(error != 0)
                    {
                        return error;
                    }
                    list.length = list.length + 1;
                    state = S_STRING_START;
                }
                else if(loadChar == '\\')
                {
                    error = DLL_insertLast(&list, '\\');
                    if(error != 0)
                    {
                        return error;
                    }
                    list.length = list.length + 1;
                    state = S_STRING_START;
                }
                else if(loadChar == 't')
                {
                    error = DLL_insertLast(&list, '\t');
                    if(error != 0)
                    {
                        return error;
                    }
                    list.length = list.length + 1;
                    state = S_STRING_START;
                }
                else if(loadChar == '0')
                {
                    state = S_STRING_BACKSLASH_ZERO;
                }
                else if(loadChar == '1')
                {
                    backslashLen = 100;
                    state = S_STRING_BACKSLASH_ONE;
                }
                else if(loadChar == '2')
                {
                    backslashLen = 200;
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
                    state = S_STRING_BACKSLASH_ZERO_ZERO;
                }
                else if(loadChar >= '1' && loadChar <= '9')
                {
                    backslashLen = ((int)(loadChar) - 48) * 10;
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
                    backslashLen = (int)(loadChar) - 48;
                    //backslashChar = '\000' + backslashLen;
                    error = DLL_insertLast(&list, backslashLen);
                    if(error != 0)
                    {
                        return error;
                    }
                    list.length = list.length + 1;
                    backslashLen = 0;
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
                    backslashLen = backslashLen + ((int)(loadChar) - 48);
                    //backslashChar = '\000' + backslashLen;
                    error = DLL_insertLast(&list, backslashLen);
                    if(error != 0)
                    {
                        return error;
                    }
                    list.length = list.length + 1;
                    backslashLen = 0;
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
                    backslashLen = backslashLen + ((int)(loadChar) - 48) * 10;
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
                    backslashLen = backslashLen + ((int)(loadChar) - 48) * 10;
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
                    backslashLen = backslashLen + ((int)(loadChar) - 48);
                    //backslashChar = '\000' + backslashLen;
                    error = DLL_insertLast(&list, backslashLen);
                    if(error != 0)
                    {
                        return error;
                    }
                    list.length = list.length + 1;
                    backslashLen = 0;
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
                    error = DLL_insertLast(&list, loadChar);
                    if(error != 0)
                    {
                        return error;
                    }
                    list.length = list.length + 1;
                    state = S_ID;
                }
                else
                {
                    ungetc(loadChar, stdin);
                    tmp = IdentifyKW(&list);
                    currentToken->type = tmp.type;
                    currentToken->value.string = tmp.value.string;
                    return 0;
                }
                break;

            default:
                return 1;
        }
    }
    return 0;
}

Token IdentifyKW(DLList *list)
{
    Token myToken;
    int len = list->length;
    myToken.value.string = malloc(len + 1 * sizeof(char));
    DLLnodePtr tmp= list->firstElement;

    for(int i = 0; i < len; i++)
    {
        myToken.value.string[i] = tmp->data;
        tmp = tmp->next;
    }

    myToken.value.string[len] = '\0';

    if(strcmp(myToken.value.string, "do") == 0)
    {
        myToken.type = T_KW_DO;

    }
    else if(strcmp(myToken.value.string, "else") == 0)
    {
        myToken.type = T_KW_ELSE;
    }
    else if(strcmp(myToken.value.string, "end") == 0)
    {
        myToken.type =  T_KW_END;
    }
    else if(strcmp(myToken.value.string, "function") == 0)
    {
        myToken.type =  T_KW_FUNCTION;
    }
    else if(strcmp(myToken.value.string, "global") == 0)
    {
        myToken.type =  T_KW_GLOBAL;
    }
    else if(strcmp(myToken.value.string, "if") == 0)
    {
        myToken.type =  T_KW_IF;
    }else if(strcmp(myToken.value.string, "local") == 0)
    {
        myToken.type =  T_KW_LOCAL;
    }
    else if(strcmp(myToken.value.string, "nil") == 0)
    {
        myToken.type =  T_KW_NIL;
    }
    else if(strcmp(myToken.value.string, "integer") == 0)
    {
        myToken.type =  T_KW_INTEGER;
    }else if(strcmp(myToken.value.string, "number") == 0)
    {
        myToken.type =  T_KW_NUMBER;
    }else if(strcmp(myToken.value.string, "require") == 0)
    {
        myToken.type =  T_KW_REQUIRE;
    }else if(strcmp(myToken.value.string, "return") == 0)
    {
        myToken.type =  T_KW_RETURN;
    }else if(strcmp(myToken.value.string, "string") == 0)
    {
        myToken.type =  T_KW_STRING;
    }else if(strcmp(myToken.value.string, "then") == 0)
    {
        myToken.type =  T_KW_THEN;
    }else if(strcmp(myToken.value.string, "while") == 0)
    {
        myToken.type =  T_KW_WHILE;
    }
    else
    {
        myToken.type =  T_ID;
        DLL_dispose(&(*list));
        return myToken;
    }

    DLL_dispose(&(*list));

    free(myToken.value.string);

    return myToken;
}

Token GetString(DLList *list)
{
    Token myToken;
    int len = list->length;
    myToken.value.string = malloc(len + 1 * sizeof(char));
    DLLnodePtr tmp= list->firstElement;

    for(int i = 0; i < len; i++)
    {
        myToken.value.string[i] = tmp->data;
        tmp = tmp->next;
    }

    myToken.value.string[len] = '\0';

    myToken.type = T_STRING;

    DLL_dispose(&(*list));

    return myToken;
}

Token GetInteger(DLList *list)
{
    Token myToken;
    int len = list->length;
    myToken.value.string = malloc(len + 1 * sizeof(char));
    DLLnodePtr tmp= list->firstElement;

    for(int i = 0; i < len; i++)
    {
        myToken.value.string[i] = tmp->data;
        tmp = tmp->next;
    }

    myToken.value.string[len] = '\0';

    int x = atoi(myToken.value.string);

    free(myToken.value.string);

    myToken.value.integer = x;

    myToken.type = T_NUM_INTEGER;

    DLL_dispose(&(*list));

    return myToken;
}

Token GetNumber(DLList *list)
{
    Token myToken;
    int len = list->length;
    char new[len + 1];
    DLLnodePtr tmp = list->firstElement;
    char *ptr;

    for(int i = 0; i < len; i++)
    {
        new[i] = tmp->data;
        tmp = tmp->next;
    }

    new[len] = '\0';

    myToken.value.number = strtod(new, &ptr);
    myToken.type = T_NUM_NUMBER;

	//free(myToken.value.string);

    DLL_dispose(&(*list));

    return myToken;
}

//-----------------------------TOKEN LIST------------------------------------------------------

int TListInit(TList *list)
{
  if(list == NULL)
  {
      return 99;
  }
  list->first = NULL;
  list->last = NULL;
  list->active = NULL;
  return 0;
}

int TListInsert(TList *list, Token token)
{
  if(list == NULL)
  {
    return 99;
  }
  TNodePtr new = (TNodePtr)malloc(sizeof(struct TNode));
  if(new == NULL)
  {
      return 99;
  }
  new->token = token;
  new->next = NULL;
  new->prev = NULL;
  if(list->first == NULL)
  {
    list->first = new;
    list->last = new;
  }
  else
  {
    list->last->next = new;
    new->next = NULL;
    new->prev = list->last;
    list->last = new;
  }
  return 0;
}

int TListDeleteActive(TList *list)
{
  if(list == NULL)
  {
      return 99;
  }
  if(list->active == NULL)
  {
    return 0;
  }
  else
  {
    if(list->active->token.type == T_ID || list->active->token.type == T_STRING)
    {
      free(list->active->token.value.string);
    }
    if(list->active == list->first)
    {
      list->first = list->active->next;
      free(list->active);
      list->active = list->first;
    }
    else if(list->active == list->last)
    {
      list->last = list->active->prev;
      free(list->active);
      list->active = NULL;
    }
    else
    {
      list->active->prev->next = list->active->next;
      free(list->active);
      list->active = list->active->next;
    }
  }
  return 0;
}

int TListTokenNext(TList *list)
{
  if(list == NULL)
  {
      return 99;
  }
  if(list->active == NULL)
  {
    return 0;
  }
  else
  {
    list->active = list->active->next;
  }
  return 0;
}

int TListTokenPrev(TList *list)
{
  if(list == NULL)
  {
      return 99;
  }
  if(list->active == NULL)
  {
    return 0;
  }
  else
  {
    list->active = list->active->prev;
  }
  return 0;
}

int TListDispose(TList *list)
{
    int error;

  if(list == NULL)
  {
      return 99;
  }
  list->active = list->first;
  while(list->active != NULL)
  {
    error =  TListDeleteActive(list);
    if(error != 0)
    {
        return error;
    }
  }
  return 0;
}

int GetTokenList(TList *list)
{
  int line = 1;
  int error = 0;
  Token myToken;
  myToken.type = T_EMPTY;

  while(myToken.type != T_EOF)
  {
    error = ScannerGetToken(&myToken);
    if(error != 0)
    {
        return error;
    }
    myToken.line = line;
    if(myToken.type == T_EOL)
    {
      line++;
    }
    else
    {
        error = TListInsert(&(*list), myToken);
        if(error != 0)
        {
            return error;
        }
    }
  }
  return 0;
}

void PrintToken(Token_type token)
{
    if(token == T_ID)
    {
        fprintf(stderr, "ID");
    }
    else if(token == T_KW_DO)
    {
        fprintf(stderr, "KW_DO");
    }
    else if(token == T_KW_ELSE)
    {
        fprintf(stderr, "KW_ELSE");
    }
    else if(token == T_KW_END)
    {
        fprintf(stderr, "KW_END");
    }
    else if(token == T_KW_FUNCTION)
    {
        fprintf(stderr, "KW_FUNCTION");
    }
    else if(token == T_KW_GLOBAL)
    {
        fprintf(stderr, "KW_GLOBAL");
    }
    else if(token == T_KW_IF)
    {
        fprintf(stderr, "KW_IF");
    }
    else if(token == T_KW_LOCAL)
    {
        fprintf(stderr, "KW_LOCAL");
    }
    else if(token == T_KW_NIL)
    {
        fprintf(stderr, "KW_NIL");
    }
    else if(token == T_KW_INTEGER)
    {
        fprintf(stderr, "KW_INTEGER");
    }
    else if(token == T_KW_NUMBER)
    {
        fprintf(stderr, "KW_NUMBER");
    }
    else if(token == T_KW_REQUIRE)
    {
        fprintf(stderr, "KW_REQUIRE");
    }
    else if(token == T_KW_RETURN)
    {
        fprintf(stderr, "KW_RETURN");
    }
    else if(token == T_KW_STRING)
    {
        fprintf(stderr, "KW_STRING");
    }
    else if(token == T_KW_THEN)
    {
        fprintf(stderr, "KW_THEN");
    }
    else if(token == T_KW_WHILE)
    {
        fprintf(stderr, "KW_WHILE");
    }
    else if(token == T_STRLEN)
    {
        fprintf(stderr, "STRLEN");
    }
    else if(token == T_ADD)
    {
        fprintf(stderr, "ADD");
    }
    else if(token == T_SUB)
    {
        fprintf(stderr, "SUB");
    }
    else if(token == T_MUL)
    {
        fprintf(stderr, "MUL");
    }
    else if(token == T_DIV_NUMBER)
    {
        fprintf(stderr, "DIV_NUMBER");
    }
    else if(token == T_DIV_INTEGER)
    {
        fprintf(stderr, "DIV_INTEGER");
    }
    else if(token == T_CONCATENATION)
    {
        fprintf(stderr, "CONCATENATION");
    }
    else if(token == T_LT)
    {
        fprintf(stderr, "LT");
    }
    else if(token == T_GT)
    {
        fprintf(stderr, "GT");
    }
    else if(token == T_LET)
    {
        fprintf(stderr, "LET");
    }
    else if(token == T_GET)
    {
        fprintf(stderr, "GET");
    }
    else if(token == T_EQ)
    {
        fprintf(stderr, "EQ");
    }
    else if(token == T_NEQ)
    {
        fprintf(stderr, "NEQ");
    }
    else if(token == T_EOF)
    {
        fprintf(stderr, "EOF");
    }
    else if(token == T_EOL)
    {
        fprintf(stderr, "EOL");
    }
    else if(token == T_NUM_INTEGER)
    {
        fprintf(stderr, "NUM_INTEGER");
    }
    else if(token == T_NUM_NUMBER)
    {
        fprintf(stderr, "NUM_NUMBER");
    }
    else if(token == T_BRACKET_RIGHT)
    {
        fprintf(stderr, "BRACKET_RIGHT");
    }
    else if(token == T_BRACKET_LEFT)
    {
        fprintf(stderr, "BRACKET_LEFT");
    }
    else if(token == T_COLON)
    {
        fprintf(stderr, "COLON");
    }
    else if(token == T_STRING)
    {
        fprintf(stderr, "STRING");
    }
    else if(token == T_SETVALUE)
    {
        fprintf(stderr, "SETVALUE");
    }
    else if(token == T_EMPTY)
    {
        fprintf(stderr, "EMPTY");
    }
    else if(token == T_COMMA)
    {
        fprintf(stderr, "COMMA");
    }
    else
    {
        fprintf(stderr, "[TOKEN TYPE ERROR]");
    }
}
