#include "parser.h"

int main()
{
    int error = 0;

    TList list;
    TListInit(&list);

    error = GetTokenList(&list);
    if(error != 0)
    {
        TListDispose(&list);
        printf("%d\n", error);
        return 1;
    }

    error = Parse(&list);
    if(error != 0)
    {
        TListDispose(&list);
        printf("%d\n", error);
        return 1;
    }

    printf("%d\n", error);
    return 0;
}