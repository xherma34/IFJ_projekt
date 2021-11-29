#include "scanner.h"

int Parse(TList *);
int Program(TList *);
int MainBody(TList *);
int DecFunction(TList *);
int DataTypes(TList *);
int DefFunction(TList *);
int DataType(TList *);
int Params(TList *);
int Param(TList *);
int ReturnTypes(TList *);
int ReturnType(TList *);
int FceBody(TList *);
int DefVar(TList *);
int Assign(TList *);
int Ids(TList *);
int Exps(TList *);
int Cond(TList *);
int Cycle(TList *);
int CallFunction(TList *);
int Return(TList *);

int Exp(TList *);
void PrintToken(Token_type);