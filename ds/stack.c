#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct {
  int* data;
  size_t capacity;
  int top;
} Stack;

Stack* stack_create(size_t initial_capacity){
   Stack* stack = (Stack*) malloc(sizeof(Stack));
   if (stack == NULL){
      printf("Could not created stack.\n");
      return NULL;
   }
   stack->data = (int*) malloc(initial_capacity * sizeof(int));
   if(!stack->data){
     printf("Can not create a stack.\n");
     free(stack);
     return NULL;
   }
   stack->top = -1;
   stack->capacity = initial_capacity;
   return stack;
}

bool stack_resize(Stack* stack, size_t new_capacity){
  int* new_data = (int*) realloc(stack->data, new_capacity * sizeof(int));
  if(!new_data){
     printf("Error on reallocation\n");
     return false;
  }
  stack->data = new_data;
  stack->capacity = new_capacity;
  printf("\n===> Resized... New capacity is : %d\n", (int)stack->capacity);
  return true;
}


void stack_destroy(Stack* stack){
  free(stack->data);
  free(stack);
}

bool stack_push(Stack* stack, int item){
   if (stack->top >= (int)stack->capacity - 1){
     if(!stack_resize(stack, 2* stack->capacity)) {return false;}
   }
  
   stack->top = stack->top + 1;
   stack->data[stack->top] = item;
   return true;
}

bool stack_pop(Stack* stack, int* item){
  if (stack->top == -1){
     printf("Stack is already empty. \n");
     return false;
  }

  *item = stack->data[stack->top];
  stack->top--;

  if (stack->capacity > 2 && stack->top < stack->capacity/4){
    stack_resize(stack, stack->capacity/2);
  }
  return true;
}

void stack_peek(Stack* stack, int* item){
  *item =  stack->data[stack->top];
}

void stack_print(Stack* stack){
  printf("[");
  for(int i = 0; i <= stack->top; i++){
    printf("%d",stack->data[i]);
    if (i <= stack->top-1){ printf(", "); }
  }
  printf("]\n");
}

int main()
{
  Stack* stack = stack_create(2);
 
for(int i = 1; i < 101; i++){
 stack_push(stack,i);
}
 stack_print(stack);

 int peek;
 stack_peek(stack, &peek);
 printf("peek: %d\n", peek),

 stack_push(stack,4);
 stack_print(stack);

 int pop;
 stack_pop(stack, &pop);
 printf("%d popped\n",pop);
 stack_print(stack);

 stack_pop(stack, &pop);
 printf("%d popped\n",pop);
 stack_print(stack);

 stack_pop(stack, &pop);
 printf("%d popped\n",pop);
 stack_print(stack);

 stack_pop(stack, &pop);
 printf("%d popped\n",pop);
 stack_print(stack);

 stack_destroy(stack);
  return 0;
}
