#include <stdio.h>
#include "pq.h"
#include <stdlib.h>
#include <inttypes.h>

struct PriorityQueue {
        uint32_t total;
        uint32_t capacity;
        Node **values;
};

//start of the heap sort implementation
uint32_t l_child(uint32_t n){
	return 2 * n + 1;
}

uint32_t r_child(uint32_t n){
        return 2 * n + 2;
}

uint32_t parent(uint32_t n){
	if(n==0){
		return 0;
	}
        return ((n-1)/2);
}

//this function is taken from the stats struct provided in assignment 4 from the professor
int cmp(PriorityQueue *q, uint32_t x, uint32_t y) {
    Node *one = q->values[x];
    Node *two = q->values[y];
    	if (one->frequency < two->frequency) {
        	return -1;
    	} else if (one->frequency > two->frequency) {
        	return 1;
    	} else {
        	return 0;
    	}
}

//this function is taken from the stats struct provided in assignment 4 from the professor
void swap(PriorityQueue *q, uint32_t j, uint32_t k) {
    //store the node to be swapped
    Node *store = q->values[j]; 
    q->values[j] = q->values[k];
    q->values[k] = store;
}

//For up_heap, only swap if the current value is less than it's parent
void up_heap(PriorityQueue *q, uint32_t n){
	//uint32_t n = q->total-1;
	printf("%"PRIu32"\n", n);
	printf("%"PRIu32"\n", parent(n));
	while(parent(n)>=0 && cmp(q, n, parent(n))==-1){
		swap(q, parent(n), n);
		n = parent(n);
	}
}

//For down_heap, the smaller value is considered rather than the larger value from max heap, changes to the
//signs
void down_heap(PriorityQueue *q, uint32_t n_elements){
	uint32_t n = 0;
	while (l_child(n)<n_elements){
		uint32_t smaller;
		if (r_child(n)==n_elements){
			smaller = l_child(n);
		}
		else {
			if(cmp(q, l_child(n), r_child(n))==-1){
				smaller = l_child(n);
			}
			else{
				smaller = r_child(n);
			}
		}
		if(cmp(q, n, smaller)==-1){
			break;
		}
		swap(q, n, smaller);
		n = smaller;
	}
}

//build_heap stays the same as what is given for max heap
/*
Node **build_heap(Node **arr, uint32_t n_elements){
	Node **heap = (Node **)calloc(n_elements, sizeof(Node));
	for(uint32_t n = 0; n<n_elements; n+=1){
		heap[n] = arr[n];
		up_heap(heap, n);
	}
	return heap;
}


//For the sort, the pseudocode from the assignment is used except the original array is used instead of
//a new one being created. The current value is set to zero before sort begins
void heap_sort(Node **arr, uint32_t n_elements){
	Node **heap = build_heap(arr, n_elements);
	for(uint32_t n = 0; n<n_elements; n+=1){
		arr[n] = 0;
		arr[n] = heap[0];
		heap[0] = heap[n_elements - n - 1];
		down_heap(heap, n_elements - n);
	}
	free(heap);
}
*/
//end of the heap sort implementation





PriorityQueue *pq_create(uint32_t capacity){
	//create and allocate space for a new queue
	PriorityQueue *q = (PriorityQueue *)malloc(sizeof(PriorityQueue));
	//q->tail = 0;
	q->capacity = capacity;
	q->total = 0;
	//allocate space for the list of node values
	q->values = (Node **)calloc(capacity,sizeof(Node));
	if(q->values==NULL){
		free(q);
		q = NULL;
	}
	return q;
}

void pq_delete(PriorityQueue **q){
	//if the queue is not already null
	if(*q){
		//free space associated with list of nodes
		free((*q)->values);
		//set queue's list equal to null
		(*q)->values = NULL;
		free(*q);
		*q = NULL;
	}
}

bool pq_empty(PriorityQueue *q){
	//if the top value of the queue is 0, it is empty (no values in the list)
	if(q->total==0){
		return true;
	}
	return false;
}

bool pq_full(PriorityQueue *q){
	//if the total values equals the capacity set from the beginning, the queue is full
	if(q!=NULL){
		if(q->total==q->capacity){
			return true;
		}
	}
	return false;
}

uint32_t pq_size(PriorityQueue *q){
	//the size of the queue is equal to the total attribute that increments as values are enqueued
	if(q!=NULL && pq_empty(q)==false){
		return q->total;
	}
	return 0;
}


bool enqueue(PriorityQueue *q, Node *n){
	//before enqueuing, ensure that queue is not full
	if(pq_full(q)==false){
		//printf("first\n");
		//node_print(n);
		//printf("%"PRIu32"\n", q->total);
		//the value at the head of the queue should be the value that needs to get enqueued
		q->values[q->total] = n;
		//increment the total values in the queue because one has been added
		q->total++;
		//after the new value is added, call up-heap to rebuild the heap as a minimum heap, idea from assignment 4 heap sort
		up_heap(q, q->total-1);
		//return true because it is a successful enqueue
		return true;
	}
	//queue is full so false would be returned
	return false;
}

bool dequeue(PriorityQueue *q, Node **n){
	//if the queue is empty, no values can be dequeued, so return false
	if(pq_empty(q)==true){
		return false;
	}
	//store the last item of the queue in the double pointer because it is the tail of the queue
	*n = q->values[0];
	//set the tail of the queue equal to the second to last value because that will become the new tail of the queue
	q->values[0] = q->values[q->total - 1];
	//decrement the total because a value got dequeued
	q->total-=1;
	//call down_heap to re-heapify the queue as a minimum heap
	down_heap(q, q->total);
	//return true as this was a success
	return true;
}

void pq_print(PriorityQueue *q){
	if(q){
		//because the heap sort is almost fully sorted, use bubble sort to sort the rest (not as bad for time complexity because the array is mostly sorted)
		for(uint32_t i = 0; i<(q->total-1); i+=1){
		//Use a bool zero or one value for whether a swap happened
			int swapped = 0;
			for(uint32_t j = (q->total - 1); j>i; j--){
			//compare two adjacent values
				if (cmp(q, j, j-1)==-1){
				//swap if condition met
					swap(q, j, j-1);
					swapped = 1;
				}
			}
		//If no swap happened, break
			if (swapped == 0){
				break;
			}
		}
		for (uint32_t i = 0; i < q->total; i += 1) {
                      node_print(q->values[i]);
                }
	}
}


