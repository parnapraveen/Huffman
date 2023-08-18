#include <stdio.h>
#include "code.h"
#include <inttypes.h>

Code code_init(void){
	//initialize a new code object and set its attributes to 0
	Code c = {.bits = {0}};
	c.top = 0;
	return c;
}

uint32_t code_size(Code *c){
	//top value is how many values are in the stack
	return c->top;
}

bool code_empty(Code *c){
	//if the top value is 0 the stack is empty 
	if(c->top==0==true){
		return true;
	}
	return false;
}

bool code_full(Code *c){
	//if the top is at the max number then it's full, in this case it's 256 (static variable declares in defines.h)
	if(c->top==ALPHABET){
		return true;
	}
	return false;	
}

bool code_set_bit(Code *c, uint32_t i){
	//setting the bit at the specified index
	if(i<ALPHABET && c!=NULL){
		unsigned long one = 1;
		c->bits[i/8] |= (one << (i%8));
		//bool val = code_get_bit(c, i);
                //printf("actual value: %d\n", val);
		return true;
	}
	return false;
}

bool code_clr_bit(Code *c, uint32_t i){
	//clear the bit at the index specified
	if(i<ALPHABET && c!=NULL){
		//unsigned long one = 1;
		c->bits[i/8] &= ~(1 << (i%8));
		return true;
	}
	return false;
}

bool code_get_bit(Code *c, uint32_t i){
	//get the right byte then then set the right bit inside, got this logic from my bit vector implementation on assignment 6
	if(i<ALPHABET && c!=NULL){
		unsigned long one = 1;
		uint8_t val = ((c->bits[i/8]) & (one<<(i%8)))>>(i%8);
	        if(val==1){
			return true;
		}
	}
	return false;
}

bool code_push_bit(Code *c, uint8_t bit){
	//push a bit to the top of the stack
	if(code_full(c)==false){
		//c->top++;
		if(bit==0){
			//if it's 0, clear the bit
			code_clr_bit(c, c->top);
			//printf("value is 0\n");
		}
		else{
			//if the value is 1, set the bit to 1
			code_set_bit(c, c->top);
			//printf("value is 1\n");
		}
		c->top++;
		return true;
	}
	return false;
}

bool code_pop_bit(Code *c, uint8_t *bit){
	//pop a bit if the stack isn't empty
	if(code_empty(c)==false){
		*bit = code_get_bit(c, c->top-1);
		code_clr_bit(c, c->top-1);
		c->top--;
		return true;
	}
	return false;
}

void code_print(Code *c){
	//debug function
	for(uint8_t i = 0; i<c->top; i++){ 
		bool val = code_get_bit(c, i);
		printf("%d", val);
		/*
		if(val==1){
			printf("1");
		}
		else{
			printf("0");
		}
		*/
		//printf("%" PRIu8 "", val);
	}
	printf("\n");
}

