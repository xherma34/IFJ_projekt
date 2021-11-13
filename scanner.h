#include <stdio.h>
#include <stdlib.h>
#define FILE_INPUT = "file.txt"

typedef enum
{
    //Identifikator
    T_ID,

    //Klicova slova
    T_KW_DO,
    T_KW_ELSE,
    T_KW_END,
    T_KW_FUNCTION,
    T_KW_GLOBAL,
    T_KW_IF,
    T_KW_LOCAL,
    T_KW_NIL,
    T_KW_INTEGER,      
    T_KW_NUMBER,       
    T_KW_REQUIRE,
    T_KW_RETURN,
    T_KW_STRING,
    T_KW_THEN,
    T_KW_WHILE,

    //Operatory
    T_STRLEN,           //  #
    T_ADD,              //  +
    T_SUB,              //  -
    T_MUL,              //  *
    T_DIV_NUMBER,       //  /   -> NORMALNI DELENI
    T_DIV_INTEGER,      //  //  -> CELOCISELNE DELENI

    //Relacni operatory
    T_LT,       //  <
    T_GT,       //  >
    T_LET,      //  <=
    T_GET,      //  >=
    T_EQ,       //  ==
    T_NEQ,      //  ~=

    //Zbytek
    T_EOL,
    T_EOF,
    T_NUM_INTEGER,
    T_NUM_NUMBER,
    T_BRACKET_RIGHT,
    T_BRACKET_LEFT,
    T_COLON,
    T_STRING,
    T_SET,      //  = -> prirazeni hodnoty
    T_EMPTY,
} Token_type;

typedef union 
{
    int integer;
    double number;
    char *string;
} Token_value;


typedef struct 
{
    Token_type type;
    Token_value value;

} Token;

typedef struct
{
    Token token;
    struct Node *prev;
    struct Node *next;
} TNode;

typedef struct 
{
    TNode first;
    TNode last;
    TNode active;
} TList;
