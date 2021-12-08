/*
* Project: IFJ 2021
*
* @file symtable.c
*
* @brief Implementace tabulky symbolů
*
* @author Jan Zdeněk (xzdene01)
* @author Alexander Sila (xsila00)
*/

#include "symtable.h"

int SListInit(SList *list)
{
	//kontrola neexistujiciho seznamu
	if(list == NULL)
	{
		return 99;
	}

	//pripsani vychozich hodnot
	list->first = NULL;
	list->last = NULL;
	list->active = NULL;
	list->lastFunc = NULL;
	return 0;
}

int SListNext(SList *list)
{
	//kontrola neexistujiciho seznamu
	if(list == NULL)
	{
		return 99;
	}

	//kontrola aktivniho prvku
	if(list->active == NULL)
	{
		return 99;
	}
	else
	{
		//posunuti aktivity
		list->active = list->active->next;
	}

	return 0;
}

int SListPrev(SList *list)
{
	//kontrola neexistujiciho seznamu
	if(list == NULL)
	{
		return 99;
	}

	//kontrola aktivniho prvku
	if(list->active == NULL)
	{
		return 99;
	}
	else
	{
		//posunuti aktivity
		list->active = list->active->prev;
	}

	return 0;
}

int SListInsertCond(SList *list)
{
	//kontrola neexistujiciho seznamu
	if(list == NULL)
	{
		return 99;
	}

	//alokace a kontrola alokace noveho prvku
	SNodePtr new = (SNodePtr)malloc(sizeof(struct SNode));
	if(new == NULL)
	{
		return 99;
	}

	//pripsani vychozich hodnot
	new->func = false;
	new->var = false;
	new->cond = true;
	new->cycle = false;

	new->next = NULL;
	new->prev = NULL;

	//vlozeni prvku mezi ostatni prvky
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
	//kontrola neexistujiciho seznamu
	if(list == NULL)
	{
		return 99;
	}

	//alokace a kontrola alokace noveho prvku
	SNodePtr new = (SNodePtr)malloc(sizeof(struct SNode));
	if(new == NULL)
	{
		return 99;
	}

	//pripsani vychozich hodnot
	new->func = false;
	new->var = false;
	new->cond = false;
	new->cycle = true;

	new->next = NULL;
	new->prev = NULL;

	//vlozeni prvku mezi ostatni prvky
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
	//kontrola pradneho seznamu
	if(list == NULL)
	{
		return 99;
	}

	//alokace a kontrola alokace noveho prvku
	SNodePtr new = (SNodePtr)malloc(sizeof(struct SNode));
	if(new == NULL)
	{
		return 99;
	}

	//pripsani vychozich hodnot
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

	//vlozeni noveho prvku do seznamu
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
	int error;

	//kontrola neexistujiciho seznamu
	if(list == NULL)
	{
		return 99;
	}

	//vlozeni tokenu do seznamu parametru
	Token token;
	token.type = tokentype;

	error = LastFunc(list); //nastaveni posledni funkce a kontrola provedeni
	if(error != 0)
	{
		return error;
	}

	error = TListInsert(&list->lastFunc->params, token);
	if(error != 0)
	{
		return error;
	}

	list->lastFunc->numParams ++;
	list->lastFunc->params.last->index = list->lastFunc->numParams;

	return 0;
}

int SListInsertReturn(SList *list, Token_type tokentype)
{
	int error;

	//kontrola neexistujiciho seznamu
	if(list == NULL)
	{
		return 99;
	}

	//vlozeni tokenu do seznamu navratovych typu
	Token token;
	token.type = tokentype;

	error = LastFunc(list); //nastaveni posledni funkce a kontrola provedeni
	if(error != 0)
	{
		return error;
	}

	error = TListInsert(&list->lastFunc->returns, token);
	if(error != 0)
	{
		return error;
	}

	list->lastFunc->numReturns ++;
	list->lastFunc->returns.last->index = list->lastFunc->numReturns;

	return 0;
}

int SListInsertVar(SList *list, Token *token, Token_type tokentype)
{
	//kontrola neexistujiciho seznamu
	if(list == NULL)
	{
		return 99;
	}

	//alokace a kontrola alokace noveho prvku
	SNodePtr new = (SNodePtr)malloc(sizeof(struct SNode));
	if(new == NULL)
	{
		return 99;
	}

	//pripsani vychozich hodnot
	new->var = true;
	new->func = false;
	new->cond = false;
	new->cycle = false;

	new->id_var = token;
	new->type = tokentype;

	new->next = NULL;
	new->prev = NULL;

	//vlozeni noveho prvku do seznamu
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
	int error;

	//kontrola neexistujiciho seznamu
	if(list == NULL)
	{
		return 99;
	}

	if(list->last == NULL) //pro prazdny seznam
	{
		return 0;
	}
	else if(list->first == list->last) //pro jednoprvkovy seznam
	{
		//uvolneni alokovanych seznamu
		if(list->last->func == true)
		{
		  error = TListDispose(&list->last->params);
		  if(error != 0)
		  {
			  return error;
		  }
		  error = TListDispose(&list->last->returns);
		  if(error != 0)
		  {
			  return error;
		  }
		}
		free(list->last);
		list->last = NULL;
		list->first = NULL;
	}
	else //pro viceprvkovy seznam
	{
		//uvolneni alokovanych seznamu
		if(list->last->func == true)
		{
			error = TListDispose(&list->last->params);
  		  if(error != 0)
  		  {
  			  return error;
  		  }
  		  	error = TListDispose(&list->last->returns);
  		  if(error != 0)
  		  {
  			  return error;
  		  }
		}
		list->last = list->last->prev;
		free(list->last->next);
	}
	return 0;
}

int SListDispose(SList *list)
{
	int error;

	//kontrola neexistujiciho seznamu
	if(list == NULL)
	{
		return 99;
	}

	//mazu dokud seznam neni prazdny
	while(list->last != NULL)
	{
		error = SListDeleteLast(&(*list));
		if(error != 0)
		{
			return error;
		}
	}
	return 0;
}

int IsDeclaredFunc(SList *list, Token *token)
{
	int error;

	//kontrola neexistujiciho seznamu
	if(list == NULL)
	{
		return 99;
	}

	//kontrola ze hledany token je identifikator
	if(token->type != T_ID)
	{
		return 99;
	}

	//seznam je prazdny
	if(list->first == NULL)
	{
		return 3;
	}

	//ulozeni hledaneho vyrazu
	char *str = token->value.string;

	//nastaveni aktivity na posledni prvek
	list->active = list->last;

	while(1)
	{
		if(list->active->func == true) //aktivni prvek je funkce
		{
			//dosel jsem na prvni prvek a vyrazy se nerovnaji
			if(list->active == list->first &&
			strcmp(str, list->active->id_func->value.string) != 0)
			{
				return 3;
			}
			//vyrazy se rovnaji
			else if(strcmp(str, list->active->id_func->value.string) == 0)
			{
				//funkce je i definovana
				if(list->active->def_func == true)
				{
					return 1;
				}
				return 0;
			}
		}
		else //aktivni prvek neni funkce
		{
			if(list->active == list->first) //dosel jsem na zacatek seznamu
			{
				return 3;
			}
		}
		error = SListPrev(&(*list)); //posunuti aktivity doleva
		if(error != 0)
		{
			return error;
		}
	}
}

int IsDeclaredJump(SList *list, Token *token)
{
	int error;

	//kontrola neexistujiciho seznamu
	if(list == NULL)
	{
		return 99;
	}

	//kontrola ze hledany token je identifikator
	if(token->type != T_ID)
	{
		return 99;
	}

	//seznam je prazdny
	if(list->first == NULL)
	{
		return 3;
	}

	//ulozeni hledaneho vyrazu
	char *str = token->value.string;

	//nastaveni posledni funkce
	error = LastFunc(list);
	if(error != 0)
	{
		return error;
	}

	if(list->lastFunc->prev == NULL)
	{
		return 3;
	}

	//aktivni prvek se nastavi doleva od posledni funkce
	list->active = list->lastFunc->prev;


	while(1)
	{
		if(list->active->func == true) //aktivni prvek je funkce
		{
			//dosel jsem na prvni prvek a vyrazy se nerovnaji
			if(list->active == list->first &&
			strcmp(str, list->active->id_func->value.string) != 0)
			{
				return 3;
			}
			//vyrazy se rovnaji
			else if(strcmp(str, list->active->id_func->value.string) == 0)
			{
				//funkce je i definovana
				if(list->active->def_func == true)
				{
					return 1;
				}
				return 0;
			}
		}
		else //aktivni prvek neni funkce
		{
			if(list->active == list->first) //dosel jsem na zacatek seznamu
			{
				return 3;
			}
		}
		error = SListPrev(&(*list)); //posunuti aktivity doleva
		if(error != 0)
		{
			return error;
		}
	}
}

int IsDeclaredVar(SList *list, Token *token)
{
	int error;

	//kontrola neexistujiciho seznamu
	if(list == NULL)
	{
		return 99;
	}

	//kontrola ze hledany token je identifikator
	if(token->type != T_ID)
	{
		return 99;
	}

	//nastaveni aktivity na posledni prvek
	list->active = list->last;

	while(1)
	{
		if(list->active->var == true) //aktivni prvek je promenna
		{
			//dosel jsem na prvni prvek a vyrazy se nerovnaji
			if(list->active == list->first &&
			strcmp(token->value.string, list->active->id_var->value.string) != 0)
			{
				return 3;
			}
			//vyrazy se rovnaji
			else if(strcmp(token->value.string, list->active->id_var->value.string) == 0)
			{
				return 0;
			}
		}
		else //aktivni prvek neni promenna
		{
			if(list->active == list->first) //dosel jsem na zacatek seznamu
			{
				return 3;
			}
		}
		error = SListPrev(&(*list)); //posunuti aktivity doleva
		if(error != 0)
		{
			return error;
		}
	}

	return 99;
}

int IsDeclaredVarInScope(SList *list, Token *token)
{
	int error;

	//kontrola neexistujiciho seznamu
	if(list == NULL)
	{
		return 99;
	}

	//kontrola ze hledany token je identifikator
	if(token->type != T_ID)
	{
		return 99;
	}

	//nastaveni aktivity na posledni prvek
	list->active = list->last;

	while(1)
	{
		if(list->active->var == true) //aktivni prvek je promenna
		{
			//dosel jsem na konec scopu a vyrazy se nerovnaji
			if((list->active == list->first || list->active->cond == true || list->active->cycle == true) &&
			strcmp(token->value.string, list->active->id_var->value.string) != 0)
			{
				return 3;
			}
			//vyrazy se rovnaji
			else if(strcmp(token->value.string, list->active->id_var->value.string) == 0)
			{
				return 0;
			}
		}
		else //aktivni prvek neni promenna
		{
			//dosel jsem na konec scopu
			if(list->active == list->first || list->active->cond == true || list->active->cycle == true)
			{
				return 3;
			}
		}
		error = SListPrev(&(*list)); //posunuti aktivity doleva
		if(error != 0)
		{
			return error;
		}
	}
}

int DeleteScope(SList *list)
{
	int error;

	//kontrola neexistujiciho seznamu
	if(list == NULL)
	{
		return 99;
	}

	//kontrola prazdneho seznamu
	if(list->last == NULL)
	{
		return 0;
	}

	//dokud prvek bude promenna tak mazu
	while(list->last->var == true)
	{
		error = SListDeleteLast(list);
		if(error != 0)
		{
			return error;
		}
	}

	//pokud je prvek podminka nebo cyklus taky mazu
	if(list->last->func != 1)
	{
		error = SListDeleteLast(list);
		if(error != 0)
		{
			return error;
		}
	}
	return 0;
}

int IsInteger(SList *list, Token *token)
{
	//kontrola deklarace promenne
	if(IsDeclaredVar(list, token) == 0)
	{
		//kontrola ze promenna je integer
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
	//kontrola deklarace promenne
	if(IsDeclaredVar(list, token) == 0)
	{
		//kontrola ze promenna je integer
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
	//kontrola deklarace promenne
	if(IsDeclaredVar(list, token) == 0)
	{
		//kontrola ze promenna je integer
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
	//kontrola deklarace promenne
	if(IsDeclaredVar(list, token) == 0)
	{
		//kontrola ze promenna je integer
		if(list->active->type == T_KW_NIL)
		{
			return 0;
		}
		return 4;
	}
	return 3;
}

int LastFunc(SList *list)
{
	//kontrola pro neexistujici seznam
	if(list == NULL)
	{
		return 99;
	}

	//kontrola pro prazdny seznam
	if(list->first == NULL)
	{
		return 99;
	}

	//nastaveni posledni funkce na posledni prvek v seznamu
	list->lastFunc = list->last;

	//dokud poslkedni funkce nebude funkce
	while(list->lastFunc->func != true)
	{
		//doslo misto v seznamu
		if(list->lastFunc->prev == NULL)
		{
			return 99;
		}

		//posouvam posledni funkci doleva
		list->lastFunc = list->lastFunc->prev;
	}
	return 0;
}

int IsClone(SNodePtr node1, SNodePtr node2)
{
	if(node1 == NULL || node2 == NULL)
	{
		return 99;
	}

	if(node1->func == true)
	{
		if(node2->func == false)
		{
			return 1;
		}

		if(node1->numParams != node2->numParams)
		{
			return 1;
		}

		node1->params.active = node1->params.first;
		node2->params.active = node2->params.first;
		for(int i = 0; i < node1->numParams; i++)
		{
			if(node1->params.active->token.type != node2->params.active->token.type)
			{
				if(node1->params.active->token.type == T_KW_INTEGER &&
					node2->params.active->token.type == T_KW_NUMBER)
				{
					node1->params.active = node1->params.active->next;
					node2->params.active = node2->params.active->next;
				}
				else
				{
					return 1;
				}
			}
			else
			{
				node1->params.active = node1->params.active->next;
				node2->params.active = node2->params.active->next;
			}
		}

		if(node1->numReturns != node2->numReturns)
		{
			return 2;
		}

		node1->returns.active = node1->returns.first;
		node2->returns.active = node2->returns.first;
		for(int i = 0; i < node1->numReturns; i++)
		{
			if(node1->returns.active->token.type != node2->returns.active->token.type)
			{
				if(node2->returns.active->token.type == T_KW_INTEGER &&
					node1->returns.active->token.type == T_KW_NUMBER)
				{
					node1->returns.active = node1->returns.active->next;
					node2->returns.active = node2->returns.active->next;
				}
				else
				{
					return 2;
				}
			}
			else
			{
				node1->returns.active = node1->returns.active->next;
				node2->returns.active = node2->returns.active->next;
			}
		}

		return 0;
	}

	return 0;
}

/*------------DOCASNE FUNKCE------------------------------------------*/

void PrintSList(SList *list)
{
  if(list == NULL)
  {
    return;
  }

  if(list->last == NULL)
  {
    fprintf(stderr, "empty list\n");
  }

  list->active = list->last;

  while(list->active != NULL)
  {
    if(list->active->func == true)
    {
      fprintf(stderr, "function :: def(%d)\n", list->active->def_func);
      fprintf(stderr, "\tname : %s\n", list->active->id_func->value.string);
      fprintf(stderr, "\tparams (%d) : ", list->active->numParams);
      list->active->params.active = list->active->params.first;
      while(list->active->params.active != NULL)
      {
        PrintToken(list->active->params.active->token.type);
        fprintf(stderr, "(%d) ", list->active->params.active->index);
        TListTokenNext(&list->active->params);
      }
      fprintf(stderr, "\n");
      fprintf(stderr, "\treturns (%d) : ", list->active->numReturns);
      list->active->returns.active = list->active->returns.first;
      while(list->active->returns.active != NULL)
      {
        PrintToken(list->active->returns.active->token.type);
        fprintf(stderr, "(%d) ", list->active->returns.active->index);
        TListTokenNext(&list->active->returns);
      }
      fprintf(stderr, "\n");
    }
    else if(list->active->var == true)
    {
      fprintf(stderr, "variable\n");
      fprintf(stderr, "\tname : %s\n", list->active->id_var->value.string);
      fprintf(stderr, "\ttype : ");
      PrintToken(list->active->type);
      fprintf(stderr, "\n");
    }
    else if(list->active->cond == true)
    {
      fprintf(stderr, "condition\n");
    }
    else if(list->active->cycle == true)
    {
      fprintf(stderr, "cycle\n");
    }
    else
    {
      fprintf(stderr, "error\n");
    }
    SListPrev(&(*list));
  }
}
