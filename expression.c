/*
* Project: IFJ 2021
*
* @file expression.c
*
* @brief Implementace precedenční syntaktické analýzy
*
* @author Pavel Heřmann (xherma34)
* @author Maxim Plička (xplick04)
*/

#include "expression.h"

#define PTSIZE 9

/**
 * @brief returnuje sign z precedencni tabulky podle dvou prichozich 1. pozice stack 2. prichozi symbol
 *
 */
char PrecedenceTable[PTSIZE][PTSIZE] =
{
						//   +  *  <  .. #  (  )  i  $
/*+,-*/         {R, S, R, R, S, S, R, S, R},
/**,/,//*/      {R, R, R, R, S, S, R, S, R},
/*R.Operator*/  {S, S, X, S, S, S, R, S, R},
/*..*/          {S, S, R, S, X, S, R, S, R},
/*#*/           {R, R, R, R, X, S, R, S, R},
/*(*/           {S, S, S, S, S, S, E, S, X},
/*)*/           {R, R, R, R, R, X, R, X, R},
/*i*/           {R, R, R, R, X, X, R, X, R},
/*$*/           {S, S, S, S, S, S, X, S, X}
};

void getIndex(TNodePtr node, SList *slist)
{
	switch (node->token.type) {
		case T_ID :
			//Kontroluji zda neni t_id funkce, pokud ano nastavuji I_DOLLAR -> zbytek se vyresi sam v exp funkci
			if(!IsDeclaredFunc(slist, &node->token)) node->token.PTindex = I_DOLLAR;
			//Pokud nactu operand na operand, vracim jen prvni expression a 0
			else if((node->prev->token.PTindex == I_ID ||
			node->prev->token.PTindex == I_R_BRACKET) &&
			node->prev != NULL)
			{
				node->token.PTindex = I_DOLLAR;
			}
			//Vse ostatni
			else
			{
				node->token.PTindex = I_ID;
			}
			break;
		case T_STRING :
		case T_NUM_INTEGER :
		case T_NUM_NUMBER :
		case T_KW_NIL :
			//Pokud nactu operand na operand, vracim jen prvni expression a 0
			if(node->prev->token.PTindex == I_ID && node->prev != NULL)
			{
				//Poresit case, kdy pri ...= a a = a + b, jestli se to pred assign bude zpracovavat v EXP funkci
				node->token.PTindex = I_DOLLAR;
			}
			//Vse ostatni
			else
			{
				node->token.PTindex = I_ID;
			}
			break;

		case T_ADD:
		case T_SUB:
			node->token.PTindex = I_ADD_SUB;
			break;

		case T_MUL:
		case T_DIV_NUMBER:
		case T_DIV_INTEGER:
			node->token.PTindex = I_MUL_DIV;
			break;

		case T_LT:
		case T_GT:
		case T_LET:
		case T_GET:
		case T_EQ:
		case T_NEQ:
			node->token.PTindex = I_REL_OPERATOR;
			break;

		case T_STRLEN:
			node->token.PTindex = I_STRLEN;
			break;
		case T_CONCATENATION:
			node->token.PTindex = I_CONCATENATION;
			break;

		case T_BRACKET_LEFT:
			node->token.PTindex = I_L_BRACKET;
			break;
		case T_BRACKET_RIGHT:
			node->token.PTindex = I_R_BRACKET;
			break;

		default:
			node->token.PTindex = I_DOLLAR;
			break;
	}
}

int getIndexValue(Token *token)
{
	//vraci pozici radku/sloupce v tabulce
	switch (token->PTindex) {
		case I_ADD_SUB:
			return 0;
		case I_MUL_DIV:
			return 1;
		case I_REL_OPERATOR:
			return 2;
		case I_CONCATENATION:
			return 3;
		case I_STRLEN:
			return 4;
		case I_L_BRACKET:
			return 5;
		case I_R_BRACKET:
			return 6;
		case I_ID:
			return 7;
		case I_DOLLAR:
			return 8;
		default:
			return -1;
	}
}

PTaction getAction(Token *stackTop, Token *listToken)
{
		return PrecedenceTable[getIndexValue(stackTop)][getIndexValue(listToken)];
}

//Az bude symtable, pridat hlidani datovych typu
//Seamnticka kontrola t1,t2, pokud je pravidlo #E
//Semanticka kontrolat t1, t3, pokud je pravidlo E_OPERATOR_E
//KOntrola, pokud t1/t3 == NIL, t2 == NEQ/EQ else return error
PTRule getRule(int cnt, Token *token1, Token *token2, Token *token3)
{
	//Token 3 = pred-predposledni prvek ve stacku
	//TOken 2 = predposledni prvek ve stacku
	//Token 1 = stack top

	//E -> i
	if(cnt == 1)
	{
		switch (token1->type)
		{
		case T_KW_NIL:
		case T_NUM_INTEGER:
		case T_NUM_NUMBER:
		case T_ID:
		case T_STRING:
			return E_ID;

		default:
			return NOT_A_RULE;
		}
	}
	//E -> #i
	else if(cnt == 2)
	{
		if(token1->PTindex == I_E && token2->type == T_STRLEN)
		{
			return STRLEN_E;
		}
		else
		{
			return NOT_A_RULE;
		}
	}
	//E -> E + E atd.
	else if(cnt == 3)
	{
		//E -> (E)
		if(token1->type == T_BRACKET_RIGHT && token2->PTindex == I_E && token3->type == T_BRACKET_LEFT)
		{
			return E_BRACKETS;
		}
		else if(token1->PTindex == I_E && token3->PTindex == I_E) //tokeny prijdou naopak t1 = posledni, t3 = 3. prvek od stackTopu
		{
			switch (token2->type)
			{
			case T_ADD:
				return E_ADD_E;
			case T_SUB:
				return E_SUB_E;
			case T_MUL:
				return E_MUL_E;
			case T_DIV_INTEGER:
				return E_DIV_INT_E;
			case T_DIV_NUMBER:
				return E_DIV_NUM_E;
			case T_LT:
				return E_LT_E;
			case T_LET:
				return E_LET_E;
			case T_GT:
				return E_GT_E;
			case T_GET:
				return E_GET_E;
			case T_EQ:
				return E_EQ_E;
			case T_NEQ:
				return E_NEQ_E;
			case T_CONCATENATION:
				return E_CONCATENATION_E;
			default:
				return NOT_A_RULE;
			}
		}
		else
		{
			return NOT_A_RULE;
		}
	}
	return NOT_A_RULE;
}

int reduceByRule(TStack *stack, Token_type *final_type, SList *slist)
{
	int SSerror = 0;
	PTRule rule;
	Token *t1, *t2, *t3;
	final_type = final_type;
	int cnt = TStackUntillStopSign(stack);
	if(cnt == -1)
	{
		return 2; //stop neni nalezen
	}
	if(cnt == 1)
	{
		t1 = stack->stackToken[stack->topIndex];
	}
	if(cnt == 2)
	{
		t1 = stack->stackToken[stack->topIndex];
		t2 = stack->stackToken[stack->topIndex-1];
	}
	if(cnt == 3)
	{
		t1 = stack->stackToken[stack->topIndex];
		t2 = stack->stackToken[stack->topIndex-1];
		t3 = stack->stackToken[stack->topIndex-2];
	}

	rule = getRule(cnt, t1, t2, t3);

	Token *compressedExpression = malloc(sizeof(Token));
	if(compressedExpression == NULL)
	{
		return 99;
	}
	compressedExpression->PTindex = I_E;

	switch (rule)
	{
	case E_ADD_E:
	case E_SUB_E:
	case E_MUL_E:
	case E_DIV_INT_E:
	case E_DIV_NUM_E:
	case E_CONCATENATION_E:
	case E_LT_E:
	case E_GT_E:
	case E_LET_E:
	case E_GET_E:
		SSerror = CheckSS(rule, t1, t2, t3, final_type, slist);
		if(t1->type == T_ID && t3->type == T_ID)
		{
			compressedExpression->ID = true;
		}
		if(SSerror != 0)
		{
			free(compressedExpression);
			return SSerror;
		}
		compressedExpression->type = *final_type;
		if(t1->PTindex == I_E)
		{
			free(stack->stackToken[stack->topIndex]);
		}
		if(t3->PTindex == I_E)
		{
			free(stack->stackToken[stack->topIndex-2]);
		}
		//Pro generovani budeme muset uchovavat, zda byl prvek uz reducnuty pravidlem
		if(TStackPop(stack)) return 99;
		if(TStackPop(stack)) return 99;
		if(TStackPop(stack)) return 99;
		free(stack->stackToken[stack->topIndex]);
		if(TStackPop(stack)) return 99;
		break;

	case E_EQ_E:
	case E_NEQ_E:
		if(t1->type == T_ID && t3->type == T_ID)
		{
			compressedExpression->ID = true;
		}
		//Pokud prijde nil -> volam semantickou kontrolu
		SSerror = CheckSS(rule, t1, t2, t3, final_type, slist);
		if(SSerror != 0)
		{
			free(compressedExpression);
			return SSerror;
		}
		compressedExpression->type = *final_type;

		if(t1->PTindex == I_E)
		{
			free(stack->stackToken[stack->topIndex]);
		}
		else if(t3->PTindex == I_E)
		{
			free(stack->stackToken[stack->topIndex-2]);
		}

		if(TStackPop(stack)) return 99;
		if(TStackPop(stack)) return 99;
		if(TStackPop(stack)) return 99;
		free(stack->stackToken[stack->topIndex]);
		if(TStackPop(stack)) return 99;
		break;

	case STRLEN_E:
		SSerror = CheckSS(rule, t1, t2, t3, final_type, slist);
		if(SSerror != 0)
		{
			free(compressedExpression);
			return SSerror;
		}

		if(t1->type == T_ID)
		{
			compressedExpression->ID = true;
		}
		compressedExpression->type = *final_type;
		//Nastavuje se na 1, kvuli tomu aby pri A / #"ah" nenastala chyba deleni 0, protoze se nasledne T_STRING pretypuje na final_type integer
		compressedExpression->value.integer = 1;
		free(stack->stackToken[stack->topIndex]);
		if(TStackPop(stack)) return 99;
		if(TStackPop(stack)) return 99;
		free(stack->stackToken[stack->topIndex]);
		if(TStackPop(stack)) return 99;
		//Semanticka kontrola -> t2, musi byt t_string || string id
		break;

	case E_BRACKETS:
		if(TStackPop(stack)) return 99;
		SSerror = CheckSS(rule, t1, t2, t3, final_type, slist);
		if(SSerror != 0)
		{
			free(compressedExpression);
			return SSerror;
		}
		if(t2->type == T_NUM_NUMBER)
		{
			compressedExpression->value.number = t2->value.number;
		}
		else if(t2->type == T_NUM_INTEGER)
		{
			compressedExpression->value.integer = t2->value.integer;
		}
		else if(t2->type == T_ID)
		{
			compressedExpression->ID = true;
		}

		compressedExpression->type = *final_type;
		free(stack->stackToken[stack->topIndex]);
		if(TStackPop(stack)) return 99;
		if(TStackPop(stack)) return 99;
		free(stack->stackToken[stack->topIndex]);
		if(TStackPop(stack)) return 99;
		break;

	case E_ID:

		if(TStackPop(stack)) return 99;
		SSerror = CheckSS(rule, t1, t2, t3, final_type, slist);
		if(SSerror != 0)
		{
			free(compressedExpression);
			return SSerror;
		}
		//Pro deleni nulopu musim predat hodn.
		if(t1->type != T_ID)
		{
			if(t1->type == T_NUM_NUMBER)
			{
				compressedExpression->value.number = t1->value.number;
			}
			else if(t1->type == T_NUM_INTEGER)
			{
				compressedExpression->value.integer = t1->value.integer;
			}
		}
		else //deleni neznamou
		{
			compressedExpression->ID = true;	//Pokud bylo T_ID, musim zachovat
			compressedExpression->value.number = 1;
			compressedExpression->value.integer = 1;
		}


		compressedExpression->PTindex = I_E;
		compressedExpression->type = *final_type;
		free(stack->stackToken[stack->topIndex]);
		if(TStackPop(stack)) return 99;
		break;

	case NOT_A_RULE:
		free(compressedExpression);
		return 2;
	}
if(TStackPush(stack, compressedExpression)) return 99;
return 0;
}

int CheckSS(PTRule rule, Token *t1, Token *t2, Token *t3, Token_type *final_type, SList *slist)
{
	//Pomocne promenne
	bool string_correct = false;
	bool num_correct = false;
	bool string_correct_id = false;
	bool num_correct_id = false;
	bool nil_correct = false;

	switch (rule)
	{
	case E_ADD_E:
	case E_SUB_E:
	case E_MUL_E:
		//Osetreni spatneho typu u aretmicke operace
		if(t1->type == T_STRING)
		{
			return 6;
		}
		else if(t1->type == T_KW_NIL)
		{
			return 8;
		}
		//Pokud je number, musi byt i finalni typ pro print number
		else if(t1->type == T_NUM_NUMBER)
		{
			*final_type = T_NUM_NUMBER;
		}

		//Osetreni spatneho typu u aretmicke operace
		if(t3->type == T_STRING)
		{
			return 6;
		}
		else if(t3->type == T_KW_NIL)
		{
			return 8;
		}
		//Pokud je number, musi byt i finalni typ pro print number
		else if(t3->type == T_NUM_NUMBER)
		{
			*final_type = T_NUM_NUMBER;
		}
		//Pokud se neprenastavil final type znamena to, ze final type bude int
		if(*final_type != T_NUM_NUMBER)
		{
			*final_type = T_NUM_INTEGER;
		}
		return 0;

	case E_DIV_INT_E:

		//Osetreni spatneho typu u aritmeticke operace
		if(t3->type == T_STRING)
		{
			return 6;
		}
		else if(t3->type == T_KW_NIL)
		{
			return 8;
		}
		//Nastaveni finalniho datoveho typu
		else if(t3->type == T_NUM_NUMBER)
		{
			*final_type = T_NUM_NUMBER;
		}

		//Osetreni spatneho typu u aritmeticke operace
		if(t1->type == T_STRING)
		{
			return 6;
		}
		else if(t1->type == T_KW_NIL)
		{
			return 8;
		}
		//Kontrola deleni nulou
		else if(t1->type == T_NUM_NUMBER)
		{
			if(t1->value.number == 0) return 9;
		}
		else if(t1->type == T_NUM_INTEGER)
		{
			if(t1->value.integer == 0) return 9;
		}

		*final_type = T_NUM_INTEGER;
		return 0;

	case E_DIV_NUM_E:

		//Osetreni spatneho typu u aretmicke operace
		if(t3->type == T_STRING)
		{
			return 6;
		}
		else if(t3->type == T_KW_NIL)
		{
			return 8;
		}
		//Pokud prisel number, final type bude number
		else if(t3->type == T_NUM_NUMBER)
		{
			*final_type = T_NUM_NUMBER;
		}

		//Osetreni spatneho typu u aretmicke operace
		if(t1->type == T_STRING)
		{
			return 6;
		}
		else if(t1->type == T_KW_NIL)
		{
			return 8;
		}
		//Kontrola ze nemuzu delit nulou
		else if(t1->type == T_NUM_NUMBER)
		{
			if(t1->value.number == 0) return 9;
		}
		else if(t1->type == T_NUM_INTEGER)
		{
			if(t1->value.integer == 0) return 9;
		}

		*final_type = T_NUM_NUMBER;
		return 0;

	case E_CONCATENATION_E:
		//Final type po redukci konkatenace bude vzdy string
		*final_type = T_STRING;

		//Pokud je t1 string, nastavim string correct na true
		if(t1->type == T_STRING) string_correct = true;
		//Kontrola typu v konkatenaci
		else if(t1->type == T_KW_NIL) return 8;

		if(string_correct)
		{
			if(t3->type == T_STRING) return 0;
			else if(t3->type == T_KW_NIL) return 8;
		}
		//Pokud jsem nacetl cokoliv krom string/nil vracim 6 -> spatna prace s datovym typem u operace
		return 6;

	case E_LT_E:
	case E_GT_E:
	case E_LET_E:
	case E_GET_E:
		*final_type = T_NUM_INTEGER;
		//Pokud bylo cislo, tak num_correct = true
		if(t1->type == T_NUM_INTEGER || t1->type == T_NUM_NUMBER) num_correct = true;
		//Pokud byl string, tak string_correct = true
		else if(t1->type == T_STRING) string_correct = true;
		//Nil nelze pouzit u nerovnosti <, >, <=, >=
		else if(t1->type == T_KW_NIL) return 8;

		//Pokud jsem predesly nacetl cislo a tento je take cislo
		if((t3->type == T_NUM_INTEGER || t3->type == T_NUM_NUMBER) && num_correct) return 0;
		//Pokud jsem predesly nacetl string a tento je take string
		else if(t3->type == T_STRING && string_correct) return 0;
		//Nil nelze pouzit u nerovnosti <, >, <=, >=
		else if(t3->type == T_KW_NIL) return 8;
	//Vse ostatni vracim 6 -> spatna prace s datovym typem u operace
	return 6;

	case E_EQ_E:
	case E_NEQ_E:

		//Vysledkem je pravdivostni hodn. tzn. integer
		*final_type = T_NUM_INTEGER;

		//Pokud byl prvek nacten jako ID (musi se kontrolovat kvuli porovnavani s nil)
		if(t1->ID == true)
		{
			//Nastaveni booleanu podle toho, co jsem nacetl za dat. typ
			if(t1->type == T_NUM_INTEGER || t1->type == T_NUM_NUMBER) num_correct_id = true;
			else if(t1->type == T_STRING) string_correct_id = true;
			else if(t1->type == T_KW_NIL) nil_correct = true;
		}
		//Pokud prvek nebyl nacten jako ID, tzn. raw integer (5 == )
		else if(t1->type != T_ID)
		{
			//Nastaveni booleanu podle toho, co jsem nacetl za dat. typ
			if(t1->type == T_NUM_INTEGER || t1->type == T_NUM_NUMBER) num_correct = true;
			else if(t1->type == T_STRING) string_correct = true;
			else if(t1->type == T_KW_NIL) nil_correct = true;
		}

		//POkud byl druhy prvek nacten jako ID
		if(t3->ID == true)
		{
			//Lze porovnat int,num s num,int,nil => return 0
			if((t3->type == T_NUM_INTEGER || t3->type == T_NUM_NUMBER) && (num_correct || nil_correct || num_correct_id)) return 0;
			//Lze porovnat string s string, nil
			else if(t3->type == T_STRING && (string_correct || nil_correct || string_correct_id)) return 0;
			//Pokud byl predesly ID a ted jsem nacetl nil
			else if(t3->type == T_KW_NIL)
			{
				if(string_correct_id || nil_correct || num_correct_id)
				{
					return 0;
				}
				else
				{
					return 6;
				}
			}
		}
		else if(t3->type != T_ID)
		{
			//Lze porovnat int, num, s int, num => tentokrat nelze s nil, nejedna se totiz o ID
			if((t3->type == T_NUM_INTEGER || t3->type == T_NUM_NUMBER) && (num_correct || num_correct_id)) return 0;
			//Lze porovnat string s string, nil => tentokrat nelze s nil, nejedna se totiz o ID
			else if(t3->type == T_STRING && (string_correct || string_correct_id)) return 0;
			//POkud jsem nacetl nil, predesly musi byt id
			else if(t3->type == T_KW_NIL)
			{
				if(string_correct_id || nil_correct || num_correct_id)
				{
					return 0;
				}
				else
				{
					return 6;
				}
			}
		}
		//Pokud se nesplnila ani jedna podminka s nilem => return 8
		if(nil_correct) return 8;
		//Vse ostatni tzn. (int == string atp.) => return 6, spatne pouziti datovych typu u relacniho operatoru
		else return 6;

	case STRLEN_E:
		//Vysledkem operatoru # je int
		*final_type = T_NUM_INTEGER;
		//Lze pouzit s unarnim # pouze ID typu string || T_STRING
		if(t1->type == T_STRING) return 0;
		//Vsechny ostatni datove typy nelze provadet
		return 6;

	case E_BRACKETS:

		*final_type = t2->type;
		return 0;

	case E_ID:
		//Kontrola pro ID
		if(t1->type == T_ID)
		{
			//Pokud neni vubec deklarovany
			if(IsDeclaredVar(slist,t1) == 3) return 3;
			//Kontrola datoveho typu
			if(!IsString(slist, t1)) *final_type = T_STRING;
			if(!IsInteger(slist, t1)) *final_type = T_NUM_INTEGER;
			if(!IsNumber(slist, t1)) *final_type = T_NUM_NUMBER;
			if(!IsNil(slist, t1)) *final_type = T_KW_NIL;
		}
		else if(t1->type != T_ID)
		{
			*final_type = t1->type;
		}
		return 0;

	case NOT_A_RULE:
		return 2;
	}
return 2;
}

int Exp(TList *list, SList *slist, Token_type *final_type)
{
	//Inicializace a alokace stacku
	TStack *stack = malloc(sizeof(TStack));
	if(stack == NULL)
	{
		return 99;
	}
	TStackInit(stack);
	//Pomocna promenna pro drzeni stackTopu
	Token top;
	//Pomocne promenne
	PTaction action;
	bool tokenNext = true;
	int reduceError;

	while(1)
	{
		//pokud T_ID bude funkce, break
		getIndex(list->active, slist);
		//Funkce, ktera bere top, ktery neni E||StopSign
		TStackTopNotE(stack, &top);
		action = getAction(&top, &list->active->token); //do action se ulozi operace podle PrecedenceTable
		switch (action)
		{
		//Shift case
		case S:
			//Pushnu na stack stopSign
			if(TStackPushStop(stack))
			{
				freeStackE(stack);
				free(stack);
				return 99;
			}
			//Pushnu na stack nacteny prvek
			if(TStackPush(stack, &list->active->token))
			{
				freeStackE(stack);
				free(stack);
				return 99;
			}
			//Zapinam nacitani dalsiho tokenu
			tokenNext = true;
			break;
		//Reduce case
		case R:
			//Vypinam nacitani dalsiho tokenu
			tokenNext = false;
			//Zavolam funkci reduceByRule a kontroluji error
			reduceError = reduceByRule(stack, final_type, slist);
			if(reduceError != 0)
			{
				freeStackE(stack);
				free(stack);
				return reduceError;
			}
			break;
		//Equals case
		case E:
			//Pushnu na stack
			if(TStackPush(stack, &list->active->token))
			{
				freeStackE(stack);
				free(stack);
				return 99;
			}
			//Zapinam nacitani dalsiho tokenu
			tokenNext = true;
		break;
		//Error/end case
		case X:
			//Stacktop == dollar && list->active == dollar -> prislo neco co neni expression
			if(stack->stackToken[stack->topIndex]->PTindex == I_DOLLAR && list->active->token.PTindex == I_DOLLAR)
			{
				//Uvolnim pamet a vracim 1 => notAnExpression
				freeStackE(stack);
				free(stack);
				return 1;
			}
			//Stacktop == E, AfterTop == dollar, list->actie == dolar -> Spravne zpracovany expression
			else if(stack->stackToken[stack->topIndex]->PTindex == I_E && list->active->token.PTindex == I_DOLLAR && stack->stackToken[stack->topIndex-1]->PTindex == I_DOLLAR)
			{
				//Uvolnim pamet a vracim 0 => IsExpression
				freeStackE(stack);
				free(stack);
				return 0;
			}
			else //ostatni stavy
			{
				freeStackE(stack);
				free(stack);
				return 2;
			}
		break;
		}
		//Pokud je tokenNext true posouvam se
		if(tokenNext)
		{
			if(TListTokenNext(list) == 99)
			{
				freeStackE(stack);
				free(stack);
				return 99;
			}
		}
	}
//Pokud se dostane sem znamena to chybu nacitani => notAnExpression
//Testovano, nemelo by se sem nikdy dostat
return 1;
}
