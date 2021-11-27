#include <stdio.h>

#define MAX_STACK 20

typedef struct {
	char array[MAX_STACK];
	int topIndex;
} Stack;

//DODELAT POPISKY !!!
/**
 * @brief Funkce pro ziskani tokenu ze stdin
 * @param Token do ktereho chceme ulozit typ a hodnotu
 * @return Integer, pokud =0 -> vse probehlo bez chyb, jinak vraci hodnotu nastale chyby
 */
void Stack_Init( Stack * );
/**
 * @brief Funkce pro ziskani tokenu ze stdin
 * @param Token do ktereho chceme ulozit typ a hodnotu
 * @return Integer, pokud =0 -> vse probehlo bez chyb, jinak vraci hodnotu nastale chyby
 */
int Stack_IsEmpty( const Stack * );
/**
 * @brief Funkce pro ziskani tokenu ze stdin
 * @param Token do ktereho chceme ulozit typ a hodnotu
 * @return Integer, pokud =0 -> vse probehlo bez chyb, jinak vraci hodnotu nastale chyby
 */
int Stack_IsFull( const Stack * );
/**
 * @brief Funkce pro ziskani tokenu ze stdin
 * @param Token do ktereho chceme ulozit typ a hodnotu
 * @return Integer, pokud =0 -> vse probehlo bez chyb, jinak vraci hodnotu nastale chyby
 */
void Stack_Top( const Stack *, char * );
/**
 * @brief Funkce pro ziskani tokenu ze stdin
 * @param Token do ktereho chceme ulozit typ a hodnotu
 * @return Integer, pokud =0 -> vse probehlo bez chyb, jinak vraci hodnotu nastale chyby
 */
void Stack_Pop( Stack * );
/**
 * @brief Funkce pro ziskani tokenu ze stdin
 * @param Token do ktereho chceme ulozit typ a hodnotu
 * @return Integer, pokud =0 -> vse probehlo bez chyb, jinak vraci hodnotu nastale chyby
 */
void Stack_Push( Stack *, char );
