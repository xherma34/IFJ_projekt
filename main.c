#include "parser.h"

int main()
{
    int error = 0;

    TList list;
    error = TListInit(&list);

    if(error != 0)
    {
        printf("[INTERNAL ERROR]");
        printf(" Error %d\n", error);
        TListDispose(&list);
        return error;
    }

    error = GetTokenList(&list);
    if(error != 0)
    {
        if(error != 1)
        {
            printf("[INTERNAL ERROR]");
            printf(" Error %d\n", error);
            TListDispose(&list);
            return error;
        }
        printf("[LEXICAL ERROR]");
        printf(" Error %d\n", error);
        TListDispose(&list);
        return error;
    }

    error = Parse(&list);
    if(error != 0)
    {
        printf("[SYNTAX ERROR]");
        printf(" Error %d", error);
        printf(" on line %d:", list.active->token.line);
        printf(" near token type '");
        PrintToken(list.active->token.type);
        printf("'\n");
        TListDispose(&list);
        return error;
    }

    printf("Compilation succesfull!\n");

    TListDispose(&list);
    return 0;
}
