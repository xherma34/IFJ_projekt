/*
* Project: IFJ 2021
*
* @file TStack.c
*
* @brief Implementace zásobníku tokenů
*
* @author Pavel Heřmann (xherma34)
* @author Maxim Plička (xplick04)
*/

#include "TStack.h"

void TStackInit(TStack *stack)
{
	//Pro max_size stacku nastavi vsechny prvky stacku na null
	for (int i = 0; i < 100; i++)
	{
		stack->stackToken[i] = NULL; //Pro korektni inicializaci radsi vynuluji vsechny indexi stacku
	}
	//Index -1 znaci prazdny stack
	stack->topIndex = -1;
	//Naalokuji misto pro I_DOLLAR ktery znaci dno zasobniku pro expression handling
	Token *bottom = malloc(sizeof(Token));
	bottom->PTindex = I_DOLLAR;
	//Pushnu dollar na stack
	TStackPush(stack, bottom);
}

int TStackPush(TStack *stack, Token *token)
{
	//Osetreni ze neni plny stack
	if(stack->topIndex == MAX_STACK-1)
	{
		return 1;
	}
	stack->topIndex += 1;
	stack->stackToken[stack->topIndex] = token;

	return 0;
}

void TStackTop(TStack *stack, Token *token)
{
	//Prepisu vlozenemu tokenu hodnoty stackTopu
	token->type = stack->stackToken[stack->topIndex]->type;
	token->PTindex = stack->stackToken[stack->topIndex]->PTindex;
	token->value = stack->stackToken[stack->topIndex]->value;
	token->line = stack->stackToken[stack->topIndex]->line;
}

int TStackPop(TStack *stack)
{
	//Osetreni, ze neni prazdny stack
	if(stack->topIndex != -1)
	{
		stack->topIndex -= 1;
		return 0;
	}
	return 1;
}

void TStackTopNotE(TStack *stack, Token *token)
{
	//Pokud stackTOp neni I_E muzu vracet stack top
	if(stack->stackToken[stack->topIndex]->PTindex != I_E)
	{
		token->PTindex = stack->stackToken[stack->topIndex]->PTindex;
	}
	else
	{
		//Pokud je stack top I_E a pod nim je cokoliv jineho nez stopSign, vracim afterTop prvek
		if(stack->stackToken[stack->topIndex-1]->PTindex != I_STOP)
		{
			token->PTindex = stack->stackToken[stack->topIndex-1]->PTindex;
		}
		//POkud je stack top I_E pod nim StopSign, vracim prvek az za StopSignem
		else
		{
			token->PTindex = stack->stackToken[stack->topIndex-2]->PTindex;
		}
	}

}

int TStackUntillStopSign(TStack *stack)
{
	int count = stack->topIndex;
	//Kontrola ze neni prazdny stack
	if(stack->topIndex == -1)
	{
		return -1; //chyba
	}
	while(1)
	{
		//Pokud jsem narazil na StopSign, breakuji
		if(stack->stackToken[count]->PTindex == I_STOP)
		{
			break;
		}
		//Pokud jsem narazil na I_DOLLAR znamena to, ze se funkce zavolala u stacku, kde neni zadny StopSign, return chybu
		if(stack->stackToken[count]->PTindex == I_DOLLAR)
		{
			return -1;
		}
		count--;
	}
	//TopIndex - count = pocet prvku mezi stackTop a stopSign
	return (stack->topIndex - count);
}

int TStackPushAfterTop(TStack *stack, Token *token)
{
	//Kontrola ze neni stack prazdny
	if(stack->topIndex == -1)
	{
		return 1;
	}

	stack->stackToken[stack->topIndex+1] = stack->stackToken[stack->topIndex];
	stack->stackToken[stack->topIndex] = token;
	stack->stackToken[stack->topIndex]->PTindex = token->PTindex;
	stack->topIndex += 1;

	return 0;
}

int TStackPushStop(TStack *stack)
{
	//Naalokuji stopSign a inicializuji
	Token *stop = malloc(sizeof(Token));
	stop->PTindex = I_STOP;

	//Pokud neni stackTOp stopSign||I_E vracim prvek, pushuji
	if(stack->stackToken[stack->topIndex]->PTindex != I_STOP && stack->stackToken[stack->topIndex]->PTindex != I_E)
	{
		TStackPush(stack, stop);
		return 0;
	}
	//Handle pokud je stackTop I_E v kombinaci se stopSign
	else
	{
		if(stack->stackToken[stack->topIndex-1]->PTindex != I_STOP && stack->stackToken[stack->topIndex-1]->PTindex != I_E)
		{
			TStackPushAfterTop(stack, stop);
		}
		else
		{
			stack->stackToken[stack->topIndex+1] = stack->stackToken[stack->topIndex];
			stack->stackToken[stack->topIndex] = stack->stackToken[stack->topIndex-1];
			stack->stackToken[stack->topIndex-1] = stop;
			stack->stackToken[stack->topIndex-1]->PTindex = stop->PTindex;
			stack->topIndex += 1;
		}
	return 0;
	}
return 1;
}

void freeStackE(TStack *stack)
{
	//Freeovani vsech I_E, I_STOP, I_DOLLAR, ktere nejsou z puvodniho listu tokenu
	for(int i = stack->topIndex; i > -1; i--)
	{
		if(stack->stackToken[i]->PTindex == I_E || stack->stackToken[i]->PTindex == I_STOP || stack->stackToken[i]->PTindex == I_DOLLAR)
		{
			free(stack->stackToken[i]);
		}
	}
}
