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

    if(list->active->token.type != T_KW_REQUIRE)
    {
        return 2;
    }

    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        return error;
    }

    if(list->active->token.type != T_STRING || strcmp(list->active->token.value.string, "ifj21") != 0)
    {
        return 2;
    }

    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        return error;
    }

    if(list->active->token.type != T_EOF)
    {
        error = MainBody(&(*list));
        return error;
    }
    else
    {
        return 0;
    }


}

int MainBody(TList *list)
{
    int error = 0;

    if(list->active->token.type != T_KW_GLOBAL && list->active->token.type != T_KW_FUNCTION
       && list->active->token.type != T_ID)
    {
        return 2;
    }

    if(list->active->token.type == T_KW_GLOBAL)
    {
        error = TListTokenNext(&(*list));
        if(error != 0)
        {
            return error;
        }

        error = DecFunction(&(*list));
    }
    else if(list->active->token.type == T_KW_FUNCTION)
    {
        error = TListTokenNext(&(*list));
        if(error != 0)
        {
            return error;
        }

        error = DefFunction(&(*list));
    }

    else if(list->active->token.type == T_ID)
    {

        error = TListTokenNext(&(*list));
        if(error != 0)
        {
            return error;
        }

        error = CallFunction(&(*list));
    }

    if(error != 0 || list->active->token.type == T_EOF)
    {
        return error;
    }

    error = MainBody(&(*list));

    return error;
}

int DecFunction(TList *list)
{
    int error;

    if(list->active->token.type != T_ID)
    {
        return 2;
    }

    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        return error;
    }

    if(list->active->token.type != T_COLON)
    {
        return 2;
    }

    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        return error;
    }

    if(list->active->token.type != T_KW_FUNCTION)
    {
        return 2;
    }

    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        return error;
    }

    if(list->active->token.type != T_BRACKET_LEFT)
    {
        return 2;
    }

    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        return error;
    }

    if(list->active->token.type != T_BRACKET_RIGHT)
    {
        error = DataTypes(&(*list));

        if(error != 0)
        {
            return error;
        }
    }

    if(list->active->token.type != T_BRACKET_RIGHT)
    {
        return 2;
    }

    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        return error;
    }

    error = ReturnTypes(&(*list));

    return error;
}

int DataTypes(TList *list)
{
    int error;

    error = DataType(&(*list));

    if(error != 0 || list->active->token.type != T_COMMA)
    {
        return error;
    }

    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        return error;
    }

    error = DataTypes(&(*list));

    return error;
}

int DataType(TList *list)
{
    int error = 2;

    if(list->active->token.type == T_KW_INTEGER || list->active->token.type == T_KW_NUMBER
    || list->active->token.type == T_KW_STRING || list->active->token.type == T_KW_NIL)
    {
        error = 0;
    }

    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        return error;
    }

    return error;
}

int ReturnTypes(TList *list)
{
    int error;

    if(list->active->token.type == T_COLON)
    {
        error = TListTokenNext(&(*list));
        if(error != 0)
        {
            return error;
        }

        error = ReturnType(&(*list));
        return error;
    }
    else
    {
        return 0;
    }
}

int ReturnType(TList *list)
{
    int error;

    error = DataType(&(*list));

    if(error != 0)
    {
        return error;
    }

    if(list->active->token.type == T_COMMA)
    {

        error = TListTokenNext(&(*list));
        if(error != 0)
        {
            return error;
        }

        error = ReturnType(&(*list));
    }

    return error;
}

int DefFunction(TList *list)
{
    int error = 0;

    if(list->active->token.type != T_ID)
    {
        return 2;
    }

    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        return error;
    }

    if(list->active->token.type != T_BRACKET_LEFT)
    {
        return 2;
    }

    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        return error;
    }

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
        return 2;
    }

    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        return error;
    }

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
        return 2;
    }

    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        return error;
    }

    return 0;
}

int Params(TList *list)
{
    int error;

    error = Param(&(*list));

    if(error != 0)
    {
        return error;
    }

    if(list->active->token.type != T_COMMA)
    {
        return error;
    }

    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        return error;
    }

    error = Params(&(*list));
    return error;
}

int Param(TList *list)
{
    int error;

    if(list->active->token.type != T_ID)
    {
        return 2;
    }

    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        return error;
    }

    if(list->active->token.type != T_COLON)
    {
        return 2;
    }

    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        return error;
    }

    error = DataType(&(*list));

    return error;
}

int CallFunction(TList *list)
{
    int error;

    if(list->active->token.type != T_BRACKET_LEFT)
    {
        return 2;
    }

    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        return error;
    }

    if(list->active->token.type != T_BRACKET_RIGHT)
    {
        error = Ids_Datatypes(&(*list));
        if(error != 0)
        {
            return error;
        }
    }

    if(list->active->token.type != T_BRACKET_RIGHT)
    {
        return 2;
    }

    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        return error;
    }

    return 0;
}

int Ids_Datatypes(TList *list)
{
    int error;

    if(list->active->token.type != T_ID && list->active->token.type != T_STRING &&
    list->active->token.type != T_NUM_INTEGER && list->active->token.type != T_NUM_NUMBER &&
    list->active->token.type != T_KW_NIL)
    {
        return 2;
    }

    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        return error;
    }

    if(list->active->token.type == T_COMMA)
    {
        error = TListTokenNext(&(*list));
        if(error != 0)
        {
            return error;
        }

        error = Ids_Datatypes(&(*list));

        return error;
    }
    return 0;
}

int FceBody(TList *list)
{
    int error = 0;

    if(list->active->token.type != T_KW_LOCAL && list->active->token.type != T_KW_IF &&
    list->active->token.type != T_KW_WHILE && list->active->token.type != T_ID &&
    list->active->token.type != T_KW_RETURN)
    {
        return 2;
    }

    if(list->active->token.type == T_KW_LOCAL)
    {
        error = TListTokenNext(&(*list));
        if(error != 0)
        {
            return error;
        }
        error = DefVar(&(*list));
    }
    else if(list->active->token.type == T_KW_IF)
    {
        error = TListTokenNext(&(*list));
        if(error != 0)
        {
            return error;
        }
        error = Cond(&(*list));
    }
    else if(list->active->token.type == T_KW_WHILE)
    {
        error = TListTokenNext(&(*list));
        if(error != 0)
        {
            return error;
        }
        error = Cycle(&(*list));
    }
    else if(list->active->token.type == T_ID)
    {
        error = TListTokenNext(&(*list));
        if(error != 0)
        {
            return error;
        }

        if(list->active->token.type == T_BRACKET_LEFT)
        {
            error = CallFunction(&(*list));
        }
        else
        {
            TListTokenPrev(&(*list));
            error = Assign(&(*list));
        }
    }
    else if(list->active->token.type == T_KW_RETURN)
    {
        error = TListTokenNext(&(*list));
        if(error != 0)
        {
            return error;
        }
        error = Return(&(*list));
    }

    if(error != 0 || list->active->token.type == T_KW_END || list->active->token.type == T_KW_ELSE)
    {
        return error;
    }

    error = FceBody(&(*list));
    return error;
}

int DefVar(TList *list)
{
    int error;

    error = Ids(&(*list));

    if(error != 0)
    {
        return error;
    }

    if(list->active->token.type != T_COLON)
    {
        return 2;
    }

    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        return error;
    }

    error = DataTypes(&(*list));

    if(error != 0)
    {
        return 2;
    }

    if(list->active->token.type == T_SETVALUE)
    {
        error = TListTokenNext(&(*list));
        if(error != 0)
        {
            return error;
        }

        error = Exps_Strings(&(*list));
    }

    return error;
}

int Cond(TList *list)
{
    int error;

    error = Exp(&(*list));

    if(error != 0)
    {
        return 2;
    }

    if(list->active->token.type != T_KW_THEN)
    {
        return 2;
    }

    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        return error;
    }

    if(list->active->token.type != T_KW_ELSE)
    {
        error = FceBody(&(*list));

        if(error != 0)
        {
            return 2;
        }
    }


    if(list->active->token.type != T_KW_ELSE)
    {
        return 2;
    }

    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        return error;
    }

    if(list->active->token.type == T_KW_END)
    {
        error = TListTokenNext(&(*list));
        if(error != 0)
        {
            return error;
        }
        return 0;
    }

    error = FceBody(&(*list));

    if(error != 0)
    {
        return 2;
    }

    if(list->active->token.type != T_KW_END)
    {
        return 2;
    }

    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        return error;
    }

    return 0;
}

int Cycle(TList *list)
{
    int error;

    Exp(&(*list));

    if(list->active->token.type != T_KW_DO)
    {
        return 2;
    }

    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        return error;
    }

    error = FceBody(&(*list));

    if(error != 0)
    {
        return error;
    }

    if(list->active->token.type != T_KW_END)
    {
        return 2;
    }

    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        return error;
    }

    return 0;
}

int Return(TList *list)
{
    if(IsExp(&(*list)) == 2)
    {
        Exps(&(*list));
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
        return 2;
    }

    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        return error;
    }

    error = Exps(&(*list));

    return error;
}

int Ids(TList *list)
{
    int error = 0;

    if(list->active->token.type != T_ID)
    {
        return 2;
    }

    error = TListTokenNext(&(*list));
    if(error != 0)
    {
        return error;
    }

    if(list->active->token.type == T_COMMA)
    {
        error = TListTokenNext(&(*list));
        if(error != 0)
        {
            return error;
        }

        error = Ids(&(*list));
    }

    return error;
}

int Exps(TList *list)
{
    int error;

    Exp(&(*list));

    if(list->active->token.type == T_COMMA)
    {
        error = TListTokenNext(&(*list));
        if(error != 0)
        {
            return error;
        }
        error = Exps(&(*list));
    }

    return error;
}

int Exp(TList *list)
{
    int error;

    int line = list->active->token.line;

    while(line == list->active->token.line)
    {
        error = TListTokenNext(&(*list));
        if(error != 0)
        {
            return error;
        }
    }
    return 0;
}

int Exps_Strings(TList *list)
{
    int error = 0;

    if(list->active->token.type != T_STRING)
    {
        error = Exp(&(*list));
    }
    else
    {
        error = TListTokenNext(&(*list));
        if(error != 0)
        {
            return error;
        }
    }

    if(list->active->token.type == T_COMMA)
    {
        error = TListTokenNext(&(*list));
        if(error != 0)
        {
            return error;
        }

        error = Exps_Strings(&(*list));
    }

    return error;
}

//-------------------------------------NOT ACTUALLY HELPFULL-------------------------------------------

int IsExp(TList *list)
{
    int is = 0;

    if(list->active->token.type == T_ID || list->active->token.type == T_NUM_INTEGER ||
    list->active->token.type == T_NUM_NUMBER || list->active->token.type == T_STRING ||
    list->active->token.type == T_STRLEN || list->active->token.type == T_KW_NIL)
    {
        is = 1;
    }

    return is;
}

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
