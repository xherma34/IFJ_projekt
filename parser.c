/*
* Project: IFJ 2021
*
* @file parser.c
*
* @brief Implementace syntaktického a sémantického analyzátoru
*
* @author Jan Zdeněk (xzdene01)
* @author Alexander Sila (xsila00)
*/

#include "parser.h"

/*-----FUNKCE MIMO PROGRAM-----*/

int Parse(TList *list, SList *slist)
{
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
    int error;

	//deklarace preddefinovanych funkci
	Token tmp_reads;
	tmp_reads.value.string = "reads";
	SListInsertFunc(slist, true, &tmp_reads);
	SListInsertReturn(slist, T_KW_STRING);

	Token tmp_readi;
	tmp_readi.value.string = "readi";
	SListInsertFunc(slist, true, &tmp_readi);
	SListInsertReturn(slist, T_KW_INTEGER);

	Token tmp_readn;
	tmp_readn.value.string = "readn";
	SListInsertFunc(slist, true, &tmp_readn);
	SListInsertReturn(slist, T_KW_NUMBER);

	Token tmp_tointeger;
	tmp_tointeger.value.string = "tointeger";
	SListInsertFunc(slist, true, &tmp_tointeger);
	SListInsertParam(slist, T_KW_NUMBER);
	SListInsertReturn(slist, T_KW_INTEGER);

	Token tmp_substr;
	tmp_substr.value.string = "substr";
	SListInsertFunc(slist, true, &tmp_substr);
	SListInsertParam(slist, T_KW_STRING);
	SListInsertParam(slist, T_KW_NUMBER);
	SListInsertParam(slist, T_KW_NUMBER);
	SListInsertReturn(slist, T_KW_STRING);

	Token tmp_ord;
	tmp_ord.value.string = "ord";
	SListInsertFunc(slist, true, &tmp_ord);
	SListInsertParam(slist, T_KW_STRING);
	SListInsertParam(slist, T_KW_INTEGER);
	SListInsertReturn(slist, T_KW_INTEGER);

	Token tmp_chr;
	tmp_chr.value.string = "chr";
	SListInsertFunc(slist, true, &tmp_chr);
	SListInsertParam(slist, T_KW_INTEGER);
	SListInsertReturn(slist, T_KW_STRING);

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
        return error;
    }

    //kontrola pro token T_EOF
    if(list->active->token.type != T_EOF)
    {
        //v pripade absence program obsahuje hlavni telo
        error = MainBody(list, slist);
    }
    return error;
}

int MainBody(TList *list, SList *slist)
{
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
            return error;
        }
        //provedu kontrolu pro definici funkce a ulozim navratovou hodnotu
        error = DefFunction(list, slist);
    }
    else if(list->active->token.type == T_ID)
    {
        //jedna se o volani funkce, posun aktivity v seznamu na dalsi prvek
        error = TListTokenNext(list);
        if(error != 0)
        {
            return error;
        }

		//kontrola pro write
		if(strcmp(list->active->prev->token.value.string, "write") == 0)
		{
			error = WriteFunction(list, slist);
		}
		else
		{
			//provedu kontrolu pro volani funkce a ulozim navratovou hodnotu
			error = CallFunction(list, slist, 0);
		}

    }

    //pokud jsem nasel chybu nebo jsem na konci hlavniho tela
    if(error != 0 || list->active->token.type == T_EOF)
    {
        return error;
    }

    //rekurzivne opakuju kontrolu hlavniho tela dokud nenastane chyba nebo EOF
    error = MainBody(list, slist);

    return error;
}

/*-----FUNKCE NA UROVNI HLAVNIHO TELA PROGRAMU-----*/

int DecFunction(TList *list, SList *slist)
{
    int error;

    //kontrola pro token T_ID
    if(list->active->token.type != T_ID)
    {
        //v pripade absence tokenu vracim 2 (syntax error)
        return 2;
    }

	//kontrola redeklarace a redefinice funkce
	if(IsDeclaredFunc(slist, &(list->active->token)) == 3)
    {
		//vlozim funkci do seznamu symbolu
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
        return error;
    }

    //kontrola pro token T_BRACKET_RIGHT
    if(list->active->token.type != T_BRACKET_RIGHT)
    {
        //seznam datatypu neni prazdny
		int commaCount = 0;
		int varCount = 0;
        error = DataTypes(list, slist, &commaCount, varCount);
        if(error != 0)
        {
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
        return error;
    }

    //provedu kontrolu pro vracene typy a ulozim navratovou hodnotu
    error = ReturnTypes(list, slist, false);

    return error;
}

int DefFunction(TList *list, SList *slist)
{
    int error;

    //kontrola pro token T_ID
    if(list->active->token.type != T_ID)
    {
        return 2;
    }

	//kontrola pro redefinici funkce
    if(IsDeclaredFunc(slist, &list->active->token) != 1)
    {
		//vlozim funkci do seznamu symbolu
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
        return error;
    }

    //kontrola pro token T_BRACKET_RIGHT
    if(list->active->token.type != T_BRACKET_RIGHT)
    {
        //funkce obsahuje parametry
        error = Params(list, slist);
        if(error != 0)
        {
            return error;
        }
    }

	//nastaveni posledni funkce
	error = LastFunc(slist);
	if(error != 0)
	{
		return error;
	}

	//kontrola poctu parametru pri definici deklarovane funkce
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
        return error;
    }

    //provedu kontrolu pro vracene typy a ulozim navratovou hodnotu
    error = ReturnTypes(list, slist, true);
    if(error != 0)
    {
        return error;
    }

	//nastaveni posledni funkce
	error = LastFunc(slist);
	if(error != 0)
	{
		return error;
	}

	//kontrola poctu parametru pri definici jiz deklarovane funkce
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
        //funkce obsahuje telo
        error = FceBody(list, slist);
        if(error != 0)
        {
            return error;
        }
    }

    //kontrola pro token T_KW_END
    if(list->active->token.type != T_KW_END)
    {
        //v pripade absence tokenu vracim 2 (syntax error)
        return 2;
    }

	//smazu scope
	error = DeleteScope(slist);
	if(error != 0)
	{
		return error;
	}

    //posunuti aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);

    return error;
}

int CallFunction(TList *list, SList *slist, int varCount)
{
    int error;

	error = SListInsertFunc(slist, false, &list->active->prev->token);
	if(error != 0)
	{
		return error;
	}

	if(varCount > 0)
	{
		TNodePtr tmp_active_list = list->active;

		for(int i = 0; i < (varCount * 2); i++)
		{
			error = TListTokenPrev(list);
			if(error != 0)
			{
				return error;
			}
		}

		for(int i = 0; i < varCount; i++)
		{
			error = IsDeclaredVar(slist, &list->active->token);
			if(error != 0)
			{
				return error;
			}

			SListInsertReturn(slist,slist->active->type);
			error = TListTokenNext(list);
			if(error != 0)
			{
				return error;
			}
			error = TListTokenNext(list);
			if(error != 0)
			{
				return error;
			}
		}

		list->active = tmp_active_list->next;
	}

	if(IsDeclaredJump(slist, &list->active->prev->token) > 1)
	{
		return 3;
	}

	SNodePtr tmp = slist->active;

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
        return error;
    }

    //kontrola pro token T_BRACKET_RIGHT
    if(list->active->token.type != T_BRACKET_RIGHT)
    {
        //volani funkce obsahuje parametry
        error = Ids_Datatypes(list, slist);
        if(error != 0)
        {
            return error;
        }

    }

	error = IsClone(slist->last, tmp);
	if(error == 1)
	{
		return 5;
	}
	else if(error == 2 && varCount > 0)
	{
		return 5;
	}
	else if(error > 2)
	{
		return error;
	}

	SListDeleteLast(slist);

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
        return error;
    }
    return 0;
}

/*-----FUNKCE NA UROVNI DEFINICE FUNKCE-----*/

int FceBody(TList *list, SList *slist)
{
	int error = 0;

    //kontrola pro validni token
    if(list->active->token.type != T_KW_LOCAL &&
		list->active->token.type != T_KW_IF &&
		list->active->token.type != T_KW_WHILE &&
		list->active->token.type != T_ID &&
		list->active->token.type != T_KW_RETURN)
    {
		// if(list->active->token.type == T_KW_END)
		// {
		// 	return 0;
		// }
        //v pripade absence validniho tokenu vracim 2 (syntax error)
        return 2;
    }

    if(list->active->token.type == T_KW_LOCAL)
    {
        //jedna se o definici promenne, posun aktivity v seznamu na dalsi prvek
        error = TListTokenNext(list);
        if(error != 0)
        {
            return error;
        }
        //provedu kontrolu pro definici promenne a ulozim navratovou hodnotu
        error = DefVar(list, slist);
    }
    else if(list->active->token.type == T_KW_IF)
    {
        //jedna se o podminku, posun aktivity v seznamu na dalsi prvek
        error = TListTokenNext(list);
        if(error != 0)
        {
            return error;
        }
        //provedu kontrolu pro podminku a ulozim navratovou hodnotu
        error = Cond(list, slist);
    }
    else if(list->active->token.type == T_KW_WHILE)
    {
        //jedna se o cyklus, posun aktivity v seznamu na dalsi prvek
        error = TListTokenNext(list);
        if(error != 0)
        {
            return error;
        }
        //provedu kontrolu pro cyklus a ulozim navratovou hodnotu
        error = Cycle(list, slist);
    }
    else if(list->active->token.type == T_ID)
    {
        //jedna se o volani funkce nebo pripsani hodnoty promenne
        //posunu aktivitu v seznamu na dalsi prvek
        error = TListTokenNext(list);
        if(error != 0)
        {
            return error;
        }

        if(list->active->token.type == T_BRACKET_LEFT)
        {
            //jedna se o volani funkce
			if(strcmp(list->active->prev->token.value.string, "write") == 0)
			{
				error = WriteFunction(list, slist);
			}
			else
			{
				error = CallFunction(list, slist, 0);
			}
        }
        else
        {
            //jedna se o pripsani hodnoty promenne
            //posunu aktivitu v seznamu na predchozi prvek
            error = TListTokenPrev(list);
			if(error != 0)
			{
				return error;
			}

            //provedu kontrolu pro pripsani hodnoty promenne a ulozim navratovou hodnotu
            error = Assign(list, slist);
        }
    }
    else if(list->active->token.type == T_KW_RETURN)
    {
        //jedna se o return, posunu aktivitu v seznamu na dalsi prvek
        error = TListTokenNext(list);
        if(error != 0)
        {
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

    return error;
}

/*-----FUNKCE NA UROVNI JEDNOTLIVYCH TEL FUNKCI-----*/

int DefVar(TList *list, SList *slist)
{
    int error;

    if(list->active->token.type != T_ID)
	{
		//v pripade absence validniho tokenu vracim 2 (syntax error)
		return 2;
	}

	//kontrola pro redeklaraci funkce jako promenne
	if(IsDeclaredFunc(slist, &list->active->token) < 2)
	{
		return 3;
	}

	//kontrola pro redeklaraci promenne ve scopu
	if(IsDeclaredVarInScope(slist, &list->active->token) == 0)
	{
		return 3;
	}

	//vlozeni promenne prozatim s prazdnym typem
	error = SListInsertVar(slist, &list->active->token, T_EMPTY);
	if(error != 0)
	{
		return error;
	}

	//posun aktivity v seznamu na dalsi prvek
	error = TListTokenNext(list);
	if(error != 0)
	{
		return error;
	}

    //kontrola pro token T_COLON
    if(list->active->token.type != T_COLON)
    {
		//v pripade absence validniho tokenu vracim 2 (syntax error)
        return 2;
    }

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);
    if(error != 0)
    {
        return error;
    }

    //provedu kontrolu pro datovy typ promenne
    if(list->active->token.type == T_KW_INTEGER ||
		list->active->token.type == T_KW_NUMBER ||
		list->active->token.type == T_KW_STRING ||
		list->active->token.type == T_KW_NIL)
	{
		//priradim datovy typ k promenne
		slist->last->type = list->active->token.type;
	}
	else
	{
		//v pripade absence validniho tokenu vracim 2 (syntax error)
		return 2;
	}

	//posun aktivity v seznamu na dalsi prvek
	error = TListTokenNext(list);
    if(error != 0)
    {
        return error;
    }

    //kontrola pri pripisovani hodnoty promenne
    if(list->active->token.type != T_SETVALUE)
    {
        return error;
    }

	if(list->active->next->next->token.type == T_BRACKET_LEFT)
	{
		error = TListTokenNext(list);
		if(error != 0)
		{
			return error;
		}

		if(IsDeclaredFunc(slist, &list->active->token) > 1)
		{
			return IsDeclaredFunc(slist, &list->active->token);
		}


		if(slist->active->numReturns == 0)
		{
			return 5;
		}

		Token_type tmp_type = slist->active->returns.first->token.type;

		if(tmp_type != list->active->prev->prev->token.type)
		{
			if(tmp_type == T_KW_NUMBER && list->active->prev->prev->token.type == T_KW_INTEGER)
			{
				return 0;
			}
			return 4;
		}

		error = TListTokenNext(list);
		if(error != 0)
		{
			return error;
		}

		error = CallFunction(list, slist, 0);
		if(error != 0)
		{
			return error;
		}

		return 0;
	}

	Token_type type;

	error = TListTokenNext(list);
	if(error != 0)
	{
		return error;
	}

	error = Exp(list, slist, &type);
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

	if(type == T_STRING && slist->last->type != T_KW_STRING)
	{
		return 4;
	}
	else if(type == T_NUM_INTEGER && slist->last->type != T_KW_INTEGER)
	{
		if(slist->last->type != T_KW_NUMBER)
		{
			return 4;
		}
		return error;
	}
	else if(type == T_NUM_NUMBER && slist->last->type != T_KW_NUMBER)
	{
		return 4;
	}
	else if(type == T_KW_NIL && slist->last->type != T_KW_NIL)
	{
		return 4;
	}
	else
	{
		return error;
	}
}

int Assign(TList *list, SList *slist)
{
    int error;

	//kontrola pro ids a ulozeni jejich poctu (pocet je -1 protoze pocitam carky)
	int varCount = 0;
    error = Ids(list, slist, &varCount);
    if(error != 0)
    {
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
		return error;
	}

	if(list->active->next->token.type == T_BRACKET_LEFT)
	{
		error = CallFunction(list, slist, varCount + 1);

		return error;
	}

	//kontrola expresi, posilam pocet promennych -1 a pocet pripisovanych hodnot -1
	int commaCount = 0;
    error = Exps(list, slist, varCount, &commaCount);

	//pokud se lisi pocet promennych a pocet pripisovanych hodnot vracim 7
	if(varCount > commaCount)
	{
		return 7;
	}

    return error;
}

int Cond(TList *list, SList *slist)
{
    int error;

	Token_type type;

    //provedu kontrolu pro expresi
    error = Exp(list, slist, &type);
    if(error != 0)
    {
        return error;
    }


    //kontrola pro token T_KW_WHEN
    if(list->active->token.type != T_KW_THEN)
    {
        //v pripade absence tokenu vracim 2 (syntax error)
        return 2;
    }

	//vlozim podminku do seznamu symbolu
	error = SListInsertCond(slist);
	if(error != 0)
	{
		return error;
	}

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);
    if(error != 0)
    {
        return error;
    }

    if(list->active->token.type != T_KW_ELSE)
    {
        //telo podminky neni prazdne
        error = FceBody(list, slist);
        if(error != 0)
        {
            return error;
        }
    }
    if(list->active->token.type != T_KW_ELSE)
    {
        //v pripade absence tokenu vracim 2 (syntax error)
        return 2;
    }

	//smazu scope podminky
	error = DeleteScope(slist);
	if(error != 0)
	{
		return error;
	}

	//vlozim podminku do seznamu symbolu
	error = SListInsertCond(slist);
	if(error != 0)
	{
		return error;
	}

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);
    if(error != 0)
    {
        return error;
    }

    if(list->active->token.type == T_KW_END)
    {
		//telo else v podmince je prazdne
		//smazu scope podminky
		error = DeleteScope(slist);
		if(error != 0)
		{
			return error;
		}
        //posun aktivity v seznamu na dalsi prvek
        error = TListTokenNext(list);
        if(error != 0)
        {
            return error;
        }

        return 0;
    }

    //provedu kontrolu pro telo funkce a ulozim navratovou hodnotu
    error = FceBody(list, slist);
    if(error != 0)
    {
        return error;
    }

    if(list->active->token.type != T_KW_END)
    {
        //v pripade absence tokenu vracim 2 (syntax error)
        return 2;
    }

	//smaze scope podminky
	error = DeleteScope(slist);
	if(error != 0)
	{
		return error;
	}

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);

    return error;
}

int Cycle(TList *list, SList *slist)
{
    int error;

	Token_type type;

    //provedu kontrolu pro expresi a ulozim navratovou hodnotu
    error = Exp(list, slist, &type);
    if(error != 0)
    {
        return error;
    }

    if(list->active->token.type != T_KW_DO)
    {
        //v pripade absence tokenu vracim 2 (syntax error)
        return 2;
    }

	//vlozim cyklus do tabulky symbolu
	error = SListInsertCycle(slist);
	if(error != 0)
	{
		return error;
	}

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);
    if(error != 0)
    {
        return error;
    }

	if(list->active->token.type != T_KW_END)
	{
		//provedu kontrolu pro telo funkce a ulozim navratovou hodnotu
		error = FceBody(list, slist);
		if(error != 0)
		{
			return error;
		}
	}
	
    if(list->active->token.type != T_KW_END)
    {
        //v pripade absence tokenu vracim 2 (syntax error)
        return 2;
    }

	//smazu scope cyklu
	error = DeleteScope(slist);
	if(error != 0)
	{
		return error;
	}

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);

    return error;
}

int Return(TList *list, SList *slist)
{
	int error;

	//kontrola neexistujiciho listu
	if(list == NULL)
	{
		return 99;
	}

	//kontrola prazdneho listu
	if(list->first == NULL)
	{
		return 99;
	}

	//nastavim aktivni prvek na posledni
	slist->active = slist->last;

	error = LastFunc(slist);
	if(error != 0)
	{
		return error;
	}

	int expCount = 0; //pocet expresi
	Token_type type;

	//nastaveni aktivniho prvku v returns na prvni prvek
	slist->lastFunc->returns.active = slist->lastFunc->returns.first;

	while(1)
	{
		//zavolam funkci pro zpracovani exprese
		error = Exp(list, slist, &type);
		if(error != 0)
		{
			if(error == 1) //chybejici exprese
			{
				if(expCount == 0) //chybi prvni exprese -> validni chovani
				{
					return 0;
				}
				return 2; //chybi jina nez prvni exprese -> nevalidni chovani
			}
			return error; //jiny error exprese
		}

		expCount++; //prictu pocet expresi

		if(slist->lastFunc->returns.active != NULL)
		{
			if(type == T_STRING && slist->lastFunc->returns.active->token.type == T_KW_STRING)
			{
				slist->lastFunc->returns.active = slist->lastFunc->returns.active->next;
			}
			else if(type == T_NUM_INTEGER && slist->lastFunc->returns.active->token.type == T_KW_INTEGER)
			{
				slist->lastFunc->returns.active = slist->lastFunc->returns.active->next;
			}
			else if(type == T_NUM_NUMBER && slist->lastFunc->returns.active->token.type == T_KW_NUMBER)
			{
				slist->lastFunc->returns.active = slist->lastFunc->returns.active->next;
			}
			else if(type == T_KW_NIL && slist->lastFunc->returns.active->token.type == T_KW_NIL)
			{
				slist->lastFunc->returns.active = slist->lastFunc->returns.active->next;
			}
			else if(type == T_NUM_INTEGER && slist->lastFunc->returns.active->token.type == T_KW_NUMBER)
			{
				slist->lastFunc->returns.active = slist->lastFunc->returns.active->next;
			}
			else
			{
				return 5;
			}
		}

		//kontrola zda nasleduje dalsi exprese
		if(list->active->token.type != T_COMMA)
		{
			if(expCount < slist->lastFunc->numReturns)
			{
				return 5;
			}
			return error;
		}
		else
		{
			//posunuti aktivity v seznamu na dalsi prvek
			error = TListTokenNext(list);
			if(error != 0)
			{
				return error;
			}
		}
	}

	error = LastFunc(slist);
	if(error != 0)
	{
		return error;
	}
	if(expCount < slist->lastFunc->numReturns)
	{
		return 5;
	}
    return 0;
}

/*-----POMOCNE FUNKCE PRO KONTROLU SYNTAXE-----*/

int DataTypes(TList *list, SList *slist, int *commaCount, int varCount)
{
    int error;

    //provedu kontrolu pro datovy typ a ulozim navratovou hodnotu
    error = DataType(list, slist, false);
	if(error != 0)
	{
		return error;
	}

	// //posunu aktivitu v seznamu na predchozi prvek
	// error =TListTokenPrev(list);
	// if(error != 0)
	// {
	// 	return error;
	// }
	//
	// //nastavim aktivitu na posledni prvek v seznamu
	// slist->active = slist->last;
	// if(varCount < *commaCount)
	// {
	// 	return 7;
	// }
	// for(int i = 0; i < varCount - (*commaCount); i++)
	// {
	// 	SListPrev(slist);
	// }
	// slist->active->type = list->active->token.type;
	// slist->active = slist->last;
	//
	// //posunu aktivitu v seznamu na dalsi prvek
	// error = TListTokenNext(list);
	// if(error != 0)
	// {
	// 	return error;
	// }

    //pokud uz nenasleduje dalsi datovy typ
    if(list->active->token.type != T_COMMA)
    {
        return error;
    }

	(*commaCount)++; //prictu pocet

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);
    if(error != 0)
    {
        return error;
    }

    //provedu kontrolu pro datove typy a ulozim navratovou hodnotu
    error = DataTypes(list, slist, commaCount, varCount);

    return error;
}

int DataType(TList *list, SList *slist, bool isDef)
{
	//kontrola neexistujiciho seznamu
	if(slist == NULL)
	{
		return 99;
	}

    int error;

    //kontrola pro validni token
    if(list->active->token.type == T_KW_INTEGER || list->active->token.type == T_KW_NUMBER
    || list->active->token.type == T_KW_STRING || list->active->token.type == T_KW_NIL)
    {
        //v pripade validniho typu prepisu chybu na 0 (no error)
        error = 0;

		//vlozim parametr do seznamu parametru posledni funkce
		error = SListInsertParam(slist, list->active->token.type);
		if(error != 0)
		{
			return error;
		}

		//kontrola definice funkce
        if(isDef)
        {
			//nastavim posledni funkci
			error = LastFunc(slist);
			if(error != 0)
			{
				return error;
			}

			//kontrola pro definici jiz deklarovane funkce
			error = IsDeclaredJump(slist, slist->lastFunc->id_func);
			if(error != 3)
			{
				//nastavim aktivni prvek v seznamu parametru na prvni prvek
				slist->active->params.active = slist->active->params.first;
				//slist->lastFunc->params.last -> prave nacteny parametr
				for(int i = 0; i < slist->lastFunc->params.last->index - 1; i++)
				{
					if(slist->active->params.active->next == NULL)
					{
						return 5;
					}
					//posouvam aktivitu v seznamu parametru podle indexu prave nacteneho parametru
					TListTokenNext(&slist->active->params);
				}

				//pokud se typ nacteneho parametru neshoduje s parametrem v deklaraci funkce na stejnem indexu
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
	//kontrola neexistujiciho seznamu
	if(slist == NULL)
	{
		return 99;
	}

	int error;

	//kontrola pro validni token
	if(list->active->token.type == T_KW_INTEGER || list->active->token.type == T_KW_NUMBER
	|| list->active->token.type == T_KW_STRING || list->active->token.type == T_KW_NIL)
	{
		//v pripade validniho typu prepisu chybu na 0 (no error)
		error = 0;

		//vlozim parametr do seznamu vracenych typu posledni funkce
		error = SListInsertReturn(slist, list->active->token.type);
		if(error != 0)
		{
			return error;
		}

		//kontrola definice funkce
		if(isDef)
		{
			//nastavim posledni funkci
			error = LastFunc(slist);
			if(error != 0)
			{
				return error;
			}

			//kontrola pro definici jiz deklarovane funkce
			error = IsDeclaredJump(slist, slist->lastFunc->id_func);
			if(error != 3)
			{
				//nastavim aktivni prvek v seznamu vracenych typu na prvni prvek
				slist->active->returns.active = slist->active->returns.first;
				//slist->lastFunc->returns.last -> prave nacteny vraceny typ
				for(int i = 0; i < slist->lastFunc->returns.last->index - 1; i++)
				{
					if(slist->active->returns.active->next == NULL)
					{
						return 5;
					}
					//posouvam aktivitu v seznamu vracenych typu podle indexu prave nacteneho vraceneho typu
					TListTokenNext(&slist->active->returns);
				}
				if(slist->active->returns.first == NULL)
				{
					return 5;
				}
				//pokud se typ nacteneho vraceneho typu neshoduje s vracenym typem v deklaraci funkce na stejnem indexu
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
    int error;

    //provedu kontrolu pro parametr a ulozim navratovou hodnotu
    error = Param(list, slist);
    if(error != 0)
    {
        return error;
    }

    if(list->active->token.type != T_COMMA)
    {
        //v pripade absence tokenu vracim chybu
        return error;
    }

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);
    if(error != 0)
    {
        return error;
    }

    //provedu kontrolu pro parametry a ulozim navratovou hodnotu
    error = Params(list, slist);

    return error;
}

int Param(TList *list, SList *slist)
{
    int error;

    //kontrola pro token T_ID
    if(list->active->token.type != T_ID)
    {
        //v pripade absence tokenu vracim 2 (syntax error)
        return 2;
    }

	//kontrola redefinice funkce promennou
	if(IsDeclaredFunc(slist, &list->active->token) < 2)
	{
		return 3;
	}

	//vlozeni promenne do listu symbolu
	error = SListInsertVar(slist, &list->active->token, T_EMPTY);
	if(error != 0)
	{
		return error;
	}

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);
    if(error != 0)
    {
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
        return error;
    }

	//kontrola pro datovy typ
    error = DataType(list, slist, true);
	if(error != 0)
	{
		return error;
	}

	error = TListTokenPrev(list);
	if(error != 0)
	{
		return error;
	}

	//nastavim datovy typ pro promennou
	slist->last->type = list->active->token.type;

	error = TListTokenNext(list);
	if(error != 0)
	{
		return error;
	}

    return error;
}

int ReturnTypes(TList *list, SList *slist, bool isDef)
{
    int error;

    //kontrola pro token T_COLON
    if(list->active->token.type == T_COLON)
    {
        //posun aktivity v seznamu na dalsi prvek
        error = TListTokenNext(list);
        if(error != 0)
        {
            return error;
        }

        //provedu kontrolu pro vraceny typ a ulozim navratovou hodnotu
        error = ReturnType(list, slist, isDef);

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
    int error;

    //provedu kontrolu pro datovy typ a ulozim navratovou hodnotu
    error = DataTypeReturn(list, slist, isDef);
    if(error != 0)
    {
        return error;
    }

    //kontrola pro token T_COMMA
    if(list->active->token.type == T_COMMA)
    {
        //nasleduje dalsi vraceny typ
        error = TListTokenNext(list);
        if(error != 0)
        {
            return error;
        }

        //rekurzivne provedu kontrolu pro vraceny typ a ulozim navratovou hodnotu
        error = ReturnType(list, slist, isDef);
    }
    return error;
}

int Ids(TList *list, SList *slist, int *varCount)
{
    int error;

    //kontrola pro token T_ID
    if(list->active->token.type != T_ID)
    {
        //v pripade absence tokenu vracim 2 (syntax error)
        return 2;
    }

	//kontrola zda je promenna definovana -> pokud ne vracim 3
	if(IsDeclaredVar(slist, &list->active->token) != 0)
	{
		return 3;
	}

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);
    if(error != 0)
    {
        return error;
    }

    //kontrola pro token T_COMMA
    if(list->active->token.type == T_COMMA)
    {
		(*varCount)++; //prictu pocet promennych
        error = TListTokenNext(list);
        if(error != 0)
        {
            return error;
        }
        //rekurzivne provedu kontrolu pro identifikatory a ulozim navratovou hodnotu
        error = Ids(list, slist, varCount);
    }
    return error;
}

int Exps(TList *list, SList *slist, int varCount, int *commaCount)
{
    int error;

	Token_type type;
	error = Exp(list, slist, &type);
	if(error != 0)
	{
		return error;
	}

	TNodePtr tmp = list->active;

	while(list->active->token.type != T_SETVALUE)
	{
		if(list->active->prev == NULL)
		{
			return 99;
		}
		TListTokenPrev(list);
	}

	if(list->active->prev == NULL)
	{
		return 99;
	}
	TListTokenPrev(list);

	for(int i = 0; i < (varCount - *commaCount)*2; i++)
	{
		if(list->active->prev == NULL)
		{
			return 99;
		}
		TListTokenPrev(list);
	}


	error = IsDeclaredVar(slist, &list->active->token);
	if(error > 1)
	{
		return 99;
	}

	list->active = tmp;

	if(type == T_STRING && slist->active->type != T_KW_STRING)
	{
		return 4;
	}
	else if(type == T_NUM_INTEGER && slist->active->type != T_KW_INTEGER)
	{
		if(slist->active->type != T_KW_NUMBER)
		{
			return 4;
		}
	}
	else if(type == T_NUM_NUMBER && slist->active->type != T_KW_NUMBER)
	{
		return 4;
	}
	else if(type == T_KW_NIL && slist->active->type != T_KW_NIL)
	{
		return 4;
	}


    //kontrola pro token T_COMMA
    if(list->active->token.type == T_COMMA)
    {
        //posun aktivity v seznamu na dalsi prvek
        error = TListTokenNext(list);
        if(error != 0)
        {
            return error;
        }

        //provedu kontrolu pro exprese a ulozim navratovou hodnotu
		(*commaCount)++;
        error = Exps(list, slist, varCount, commaCount);
    }

    return error;
}

int Ids_Datatypes(TList *list, SList *slist)
{
    int error;

    //kontrola pro validni token
    if(list->active->token.type != T_ID && list->active->token.type != T_STRING &&
    	list->active->token.type != T_NUM_INTEGER && list->active->token.type != T_NUM_NUMBER &&
    	list->active->token.type != T_KW_NIL)
    {
        //v pripade absence tokenu vracim 2 (syntax error)
        return 2;
    }

	if(list->active->token.type == T_STRING)
	{
		SListInsertParam(slist, T_KW_STRING);
	}
	else if(list->active->token.type == T_NUM_INTEGER)
	{
		SListInsertParam(slist, T_KW_INTEGER);
	}
	else if(list->active->token.type == T_NUM_NUMBER)
	{
		SListInsertParam(slist, T_KW_NUMBER);
	}
	else if(list->active->token.type == T_KW_NIL)
	{
		SListInsertParam(slist, T_KW_NIL);
	}
	else if(list->active->token.type == T_ID)
	{
		error = IsInteger(slist, &list->active->token);
		if(error == 0)
		{
			SListInsertParam(slist, T_KW_INTEGER);
		}
		else if(error == 3)
		{
			return error;
		}

		error = IsNumber(slist, &list->active->token);
		if(error == 0)
		{
			SListInsertParam(slist, T_KW_NUMBER);
		}

		error = IsString(slist, &list->active->token);
		if(error == 0)
		{
			SListInsertParam(slist, T_KW_STRING);
		}

		error = IsNil(slist, &list->active->token);
		if(error == 0)
		{
			SListInsertParam(slist, T_KW_NIL);
		}

	}

	//posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(list);
    if(error != 0)
    {
        return error;
    }

    //kontrola pro token T_COMMA
    if(list->active->token.type == T_COMMA)
    {
        //nasleduje dalsi identifikator nebo datovy typ
        //posun aktivity v seznamu na dalsi prvek
        error = TListTokenNext(list);
        if(error != 0)
        {
            return error;
        }

        //rekurzivne provedu kontrolu pro identifikatory nebo datove typy a ulozim navratovou hodnotu
        error = Ids_Datatypes(list, slist);
    }

    return error;
}

int WriteFunction(TList *list, SList *slist)
{
	int error;

	if(list->active->token.type != T_BRACKET_LEFT)
	{
		return 2;
	}

	error = TListTokenNext(list);
	if(error != 0)
	{
		return error;
	}

	while(list->active->token.type == T_ID || list->active->token.type == T_STRING)
	{
		if(list->active->token.type == T_ID)
		{
			error = IsDeclaredVar(slist, &list->active->token);
			if(error != 0)
			{
				return error;
			}

			if(slist->active->type == T_KW_NIL)
			{
				return 5;
			}
		}
		error = TListTokenNext(list);
		if(error != 0)
		{
			return error;
		}

		if(list->active->token.type == T_COMMA)
		{
			error = TListTokenNext(list);
			if(error != 0)
			{
				return error;
			}
		}
		else
		{
			break;
		}
	}

	if(list->active->token.type != T_BRACKET_RIGHT)
	{
		return 2;
	}
	else
	{
		error = TListTokenNext(list);
		return error;
	}
}
