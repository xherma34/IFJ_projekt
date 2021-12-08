#include "TStack.h"

void TStackInit(TStack *stack)
{

  for (int i = 0; i < 100; i++)
  {
    stack->stackToken[i] = NULL; //Pro korektni inicializaci radsi vynuluji vsechny indexi stacku
  }

  stack->topIndex = -1;
  Token *bottom = malloc(sizeof(Token));
  bottom->PTindex = I_DOLLAR;
  TStackPush(stack, bottom);
}

int TStackPush(TStack *stack, Token *token)
{
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
  token->type = stack->stackToken[stack->topIndex]->type;
  token->PTindex = stack->stackToken[stack->topIndex]->PTindex;
  token->value = stack->stackToken[stack->topIndex]->value;
  token->line = stack->stackToken[stack->topIndex]->line;
}

int TStackPop(TStack *stack)
{
  if(stack->topIndex != -1)
  {
    stack->topIndex -= 1;
    return 0;
  }
  return 1;
}

void TStackTopNotE(TStack *stack, Token *token)
{
  if(stack->stackToken[stack->topIndex]->PTindex != I_E)
  {
    token->PTindex = stack->stackToken[stack->topIndex]->PTindex;
  }
  else
  {
    if(stack->stackToken[stack->topIndex-1]->PTindex != I_STOP) //kdyby byl pod E shift
    {
      token->PTindex = stack->stackToken[stack->topIndex-1]->PTindex;
    }
    else
    {
      token->PTindex = stack->stackToken[stack->topIndex-2]->PTindex;
    }
  }

}

int TStackUntillStopSign(TStack *stack)
{
  int count = stack->topIndex;
  if(stack->topIndex == -1)
  {
    return -1; //chyba
  }
  while(1)
  {
    if(stack->stackToken[count]->PTindex == I_STOP)
    {
      break;
    }
    if(stack->stackToken[count]->PTindex == I_DOLLAR)
    {
      return -1;
    }
    count--;
  }

  return (stack->topIndex - count);
}

int TStackPushAfterTop(TStack *stack, Token *token)
{
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
  Token *stop = malloc(sizeof(Token));
  stop->PTindex = I_STOP;

  if(stack->stackToken[stack->topIndex]->PTindex != I_STOP && stack->stackToken[stack->topIndex]->PTindex != I_E)
  {
    TStackPush(stack, stop);
    return 0;
  }
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
  for(int i = stack->topIndex; i > -1; i--)
  {
    if(stack->stackToken[i]->PTindex == I_E || stack->stackToken[i]->PTindex == I_STOP || stack->stackToken[i]->PTindex == I_DOLLAR)
    {
      free(stack->stackToken[i]);
    }
  }
}
