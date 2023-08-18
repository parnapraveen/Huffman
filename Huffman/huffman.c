#include <stdio.h>
#include "huffman.h"
#include "pq.h"
#include "code.h"
#include "io.h"
#include "stack.h"

//psuedocode for this is provided on page 14 of the lab document
Node *build_tree(uint64_t hist[static ALPHABET]){
	Node *node;
	Node *left;
	Node *right;
	Node *join;
	//initialize the priority queue for the histogram
	PriorityQueue *q = pq_create(ALPHABET);
	//make sure each letter in the alphabet is enqueued as a character into the histogram. There are 256 characters so use the static variable in defines.h
	int i = 0;
	while(i<ALPHABET){
		if(hist[i]!=0){
			node = node_create(i, hist[i]);
			enqueue(q, node);
		}
		i++;
	}
	//provided psuedocode for build_tree
	//the left and right nodes are dequeued from the histogram constructed above, then joined together to create a tree
	while(pq_size(q)>1){
		dequeue(q, &right);
		dequeue(q, &left);
		join = node_join(left, right);
		enqueue(q, join);
	}
	//when the length is 1, the last one is dequeued as the root node of the tree
	dequeue(q, &join);
	//when done using the priority queue, clear it from memory to save space
	pq_delete(&q);
	//return the root of the tree which is the final node that gets joined
	return join;
}

//psuedocode for this is provided on page 14 of the lab document
void build_codes(Node *root, Code table[static ALPHABET]){
	//initialize code with the code_init function
	Code code = code_init();
	//bit value that needs to get removed
    	uint8_t popped_bit;
    	//if the root node is not null
	if (root!=NULL) {
        //when a node is a leaf, both of its children are null, so the next check is for this
        	if (root->right == NULL && root->left == NULL) {
			//set the node's symbol equal to its code
            		table[root->symbol] = code;
        	} else {
            		//if the current node is not a leaf, the function will be called recursively on the right and left nodes until the leaf is reached
			//push a 0 into the code stack for left
            		code_push_bit(&code, 0);
            		build_codes(root->left, table);
            		code_pop_bit(&code, &popped_bit);
			//push a 1 into the code stack for right
            		code_push_bit(&code, 1);
            		build_codes(root->right, table);
            		code_pop_bit(&code, &popped_bit);
        	}
    	}
    	return;
}

//psuedocode for this is provided on page 14 of the lab document
void dump_tree(int outfile, Node *root){
	//required symbols for l
	uint8_t L_symbol = 'L';
        uint8_t I_symbol = 'I';
	//if the root is not null
	if (root!=NULL) {
		//recursively call dump_tree as written in pseudocode to reach the leaf nodes of the tree and write to the outfile
		//this is breadth first search because each node is visited
        	dump_tree(outfile, root->left);
        	dump_tree(outfile, root->right);
		//if both the children are null, a leaf has been reached, so mark the node as a leaf
        	if (root->left == NULL && root->right == NULL) {
           		//the number of bytes written is 1 because each byte is 8 bits and the symbol is 8 bits
			write_bytes(outfile, &L_symbol, 1);
            		write_bytes(outfile, &root->symbol, 1);
		//if it's not a leaf node, write out "I" to the outfile because it is an interior node
        	} else {
            		write_bytes(outfile, &I_symbol, 1);
        	}
    	}
    	return;
}

//the tree is re-built using post-order tree dump 
//the length of bytes in tree-dump is given by n_bytes
//return root of reconstructed tree
Node *rebuild_tree(uint16_t nbytes, uint8_t tree[static nbytes]){
	//according to the description, we need to return the root, so include a node to store that, then a right pointer and a left pointer
	Node *root;
	Node *left;
	Node *right;
	//uint8_t L_symbol = 'L';
        uint8_t I_symbol = 'I';
	//initialize a stack for the nodes
	Stack *stack = stack_create(ALPHABET);
	//iterate over each value in the tree dump, the length being nbytes
	int i = 0;
	while(i<nbytes){
		//when the node is an interior node with symbol "I", we have to pop the stack twice to get two nodes
		//first node is right child
		//second node is left child
		if(tree[i]==I_symbol){
			stack_pop(stack, &right);
			stack_pop(stack, &left);
			//join the left and right nodes and push back into the stack to form the tree again
			stack_push(stack, node_join(left, right));
		}
		//if it's not an interior node, then it must be a leaf or the root, in this case, create a node and push it into the stack
		else{
			i++;
			//create the node
			root = node_create(tree[i], 0);
			stack_push(stack, root);
		}
		i++;
	}
	//when tree is rebuilt, then pop the first value because it represents the root node
	stack_pop(stack, &root);
	//delete the stack to save memory
	stack_delete(&stack);
	return root;
}

void delete_tree(Node **root){
	//if the root of the tree is not null
	//using post-order traversal to reach the bottom-most left and bottom-most right node then deleting node by node
	if(*root!=NULL){
		//recursively call left
		delete_tree(&(*root)->left);
		//recursively call right
		delete_tree(&(*root)->right);
		//delete the root because there is a "root" in each sub-tree (the leaf is the root of a tree with two null children)
		//it stops when the child is null because null cannot be a parent
		node_delete(root);
	}
}

