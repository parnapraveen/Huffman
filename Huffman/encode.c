#include "defines.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "header.h"
#include "stack.h"
#include "code.h"
#include "pq.h"
#include "io.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "huffman.h"
#include <inttypes.h>

int main(int argc, char **argv){
        int opt = 0;
	//Stack *s;
	//Code c;
	//PriorityQueue *q;
	//stdin is 0, stdout is 1, stderr is 3
	int in_file = 0;
        int out_file = 1;
	//size of histogram is the macro static specified in defines.h
	uint64_t hist[ALPHABET];
	int verbose = 0;
	//the buffer that will be used in io
	uint8_t buf[BLOCK];
	Node *root;
	Code table[ALPHABET];
	Header header;
	int i = 0;
	hist[0] += 1;
	hist[255] += 1;
	//initalize everything in the histogram with 0
	while(i<ALPHABET){
		hist[i] = 0;
		i++;
	}
	int val;
	//stat struct for the stats in the header
	struct stat header_stats;
        while ((opt = getopt(argc, argv, "vi:o:h"))!=-1){
		//switch(opt){
			if(opt=='i'){
				//if the infile doesn't open, give an error message, otherwise open the file and set it to read
        //got this idea from assignment 5 (my implementation)
	//learned about O_RDONLY from: https://man7.org/linux/man-pages/man2/open.2.html
				in_file = open(optarg, O_RDONLY);
				if (in_file==-1){
                        		fprintf(stderr, "Couldn't open infile: No such file or directory.\n");
                        		return 1;
                		}
			}
			else if (opt=='o'){
				 //if the outfile doesn't open, give error message, otherwise open or create file and set it to write
        //got this from assignment 5 as well (my implementation)
				out_file = open(optarg, O_WRONLY | O_CREAT | O_TRUNC);
			}
			else if (opt== 'v'){
				verbose = 1;
			}
			//testing case for ADTs
			/*
			else if (opt=='t'){
				//stack testing code
				
				s = stack_create(6);
				stack_print(s);
				Node *n = node_create('$', 8);
				stack_push(s, n);
    				Node *n1 = node_create('!', 1);
    				stack_push(s, n1);
				Node *n2 = node_create('#', 5);
                                stack_push(s, n2);
				Node *n3 = node_create('*', 3);
                                stack_push(s, n3);
				Node *n4 = node_create('%', 9);
                                stack_push(s, n4);
				Node *n5 = node_create('@', 4);
                                stack_push(s, n5);
				Node *n6 = node_create(')', 7);
                                stack_push(s, n6);
				stack_print(s);
				printf("popped\n");
				Node *pop = node_create('(', 5);
				stack_pop(s, &pop);
				stack_print(s);
				//code testing code
				c = code_init();
   				code_print(&c);
    				code_push_bit(&c, 1);
    				code_print(&c);
				code_push_bit(&c, 0);
                                code_print(&c);
				code_push_bit(&c, 1);
                                code_print(&c);
				code_push_bit(&c, 0);
                                code_print(&c);
				code_push_bit(&c, 0);
                                code_print(&c);
				code_push_bit(&c, 1);
                                code_print(&c);
				code_push_bit(&c, 1);
                                code_print(&c);
    				uint8_t p;
    				code_pop_bit(&c, &p);
   				printf("popped value: %d\n", p);
				code_print(&c);
				
				//priority queue code
				q = pq_create(10);
    				//pq_print(q);
    				Node *n = node_create('$', 20);
    				enqueue(q, n);
    				//pq_print(q);
				Node *n1 = node_create('!', 1);
    				enqueue(q, n1);
    				//pq_print(q);
				Node *n2 = node_create('#', 13);
    				enqueue(q, n2);
    				//pq_print(q);
				Node *n3 = node_create('#', 18);
                                enqueue(q, n3);
                                //pq_print(q);
				Node *n4 = node_create('#', 7);
                                enqueue(q, n4);
                                //pq_print(q);
				Node *n5 = node_create('#', 2);
                                enqueue(q, n5);
                                //pq_print(q);
				Node *n6 = node_create('#', 10);
                                enqueue(q, n6);
                                //pq_print(q);
				Node *n7 = node_create('#', 10);
                                enqueue(q, n7);
                                //pq_print(q);
				Node *n8 = node_create('#', 10);
                                enqueue(q, n8);
                                pq_print(q);
				printf("dequeued\n");
    				dequeue(q, &n);
    				pq_print(q);
				printf("dequeued\n");
                                dequeue(q, &n);
                                pq_print(q);
				printf("dequeued\n");
                                dequeue(q, &n);
                                pq_print(q);
			}
			*/
			else if (opt=='h'){
				fprintf(stderr,
"SYNOPSIS\n"
"  A Huffman encoder.\n"
"  Compresses a file using the Huffman coding algorithm.\n\n"
"USAGE\n"
"  ./encode [-h] [-i infile] [-o outfile]\n\n"
"OPTIONS\n"
"  -h             Program usage and help.\n"
"  -v             Print compression statistics.\n"
"  -i infile      Input file to compress.\n"
"  -o outfile     Output of compressed data.\n");
				return 0;
			}
			else{
				fprintf(stderr,
"SYNOPSIS\n"
"  A Huffman encoder.\n"
"  Compresses a file using the Huffman coding algorithm.\n\n"
"USAGE\n"
"  ./encode [-h] [-i infile] [-o outfile]\n\n"
"OPTIONS\n"
"  -h             Program usage and help.\n"
"  -v             Print compression statistics.\n"
"  -i infile      Input file to compress.\n"
"  -o outfile     Output of compressed data.\n");
				return 1;
			}
		
	}

	//printf("here1");
	//file permissions for infile and outfile, the directions for this are on page 11 on lab document. 
	fstat(in_file, &header_stats);
	//st_mode is a function that combines permissions and the file's type into one encoded structure
	//source: https://aljensencprogramming.wordpress.com/tag/st_mode/
	fchmod(out_file, header_stats.st_mode);
	//read through the specified infile to construct histogram which is a simple array of 256 uint64_ts
	
	val = read_bytes(in_file, buf, BLOCK);
	while((val>0)){
		//printf("here3");
		for(int i = 0; i<val; i++){
                        hist[buf[i]]++;
                }
		val = read_bytes(in_file, buf, BLOCK);
	}

	//when there is a unique value, increment its count
	uint16_t unique = 0;
	int k = 0;
	while(k<ALPHABET){
		if(hist[k]>0){
			unique++;
		}
		k++;
	}
	
	//create a tree and the codes using the info on the histogram
	root = build_tree(hist);


	//build the codes using the code table
	build_codes(root, table);
	//header:
	header.magic = MAGIC;
	//st_size gives file size, used man page for this
    	header.file_size = header_stats.st_size;
	//st_mode used again to get permissions and file type
	header.permissions = header_stats.st_mode;
	//this is explained on page 11, it says that the tree's size is 3*unique symbols minus 1
    	header.tree_size = (3 * unique) - 1;
    	write_bytes(out_file, (uint8_t *) &header, sizeof(Header));

	//write the constructred huffman tree to outfile using dump_tree(), explained on page 12
	dump_tree(out_file, root);
	//start at beginning of infile and write corresponding codes for each symbol into outfile with write_code(), then flush
	//since we wrote to the end of the file, we have to return to the beginning of the file again:
	//use lseek
	//got help on this from stackoverflow: https://stackoverflow.com/questions/57763238/reset-the-file-location-using-lseek-system-call
	//printf("here");
	lseek(in_file, 0, SEEK_SET);
	int writing = read_bytes(in_file, buf, BLOCK);
        while(writing > 0){
                for(int i = 0; i<writing; i++){
			Code *c = &table[buf[i]];
                        write_code(out_file, c);
                }
                writing = read_bytes(in_file, buf, BLOCK);
        }
	//flush remaining bits
	flush_codes(out_file);

	//verbose output for stats in standard error
	if(verbose==1){
		//print out the uncompressed file size, the compressed file size, and the space saving:
		fprintf(stderr, "Uncompressed file size:: %" PRIu64 " bytes\n", header_stats.st_size);
        	//compressed size using the amount of bytes that actually get written
		fprintf(stderr, "Compressed file size: %" PRIu64 " bytes\n", bytes_written);
		//the space saving has two decimal places, so use %.2
        	fprintf(stderr, "Space saving: %.2f%%\n", 100 * (1 - ((double) bytes_written / header_stats.st_size)));
    	}
	
	//delete the tree to free space and close the files
	delete_tree(&root);
	close(in_file);
	close(out_file);


	return 0;
}



