#include "symtable.h"

int main()
{
  int error;

  SList slist;

  error = SListInit(&slist);
  if(error != 0)
  {
    return error;
  }

/*----------*/
  Token token_func_1;
  Token token_func_2;
  Token token_func_3;
  Token token_func_4;
  Token token_func_5;

  token_func_1.type = T_ID;
  token_func_2.type = T_ID;
  token_func_3.type = T_ID;
  token_func_4.type = T_ID;
  token_func_5.type = T_ID;

  token_func_1.value.string = "function1";
  token_func_2.value.string = "function2";
  token_func_3.value.string = "function3";
  token_func_4.value.string = "function4";
  token_func_5.value.string = "function5";

  Token token_var_1;
  Token token_var_2;
  Token token_var_3;
  Token token_var_4;
  Token token_var_5;

  token_var_1.type = T_ID;
  token_var_2.type = T_ID;
  token_var_3.type = T_ID;
  token_var_4.type = T_ID;
  token_var_5.type = T_ID;

  token_var_1.value.string = "variable1";
  token_var_2.value.string = "variable2";
  token_var_3.value.string = "variable3";
  token_var_4.value.string = "variable1";
  token_var_5.value.string = "variable5";

  SListInsertFunc(&slist, true, &token_func_1);

  SListInsertParam(&slist, T_KW_INTEGER);
  SListInsertParam(&slist, T_KW_STRING);
  SListInsertParam(&slist, T_KW_NUMBER);

  SListInsertReturn(&slist, T_KW_NIL);

  printf("1----------\n");
  PrintSList(&slist);
  printf("----------\n");

  if(IsInteger(&slist, &token_var_1) == 3)
  {
    printf(">>>Variable is not declared.\n");
  }

  SListInsertVar(&slist, &token_var_1, T_KW_NIL);
  SListInsertVar(&slist, &token_var_2, T_KW_NUMBER);

  printf("2----------\n");
  PrintSList(&slist);
  printf("----------\n");

  SListInsertCond(&slist);

  printf("3----------\n");
  PrintSList(&slist);
  printf("----------\n");

  SListInsertVar(&slist, &token_var_3, T_KW_INTEGER);

  if(IsInteger(&slist, &token_var_1) == 0)
  {
    printf(">>>Variable is declared as integer.\n");
  }

  SListInsertVar(&slist, &token_var_4, T_KW_STRING);

  if(IsInteger(&slist, &token_var_1) == 4)
  {
    printf(">>>Variable is not declared as integer.\n");
  }

  printf("4----------\n");
  PrintSList(&slist);
  printf("----------\n");

  DeleteScope(&slist);

  if(IsInteger(&slist, &token_var_1) == 0)
  {
    printf(">>>Variable is declared as integer.\n");
  }

  printf("5----------\n");
  PrintSList(&slist);
  printf("----------\n");

  SListInsertVar(&slist, &token_var_5, T_KW_INTEGER);

  printf("6----------\n");
  PrintSList(&slist);
  printf("----------\n");

  DeleteScope(&slist);

  printf("7----------\n");
  PrintSList(&slist);
  printf("----------\n");
/*----------*/

  SListDispose(&slist);

  printf("8----------\n");
  PrintSList(&slist);
  printf("----------\n");

  return 0;
}
