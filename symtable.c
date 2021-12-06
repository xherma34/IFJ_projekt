#include "symtable.h"

int SListInit(SList *list)
{
  if(list == NULL)
  {
    return 99;
  }

  list->first = NULL;
  list->last = NULL;
  list->active = NULL;
  list->lastFunc = NULL;
  return 0;
}

int SListNext(SList *list)
{
  if(list == NULL)
  {
    return 99;
  }

  if(list->active == NULL)
  {
    return 99;
  }
  else
  {
    list->active = list->active->next;
  }

  return 0;
}

int SListPrev(SList *list)
{
  if(list == NULL)
  {
    return 99;
  }

  if(list->active == NULL)
  {
    return 99;
  }
  else
  {
    list->active = list->active->prev;
  }

  return 0;
}

int SListInsertCond(SList *list)
{
  if(list == NULL)
  {
    return 99;
  }

  SNodePtr new = (SNodePtr)malloc(sizeof(struct SNode));
  if(new == NULL)
  {
      return 99;
  }

  new->func = false;
  new->var = false;
  new->cond = true;
  new->cycle = false;

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

int SListInsertCycle(SList *list)
{
  if(list == NULL)
  {
    return 99;
  }

  SNodePtr new = (SNodePtr)malloc(sizeof(struct SNode));
  if(new == NULL)
  {
      return 99;
  }

  new->func = false;
  new->var = false;
  new->cond = false;
  new->cycle = true;

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

int SListInsertFunc(SList *list, bool def, Token *token)
{
  if(list == NULL)
  {
    return 99;
  }

  SNodePtr new = (SNodePtr)malloc(sizeof(struct SNode));
  if(new == NULL)
  {
      return 99;
  }

  new->func = true;
  new->var = false;
  new->cond = false;
  new->cycle = false;

  new->def_func = def;
  new->id_func = token;
  new->numParams = 0;
  new->numReturns = 0;

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

int SListInsertParam(SList *list, Token_type tokentype)
{
  if(list == NULL)
  {
    return 99;
  }

  Token token;
  token.type = tokentype;

	LastFunc(list);

  TListInsert(&list->lastFunc->params, token);
  list->lastFunc->numParams ++;
  list->lastFunc->params.last->index = list->lastFunc->numParams;

  return 0;
}

int SListInsertReturn(SList *list, Token_type tokentype)
{
  if(list == NULL)
  {
    return 99;
  }

  Token token;
  token.type = tokentype;

  LastFunc(list);

  TListInsert(&list->lastFunc->returns, token);
  list->lastFunc->numReturns ++;
  list->lastFunc->returns.last->index = list->lastFunc->numReturns;

  return 0;
}

int SListInsertVar(SList *list, Token *token, Token_type tokentype)
{
  if(list == NULL)
  {
    return 99;
  }

  SNodePtr new = (SNodePtr)malloc(sizeof(struct SNode));
  if(new == NULL)
  {
      return 99;
  }

  new->var = true;
  new->func = false;
  new->cond = false;
  new->cycle = false;

  new->id_var = token;
  new->type = tokentype;

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

int SListDeleteLast(SList *list)
{
  if(list == NULL)
  {
    return 99;
  }

  if(list->last == NULL)
  {
    return 0;
  }
  else if(list->first == list->last)
  {
    if(list->last->func == true)
    {
      TListDispose(&list->last->params);
      TListDispose(&list->last->returns);
    }
    free(list->last);
    list->last = NULL;
    list->first = NULL;
  }
  else
  {
    if(list->last->func == true)
    {
      TListDispose(&list->last->params);
      TListDispose(&list->last->returns);
    }
    list->last = list->last->prev;
    free(list->last->next);
  }

  return 0;
}

int SListDispose(SList *list)
{
  if(list == NULL)
  {
    return 99;
  }

  while(list->last != NULL)
  {
    SListDeleteLast(&(*list));
  }

  return 0;
}

int IsDeclaredFunc(SList *list, Token *token)
{
  if(list == NULL)
  {
    return 99;
  }

  if(token->type != T_ID)
  {
    return 99;
  }

  if(list->first == NULL)
  {
    return 3;
  }

  char *str = token->value.string;

  list->active = list->last;

  while(1)
  {
    if(list->active->func == true)
    {
      if(list->active == list->first &&
        strcmp(str, list->active->id_func->value.string) != 0)
      {
        return 3;
      }
      else if(strcmp(str, list->active->id_func->value.string) == 0)
      {
        if(list->active->def_func == true)
        {
          return 1;
        }
        return 0;
      }
    }
    else
    {
      if(list->active == list->first)
      {
        return 3;
      }
    }
    SListPrev(&(*list));
  }

  return 99;
}

int IsDeclaredJump(SList *list, Token *token)
{
	if(list == NULL)
	{
		return 99;
	}

	if(token->type != T_ID)
	{
		return 99;
	}

	if(list->first == NULL)
	{
		return 3;
	}

	char *str = token->value.string;

	LastFunc(list);

  	list->active = list->lastFunc->prev;

  while(1)
  {
    if(list->active->func == true)
    {
      if(list->active == list->first &&
        strcmp(str, list->active->id_func->value.string) != 0)
      {
        return 3;
      }
      else if(strcmp(str, list->active->id_func->value.string) == 0)
      {
        if(list->active->def_func == true)
        {
          return 1;
        }
        return 0;
      }
    }
    else
    {
      if(list->active == list->first)
      {
        return 3;
      }
    }
    SListPrev(&(*list));
  }

  return 99;
}

int IsDeclaredVar(SList *list, Token *token)
{
  if(list == NULL)
  {
    return 99;
  }

  if(token->type != T_ID)
  {
    return 99;
  }

  list->active = list->last;

  while(1)
  {
    if(list->active->var == true)
    {
      if(list->active == list->first &&
        strcmp(token->value.string, list->active->id_var->value.string) != 0)
      {
        return 3;
      }
      else if(strcmp(token->value.string, list->active->id_var->value.string) == 0)
      {
        return 0;
      }
    }
    else
    {
      if(list->active == list->first)
      {
        return 3;
      }
    }
    SListPrev(&(*list));
  }

  return 99;
}

int IsDeclaredVarInScope(SList *list, Token *token)
{
  if(list == NULL)
  {
    return 99;
  }

  if(token->type != T_ID)
  {
    return 99;
  }

  list->active = list->last;

  while(1)
  {
    if(list->active->var == true)
    {
      if(list->active == list->first &&
        strcmp(token->value.string, list->active->id_var->value.string) != 0)
      {
        return 3;
      }
      else if(strcmp(token->value.string, list->active->id_var->value.string) == 0)
      {
        return 0;
      }
    }
    else
    {
      if(list->active == list->first || list->active->cond == true || list->active->cycle == true)
      {
        return 3;
      }
    }
    SListPrev(&(*list));
  }

  return 99;
}

int DeleteScope(SList *list)
{
  if(list == NULL)
  {
    return 99;
  }

  if(list->last == NULL)
  {
    return 0;
  }

  while(list->last->var == true)
  {
    SListDeleteLast(list);
  }

  if(list->last->func != 1)
  {
    SListDeleteLast(list);
  }

  return 0;
}

int IsInteger(SList *list, Token *token)
{
  if(IsDeclaredVar(list, token) == 0)
  {
    if(list->active->type == T_KW_INTEGER)
    {
      return 0;
    }
    return 4;
  }
  return 3;
}

int IsNumber(SList *list, Token *token)
{
  if(IsDeclaredVar(list, token) == 0)
  {
    if(list->active->type == T_KW_NUMBER)
    {
      return 0;
    }
    return 4;
  }
  return 3;
}

int IsString(SList *list, Token *token)
{
  if(IsDeclaredVar(list, token) == 0)
  {
    if(list->active->type == T_KW_STRING)
    {
      return 0;
    }
    return 4;
  }
  return 3;
}

int IsNil(SList *list, Token *token)
{
  if(IsDeclaredVar(list, token) == 0)
  {
    if(list->active->type == T_KW_NIL)
    {
      return 0;
    }
    return 4;
  }
  return 3;
}

void LastFunc(SList *list)
{
	list->lastFunc = list->last;
	while(list->lastFunc->func != true)
	{
		list->lastFunc = list->lastFunc->prev;
	}
}

/*-------------------------------------------------------------*/

void PrintSList(SList *list)
{
  if(list == NULL)
  {
    return;
  }

  if(list->last == NULL)
  {
    printf("empty list\n");
  }

  list->active = list->last;

  while(list->active != NULL)
  {
    if(list->active->func == true)
    {
      printf("function :: def(%d)\n", list->active->def_func);
      printf("\tname : %s\n", list->active->id_func->value.string);
      printf("\tparams (%d) : ", list->active->numParams);
      list->active->params.active = list->active->params.first;
      while(list->active->params.active != NULL)
      {
        PrintToken(list->active->params.active->token.type);
        printf("(%d) ", list->active->params.active->index);
        TListTokenNext(&list->active->params);
      }
      printf("\n");
      printf("\treturns (%d) : ", list->active->numReturns);
      list->active->returns.active = list->active->returns.first;
      while(list->active->returns.active != NULL)
      {
        PrintToken(list->active->returns.active->token.type);
        printf("(%d) ", list->active->returns.active->index);
        TListTokenNext(&list->active->returns);
      }
      printf("\n");
    }
    else if(list->active->var == true)
    {
      printf("variable\n");
      printf("\tname : %s\n", list->active->id_var->value.string);
      printf("\ttype : ");
      PrintToken(list->active->type);
      printf("\n");
    }
    else if(list->active->cond == true)
    {
      printf("condition\n");
    }
    else if(list->active->cycle == true)
    {
      printf("cycle\n");
    }
    else
    {
      printf("error\n");
    }
    SListPrev(&(*list));
  }
}
