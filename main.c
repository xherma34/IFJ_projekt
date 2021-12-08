/*
* Project: IFJ 2021
*
* @file main.c
*
* @brief Implementace hlavního těla překladače
*
* @author Jan Zdeněk (xzdene01)
* @author Alexander Sila (xsila00)
*/

#include "parser.h"

int main()
{
    int error = 0;

    TList list;
    error = TListInit(&list);
    if(error == 99)
    {
        fprintf(stderr, "[INTERNAL ERROR]");
        fprintf(stderr, " Error %d\n", error);
        TListDispose(&list);
        return error;
    }

    SList slist;
    error = SListInit(&slist);
    if(error == 99)
    {
        fprintf(stderr, "[INTERNAL ERROR]");
        fprintf(stderr, " Error %d\n", error);
        TListDispose(&list);
        SListDispose(&slist);
        return error;
    }

    error = GetTokenList(&list);
    if(error == 99)
    {
        fprintf(stderr, "[INTERNAL ERROR]");
        fprintf(stderr, " Error %d\n", error);
        TListDispose(&list);
        SListDispose(&slist);
        return error;
    }
    else if(error == 1)
    {
        fprintf(stderr, "[LEXICAL ERROR]");
        fprintf(stderr, " Error %d\n", error);
        TListDispose(&list);
        SListDispose(&slist);
        return error;
    }

    error = Parse(&list, &slist);
    if(error == 99)
    {
        fprintf(stderr, "[INTERNAL ERROR]");
        fprintf(stderr, " Error %d\n", error);
        TListDispose(&list);
        SListDispose(&slist);
        return error;
    }
    else if(error == 2)
    {
        fprintf(stderr, "[SYNTAX ERROR]");
        fprintf(stderr, " Error %d", error);
        fprintf(stderr, " on line %d:", list.active->token.line);
        fprintf(stderr, " near token type '");
        PrintToken(list.active->token.type);
        fprintf(stderr, "'\n");
        TListDispose(&list);
        SListDispose(&slist);
        return error;
    }
    else if(error == 3)
    {
      fprintf(stderr, "[SEMANTIC ERROR]");
	  fprintf(stderr, " Error %d", error);
	  fprintf(stderr, " on line %d:", list.active->token.line);
	  fprintf(stderr, " near token type '");
	  PrintToken(list.active->token.type);
	  fprintf(stderr, "'. ");
      fprintf(stderr, "Not defined or redefinition of variable or function.\n");
      TListDispose(&list);
      SListDispose(&slist);
      return error;
    }
    else if(error == 4)
    {
      fprintf(stderr, "[SEMANTIC ERROR]");
	  fprintf(stderr, " Error %d", error);
	  fprintf(stderr, " on line %d:", list.active->token.line);
	  fprintf(stderr, " near token type '");
	  PrintToken(list.active->token.type);
	  fprintf(stderr, "'. ");
      fprintf(stderr, "Error in assigning type.\n");
      TListDispose(&list);
      SListDispose(&slist);
      return error;
    }
    else if(error == 5)
    {
      fprintf(stderr, "[SEMANTIC ERROR]");
	  fprintf(stderr, " Error %d", error);
	  fprintf(stderr, " on line %d:", list.active->token.line);
	  fprintf(stderr, " near token type '");
	  PrintToken(list.active->token.type);
	  fprintf(stderr, "'. ");
      fprintf(stderr, "Wrong number or type of parameters or return values.\n");
      TListDispose(&list);
      SListDispose(&slist);
      return error;
    }
	else if(error == 6)
    {
      fprintf(stderr, "[SEMANTIC ERROR]");
	  fprintf(stderr, " Error %d", error);
	  fprintf(stderr, " on line %d:", list.active->token.line);
	  fprintf(stderr, " near token type '");
	  PrintToken(list.active->token.type);
	  fprintf(stderr, "'. ");
      fprintf(stderr, "Error in type compatibility in arithmetic operation.\n");
      TListDispose(&list);
      SListDispose(&slist);
      return error;
    }
	else if(error == 7)
    {
      fprintf(stderr, "[SEMANTIC ERROR]");
	  fprintf(stderr, " Error %d", error);
	  fprintf(stderr, " on line %d:", list.active->token.line);
	  fprintf(stderr, " near token type '");
	  PrintToken(list.active->token.type);
	  fprintf(stderr, "'. ");
      fprintf(stderr, "Not defined semantic error.\n");
      TListDispose(&list);
      SListDispose(&slist);
      return error;
    }
	else if(error == 8)
    {
      fprintf(stderr, "[SEMANTIC ERROR]");
	  fprintf(stderr, " Error %d", error);
	  fprintf(stderr, " on line %d:", list.active->token.line);
	  fprintf(stderr, " near token type '");
	  PrintToken(list.active->token.type);
	  fprintf(stderr, "'. ");
      fprintf(stderr, "Error in unexpected operation with nil.\n");
      TListDispose(&list);
      SListDispose(&slist);
      return error;
    }
	else if(error == 9)
    {
      fprintf(stderr, "[SEMANTIC ERROR]");
	  fprintf(stderr, " Error %d", error);
	  fprintf(stderr, " on line %d:", list.active->token.line);
	  fprintf(stderr, " near token type '");
	  PrintToken(list.active->token.type);
	  fprintf(stderr, "'. ");
      fprintf(stderr, "Error in division with zero.\n");
      TListDispose(&list);
      SListDispose(&slist);
      return error;
    }
    else if(error != 0)
    {
      fprintf(stderr, "[INTERNAL ERROR]");
	  fprintf(stderr, " Error %d", error);
	  fprintf(stderr, " on line %d:", list.active->token.line);
	  fprintf(stderr, " near token type '");
	  PrintToken(list.active->token.type);
	  fprintf(stderr, "'. ");
      fprintf(stderr, "Error not defined.\n");
      TListDispose(&list);
      SListDispose(&slist);
      return error;
    }

    //PrintSList(&slist);

    fprintf(stderr, "Compilation successful!\n");

    TListDispose(&list);
    SListDispose(&slist);

    return 0;
}
