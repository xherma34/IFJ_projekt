#include "parser.h"

/*-----FUNKCE MIMO PROGRAM-----*/

int Parse(TList *list, SList *slist)
{
    //pomocna promenna pro chybu
    int error;

    //aktivni prvek se nastavi na zacatek, aby se mohl prochazet cely list
    list->active = list->first;

    //provedu kontrolu pro program, ulozim navratovou hodnotu
    error = Program(list, slist);

    //navraceni chyby do funkce main() v main.c
    return error;
}

/*-----FUNKCE NA UROVNI CELEHO PROGRAMU-----*/

int Program(TList *list, SList *slist)
{
    //pomocna promenna pro chybu
    int error;

    //kontrola pro token T_KW_REQUIRE
    if(list->active->token.type != T_KW_REQUIRE)
    {
        //v pripade absence tokenu vracim 2 (syntax error)
        return 2;
    }

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //kontrola pro token T_STRING a kontrola jeji hodnoty
    if(list->active->token.type != T_STRING ||
      strcmp(list->active->token.value.string, "ifj21") != 0)
    {
        //v pripade absence tokenu nebo nekorektnosti hodnoty vracim 2 (syntax error)
        return 2;
    }

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //kontrola pro token T_EOF
    if(list->active->token.type != T_EOF)
    {
        //v pripade absence program obsahuje hlavni telo
        error = MainBody(list, slist);
    }

    //vracim chybu
    return error;
}

int MainBody(TList *list, SList *slist)
{
    //pomocna promenna pro chybu
    int error = 0;

    //kontrola validniho tokenu
    if(list->active->token.type != T_KW_GLOBAL &&
      list->active->token.type != T_KW_FUNCTION &&
      list->active->token.type != T_ID)
    {
        //v pripade absence tokenu vracim 2 (syntax error)
        return 2;
    }

    //kontrola pro token T_KW_GLOBAL
    if(list->active->token.type == T_KW_GLOBAL)
    {
        //jedna se o deklaraci funkce, posun aktivity v seznamu na dalsi prvek
        error = TListTokenNext(list);
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }

        //provedu kontrolu pro deklaraci funkce a ulozim navratovou hodnotu
        error = DecFunction(list, slist);
    }
    else if(list->active->token.type == T_KW_FUNCTION)
    {
        //jedna se o definici funkce, posun aktivity v seznamu na dalsi prvek
        error = TListTokenNext(list);
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }

        //provedu kontrolu pro definici funkce a ulozim navratovou hodnotu
        error = DefFunction(list, slist);
    }

    else if(list->active->token.type == T_ID)
    {

		if(IsDeclaredFunc(slist, &list->active->token) > 1)
		{
			return IsDeclaredFunc(slist, &list->active->token);
		}

        //jedna se o volani funkce, posun aktivity v seznamu na dalsi prvek
        error = TListTokenNext(list);
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }

        //provedu kontrolu pro volani funkce a ulozim navratovou hodnotu
        error = CallFunction(list, slist);
    }

    //pokud jsem nasel chybu nebo jsem na konci hlavniho tela
    if(error != 0 || list->active->token.type == T_EOF)
    {
        //vratim hodnotu chyby
        return error;
    }

    //rekurzivne opakuju kontrolu hlavniho tela dokud nenastane chyba nebo EOF
    error = MainBody(list, slist);

    //vracim hodnotu chyby
    return error;
}

/*-----FUNKCE NA UROVNI HLAVNIHO TELA PROGRAMU-----*/

int DecFunction(TList *list, SList *slist)
{
    //pomocna promenna pro chybu
    int error;

    //kontrola pro token T_ID
    if(list->active->token.type != T_ID)
    {
        //v pripade absence tokenu vracim 2 (syntax error)
        return 2;
    }

    if(IsDeclaredFunc(slist, &(list->active->token)) == 3)
    {
      SListInsertFunc(slist, false, &list->active->token);
    }
    else
    {
      return 3;
    }

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //kontrola pro token T_COLON
    if(list->active->token.type != T_COLON)
    {
        //v pripade absence tokenu vracim 2 (syntax error)
        return 2;
    }

    //posunuti aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //kontrola pro token T_KW_FUNCTION
    if(list->active->token.type != T_KW_FUNCTION)
    {
        //v pripade absence tokenu vracim 2 (syntax error)
        return 2;
    }

    //posunuti aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //kontrola pro token T_BRACKET_LEFT
    if(list->active->token.type != T_BRACKET_LEFT)
    {
        //v pripade absence vracim 2 (syntax error)
        return 2;
    }

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //kontrola pro token T_BRACKET_RIGHT
    if(list->active->token.type != T_BRACKET_RIGHT)
    {
        //seznam datatypu neni prazdny
        //provedu kontrolu pro datatypy a ulozim navratovou hodnotu
		int commaCount = 0;
		int varCount = 0;
        error = DataTypes(list, slist, &commaCount, varCount);
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }
    }

    //kontrola pro token T_BRACKET_RIGHT
    if(list->active->token.type != T_BRACKET_RIGHT)
    {
        //v pripade absence vracim 2 (syntax error)
        return 2;
    }

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //provedu kontrolu pro vracene typy a ulozim navratovou hodnotu
    error = ReturnTypes(list, slist, false);

    //vratim chybu
    return error;
}

int DefFunction(TList *list, SList *slist)
{
    int error = 0;

    //kontrola pro token T_ID
    if(list->active->token.type != T_ID)
    {
        return 2;
    }

    if(IsDeclaredFunc(slist, &list->active->token) != 1)
    {
      SListInsertFunc(slist, true, &list->active->token);
    }
    else
    {
      return 3;
    }

    //posunuti aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //kontrola pro token T_BRACKET_LEFT
    if(list->active->token.type != T_BRACKET_LEFT)
    {
        //v pripade absence tokenu vracim 2 (syntax error)
        return 2;
    }

    //posunuti aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //kontrola pro token T_BRACKET_RIGHT
    if(list->active->token.type != T_BRACKET_RIGHT)
    {
        //v pripade absence tokenu definice funkce obsahuje parametry
        //provedu kontrolu pro parametry funkce a ulozim navratovou hodnotu
        error = Params(list, slist);
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }
    }

	LastFunc(slist);
    error = IsDeclaredJump(slist, slist->lastFunc->id_func);

    if(error != 3)
    {
      if(slist->active->numParams != slist->lastFunc->numParams)
      {
        return 5;
      }
    }

    //kontrola pro token T_BRACKET_RIGHT
    if(list->active->token.type != T_BRACKET_RIGHT)
    {
        //v pripade absence tokenu vracim 2 (syntax error)
        return 2;
    }

    //posunuti aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //provedu kontrolu pro vracene typy a ulozim navratovou hodnotu
    error = ReturnTypes(list, slist, true);
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

	LastFunc(slist);
    error = IsDeclaredJump(slist, slist->lastFunc->id_func);
    if(error != 3)
    {
      if(slist->active->numReturns != slist->lastFunc->numReturns)
      {
        return 5;
      }
    }

    //kontrola pro token T_KW_END
    if(list->active->token.type != T_KW_END)
    {
        //v pripade absence tokenu definice funkce obsahuje telo
        //provedu kontrolu pro telo funkce a ulozim navratovou hodnotu
        error = FceBody(list, slist);
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }
    }

    //kontrola pro token T_KW_END
    if(list->active->token.type != T_KW_END)
    {
        //v pripade absence tokenu vracim 2 (syntax error)
        return 2;
    }

	DeleteScope(slist);

    //posunuti aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);

    //vracim chybu
    return error;
}

int CallFunction(TList *list, SList *slist)
{
    //pomocna promenna pro chybu
    int error;

    //kontrola pro token T_BRACKET_LEFT
    if(list->active->token.type != T_BRACKET_LEFT)
    {
        //v pripade absence tokenu vracim 2 (syntax error)
        return 2;
    }

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //kontrola pro token T_BRACKET_RIGHT
    if(list->active->token.type != T_BRACKET_RIGHT)
    {
        //v pripade absence tokenu volani funkce obsahuje parametry
		int commaCount = 0;
        error = Ids_Datatypes(list, slist, &commaCount);
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }

		if(slist->active->numParams != commaCount + 1)
		{
			return 5;
		}
    }

    //kontrola pro token T_BRACKET_RIGHT
    if(list->active->token.type != T_BRACKET_RIGHT)
    {
        //v pripade absence tokenu vracim 2 (syntax error)
        return 2;
    }

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //vratim chybovou hodnotu
    return 0;
}

/*-----FUNKCE NA UROVNI DEFINICE FUNKCE-----*/

int FceBody(TList *list, SList *slist)
{
    //pomocna promenna pro chybu
    int error = 0;

    //kontrola pro validni token
    if(list->active->token.type != T_KW_LOCAL &&
      list->active->token.type != T_KW_IF &&
      list->active->token.type != T_KW_WHILE &&
      list->active->token.type != T_ID &&
      list->active->token.type != T_KW_RETURN)
    {
        //v pripade absence validniho tokenu vracim 2 (syntax error)
        return 2;
    }

    //kontrola pro token T_KW_LOCAL
    if(list->active->token.type == T_KW_LOCAL)
    {
        //jedna se o definici promenne, posun aktivity v seznamu na dalsi prvek
        error = TListTokenNext(list);
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }

        //provedu kontrolu pro definici promenne a ulozim navratovou hodnotu
        error = DefVar(list, slist);
    }
    //kontrola pro token T_KW_IF
    else if(list->active->token.type == T_KW_IF)
    {
        //jedna se o podminku, posun aktivity v seznamu na dalsi prvek
        error = TListTokenNext(list);
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }

        //provedu kontrolu pro podminku a ulozim navratovou hodnotu
        error = Cond(list, slist);
    }
    //kontrola pro token T_KW_WHILE
    else if(list->active->token.type == T_KW_WHILE)
    {
        //jedna se o cyklus, posun aktivity v seznamu na dalsi prvek
        error = TListTokenNext(list);
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }

        //provedu kontrolu pro cyklus a ulozim navratovou hodnotu
        error = Cycle(list, slist);
    }
    //kontrola pro token T_ID
    else if(list->active->token.type == T_ID)
    {
        //jedna se o volani funkce nebo pripsani hodnoty promenne
        //posunu aktivitu v seznamu na dalsi prvek
        error = TListTokenNext(list);
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }

        //kontrola pro token T_BRACKET_LEFT
        if(list->active->token.type == T_BRACKET_LEFT)
        {
            //jedna se o volani funkce

			TListTokenPrev(list);

			if(IsDeclaredFunc(slist, &list->active->token) > 1)
			{
				return 3;
			}

			TListTokenNext(list);

            //provedu kontrolu pro volani funkce a ulozim navratovou hodnotu
            error = CallFunction(list, slist);
        }
        else
        {
            //jedna se o pripsani hodnoty promenne
            //posunu aktivitu v seznamu na predchozi prvek
            TListTokenPrev(list);

            //provedu kontrolu pro pripsani hodnoty promenne a ulozim navratovou hodnotu
            error = Assign(list, slist);
			// printf("%d\n", error);
			// PrintToken(list->active->token.type);
        }
    }
    //kontrola pro token T_KW_RETURN
    else if(list->active->token.type == T_KW_RETURN)
    {
        //jedna se o return, posunu aktivitu v seznamu na dalsi prvek
        error = TListTokenNext(list);
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }

        //provedu kontrolu pro return a ulozim navratovou hodnotu
        error = Return(list, slist);
    }

    //pokud se naskytly chyba a nebo jsem na konci tela funkce vratim chybu
    if(error != 0 ||
      list->active->token.type == T_KW_END ||
      list->active->token.type == T_KW_ELSE)
    {
        return error;
    }

    //rekurzivne provedu kontrolu pro telo funkce a ulozim navratovou hodnotu
    error = FceBody(list, slist);

    //vratim chybu
    return error;
}

/*-----FUNKCE NA UROVNI JEDNOTLIVYCH TEL FUNKCI-----*/

int DefVar(TList *list, SList *slist)
{
    //pomocna promenna pro chybu
    int error;

    if(list->active->token.type != T_ID)
	{
		return 2;
	}

	if(IsDeclaredFunc(slist, &list->active->token) < 2)
	{
		return 3;
	}

	if(IsDeclaredVarInScope(slist, &list->active->token) == 0)
	{
		return 3;
	}

	error = SListInsertVar(slist, &list->active->token, T_EMPTY);
	if(error != 0)
	{
		return error;
	}

	error = TListTokenNext(list);
	if(error != 0)
	{
		return error;
	}

    //kontrola pro token T_COLON
    if(list->active->token.type != T_COLON)
    {
        return 2;
    }

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //provedu kontrolu pro datatypy a ulozim navratovou hodnotu
    if(list->active->token.type == T_KW_INTEGER ||
		list->active->token.type == T_KW_NUMBER ||
		list->active->token.type == T_KW_STRING ||
		list->active->token.type == T_KW_NIL)
	{
		slist->last->type = list->active->token.type;
	}
	else
	{
		return 2;
	}

	error = TListTokenNext(list);
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //kontrola pro token T_SETVALUE
    if(list->active->token.type != T_SETVALUE)
    {
        return error;
    }

	//Token_type type;

	error = Exp(list, slist/*, &type*/);
	if(error != 0)
	{
		if(error == 1)
		{
			return 2;
		}
		else
		{
			return error;
		}
	}

	/*if(type != slist->last->type)
	{
		return 4;
	}*/
    return error;
}

int Assign(TList *list, SList *slist)
{
    //pomocna promenna pro chybu
    int error;

    //provedu kontrolu pro identifikatory a ulozim navratovou hodnotu
	int varCount = 0;
    error = Ids(list, slist, &varCount);
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //kontrola pro token T_SETVALUE
    if(list->active->token.type != T_SETVALUE)
    {
        //v pripade absence tokenu vracim 2 (syntax error)
        return 2;
    }

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

	int commaCount = 0;
    error = Exps(list, slist, varCount, &commaCount);

	if(varCount != commaCount)
	{
		printf("%d : %d\n", varCount, commaCount);
		return 7;
	}

    //vratim chybu
    return error;
}

int Cond(TList *list, SList *slist)
{
    //pomocna promenna pro chybu
    int error;

    //provedu kontrolu pro expresi a ulozim navratovou hodnotu
    error = Exp(list, slist);
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //kontrola pro token T_KW_WHEN
    if(list->active->token.type != T_KW_THEN)
    {
        //v pripade absence tokenu vracim 2 (syntax error)
        return 2;
    }

	SListInsertCond(slist);

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //kontrola pro token T_KW_ELSE
    if(list->active->token.type != T_KW_ELSE)
    {
        //telo podminky neni prazdne
        //provedu kontrolu pro telo funkce a ulozim navratovou hodnotu
        error = FceBody(list, slist);
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }
    }
    //kontrola pro token T_KW_ELSE
    if(list->active->token.type != T_KW_ELSE)
    {
        //v pripade absence tokenu vracim 2 (syntax error)
        return 2;
    }

	DeleteScope(slist);
	SListInsertCond(slist);


    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //kontrola pro token T_KW_END
    if(list->active->token.type == T_KW_END)
    {
		DeleteScope(slist);
        //telo else v podmince je prazdne
        //posun aktivity v seznamu na dalsi prvek
        error = TListTokenNext(list);
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }

        //vracim 0 (no error)
        return 0;
    }

    //provedu kontrolu pro telo funkce a ulozim navratovou hodnotu
    error = FceBody(list, slist);
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //kontrola pro token T_KW_END
    if(list->active->token.type != T_KW_END)
    {
        //v pripade absence tokenu vracim 2 (syntax error)
        return 2;
    }

	DeleteScope(slist);

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);

    //vratim chybu
    return error;
}

int Cycle(TList *list, SList *slist)
{
    //pomocna promenna pro chybu
    int error;

    //provedu kontrolu pro expresi a ulozim navratovou hodnotu
    error = Exp(list, slist);
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //kontrola pro token T_KW_DO
    if(list->active->token.type != T_KW_DO)
    {
        //v pripade absence tokenu vracim 2 (syntax error)
        return 2;
    }

	SListInsertCycle(slist);

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //provedu kontrolu pro telo funkce a ulozim navratovou hodnotu
    error = FceBody(list, slist);
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //kontrola pro token T_KW_END
    if(list->active->token.type != T_KW_END)
    {
        //v pripade absence tokenu vracim 2 (syntax error)
        return 2;
    }

	DeleteScope(slist);

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);

    //vratim chybu
    return error;
}

int Return(TList *list, SList *slist)
{
	int error;

	if(list == NULL)
	{
		return 99;
	}

	slist->active = slist->last;
    while(slist->active->id_func == NULL)
	{
		if(slist->active->prev != NULL)
		{
			SListPrev(slist);
		}
		else
		{
			return 99;
		}
	}

	int expCount = 0;
	//Token_type type;

	slist->active->returns.active = slist->active->returns.first;

	while(1)
	{
		error = Exp(list, slist/*, &type*/);
		if(error != 0)
		{
			if(error == 1)
			{
				if(expCount == 1)
				{
					return 0;
				}
				return 2;
			}
			return error;
		}

		expCount++;

		if(expCount > slist->active->numReturns)
		{
			printf("%d : %d\n", expCount, slist->active->numReturns);
			return 5;
		}

		/*if(slist->active->returns->active->type == type)
		{
			slist->active->returns->active = slist->active->returns->active->next;
		}
		else
		{
			return 5;
		}*/

		if(list->active->token.type != T_COMMA)
		{
			return error;
		}
		else
		{
			error = TListTokenNext(list);
			if(error != 0)
			{
				return error;
			}
		}
	}

    //vratim 0 -> return nemuze vracet nic jineho
    return 0;
}

/*-----POMOCNE FUNKCE PRO KONTROLU SYNTAXE-----*/

int DataTypes(TList *list, SList *slist, int *commaCount, int varCount)
{
    //pomocna promenna pro chybu
    int error;

    //provedu kontrolu pro datovy typ a ulozim navratovou hodnotu
    error = DataType(list, slist, false);

	TListTokenPrev(list);
	slist->active = slist->last;
	if(varCount < *commaCount)
	{
		return 7;
	}
	for(int i = 0; i < varCount - (*commaCount); i++)
	{
		SListPrev(slist);
	}
	slist->active->type = list->active->token.type;
	slist->active = slist->last;
	TListTokenNext(list);

    //pokud nastala chyba nebo uz nenasleduje dalsi datovy typ
    if(error != 0 || list->active->token.type != T_COMMA)
    {
        //vratim chybu
        return error;
    }

	(*commaCount)++;

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //provedu kontrolu pro datove typy a ulozim navratovou hodnotu
    error = DataTypes(list, slist, commaCount, varCount);

    //vratim
    return error;
}

int DataType(TList *list, SList *slist, bool isDef)
{
  if(slist == NULL)
  {
    return 99;
  }
    //pomocna promenna pro chybu
    int error;

    //kontrola pro validni token
    if(list->active->token.type == T_KW_INTEGER || list->active->token.type == T_KW_NUMBER
    || list->active->token.type == T_KW_STRING || list->active->token.type == T_KW_NIL)
    {
        //v pripade validniho typu prepisu chybu na 0 (no error)
        error = 0;

		SListInsertParam(slist, list->active->token.type);

        if(isDef)
        {
			LastFunc(slist);
          error = IsDeclaredJump(slist, slist->lastFunc->id_func);

          if(error != 3)
          {
            slist->active->params.active = slist->active->params.first;
            for(int i = 0; i < slist->lastFunc->params.last->index - 1; i++)
            {
              if(slist->active->params.active->next == NULL)
              {
                return 5;
              }
              TListTokenNext(&slist->active->params);
            }
            if(slist->lastFunc->params.last->token.type != slist->active->params.active->token.type)
            {
              return 5;
            }
          }
        }
    }

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);

    return error;
}

int DataTypeReturn(TList *list, SList *slist, bool isDef)
{
  if(slist == NULL)
  {
    return 99;
  }
    //pomocna promenna pro chybu
    int error;

    //kontrola pro validni token
    if(list->active->token.type == T_KW_INTEGER || list->active->token.type == T_KW_NUMBER
    || list->active->token.type == T_KW_STRING || list->active->token.type == T_KW_NIL)
    {
        //v pripade validniho typu prepisu chybu na 0 (no error)
        error = 0;

        SListInsertReturn(slist, list->active->token.type);
        if(isDef)
        {
			LastFunc(slist);
          error = IsDeclaredJump(slist, slist->lastFunc->id_func);

          if(error != 3)
          {
            slist->active->returns.active = slist->active->returns.first;
            for(int i = 0; i < slist->lastFunc->returns.last->index - 1; i++)
            {
              if(slist->active->returns.active->next == NULL)
              {
                return 5;
              }
              TListTokenNext(&slist->active->returns);
            }
			if(slist->active->returns.first == NULL)
			{
				return 5;
			}
            if(slist->lastFunc->returns.last->token.type != slist->active->returns.active->token.type)
            {
              return 5;
            }
          }
        }
    }

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);

    return error;
}

int Params(TList *list, SList *slist)
{
    //pomocna promenna pro chybu
    int error;

    //provedu kontrolu pro parametr a ulozim navratovou hodnotu
    error = Param(list, slist);
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //kontrola pro token T_COMMA
    if(list->active->token.type != T_COMMA)
    {
        //v pripade absence tokenu vracim chybu
        return error;
    }

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //provedu kontrolu pro parametry a ulozim navratovou hodnotu
    error = Params(list, slist);


    //vratim chybu
    return error;
}

int Param(TList *list, SList *slist)
{
    //pomocna promenna pro chybu
    int error;

    //kontrola pro token T_ID
    if(list->active->token.type != T_ID)
    {
        //v pripade absence tokenu vracim 2 (syntax error)
        return 2;
    }

	if(IsDeclaredFunc(slist, &list->active->token) < 2)
	{
		return 3;
	}

	SListInsertVar(slist, &list->active->token, T_EMPTY);

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //kontrola pro token T_COLON
    if(list->active->token.type != T_COLON)
    {
        //v pripade absence tokenu vracim 2 (syntax error)
        return 2;
    }

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    error = DataType(list, slist, true);
	if(error != 0)
	{
		return error;
	}

	TListTokenPrev(list);
	slist->last->type = list->active->token.type;
	TListTokenNext(list);

    //vratim chybu
    return error;
}

int ReturnTypes(TList *list, SList *slist, bool isDef)
{
    //pomocna promenna pro chybu
    int error;

    //kontrola pro token T_COLON
    if(list->active->token.type == T_COLON)
    {
        //posun aktivity v seznamu na dalsi prvek
        error = TListTokenNext(list);
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }

        //provedu kontrolu pro vraceny typ a ulozim navratovou hodnotu
        error = ReturnType(list, slist, isDef);

        //vratim chybu
        return error;
    }
    else
    {
        //vracene typy jsou prazdne, vratim 0 (no error)
        return 0;
    }
}

int ReturnType(TList *list, SList *slist, bool isDef)
{
    //pomocna promenna pro chybu
    int error;

    //provedu kontrolu pro datovy typ a ulozim navratovou hodnotu
    error = DataTypeReturn(list, slist, isDef);
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //kontrola pro token T_COMMA
    if(list->active->token.type == T_COMMA)
    {
        //nasleduje dalsi vraceny typ
        //posun aktivity v seznamu na dalsi prvek
        error = TListTokenNext(list);
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }

        //rekurzivne provedu kontrolu pro vraceny typ a ulozim navratovou hodnotu
        error = ReturnType(list, slist, isDef);
    }

    return error;
}

int Ids(TList *list, SList *slist, int *varCount)
{
    //pomocna promenna pro chybu
    int error;

    //kontrola pro token T_ID
    if(list->active->token.type != T_ID)
    {
        //v pripade absence tokenu vracim 2 (syntax error)
        return 2;
    }

	if(IsDeclaredVar(slist, &list->active->token) != 0)
	{
		return IsDeclaredVar(slist, &list->active->token);
	}

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //kontrola pro token T_COMMA
    if(list->active->token.type == T_COMMA)
    {
		(*varCount)++;
        //posun aktivity v seznamu na dalsi prvek
        error = TListTokenNext(list);
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }

        //provedu kontrolu pro identifikatory a ulozim navratovou hodnotu
        error = Ids(list, slist, varCount);
    }

    //vratim chybu
    return error;
}

int Exps(TList *list, SList *slist, int varCount, int *commaCount)
{
    //pomocna promenna pro chybu
    int error;
	if(varCount < (*commaCount))
	{
		return 7;
	}

	//Token_type type;
	error = Exp(list, slist/*, &type*/);
	if(error != 0)
	{
		//v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
		return error;
	}

	slist->active = slist->last;
	for(int i = 0; i < varCount - (*commaCount); i++)
	{
		error = SListPrev(slist);
		if(error != 0)
		{
			return error;
		}
	}

	// if(slist->active->type != type)
	// {
	// 	return 0;
	// }


    //kontrola pro token T_COMMA
    if(list->active->token.type == T_COMMA)
    {
        //posun aktivity v seznamu na dalsi prvek
        error = TListTokenNext(list);
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }

        //provedu kontrolu pro exprese a ulozim navratovou hodnotu
		(*commaCount)++;
        error = Exps(list, slist, varCount, commaCount);
    }

    //vratim chybu
    return error;
}

int Ids_Datatypes(TList *list, SList *slist, int *commaCount)
{
    //pomocna promenna pro chybu
    int error;

    //kontrola pro validni token
    if(list->active->token.type != T_ID && list->active->token.type != T_STRING &&
    list->active->token.type != T_NUM_INTEGER && list->active->token.type != T_NUM_NUMBER &&
    list->active->token.type != T_KW_NIL)
    {
        //v pripade absence tokenu vracim 2 (syntax error)
        return 2;
    }

	slist->active->params.active = slist->active->params.first;
	for(int i = 0; i < *commaCount; i++)
	{
		TListTokenNext(&slist->active->params);
	}

	if(list->active->token.type == T_NUM_INTEGER && slist->active->params.active->token.type != T_KW_INTEGER)
	{
		return 5;
	}
	else if(list->active->token.type == T_STRING && slist->active->params.active->token.type != T_KW_STRING)
	{
		return 5;
	}
	else if(list->active->token.type == T_NUM_NUMBER && slist->active->params.active->token.type != T_KW_NUMBER)
	{
		return 5;
	}
	else if(list->active->token.type == T_KW_NIL && slist->active->params.active->token.type != T_KW_NIL)
	{
		return 5;
	}
	else if(list->active->token.type == T_ID)
	{
		if(IsDeclaredVar(slist, &list->active->token) != 0)
		{
			return 3;
		}
		else
		{
			if(IsInteger(slist, &list->active->token) == 0)
			{
				if(slist->active->params.active->token.type != T_KW_INTEGER)
				{
					return 5;
				}
			}
			else if(IsNumber(slist, &list->active->token) == 0)
			{
				if(slist->active->params.active->token.type != T_KW_NUMBER)
				{
					return 5;
				}
			}
			else if(IsString(slist, &list->active->token) == 0)
			{
				if(slist->active->params.active->token.type != T_KW_STRING)
				{
					return 5;
				}
			}
			else if(IsNil(slist, &list->active->token) == 0)
			{
				if(slist->active->params.active->token.type != T_KW_NIL)
				{
					return 5;
				}
			}
		}
	}

	//posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //kontrola pro token T_COMMA
    if(list->active->token.type == T_COMMA)
    {
		(*commaCount)++;
        //nasleduje dalsi identifikator nebo datovy typ
        //posun aktivity v seznamu na dalsi prvek
        error = TListTokenNext(list);
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }

        //provedu kontrolu pro identifikatory nebo datove typy a ulozim navratovou hodnotu
        error = Ids_Datatypes(list, slist, commaCount);
    }

    //vratim chybu
    return error;
}

int Exps_Strings(TList *list, SList *slist, int varCount)
{
    //pomocna promenna pro chybu
    int error = 0;

    //kontrola pro token T_STRING
    // if(list->active->token.type != T_STRING)
    // {
    //     //nasledujici vyraz neni string -> musi byt exprese
    //     //provedu kontrolu pro expresi a ulozim navratovou hodnotu
    //     error = Exp(list, slist);
    //     if(error != 0)
    //     {
    //       //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
    //       return error;
    //     }
    // }
    // else
    // {
    //     //posun aktivity v seznamu na dalsi prvek
    //     error = TListTokenNext(list);
    //     if(error != 0)
    //     {
    //         //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
    //         return error;
    //     }
    // }

	error = Exp(list, slist);

	if(error!= 0)
	{
		if(list->active->token.type != T_STRING)
		{
			return 2;
		}
	}

    //kontrola pro token T_COMMA
    if(list->active->token.type == T_COMMA)
    {
        //jeste nejseme na konci seznamu expresi a stringu
        //posun aktivity v seznamu na dalsi prvek
        error = TListTokenNext(list);
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }

        //rekurzivne provedu kontrolu pro exprese nebo retezce a ulozim navratovou hodnotu
        error = Exps_Strings(list, slist, varCount);
    }

    //vratim chybu
    return error;
}

/*-----DOCASNE POMOCNE FUNKCE-----*/

int Exp(TList *list, SList *slist)
{

  if(slist == NULL)
  {
    return 99;
  }
    int error;

    int line = list->active->token.line;

    while(line == list->active->token.line)
    {
        //posun aktivity v seznamu na dalsi prvek
        error = TListTokenNext(list);
        if(error != 0)
        {
            return error;
        }
    }
    return 0;
}

int IsExp(TList *list, SList *slist)
{
  if(slist == NULL)
  {
    return 99;
  }
    int is = 0;

    //kontrola pro validni token
    if(list->active->token.type == T_ID || list->active->token.type == T_NUM_INTEGER ||
    list->active->token.type == T_NUM_NUMBER || list->active->token.type == T_STRING ||
    list->active->token.type == T_STRLEN || list->active->token.type == T_KW_NIL)
    {
        is = 1;
    }

    return is;
}
