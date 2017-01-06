/*stack.h*/

// Michael Mei
// Linux Mint 18(Sarah) with gcc
// U. of Illinois, Chicago
// CS251, Fall 2016
// HW #14
//

//
// Stack:
//
typedef int StackElementType;
typedef struct Stack
{
  StackElementType  *Elements;  // array of elements in stack:
  int  Top;          // index of top of stack
  int  NumElements;  // # of elements currently in stack
  int  Capacity;     // max # of elements that can fit in stack
} Stack;

Stack *CreateStack(int N);
void   DeleteStack(Stack *S);
int    isEmptyStack(Stack *S);
int    Push(Stack *S, StackElementType e);
StackElementType Pop(Stack *S);
