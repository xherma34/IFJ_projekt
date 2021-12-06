#include "parser.h"

int main()
{
    int error = 0;

    TList list;
    error = TListInit(&list);
    if(error == 99)
    {
        printf("[INTERNAL ERROR]");
        printf(" Error %d\n", error);
        TListDispose(&list);
        return error;
    }

    SList slist;
    error = SListInit(&slist);
    if(error == 99)
    {
        printf("[INTERNAL ERROR]");
        printf(" Error %d\n", error);
        TListDispose(&list);
        SListDispose(&slist);
        return error;
    }

    error = GetTokenList(&list);
    if(error == 99)
    {
        printf("[INTERNAL ERROR]");
        printf(" Error %d\n", error);
        TListDispose(&list);
        SListDispose(&slist);
        return error;
    }
    else if(error == 1)
    {
        printf("[LEXICAL ERROR]");
        printf(" Error %d\n", error);
        TListDispose(&list);
        SListDispose(&slist);
        return error;
    }

    error = Parse(&list, &slist);
    if(error == 99)
    {
        printf("[INTERNAL ERROR]");
        printf(" Error %d\n", error);
        TListDispose(&list);
        SListDispose(&slist);
        return error;
    }
    else if(error == 2)
    {
        printf("[SYNTAX ERROR]");
        printf(" Error %d", error);
        printf(" on line %d:", list.active->token.line);
        printf(" near token type '");
        PrintToken(list.active->token.type);
        printf("'\n");
        TListDispose(&list);
        SListDispose(&slist);
        return error;
    }
    else if(error == 3)
    {
      printf("[SEMANTIC ERROR]");
	  printf(" Error %d", error);
	  printf(" on line %d:", list.active->token.line);
	  printf(" near token type '");
	  PrintToken(list.active->token.type);
	  printf("'. ");
      printf("Not defined or redefinition of variable or function.\n");
      TListDispose(&list);
      SListDispose(&slist);
      return error;
    }
    else if(error == 4)
    {
      printf("[SEMANTIC ERROR]");
	  printf(" Error %d", error);
	  printf(" on line %d:", list.active->token.line);
	  printf(" near token type '");
	  PrintToken(list.active->token.type);
	  printf("'. ");
      printf("Error in assigning type.\n");
      TListDispose(&list);
      SListDispose(&slist);
      return error;
    }
    else if(error == 5)
    {
      printf("[SEMANTIC ERROR]");
	  printf(" Error %d", error);
	  printf(" on line %d:", list.active->token.line);
	  printf(" near token type '");
	  PrintToken(list.active->token.type);
	  printf("'. ");
      printf("Wrong number or type of parameters or return values.\n");
      TListDispose(&list);
      SListDispose(&slist);
      return error;
    }
    else if(error != 0)
    {
      printf("[INTERNAL ERROR]");
	  printf(" Error %d", error);
	  printf(" on line %d:", list.active->token.line);
	  printf(" near token type '");
	  PrintToken(list.active->token.type);
	  printf("'. ");
      printf("Error not defined.\n");
      TListDispose(&list);
      SListDispose(&slist);
      return error;
    }

    PrintSList(&slist);

    printf("Compilation succesfull!\n");

    TListDispose(&list);
    SListDispose(&slist);

    return 0;
}
