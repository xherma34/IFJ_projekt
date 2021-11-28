#include "parser.h"


int main()
{
    int error = 1;

    TList list;
    TListInit(&list);
    GetTokenList(&list);
    error = Parse(&list);
//    list.active = list.first;
//    Token token;
//    token = list.active->token;
//    if(token.type == T_KW_REQUIRE)
//    {
//        printf("hovno");
//    }
//    Token test;
//    int end = 0;
//    list.active = list.first;
//
//    while(end == 0)
//    {
//      test = list.active->token;
//      list.active = list.active->next;
//      switch(test.type)
//      {
//          case(T_MUL):
//              printf("T_MUL\n");
//              break;
//
//          case(T_DIV_NUMBER):
//              printf("T_DIV_NUMBER\n");
//              break;
//
//          case(T_SUB):
//              printf("T_SUB\n");
//              break;
//
//          case(T_ADD):
//              printf("T_ADD\n");
//              break;
//
//          case(T_ID):
//              printf("T_ID | %s\n", test.value.string);
//              break;
//
//          case(T_STRLEN):
//              printf("T_STRLEN\n");
//              break;
//
//          case(T_DIV_INTEGER):
//              printf("T_DIV_INTEGER\n");
//              break;
//
//          case(T_CONCATENATION):
//              printf("T_CONCATENATION\n");
//              break;
//
//          case(T_LT):
//              printf("T_LT\n");
//              break;
//
//          case(T_GT):
//              printf("T_GT\n");
//              break;
//
//          case(T_LET):
//              printf("T_LET\n");
//              break;
//
//          case(T_GET):
//              printf("T_GET\n");
//              break;
//
//          case(T_EQ):
//              printf("T_EQ\n");
//              break;
//
//          case(T_NEQ):
//              printf("T_NEQ\n");
//              break;
//
//          case(T_NUM_INTEGER):
//              printf("T_NUM_INTEGER | %d\n", test.value.integer);
//              break;
//
//          case(T_NUM_NUMBER):
//              printf("T_NUM_NUMBER | %lf\n", test.value.number);
//              break;
//
//          case(T_BRACKET_LEFT):
//              printf("T_BRACKET_LEFT\n");
//              break;
//
//          case(T_BRACKET_RIGHT):
//              printf("T_BRACKET_RIGHT\n");
//              break;
//
//          case(T_COLON):
//              printf("T_COLON\n");
//              break;
//
//          case(T_STRING):
//              printf("T_STRING\n||%s||\n", test.value.string);
//              break;
//
//          case(T_SETVALUE):
//              printf("T_SETVALUE\n");
//              break;
//
//          case(T_KW_DO):
//              printf("T_KW_DO\n");
//              break;
//
//          case(T_KW_ELSE):
//              printf("T_KW_ELSE\n");
//              break;
//
//          case(T_KW_END):
//              printf("T_KW_END\n");
//              break;
//
//          case(T_KW_FUNCTION):
//              printf("T_KW_FUNCTION\n");
//              break;
//
//          case(T_KW_GLOBAL):
//              printf("T_KW_GLOBAL\n");
//              break;
//
//          case(T_KW_IF):
//              printf("T_KW_IF\n");
//              break;
//
//          case(T_KW_LOCAL):
//              printf("T_KW_LOCAL\n");
//              break;
//
//          case(T_KW_NIL):
//              printf("T_KW_NIL\n");
//              break;
//
//          case(T_KW_INTEGER):
//              printf("T_KW_INTEGER\n");
//              break;
//
//          case(T_KW_NUMBER):
//              printf("T_KW_NUMBER\n");
//              break;
//
//          case(T_KW_REQUIRE):
//              printf("T_KW_REQUIRE\n");
//              break;
//
//          case(T_KW_RETURN):
//              printf("T_KW_RETURN\n");
//              break;
//
//          case(T_KW_STRING):
//              printf("T_KW_STRING\n");
//              break;
//
//          case(T_KW_THEN):
//              printf("T_KW_THEN\n");
//              break;
//
//          case(T_KW_WHILE):
//              printf("T_KW_WHILE\n");
//              break;
//
//          case(T_EOF):
//              printf("T_EOF\n");
//              end = 1;
//              break;
//
//          case(T_COMMA):
//              printf("T_COMMA\n");
//              break;
//
//          case(T_EOL):
//              break;
//
//          default:
//              printf("ERROR\n");
//              break;
//      }
//    }
    TListDispose(&list);
    printf("%d\n", error);
    return 0;
}