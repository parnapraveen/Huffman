#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include <inttypes.h>

Node *node_create(uint8_t symbol, uint64_t frequency){
	//allocate memory for the new node
	Node *n = (Node *)malloc(sizeof(Node));
	//set attributes
	if(n){
		n->symbol = symbol;
		n->frequency = frequency;
		n->left = NULL;
		n->right = NULL;
	}
	return n;
}

void node_delete(Node **n){
	if(*n){
		//free the node and set it equal to null
		free(*n);
		*n = NULL;
	}
}

Node *node_join(Node *left, Node *right){
	//join the two child nodes to a new parent node with the frequencies added up and the left and right node pointers set the the parameters
	uint8_t symbol = '$';
	uint64_t frequency = left->frequency + right->frequency;
	Node *parent = node_create(symbol, frequency);
	if(parent){
		parent->left = left;
		parent->right = right;
	}
	return parent;
}

void node_print(Node *n){
	if(n){
		//print the symbol and frequency of the node
		node_print_sym(n);
		printf("%s%"PRIu64 "\n", " frequency: ", n->frequency);
	}

}

//two helper functions to compare node frequencies and print symbol
//Miller notes: compares the frequency of two nodes. Returns true if the first node has greater frequency than the second, false 
//otherwise
bool node_cmp(Node *n, Node *m){
	if(n==NULL || m==NULL){
		return false;
	}
	if(n->frequency>m->frequency){
		return true;
	}
	return false;
}

void node_print_sym(Node *n){
	printf("%s%c", "Current node symbol: ", n->symbol);
}

