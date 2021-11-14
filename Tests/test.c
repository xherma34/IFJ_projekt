#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "../scanner.h"



int main()
{
    Token myToken;
    printf("--TOKEN TESTS--\n");
    while(myToken.type != T_EOF)
    {
        ScannerGetToken(&myToken);
        switch(myToken.type)
        {
            case(T_MUL):
                printf("MUL\n");
                break;

            case(T_DIV_NUMBER):
                printf("DIVNUMBER\n");
                break;

            case(T_SUB):
                printf("SUB\n");
                break;

            case(T_ADD):
                printf("ADD\n");
                break;

            case(T_ID_KW):
                printf("ID KEYWORD\n");
                break;

            case(T_STRLEN):
                printf("STRLEN\n");
                break;

            case(T_DIV_INTEGER):
                printf("DIVINT\n");
                break;

            case(T_CONCATENATION):
                printf("CONCATENATION\n");
                break;

            case(T_LT):
                printf("LT\n");
                break;

            case(T_GT):
                printf("GT\n");
                break;

            case(T_LET):
                printf("LET\n");
                break;

            case(T_GET):
                printf("GET\n");
                break;

            case(T_EQ):
                printf("EQ\n");
                break;

            case(T_NEQ):
                printf("NEQ\n");
                break;

            case(T_NUM_INTEGER):
                printf("INTEGER\n");
                break;

            case(T_NUM_NUMBER):
                printf("FLOAT\n");
                break;

            case(T_BRACKET_LEFT):
                printf("BRACKET_LEFT\n");
                break;

            case(T_BRACKET_RIGHT):
                printf("BRACKET_RIGHT\n");
                break;

            case(T_COLON):
                printf("COLON\n");
                break;

            case(T_STRING):
                printf("STRING\n");
                break;

            case(T_SETVALUE):
                printf("SETVALUE\n");
                break;

            default:
                printf("ERROR\n");
                break;
        }
    }
    return 0;
}
