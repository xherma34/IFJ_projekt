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
			if(!IsDeclaredFunc(slist, &node->token)) node->token.PTindex = I_DOLLAR;
			//Pokud nactu operand na operand, vracim jen prvni expression a 0
			else if((node->prev->token.PTindex == I_ID ||
		node->prev->token.PTindex == I_R_BRACKET) &&
			node->prev != NULL)
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
		printf("hovno");
		free(compressedExpression);
		return 2;
	}
if(TStackPush(stack, compressedExpression)) return 99;
return 0;
}

int CheckSS(PTRule rule, Token *t1, Token *t2, Token *t3, Token_type *final_type, SList *slist)
{
	//dodelat print,
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

		if(t1->type == T_STRING)
		{
			return 6;
		}
		else if(t1->type == T_KW_NIL)
		{
			return 8;
		}
		else if(t1->type == T_NUM_NUMBER)
		{
			*final_type = T_NUM_NUMBER;
		}


		if(t3->type == T_STRING)
		{
			return 6;
		}
		else if(t3->type == T_KW_NIL)
		{
			return 8;
		}
		else if(t3->type == T_NUM_NUMBER)
		{
			*final_type = T_NUM_NUMBER;
		}

		if(*final_type != T_NUM_NUMBER)
		{
			*final_type = T_NUM_INTEGER;
		}
		return 0;

	case E_DIV_INT_E:


		if(t3->type == T_STRING)
		{
			return 6;
		}
		else if(t3->type == T_KW_NIL)
		{
			return 8;
		}
		else if(t3->type == T_NUM_NUMBER)
		{
			*final_type = T_NUM_NUMBER;
		}


		if(t1->type == T_STRING)
		{
			return 6;
		}
		else if(t1->type == T_KW_NIL)
		{
			return 8;
		}
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


		if(t3->type == T_STRING)
		{
			return 6;
		}
		else if(t3->type == T_KW_NIL)
		{
			return 8;
		}
		else if(t3->type == T_NUM_NUMBER)
		{
			*final_type = T_NUM_NUMBER;
		}

		if(t1->type == T_STRING)
		{
			return 6;
		}
		else if(t1->type == T_KW_NIL)
		{
			return 8;
		}
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
		*final_type = T_STRING;

		if(t1->type == T_STRING) string_correct = true;
		else if(t1->type == T_KW_NIL) return 8;

		if(string_correct)
		{
			if(t3->type == T_STRING) return 0;
			else if(t3->type == T_KW_NIL) return 8;
		}
		return 6;

	case E_LT_E:
	case E_GT_E:
	case E_LET_E:
	case E_GET_E:
		*final_type = T_NUM_INTEGER;
			if(t1->type == T_NUM_INTEGER || t1->type == T_NUM_NUMBER) num_correct = true;
			else if(t1->type == T_STRING) string_correct = true;
			else if(t1->type == T_KW_NIL) return 8;



			if((t3->type == T_NUM_INTEGER || t3->type == T_NUM_NUMBER) && num_correct) return 0;
			else if(t3->type == T_STRING && string_correct) return 0;
			else if(t3->type == T_KW_NIL) return 8;

	return 6;

	case E_EQ_E:
	case E_NEQ_E:
		num_correct_id = num_correct_id;
		string_correct_id = string_correct_id;
		*final_type = T_NUM_INTEGER;
		
		//BYLO ->TYPE == ID
		if(t1->ID == true)
		{
			if(t1->type == T_NUM_INTEGER || t1->type == T_NUM_NUMBER) num_correct_id = true;
			else if(t1->type == T_STRING) string_correct_id = true;
			else if(t1->type == T_KW_NIL) nil_correct = true;
		}
		else if(t1->type != T_ID)
		{
			if(t1->type == T_NUM_INTEGER || t1->type == T_NUM_NUMBER) num_correct = true;
			else if(t1->type == T_STRING) string_correct = true;
			else if(t1->type == T_KW_NIL) nil_correct = true;
		}

		if(t3->ID == true)
		{
			if((t3->type == T_NUM_INTEGER || t3->type == T_NUM_NUMBER) && (num_correct || nil_correct || num_correct_id)) return 0;
			else if(t3->type == T_STRING && (string_correct || nil_correct || string_correct_id)) return 0;
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
			if((t3->type == T_NUM_INTEGER || t3->type == T_NUM_NUMBER) && (num_correct || num_correct_id)) return 0;
			else if(t3->type == T_STRING && (string_correct || string_correct_id)) return 0;
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
		if(nil_correct) return 8;
		else return 6;

	case STRLEN_E:
		*final_type = T_NUM_INTEGER;

		

		if(t1->type == T_STRING) return 0;
		
		return 6;

	case E_BRACKETS:

		*final_type = t2->type;
		return 0;

	case E_ID:
		if(t1->type == T_ID)
		{
		if(IsDeclaredVar(slist,t1) == 3) return 3;
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
	slist = slist;
	TStack *stack = malloc(sizeof(TStack));
	if(stack == NULL)
	{
		return 99;
	}
	TStackInit(stack);
	Token top;
	//setDataType = NULL;
	PTaction action;
	bool tokenNext = true;
	int reduceError;

	while(1)
	{
		//pokud T_ID bude funkce, break
		getIndex(list->active, slist);
		TStackTopNotE(stack, &top);
		action = getAction(&top, &list->active->token); //do action se ulozi operace podle PrecedenceTable
		switch (action)
		{
		case S:
			if(TStackPushStop(stack))
			{
				freeStackE(stack);
				free(stack);
				return 99;
			}
			if(TStackPush(stack, &list->active->token))
			{
				freeStackE(stack);
				free(stack);
				return 99;
			}
			tokenNext = true;
			break;

		case R:
			tokenNext = false;
			reduceError = reduceByRule(stack, final_type, slist);
			if(reduceError != 0)
			{
				freeStackE(stack);
				free(stack);
				return reduceError;
			}
			break;

		case E:
			if(TStackPush(stack, &list->active->token))
			{
				freeStackE(stack);
				free(stack);
				return 99;
			}
			tokenNext = true;
		break;

		case X:
			if(stack->stackToken[stack->topIndex]->PTindex == I_DOLLAR && list->active->token.PTindex == I_DOLLAR) //prislo neco, co neni exp
			{
				freeStackE(stack);
				free(stack);
				return 1;
			}
			else if(stack->stackToken[stack->topIndex]->PTindex == I_E && list->active->token.PTindex == I_DOLLAR && stack->stackToken[stack->topIndex-1]->PTindex == I_DOLLAR)
			{
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
return 1;
}
