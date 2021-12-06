#include "scanner.h"

typedef struct SNode{
  //typ nodu
  bool func;
  bool var;
  bool cond;
  bool cycle;

  //pro funkci
  bool def_func;
  Token* id_func;
  TList params;
  TList returns;
  int numParams;
  int numReturns;

  //pro promenou
  Token* id_var;
  Token_type type;

  struct SNode *next;
  struct SNode *prev;
}*SNodePtr;

typedef struct{
  SNodePtr first;
  SNodePtr last;
  SNodePtr lastFunc;
  SNodePtr active;
}SList;

int SListInit(SList *);
int SListNext(SList *);
int SListPrev(SList *);
int SListInsertCond(SList *);
int SListInsertCycle(SList *);
int SListInsertFunc(SList *, bool, Token*);
int SListInsertParam(SList *, Token_type);
int SListInsertReturn(SList *, Token_type);
int SListInsertVar(SList *, Token *, Token_type);
int SListDeleteLast(SList *);
int SListDispose(SList *);

int IsDeclaredFunc(SList *, Token *);
int IsDeclaredJump(SList *, Token *);
int IsDeclaredVar(SList *, Token *);
int IsDeclaredVarInScope(SList *, Token *);
int DeleteScope(SList *);

int IsInteger(SList *, Token *);
int IsNumber(SList *, Token *);
int IsString(SList *, Token *);
int IsNil(SList *, Token *);

void LastFunc(SList *);

void PrintSList(SList *);
