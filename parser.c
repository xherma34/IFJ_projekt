#include "parser.h"

int Parse(TList *list)
{
    int error;

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
        if(error != 0)
        {
            return error;
        }

        if(list->active->token.type == T_EOF)
        {
            return error;
        }
        error = MainBody(&(*list));
        return error;
    }
    //check pro keyword function -> definice funkce
    else if(list->active->token.type == T_KW_FUNCTION)
    {
        //shift v seznamu
        TListTokenNext(&(*list));
        //check pro <def_function>
        error = DefFunction(&(*list));

        if(error != 0)
        {
            return error;
        }

        if(list->active->token.type == T_EOF)
        {
            return error;
        }
        error = MainBody(&(*list));
        return error;
    }
    //check pro keyword function -> volani funkce
    else if(list->active->token.type == T_ID)
    {
        //shift v seznamu
        TListTokenNext(&(*list));
        //check pro <call_function>
        error = CallFunction(&(*list));
        return error;
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

int DefFunction(TList *list)
{
    int error = 0;

    if(list->active->token.type != T_ID)
    {
        return 1;
    }

    TListTokenNext(&(*list));

    if(list->active->token.type != T_BRACKET_LEFT)
    {
        return 1;
    }

    TListTokenNext(&(*list));

    if(list->active->token.type != T_BRACKET_RIGHT)
    {
        error = Params(&(*list));
        if(error != 0)
        {
            return error;
        }
    }

    if(list->active->token.type != T_BRACKET_RIGHT)
    {
        return 1;
    }

    TListTokenNext(&(*list));

    error = ReturnTypes(&(*list));

    if(error != 0)
    {
        return error;
    }


    if(list->active->token.type != T_KW_END)
    {
        error = FceBody(&(*list));

        if(error != 0)
        {
            return error;
        }
    }

    if(list->active->token.type != T_KW_END)
    {
        return 1;
    }

    TListTokenNext(&(*list));

    return error;
}

int Params(TList *list)
{
    int error;

    error = Param(&(*list));

    if(error != 0)
    {
        return error;
    }

    TListTokenNext(&(*list));

    if(list->active->token.type != T_COMMA)
    {
        return error;
    }

    TListTokenNext(&(*list));

    error = Params(&(*list));
    return error;
}

int Param(TList *list)
{
    int error;

    if(list->active->token.type != T_ID)
    {
        return 1;
    }

    TListTokenNext(&(*list));

    if(list->active->token.type != T_COLON)
    {
        return 1;
    }

    TListTokenNext(&(*list));

    error = DataType(&(*list));

    return error;
}

int CallFunction(TList *list)
{
    int error;

    if(list->active->token.type != T_BRACKET_LEFT)
    {
        return 1;
    }

    TListTokenNext(&(*list));

    if(list->active->token.type != T_BRACKET_RIGHT)
    {
        error = Params(&(*list));
        if(error != 0)
        {
            return 1;
        }
    }

    if(list->active->token.type != T_BRACKET_RIGHT)
    {
        return 1;
    }

    TListTokenNext(&(*list));

    return 0;
}

int FceBody(TList *list)
{
    int error;

    if(list->active->token.type == T_KW_LOCAL)
    {
        TListTokenNext(&(*list));
        error = DefVar(&(*list));

        if(error != 0)
        {
            return error;
        }

        if(list->active->token.type == T_KW_END || list->active->token.type == T_KW_ELSE)
        {
            return error;
        }

        error = FceBody(&(*list));
        return error;
    }
    else if(list->active->token.type == T_KW_IF)
    {

        TListTokenNext(&(*list));
        error = Cond(&(*list));

        if(error != 0)
        {
            return error;
        }

        if(list->active->token.type == T_KW_END || list->active->token.type == T_KW_ELSE)
        {
            return error;
        }

        error = FceBody(&(*list));
        return error;
    }
    else if(list->active->token.type == T_KW_WHILE)
    {
        TListTokenNext(&(*list));
        error = Cycle(&(*list));

        if(error != 0)
        {
            return error;
        }

        if(list->active->token.type == T_KW_END || list->active->token.type == T_KW_ELSE)
        {
            return error;
        }

        error = FceBody(&(*list));
        return error;
    }
    else if(list->active->token.type == T_ID)
    {
        TListTokenNext(&(*list));

        if(list->active->token.type == T_BRACKET_LEFT)
        {
            error = CallFunction(&(*list));
        }
        else
        {
            TListTokenPrev(&(*list));
            error = Assign(&(*list));
        }

        if(error != 0)
        {
            return error;
        }

        if(list->active->token.type == T_KW_END || list->active->token.type == T_KW_ELSE)
        {
            return error;
        }

        error = FceBody(&(*list));
        return error;
    }
    else if(list->active->token.type == T_KW_RETURN)
    {
        TListTokenNext(&(*list));
        error = Return(&(*list));

        if(error != 0)
        {
            return error;
        }

        if(list->active->token.type == T_KW_END || list->active->token.type == T_KW_ELSE)
        {
            return error;
        }
        error = FceBody(&(*list));
        return error;
    }
    else
    {
        return 1;
    }
}

int DefVar(TList *list)
{
    int error;

    if(list->active->token.type != T_ID)
    {
        return 1;
    }

    TListTokenNext(&(*list));

    if(list->active->token.type != T_COLON)
    {
        return 1;
    }

    TListTokenNext(&(*list));

    error = DataType(&(*list));

    if(error != 0)
    {
        return 1;
    }

    TListTokenNext(&(*list));

    if(list->active->token.type == T_SETVALUE)
    {
        TListTokenNext(&(*list));
        if(list->active->token.type == T_STRING)
        {
            TListTokenNext(&(*list));
            return 0;
        }

        error = Exp(&(*list));

        if(error != 0)
        {
            return 1;
        }

        return 0;
    }

    return 0;
}

int Cond(TList *list)
{
    int error;

    error = Exp(&(*list));

    if(error != 0)
    {
        return 1;
    }

    if(list->active->token.type != T_KW_THEN)
    {
        return 1;
    }

    TListTokenNext(&(*list));

    if(list->active->token.type != T_KW_ELSE)
    {
        error = FceBody(&(*list));

        if(error != 0)
        {
            return 1;
        }
    }


    if(list->active->token.type != T_KW_ELSE)
    {
        return 1;
    }

    TListTokenNext(&(*list));

    if(list->active->token.type == T_KW_END)
    {
        TListTokenNext(&(*list));
        return 0;
    }

    error = FceBody(&(*list));

    if(error != 0)
    {
        return 1;
    }

    if(list->active->token.type != T_KW_END)
    {
        return 1;
    }

    TListTokenNext(&(*list));

    return 0;
}

int Cycle(TList *list)
{
    int error;

    error = Exp(&(*list));

    if(error != 0)
    {
        return 1;
    }

    if(list->active->token.type != T_KW_DO)
    {
        return 1;
    }

    TListTokenNext(&(*list));

    error = FceBody(&(*list));

    if(error != 0)
    {
        return 1;
    }

    if(list->active->token.type != T_KW_END)
    {
        return 1;
    }

    TListTokenNext(&(*list));

    return 0;
}

int Return(TList *list)
{
    int error;

    error = Exps(&(*list));

    if(error != 0)
    {
        return error;
    }

    return 0;
}

int Assign(TList *list)
{
    int error;

    error = Ids(&(*list));

    if(error != 0)
    {
        return error;
    }

    if(list->active->token.type != T_SETVALUE)
    {
        return 1;
    }

    error = Exps(&(*list));

    return error;
}

int Ids(TList *list)
{
    int error;

    if(list->active->token.type != T_ID)
    {
        return 1;
    }

    TListTokenNext(&(*list));

    if(list->active->token.type == T_COMMA)
    {
        TListTokenNext(&(*list));
        error = Ids(&(*list));

        return error;
    }
    return 0;
}

int Exps(TList *list)
{
    int error;

    error = Exp(&(*list));

    if(error != 0)
    {
        return error;
    }

    if(list->active->token.type == T_COMMA)
    {
        TListTokenNext(&(*list));
        error = Exps(&(*list));
        return error;
    }

    return error;
}

int Exp(TList *list)
{
    int line = list->active->token.line;

    while(line == list->active->token.line)
    {
        TListTokenNext(&(*list));
    }
    return 0;
}

void PrintToken(Token_type token)
{
    if(token == T_ID)
    {
        printf("ID\n");
    }
    else if(token == T_KW_DO)
    {
        printf("KW_DO\n");
    }
    else if(token == T_KW_ELSE)
    {
        printf("KW_ELSE\n");
    }
    else if(token == T_KW_END)
    {
        printf("KW_END\n");
    }
    else if(token == T_KW_FUNCTION)
    {
        printf("KW_FUNCTION\n");
    }
    else if(token == T_KW_GLOBAL)
    {
        printf("KW_GLOBAL\n");
    }
    else if(token == T_KW_IF)
    {
        printf("KW_IF\n");
    }
    else if(token == T_KW_LOCAL)
    {
        printf("KW_LOCAL\n");
    }
    else if(token == T_KW_NIL)
    {
        printf("KW_NIL\n");
    }
    else if(token == T_KW_INTEGER)
    {
        printf("KW_INTEGER\n");
    }
    else if(token == T_KW_NUMBER)
    {
        printf("KW_NUMBER\n");
    }
    else if(token == T_KW_REQUIRE)
    {
        printf("KW_REQUIRE\n");
    }
    else if(token == T_KW_RETURN)
    {
        printf("KW_RETURN\n");
    }
    else if(token == T_KW_STRING)
    {
        printf("KW_STRING\n");
    }
    else if(token == T_KW_THEN)
    {
        printf("KW_THEN\n");
    }
    else if(token == T_KW_WHILE)
    {
        printf("KW_WHILE\n");
    }
    else if(token == T_STRLEN)
    {
        printf("STRLEN\n");
    }
    else if(token == T_ADD)
    {
        printf("ADD\n");
    }
    else if(token == T_SUB)
    {
        printf("SUB\n");
    }
    else if(token == T_MUL)
    {
        printf("MUL\n");
    }
    else if(token == T_DIV_NUMBER)
    {
        printf("DIV_NUMBER\n");
    }
    else if(token == T_DIV_INTEGER)
    {
        printf("DIV_INTEGER\n");
    }
    else if(token == T_CONCATENATION)
    {
        printf("CONCATENATION\n");
    }
    else if(token == T_LT)
    {
        printf("LT\n");
    }
    else if(token == T_GT)
    {
        printf("GT\n");
    }
    else if(token == T_LET)
    {
        printf("LET\n");
    }
    else if(token == T_GET)
    {
        printf("GET\n");
    }
    else if(token == T_EQ)
    {
        printf("EQ\n");
    }
    else if(token == T_NEQ)
    {
        printf("NEQ\n");
    }
    else if(token == T_EOF)
    {
        printf("EOF\n");
    }
    else if(token == T_EOL)
    {
        printf("EOL\n");
    }
    else if(token == T_NUM_INTEGER)
    {
        printf("NUM_INTEGER\n");
    }
    else if(token == T_NUM_NUMBER)
    {
        printf("NUM_NUMBER\n");
    }
    else if(token == T_BRACKET_RIGHT)
    {
        printf("BRACKET_RIGHT\n");
    }
    else if(token == T_BRACKET_LEFT)
    {
        printf("BRACKET_LEFT\n");
    }
    else if(token == T_COLON)
    {
        printf("COLON\n");
    }
    else if(token == T_STRING)
    {
        printf("STRING\n");
    }
    else if(token == T_SETVALUE)
    {
        printf("SETVALUE\n");
    }
    else if(token == T_EMPTY)
    {
        printf("EMPTY\n");
    }
    else if(token == T_COMMA)
    {
        printf("COMMA\n");
    }
    else
    {
        printf("není\n");
    }
}
