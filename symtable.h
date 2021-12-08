/*
* Project: IFJ 2021
*
* @file symtable.h
*
* @brief Knihovna pro symtable.c
*
* @author Jan Zdeněk (xzdene01)
* @author Alexander Sila (xsila00)
*/

#include "scanner.h"

typedef struct SNode{
  //typ nodu
  bool func;
  bool var;
  bool cond;
  bool cycle;

  //pro funkci
  bool def_func; //jestli je funkce definovana
  Token* id_func; //nazev funkce v token.value.string
  TList params; //seznam parametru
  TList returns; //seznam navratovych typu
  int numParams; //pocet parametru
  int numReturns; //pocet navratovych typu

  //pro promenou
  Token* id_var; //nazev promenne v token.value.string
  Token_type type; //typ promenne

  struct SNode *next;
  struct SNode *prev;
}*SNodePtr;

typedef struct{
  SNodePtr first;
  SNodePtr last;
  SNodePtr lastFunc; //ukazatel na posledni prvek ktery je funkce
  SNodePtr active;
}SList;

/**
 * @brief Funkce pro inicializaci seznamu symbolu
 * @param SList ktery chceme inicializovat
 * @return Error
 */
int SListInit(SList *);

/**
 * @brief Funkce pro posunuti aktivity v seznamu doprava
 * @param SList ve kterem posouvame
 * @return Error
 */
int SListNext(SList *);

/**
 * @brief Funkce pro posunuti aktivity v seznamu doleva
 * @param SList ve kterem posouvame
 * @return Error
 */
int SListPrev(SList *);

/**
 * @brief Funkce vlozeni podminky do seznamu symbolu
 * @param SList do ktereho vkladame
 * @return Error
 */
int SListInsertCond(SList *);

/**
 * @brief Funkce vlozeni cyklu do seznamu symbolu
 * @param SList do ktereho vkladame
 * @return Error
 */
int SListInsertCycle(SList *);

/**
 * @brief Funkce vlozeni funkce do seznamu symbolu
 * @param SList do ktereho vkladame
 * @return Error
 */
int SListInsertFunc(SList *, bool, Token*);

/**
 * @brief Funkce vlozeni parametru do posledni funkce v seznamu
 * @param SList do ktereho vkladame
 * @return Error
 */
int SListInsertParam(SList *, Token_type);

/**
 * @brief Funkce vlozeni navratoveho typu do posledni funkce v seznamu
 * @param SList do ktereho vkladame
 * @return Error
 */
int SListInsertReturn(SList *, Token_type);

/**
 * @brief Funkce vlozeni promenne do seznamu symbolu
 * @param SList do ktereho vkladame
 * @return Error
 */
int SListInsertVar(SList *, Token *, Token_type);

/**
 * @brief Funkce pro smazani posledniho prvky ze seznamu symbolu
 * @param SList ze ktereho mazeme
 * @return Error
 */
int SListDeleteLast(SList *);

/**
 * @brief Funkce pro smazani celeho seznamu symbolu
 * @param SList ktery mazeme
 * @return Error
 */
int SListDispose(SList *);

/**
 * @brief Funkce pro urceni deklarace nebo definice funkce
 * @param SList ve kterem hledame deklarovanou nebo definovanou funkci
 * @return 0 -> je deklarovana, 1 -> je i definovana, 3 -> neni deklarovana, other -> chyba
 */
int IsDeclaredFunc(SList *, Token *);

/**
 * @brief Funkce pro urceni deklarace nebo definice funkce s preskocenim posledni funkce
 * @param SList ve kterem hledame deklarovanou nebo definovanou funkci
 * @return 0 -> je deklarovana, 1 -> je i definovana, 3 -> neni deklarovana, other -> chyba
 */
int IsDeclaredJump(SList *, Token *);

/**
 * @brief Funkce pro urceni definice promenne
 * @param SList ve kterem hledame definovanou promennou
 * @return 0 -> je deklarovana, 3 -> neni deklarovana, other -> chyba
 */
int IsDeclaredVar(SList *, Token *);

/**
 * @brief Funkce pro urceni definice promenne v aktualnim scopu
 * @param SList ve kterem hledame definovanou promennou
 * @return 0 -> je deklarovana, 3 -> neni deklarovana, other -> chyba
 */
int IsDeclaredVarInScope(SList *, Token *);

/**
 * @brief Funkce pro smazani aktualniho scopu
 * @param SList ve kterem mazeme aktualni scope
 * @return Error
 */
int DeleteScope(SList *);

/**
 * @brief Funkce pro urceni definice promenne jako integer
 * @param SList ve kterem hledame definovanou promennou
 * @return 0 -> je integer, 3 -> neni deklarovana, 4 -> neni integer other -> chyba
 */
int IsInteger(SList *, Token *);

/**
 * @brief Funkce pro urceni definice promenne jako number
 * @param SList ve kterem hledame definovanou promennou
 * @return 0 -> je number, 3 -> neni deklarovana, 4 -> neni number other -> chyba
 */
int IsNumber(SList *, Token *);

/**
 * @brief Funkce pro urceni definice promenne jako string
 * @param SList ve kterem hledame definovanou promennou
 * @return 0 -> je string, 3 -> neni deklarovana, 4 -> neni string other -> chyba
 */
int IsString(SList *, Token *);

/**
 * @brief Funkce pro urceni definice promenne jako nil
 * @param SList ve kterem hledame definovanou promennou
 * @return 0 -> je nil, 3 -> neni deklarovana, 4 -> neni nil other -> chyba
 */
int IsNil(SList *, Token *);

/**
 * @brief Funkce ktera nastavi ukazatel lastFunc
 * @param SList ve kterem nastavujeme
 * @return Error
 */
int LastFunc(SList *);

/**
 * @brief Funkce ktera porovna dva nody navzajem
 * @params SNodePtr nody ktere porovnavam
 * @return 0 -> jsou stejne, 1 -> nejsou stejne, 2 -> rozdil v returns, other -> chyba
 */
int IsClone(SNodePtr, SNodePtr);

/*-----POMOCNE FUNKCE-----*/

/**
 * @brief Funkce pro tisk celeho seznamu symbolu
 * @param SList ktery chceme tisknout
 */
void PrintSList(SList *);
