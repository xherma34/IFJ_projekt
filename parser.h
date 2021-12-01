#include "scanner.h"

/*-----FUNKCE NA UROVNI CELEHO PROGRAMU-----*/

/**
 * @brief Funkce volana z main.c, nasledne vola Program() pro parsovani vsech tokenu
 * @param TList ktery obsahuje vsechny tokeny vstupniho programu
 * @return Integer, vraci chybovou hodnotu
 */
int Parse(TList *);

/**
 * @brief Funkce volana z Parse(), kontroluje hlavicku, telo a EOF programu (<program>)
 * @param TList ktery obsahuje vsechny tokeny vstupniho programu
 * @return Integer, vraci chybovou hodnotu
 */
int Program(TList *);

/**
 * @brief Funkce volana z Program(), kontroluje syntax hlavniho tela programu (<main_body>)
 * @param TList ktery obsahuje vsechny tokeny vstupniho programu
 * @return Integer, vraci chybovou hodnotu
 */
int MainBody(TList *);

/*-----FUNKCE NA UROVNI HLAVNIHO TELA PROGRAMU-----*/

/**
 * @brief Funkce kontroluje syntax pro deklaraci funkci (<dec_function>)
 * @param TList ktery obsahuje vsechny tokeny vstupniho programu
 * @return Integer, vraci chybovou hodnotu
 */
int DecFunction(TList *);

/**
 * @brief Funkce kontroluje syntax pro definici funkci (<def_function>)
 * @param TList ktery obsahuje vsechny tokeny vstupniho programu
 * @return Integer, vraci chybovou hodnotu
 */
int DefFunction(TList *);

/**
 * @brief Funkce kontroluje syntax pro volani funkci (<call_function>)
 * @param TList ktery obsahuje vsechny tokeny vstupniho programu
 * @return Integer, vraci chybovou hodnotu
 */
int CallFunction(TList *);

/*-----FUNKCE NA UROVNI DEFINICE FUNKCE-----*/

/**
 * @brief Funkce kontroluje syntax pro telo jednotlivych definic funkci (<main_body>)
 * @param TList ktery obsahuje vsechny tokeny vstupniho programu
 * @return Integer, vraci chybovou hodnotu
 */
int FceBody(TList *);

/*-----FUNKCE NA UROVNI JEDNOTLIVYCH TEL FUNKCI-----*/

/**
 * @brief Funkce kontroluje syntax pro definici promenne (<def_var>)
 * @param TList ktery obsahuje vsechny tokeny vstupniho programu
 * @return Integer, vraci chybovou hodnotu
 */
int DefVar(TList *);

/**
 * @brief Funkce kontroluje syntax pro pripisovani hodnot (<assign>)
 * @param TList ktery obsahuje vsechny tokeny vstupniho programu
 * @return Integer, vraci chybovou hodnotu
 */
int Assign(TList *);

/**
 * @brief Funkce kontroluje syntax pro podminku (<cond>)
 * @param TList ktery obsahuje vsechny tokeny vstupniho programu
 * @return Integer, vraci chybovou hodnotu
 */
int Cond(TList *);

/**
 * @brief Funkce kontroluje syntax pro while cyklus (<cycle>)
 * @param TList ktery obsahuje vsechny tokeny vstupniho programu
 * @return Integer, vraci chybovou hodnotu
 */
int Cycle(TList *);

/**
 * @brief Funkce kontroluje syntax pro vraceni z funkce (<return>)
 * @param TList ktery obsahuje vsechny tokeny vstupniho programu
 * @return Integer, vraci chybovou hodnotu
 */
int Return(TList *);

/*-----POMOCNE FUNKCE PRO KONTROLU SYNTAXE-----*/

/**
 * @brief Pomocna funkce pro kontrolu syntaxu vice data typu (<data_types>)
 * @param TList ktery obsahuje vsechny tokeny vstupniho programu
 * @return Integer, vraci chybovou hodnotu
 */
int DataTypes(TList *);

/**
 * @brief Pomocna funkce pro kontrolu syntaxu jednoto datatypu (<data_type>)
 * @param TList ktery obsahuje vsechny tokeny vstupniho programu
 * @return Integer, vraci chybovou hodnotu
 */
int DataType(TList *);

/**
 * @brief Pomocna funkce pro kontrolu syntaxu vice parametru (<params>)
 * @param TList ktery obsahuje vsechny tokeny vstupniho programu
 * @return Integer, vraci chybovou hodnotu
 */
int Params(TList *);

/**
 * @brief Pomocna funkce pro kontrolu syntaxu jednoho parametru (<param>)
 * @param TList ktery obsahuje vsechny tokeny vstupniho programu
 * @return Integer, vraci chybovou hodnotu
 */
int Param(TList *);

/**
 * @brief Pomocna funkce pro kontrolu syntaxu vice vracenych typu (<return_types>)
 * @param TList ktery obsahuje vsechny tokeny vstupniho programu
 * @return Integer, vraci chybovou hodnotu
 */
int ReturnTypes(TList *);

/**
 * @brief Pomocna funkce pro kontrolu syntaxu jednoho vraceneho typu (<return_type>)
 * @param TList ktery obsahuje vsechny tokeny vstupniho programu
 * @return Integer, vraci chybovou hodnotu
 */
int ReturnType(TList *);

/**
 * @brief Pomocna funkce pro kontrolu syntaxu vice identifikatotu (<ids>)
 * @param TList ktery obsahuje vsechny tokeny vstupniho programu
 * @return Integer, vraci chybovou hodnotu
 */
int Ids(TList *);

/**
 * @brief Pomocna funkce pro kontrolu syntaxu vice expresi
 * @param TList ktery obsahuje vsechny tokeny vstupniho programu
 * @return Integer, vraci chybovou hodnotu
 */
int Exps(TList *);

/**
 * @brief Pomocna funkce pro kontrolu syntaxu vice identifikatoru a datatypu (<ids_datatypes>)
 * @param TList ktery obsahuje vsechny tokeny vstupniho programu
 * @return Integer, vraci chybovou hodnotu
 */
int Ids_Datatypes(TList *);

/**
 * @brief Pomocna funkce pro kontrolu syntaxu vice expresi a retezcu (<exps_strings>)
 * @param TList ktery obsahuje vsechny tokeny vstupniho programu
 * @return Integer, vraci chybovou hodnotu
 */
int Exps_Strings(TList *);

/*-----JINE POMOCNE FUNKCE------*/

/**
 * @brief Funkce pro printovani typu konkretniho tokenu
 * @param Token_type ktery chceme printovat
 */
void PrintToken(Token_type);

/*-----DOCASNE POMOCNE FUNKCE-----*/

int Exp(TList *);

int IsExp(TList *);
