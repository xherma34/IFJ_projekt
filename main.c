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
    else if(error != 0)
    {
      printf("ERROR %d", error);
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
