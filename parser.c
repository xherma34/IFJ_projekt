#include "parser.h"

/*-----FUNKCE MIMO PROGRAM-----*/

int Parse(TList *list)
{
    //pomocna promenna pro chybu
    int error;

    //aktivni prvek se nastavi na zacatek, aby se mohl prochazet cely list
    list->active = list->first;

    //provedu kontrolu pro program, ulozim navratovou hodnotu
    error = Program(&(*list));

    //navraceni chyby do funkce main() v main.c
    return error;
}

/*-----FUNKCE NA UROVNI CELEHO PROGRAMU-----*/

int Program(TList *list)
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
    error = TListTokenNext(&(*list));
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
    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //kontrola pro token T_EOF
    if(list->active->token.type != T_EOF)
    {
        //v pripade absence program obsahuje hlavni telo
        error = MainBody(&(*list));
    }

    //vracim chybu
    return error;
}

int MainBody(TList *list)
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
        error = TListTokenNext(&(*list));
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }

        //provedu kontrolu pro deklaraci funkce a ulozim navratovou hodnotu
        error = DecFunction(&(*list));
    }
    else if(list->active->token.type == T_KW_FUNCTION)
    {
        //jedna se o definici funkce, posun aktivity v seznamu na dalsi prvek
        error = TListTokenNext(&(*list));
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }

        //provedu kontrolu pro definici funkce a ulozim navratovou hodnotu
        error = DefFunction(&(*list));
    }

    else if(list->active->token.type == T_ID)
    {
        //jedna se o volani funkce, posun aktivity v seznamu na dalsi prvek
        error = TListTokenNext(&(*list));
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }

        //provedu kontrolu pro volani funkce a ulozim navratovou hodnotu
        error = CallFunction(&(*list));
    }

    //pokud jsem nasel chybu nebo jsem na konci hlavniho tela
    if(error != 0 || list->active->token.type == T_EOF)
    {
        //vratim hodnotu chyby
        return error;
    }

    //rekurzivne opakuju kontrolu hlavniho tela dokud nenastane chyba nebo EOF
    error = MainBody(&(*list));

    //vracim hodnotu chyby
    return error;
}

/*-----FUNKCE NA UROVNI HLAVNIHO TELA PROGRAMU-----*/

int DecFunction(TList *list)
{
    //pomocna promenna pro chybu
    int error;

    //kontrola pro token T_ID
    if(list->active->token.type != T_ID)
    {
        //v pripade absence tokenu vracim 2 (syntax error)
        return 2;
    }

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(&(*list));
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
    error = TListTokenNext(&(*list));
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
    error = TListTokenNext(&(*list));
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
    error = TListTokenNext(&(*list));
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
        error = DataTypes(&(*list));
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
    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //provedu kontrolu pro vracene typy a ulozim navratovou hodnotu
    error = ReturnTypes(&(*list));

    //vratim chybu
    return error;
}

int DefFunction(TList *list)
{
    int error = 0;

    //kontrola pro token T_ID
    if(list->active->token.type != T_ID)
    {
        return 2;
    }

    //posunuti aktivity v seznamu na dalsi prvek
    error = TListTokenNext(&(*list));
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
    error = TListTokenNext(&(*list));
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
        error = Params(&(*list));
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }
    }

    //kontrola pro token T_BRACKET_RIGHT
    if(list->active->token.type != T_BRACKET_RIGHT)
    {
        //v pripade absence tokenu vracim 2 (syntax error)
        return 2;
    }

    //posunuti aktivity v seznamu na dalsi prvek
    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //provedu kontrolu pro vracene typy a ulozim navratovou hodnotu
    error = ReturnTypes(&(*list));
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //kontrola pro token T_KW_END
    if(list->active->token.type != T_KW_END)
    {
        //v pripade absence tokenu definice funkce obsahuje telo
        //provedu kontrolu pro telo funkce a ulozim navratovou hodnotu
        error = FceBody(&(*list));
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

    //posunuti aktivity v seznamu na dalsi prvek
    error = TListTokenNext(&(*list));

    //vracim chybu
    return error;
}

int CallFunction(TList *list)
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
    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //kontrola pro token T_BRACKET_RIGHT
    if(list->active->token.type != T_BRACKET_RIGHT)
    {
        //v pripade absence tokenu volani funkce obsahuje parametry
        error = Ids_Datatypes(&(*list));
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }
    }

    //kontrola pro token T_BRACKET_RIGHT
    if(list->active->token.type != T_BRACKET_RIGHT)
    {
        //v pripade absence tokenu vracim 2 (syntax error)
        return 2;
    }

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //vratim chybovou hodnotu
    return 0;
}

/*-----FUNKCE NA UROVNI DEFINICE FUNKCE-----*/

int FceBody(TList *list)
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
        error = TListTokenNext(&(*list));
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }

        //provedu kontrolu pro definici promenne a ulozim navratovou hodnotu
        error = DefVar(&(*list));
    }
    //kontrola pro token T_KW_IF
    else if(list->active->token.type == T_KW_IF)
    {
        //jedna se o podminku, posun aktivity v seznamu na dalsi prvek
        error = TListTokenNext(&(*list));
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }

        //provedu kontrolu pro podminku a ulozim navratovou hodnotu
        error = Cond(&(*list));
    }
    //kontrola pro token T_KW_WHILE
    else if(list->active->token.type == T_KW_WHILE)
    {
        //jedna se o cyklus, posun aktivity v seznamu na dalsi prvek
        error = TListTokenNext(&(*list));
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }

        //provedu kontrolu pro cyklus a ulozim navratovou hodnotu
        error = Cycle(&(*list));
    }
    //kontrola pro token T_ID
    else if(list->active->token.type == T_ID)
    {
        //jedna se o volani funkce nebo pripsani hodnoty promenne
        //posunu aktivitu v seznamu na dalsi prvek
        error = TListTokenNext(&(*list));
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }

        //kontrola pro token T_BRACKET_LEFT
        if(list->active->token.type == T_BRACKET_LEFT)
        {
            //jedna se o volani funkce
            //provedu kontrolu pro volani funkce a ulozim navratovou hodnotu
            error = CallFunction(&(*list));
        }
        else
        {
            //jedna se o pripsani hodnoty promenne
            //posunu aktivitu v seznamu na predchozi prvek
            TListTokenPrev(&(*list));

            //provedu kontrolu pro pripsani hodnoty promenne a ulozim navratovou hodnotu
            error = Assign(&(*list));
        }
    }
    //kontrola pro token T_KW_RETURN
    else if(list->active->token.type == T_KW_RETURN)
    {
        //jedna se o return, posunu aktivitu v seznamu na dalsi prvek
        error = TListTokenNext(&(*list));
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }

        //provedu kontrolu pro return a ulozim navratovou hodnotu
        error = Return(&(*list));
    }

    //pokud se naskytly chyba a nebo jsem na konci tela funkce vratim chybu
    if(error != 0 ||
      list->active->token.type == T_KW_END ||
      list->active->token.type == T_KW_ELSE)
    {
        return error;
    }

    //rekurzivne provedu kontrolu pro telo funkce a ulozim navratovou hodnotu
    error = FceBody(&(*list));

    //vratim chybu
    return error;
}

/*-----FUNKCE NA UROVNI JEDNOTLIVYCH TEL FUNKCI-----*/

int DefVar(TList *list)
{
    //pomocna promenna pro chybu
    int error;

    //provedu kontrolu pro identifikatory a ulozim navratovou hodnotu
    error = Ids(&(*list));
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //kontrola pro token T_COLON
    if(list->active->token.type != T_COLON)
    {
        return 2;
    }

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //provedu kontrolu pro datatypy a ulozim navratovou hodnotu
    error = DataTypes(&(*list));
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //kontrola pro token T_SETVALUE
    if(list->active->token.type == T_SETVALUE)
    {
        //v definici promenne rovou prirazuji hodnoty
        //posun aktivity v seznamu na dalsi prvek
        error = TListTokenNext(&(*list));
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }

        //provedu kontrolu pro identifikatory a ulozim navratovou hodnotu
        error = Exps_Strings(&(*list));
    }

    //vratim chybu
    return error;
}

int Assign(TList *list)
{
    //pomocna promenna pro chybu
    int error;

    //provedu kontrolu pro identifikatory a ulozim navratovou hodnotu
    error = Ids(&(*list));
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
    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //provedu kontrolu pro identifikatory a ulozim navratovou hodnotu
    error = Exps(&(*list));

    //vratim chybu
    return error;
}

int Cond(TList *list)
{
    //pomocna promenna pro chybu
    int error;

    //provedu kontrolu pro expresi a ulozim navratovou hodnotu
    error = Exp(&(*list));
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

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(&(*list));
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
        error = FceBody(&(*list));
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

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //kontrola pro token T_KW_END
    if(list->active->token.type == T_KW_END)
    {
        //telo else v podmince je prazdne
        //posun aktivity v seznamu na dalsi prvek
        error = TListTokenNext(&(*list));
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }

        //vracim 0 (no error)
        return 0;
    }

    //provedu kontrolu pro telo funkce a ulozim navratovou hodnotu
    error = FceBody(&(*list));
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

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(&(*list));

    //vratim chybu
    return error;
}

int Cycle(TList *list)
{
    //pomocna promenna pro chybu
    int error;

    //provedu kontrolu pro expresi a ulozim navratovou hodnotu
    error = Exp(&(*list));
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

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //provedu kontrolu pro telo funkce a ulozim navratovou hodnotu
    error = FceBody(&(*list));
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

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(&(*list));

    //vratim chybu
    return error;
}

int Return(TList *list)
{
    //kontrola jestli nasledujici token muze byt exprese
    if(IsExp(&(*list)) == 2)
    {
        //nasledujici token muze byt exprese -> provedu kontrolu
        Exps(&(*list));
    }

    //vratim 0 -> return nemuze vracet nic jineho
    return 0;
}

/*-----POMOCNE FUNKCE PRO KONTROLU SYNTAXE-----*/

int DataTypes(TList *list)
{
    //pomocna promenna pro chybu
    int error;

    //provedu kontrolu pro datovy typ a ulozim navratovou hodnotu
    error = DataType(&(*list));

    //pokud nastala chyba nebo uz nenasleduje dalsi datovy typ
    if(error != 0 || list->active->token.type != T_COMMA)
    {
        //vratim chybu
        return error;
    }

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //provedu kontrolu pro datove typy a ulozim navratovou hodnotu
    error = DataTypes(&(*list));

    //vratim
    return error;
}

int DataType(TList *list)
{
    //pomocna promenna pro chybu
    int error;

    //kontrola pro validni token
    if(list->active->token.type == T_KW_INTEGER || list->active->token.type == T_KW_NUMBER
    || list->active->token.type == T_KW_STRING || list->active->token.type == T_KW_NIL)
    {
        //v pripade validniho typu prepisu chybu na 0 (no error)
        error = 0;
    }

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(&(*list));

    return error;
}

int Params(TList *list)
{
    //pomocna promenna pro chybu
    int error;

    //provedu kontrolu pro parametr a ulozim navratovou hodnotu
    error = Param(&(*list));
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
    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //provedu kontrolu pro parametry a ulozim navratovou hodnotu
    error = Params(&(*list));

    //vratim chybu
    return error;
}

int Param(TList *list)
{
    //pomocna promenna pro chybu
    int error;

    //kontrola pro token T_ID
    if(list->active->token.type != T_ID)
    {
        //v pripade absence tokenu vracim 2 (syntax error)
        return 2;
    }

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(&(*list));
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
    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //provedu kontrolu pro datovy typ a ulozim navratovou hodnotu
    error = DataType(&(*list));

    //vratim chybu
    return error;
}

int ReturnTypes(TList *list)
{
    //pomocna promenna pro chybu
    int error;

    //kontrola pro token T_COLON
    if(list->active->token.type == T_COLON)
    {
        //posun aktivity v seznamu na dalsi prvek
        error = TListTokenNext(&(*list));
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }

        //provedu kontrolu pro vraceny typ a ulozim navratovou hodnotu
        error = ReturnType(&(*list));

        //vratim chybu
        return error;
    }
    else
    {
        //vracene typy jsou prazdne, vratim 0 (no error)
        return 0;
    }
}

int ReturnType(TList *list)
{
    //pomocna promenna pro chybu
    int error;

    //provedu kontrolu pro datovy typ a ulozim navratovou hodnotu
    error = DataType(&(*list));
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
        error = TListTokenNext(&(*list));
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }

        //rekurzivne provedu kontrolu pro vraceny typ a ulozim navratovou hodnotu
        error = ReturnType(&(*list));
    }

    return error;
}

int Ids(TList *list)
{
    //pomocna promenna pro chybu
    int error;

    //kontrola pro token T_ID
    if(list->active->token.type != T_ID)
    {
        //v pripade absence tokenu vracim 2 (syntax error)
        return 2;
    }

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //kontrola pro token T_COMMA
    if(list->active->token.type == T_COMMA)
    {
        //posun aktivity v seznamu na dalsi prvek
        error = TListTokenNext(&(*list));
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }

        //provedu kontrolu pro identifikatory a ulozim navratovou hodnotu
        error = Ids(&(*list));
    }

    //vratim chybu
    return error;
}

int Exps(TList *list)
{
    //pomocna promenna pro chybu
    int error;

    //provedu kontrolu pro expresi a ulozim navratovou hodnotu
    error = Exp(&(*list));
    if(error != 0)
    {
      //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
      return error;
    }

    //kontrola pro token T_COMMA
    if(list->active->token.type == T_COMMA)
    {
        //posun aktivity v seznamu na dalsi prvek
        error = TListTokenNext(&(*list));
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }

        //provedu kontrolu pro exprese a ulozim navratovou hodnotu
        error = Exps(&(*list));
    }

    //vratim chybu
    return error;
}

int Ids_Datatypes(TList *list)
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

    //posun aktivity v seznamu na dalsi prvek
    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
        return error;
    }

    //kontrola pro token T_COMMA
    if(list->active->token.type == T_COMMA)
    {
        //nasleduje dalsi identifikator nebo datovy typ
        //posun aktivity v seznamu na dalsi prvek
        error = TListTokenNext(&(*list));
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }

        //provedu kontrolu pro identifikatory nebo datove typy a ulozim navratovou hodnotu
        error = Ids_Datatypes(&(*list));
    }

    //vratim chybu
    return error;
}

int Exps_Strings(TList *list)
{
    //pomocna promenna pro chybu
    int error = 0;

    //kontrola pro token T_STRING
    if(list->active->token.type != T_STRING)
    {
        //nasledujici vyraz neni string -> musi byt exprese
        //provedu kontrolu pro expresi a ulozim navratovou hodnotu
        error = Exp(&(*list));
        if(error != 0)
        {
          //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
          return error;
        }
    }
    else
    {
        //posun aktivity v seznamu na dalsi prvek
        error = TListTokenNext(&(*list));
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }
    }

    //kontrola pro token T_COMMA
    if(list->active->token.type == T_COMMA)
    {
        //jeste nejseme na konci seznamu expresi a stringu
        //posun aktivity v seznamu na dalsi prvek
        error = TListTokenNext(&(*list));
        if(error != 0)
        {
            //v pripade nekorektniho provedeni fce vracime jeji navratovou hodnotu
            return error;
        }

        //rekurzivne provedu kontrolu pro exprese nebo retezce a ulozim navratovou hodnotu
        error = Exps_Strings(&(*list));
    }

    //vratim chybu
    return error;
}

/*-----JINE POMOCNE FUNKCE------*/

void PrintToken(Token_type token)
{
    if(token == T_ID)
    {
        printf("ID");
    }
    else if(token == T_KW_DO)
    {
        printf("KW_DO");
    }
    else if(token == T_KW_ELSE)
    {
        printf("KW_ELSE");
    }
    else if(token == T_KW_END)
    {
        printf("KW_END");
    }
    else if(token == T_KW_FUNCTION)
    {
        printf("KW_FUNCTION");
    }
    else if(token == T_KW_GLOBAL)
    {
        printf("KW_GLOBAL");
    }
    else if(token == T_KW_IF)
    {
        printf("KW_IF");
    }
    else if(token == T_KW_LOCAL)
    {
        printf("KW_LOCAL");
    }
    else if(token == T_KW_NIL)
    {
        printf("KW_NIL");
    }
    else if(token == T_KW_INTEGER)
    {
        printf("KW_INTEGER");
    }
    else if(token == T_KW_NUMBER)
    {
        printf("KW_NUMBER");
    }
    else if(token == T_KW_REQUIRE)
    {
        printf("KW_REQUIRE");
    }
    else if(token == T_KW_RETURN)
    {
        printf("KW_RETURN");
    }
    else if(token == T_KW_STRING)
    {
        printf("KW_STRING");
    }
    else if(token == T_KW_THEN)
    {
        printf("KW_THEN");
    }
    else if(token == T_KW_WHILE)
    {
        printf("KW_WHILE");
    }
    else if(token == T_STRLEN)
    {
        printf("STRLEN");
    }
    else if(token == T_ADD)
    {
        printf("ADD");
    }
    else if(token == T_SUB)
    {
        printf("SUB");
    }
    else if(token == T_MUL)
    {
        printf("MUL");
    }
    else if(token == T_DIV_NUMBER)
    {
        printf("DIV_NUMBER");
    }
    else if(token == T_DIV_INTEGER)
    {
        printf("DIV_INTEGER");
    }
    else if(token == T_CONCATENATION)
    {
        printf("CONCATENATION");
    }
    else if(token == T_LT)
    {
        printf("LT");
    }
    else if(token == T_GT)
    {
        printf("GT");
    }
    else if(token == T_LET)
    {
        printf("LET");
    }
    else if(token == T_GET)
    {
        printf("GET");
    }
    else if(token == T_EQ)
    {
        printf("EQ");
    }
    else if(token == T_NEQ)
    {
        printf("NEQ");
    }
    else if(token == T_EOF)
    {
        printf("EOF");
    }
    else if(token == T_EOL)
    {
        printf("EOL");
    }
    else if(token == T_NUM_INTEGER)
    {
        printf("NUM_INTEGER");
    }
    else if(token == T_NUM_NUMBER)
    {
        printf("NUM_NUMBER");
    }
    else if(token == T_BRACKET_RIGHT)
    {
        printf("BRACKET_RIGHT");
    }
    else if(token == T_BRACKET_LEFT)
    {
        printf("BRACKET_LEFT");
    }
    else if(token == T_COLON)
    {
        printf("COLON");
    }
    else if(token == T_STRING)
    {
        printf("STRING");
    }
    else if(token == T_SETVALUE)
    {
        printf("SETVALUE");
    }
    else if(token == T_EMPTY)
    {
        printf("EMPTY");
    }
    else if(token == T_COMMA)
    {
        printf("COMMA");
    }
    else
    {
        printf("[TOKEN TYPE ERROR]");
    }
}

/*-----DOCASNE POMOCNE FUNKCE-----*/

int Exp(TList *list)
{
    int error;

    int line = list->active->token.line;

    while(line == list->active->token.line)
    {
        //posun aktivity v seznamu na dalsi prvek
        error = TListTokenNext(&(*list));
        if(error != 0)
        {
            return error;
        }
    }
    return 0;
}

int IsExp(TList *list)
{
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
