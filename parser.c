#include "parser.h"

int Parse(TList *list)
{
    int error = 0;

    list->active = list->first;
    error = Program(&(*list));

    return error;
}

int Program(TList *list)
{
    int error;
    //check pro require
    if(list->active->token.type != T_KW_REQUIRE)
    {
        return 1;
    }

    //shift v seznamu
    TListTokenNext(&(*list));

    //check pro "ifj21"
    if(list->active->token.type != T_STRING || strcmp(list->active->token.value.string, "ifj21") != 0)
    {
        return 1;
    }

    //shift v seznamu
    TListTokenNext(&(*list));

    //check pro EOF -> chybí <main_body>
    if(list->active->token.type != T_EOF)
    {
        //check pro <main_body>
        error = MainBody(&(*list));
        return error; //vracim errorovou hodnotu z <main_body>
    }
    else
    {
        return 0; //chybi <main_body> -> vracim 0
    }


}

int MainBody(TList *list)
{
    //pomocna promenna pro errorovou hodnotu
    int error;

    //check pro keyword global -> deklarace funkce
    if(list->active->token.type == T_KW_GLOBAL)
    {
        //shift v seznamu
        TListTokenNext(&(*list));
        //check pro <dec_function>
        error = DecFunction(&(*list));
        return error;
    }
    //check pro keyword function -> definice funkce
    else if(list->active->token.type == T_KW_FUNCTION)
    {
        //shift v seznamu
        TListTokenNext(&(*list));
        //check pro <def_function>
        //error = DefFunction(&(*list));
        //return error;
        return 0;
    }
    //check pro keyword function -> volani funkce
    else if(list->active->token.type == T_ID)
    {
        //shift v seznamu
        TListTokenNext(&(*list));
        //check pro <call_function>
        //error = CallFunction(&(*list));
        //return error;
        return 0;
    }
    //jinak error
    else
    {
        return 1;
    }
}

int DecFunction(TList *list)
{
    //pomocna promenna pro errorovou hodnotu
    int error;

    //check pro id -> jinak vracim 1
    if(list->active->token.type != T_ID)
    {
        return 1;
    }

    //shift v seznamu
    TListTokenNext(&(*list));

    //check pro colon -> jinak vracim 1
    if(list->active->token.type != T_COLON)
    {
        return 1;
    }

    //shift v seznamu
    TListTokenNext(&(*list));

    //check pro keyword function -> jinak vracim 1
    if(list->active->token.type != T_KW_FUNCTION)
    {
        return 1;
    }

    //shift v seznamu
    TListTokenNext(&(*list));

    //check pro levou zavorku -> jinak vracim 1
    if(list->active->token.type != T_BRACKET_LEFT)
    {
        return 1;
    }

    //shift v seznamu
    TListTokenNext(&(*list));

    //check pro pravou zavorku -> jinak predpokladam ze jsou definovane vstupni typy
    if(list->active->token.type != T_BRACKET_RIGHT)
    {
        //check pro vstupni typy
        error = DataTypes(&(*list));

        //check pro spravne vstupni typy -> jinak vracim dany error
        if(error != 0)
        {
            return error;
        }
    }

    //check pro pravou zavorku -> jinak vracim 1
    if(list->active->token.type != T_BRACKET_RIGHT)
    {
        return 1;
    }

    //shift v seznamu
    TListTokenNext(&(*list));

    //check pro navratove typy
    error = ReturnTypes(&(*list));

    //vracim errorovou hodnotu
    return error;
}

int DataTypes(TList *list)
{
    //pomocna promenna pro errorovou hodnotu
    int error;

    //check pro datovy typ
    error = DataType(&(*list));

    //check pro spravne datove typy -> jinak vracim dany error
    if(error != 0)
    {
        return error;
    }
    //nenaskytl se error -> shift v seznamu
    else
    {
        TListTokenNext(&(*list));
    }

    //check pro comma -> jinak vracim 0
    if(list->active->token.type != T_COMMA)
    {
        return 0;
    }
    //predpokladam dalsi datovy typ
    else
    {
        TListTokenNext(&(*list));
        //check pro datovy typ
        error = DataTypes(&(*list));
        return error;
    }
}

int DataType(TList *list)
{
    //check pro keyword integer
    if(list->active->token.type == T_KW_INTEGER)
    {
        return 0;
    }

    //check pro keyword number
    if(list->active->token.type == T_KW_NUMBER)
    {
        return 0;
    }

    //check pro keyword string
    if(list->active->token.type == T_KW_STRING)
    {
        return 0;
    }

    //check pro keyword nil
    if(list->active->token.type == T_KW_NIL)
    {
        return 0;
    }

    //nenasel jsem validni keyword -> vracim 1
    return 1;
}

int ReturnTypes(TList *list)
{
    //pomocna promenna pro errorovou hodnotu
    int error;

    //check pro colon -> <return_types> neni prazdny
    if(list->active->token.type == T_COLON)
    {
        //shift v seznamu
        TListTokenNext(&(*list));
        //check pro navratovy typ
        error = ReturnType(&(*list));
        return error;
    }
    //<return_types> muze byt prazdny -> vracim 0
    else
    {
        return 0;
    }
}

int ReturnType(TList *list)
{
    //pomocna promenna pro errorovou hodnotu
    int error;

    //check pro datovy typ
    error = DataType(&(*list));

    //v pripade chyby vracim errorovou hodnotu
    if(error != 0)
    {
        return error;
    }

    //shift v seznamu
    TListTokenNext(&(*list));

    //check pro dalsi comma -> nasleduji dalsi typy
    if(list->active->token.type == T_COMMA)
    {
        //shift v seznamu
        TListTokenNext(&(*list));
        //check dalsiho typu
        error = ReturnType(&(*list));
        return error;
    }
    //konec typu
    else
    {
        return error;
    }
}