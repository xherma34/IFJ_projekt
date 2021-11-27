#include "stack.h"

int STACK_SIZE = MAX_STACK;


void Stack_Init( Stack *stack ) {
		if(stack == NULL)
		{
			return;
		}
		stack->topIndex = -1;
}

//vraci 1, pokud je prazdny else 0
int Stack_IsEmpty( const Stack *stack ) {
			return (stack->topIndex == -1);
}

//vraci 1, pokud je plny, else 0
int Stack_IsFull( const Stack *stack ) {
		return (stack->topIndex >= STACK_SIZE-1);
}


void Stack_Top( const Stack *stack, char *dataPtr ) {
		if(Stack_IsEmpty(stack))
		{
			return;
		}
		*dataPtr = stack->array[stack->topIndex];
}


void Stack_Pop( Stack *stack ) {
		if(Stack_IsEmpty(stack)) return;
		stack->topIndex--;
}


void Stack_Push( Stack *stack, char data ) {
		if(Stack_IsFull(stack))
		{
			return;
		}
		stack->topIndex++;
		stack->array[stack->topIndex] = data;
}

// int main(int argc, char const *argv[]) {
// 	Stack stack;
// 	char data = 'a';
// 	char kok;
//   Stack_Init(&stack);
// 	//Stack_Push(&stack, data);
//
//   printf("%d\n", Stack_IsFull(&stack));
//
//   return 0;
// }
