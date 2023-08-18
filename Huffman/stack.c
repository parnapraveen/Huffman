#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

struct Stack{
	uint32_t top;
	uint32_t capacity;
	Node **items;
};

Stack *stack_create(uint32_t capacity){
	Stack *s = (Stack *)malloc(sizeof(Stack));
	//the top represents how many elements are in the stack, update this later
	if(s){
		s->top = 0;
		s->capacity = capacity;
		//allocate memory for the items in the stack, could be large
		s->items = (Node **)calloc(capacity, sizeof(Node));
		if(s->items==NULL){
			free(s);
			s=NULL;
		}
	}
	return s;
}

void stack_delete(Stack **s){
	//if the stack is null or there are no items, free the space associated with the items
	//list and the stack object
	if((*s)!=NULL && (*s)->items!=NULL){
		free((*s)->items);
	}
	free(*s);
	//set pointer to null
	*s = NULL;
}

bool stack_empty(Stack *s){
	//if the index of the top value is 0, no values are in the stack so print true that stack
	//is empty
	if(s){
		if(s->top==0){
			return true;
		}
	}
	return false;
}

bool stack_full(Stack *s){
	//if the top value of the stack is equal to the capacity, then the max number of values is
	//reached, so print true that stack is full
	if(s){
                if(s->top==s->capacity){
                        return true;
                }
        }
        return false;
}

uint32_t stack_size(Stack *s){
	//the top attribute says how many values are in the stack, so print this for the stack size
	return s->top;
}

bool stack_push(Stack *s, Node *n){
	//if the stack is full return false because nothing can be pushed
	if(stack_full(s)==true){
		return false;
	}
	//the top value of the stack should equal the new value inputted
	//increment the top value first so that a new value is added above the existing one
	s->items[s->top] = n;
	s->top++;
	return true;
}

bool stack_pop(Stack *s, Node **n){
	//if stack is empty, don't pop value, return false
	if(stack_empty(s)==true){
		return false;
	}
	//the top value of the stack right now should be set equal to the double pointer
	*n = s->items[s->top];
	//decrease the value of the top because the top value was removed
	s->top--;
	return true;

}

void stack_print(Stack *s){
	uint32_t i = 0;
	//go through stack and print each node
	while(i<s->top){
		node_print(s->items[i]);
		i++;
	}
}

