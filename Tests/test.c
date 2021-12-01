#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "../parser.h"

int main()
{
    printf("--TOKEN TESTS--\n");

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
            return 0;
        }
        printf("[LEXICAL ERROR]");
        printf(" Error %d\n", error);
        TListDispose(&list);
        return 0;
    }

    list.active = list.first;

    while(list.active->token.type != T_EOF)
    {
      printf("%d ", list.active->token.line);

      printf("T_");

      PrintToken(list.active->token.type);

      if(list.active->token.type == T_STRING)
      {
        printf("\n||%s||", list.active->token.value.string);
      }
      else if(list.active->token.type == T_NUM_INTEGER)
      {
        printf(" | %d", list.active->token.value.integer);
      }
      else if(list.active->token.type == T_NUM_NUMBER)
      {
        printf(" | %f", list.active->token.value.number);
      }
      else if(list.active->token.type == T_ID)
      {
        printf(" | %s", list.active->token.value.string);
      }

      printf("\n");

      TListTokenNext(&list);
    }

    printf("%d T_", list.active->token.line);
    PrintToken(list.active->token.type);
    printf("\n");

    TListDispose(&list);

    return 0;
}
