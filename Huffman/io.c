#include <stdio.h>
#include "io.h"
#include <stdlib.h>
#include <unistd.h>

//provided extern variables that need to be set for verbose output in encode
uint64_t bytes_written;
uint64_t bytes_read;
//the buffer that will be used to read in the bytes
static uint64_t index = 0;
//static value to count the number of bits already read
static uint64_t already_read = 0;
static uint8_t buf[BLOCK];


//notes from doc:
//wrapper function to loop calls to read() until all bytes are read that are specified by parameter nbytes into buffer buf or
//no more bytes can be read
//the number of bytes read from input file is returned
int read_bytes(int infile, uint8_t *buf, int nbytes) {
   uint64_t actual = (uint64_t)(nbytes); 
   uint64_t bytes_r = 0;
   //offset value to check if the file has already been read or not 
    int off = -4; 
     //when the number of bytes specified is not satisfied, keep the while loop going. If there aren't enough bytes to keep the offset greater than 0, then the while loop doesn't continue.
    while (bytes_r < actual && (off>0 || off==-4)) {
        off = read(infile, buf, nbytes - bytes_r);
	//if no bits were read
	if(off==0){
        	break;
        }
	bytes_r+=read(infile, buf, nbytes - bytes_r);
        bytes_r += off;
    }
    bytes_read += bytes_r;
    return (uint64_t)bytes_r;
}

//notes from doc:
//similar to read_bytes()
//loop calls to write(), but this doesn't guarantee that all the specified bytes are written, so loop the calls until everything is written to buffer or not bytes get written. return the number written out
int write_bytes(int outfile, uint8_t *buf, int nbytes) {
    uint64_t actual = (uint64_t)(nbytes);
    uint64_t bytes_w = 0;
    //same offset value used as before because 1, 0, and -1 all have values
    int off = -4;
     //when the number of bytes specified is not satisfied, keep the while loop going. If there aren't enough bytes to keep the offset greater than 0, then the while loop doesn't continue. 
    while (bytes_w < actual && (off>0 || off==-4) ) {
        //off = write(outfile, buf, nbytes - bytes_w);
        off=write(outfile, buf, nbytes - bytes_w);
	if(off==0){
                break;
        }
	bytes_w += off;
    }
    bytes_written += bytes_w;
    return bytes_w;
}

//notes from doc:
//read in a block of bytes into a buffer and dole out bits one at a time
//when all the bits in buffer are doled out, fill buffer back up with bytes from infile
//maintain static buffer of bytes and index into the buffer that tracks which but to return through pointer bit
//buffer stores BLOCK number of bytes where block is another macro in defines.h
//returns false if there are no more bits that can be read and true if there are still bits to read (like a bit vector)
//this is implemented using TA Lev's pseudocode provided in office hours
//static index value as the offset
//static uint32_t index = 0;
//static value to count the number of bits already read
//static uint32_t already_read = 0;
//the buffer that will be used to read in the bytes
//static uint8_t buffer[BLOCK];
bool read_bit(int infile, uint8_t *bit){

        //if the index is 0, start reading in the bytes from the infile
        if(index==0){
                //multiply the number of bytes read by 8 because each byte has 8 bits, so to add up the correct number of bits, multiply by 8
                already_read = read_bytes(infile, buf, BLOCK) * 8;
        }
        //get the bit at the current index value and set it equal to the bit pointer parameter
        unsigned long one = 1;
        *bit = ((buf[index/8]) >> index%8) & one;
        //bit = ((buffer[index/8]) & (one<<(index%8)))>>(index%8);
        index = (index+1)%(BLOCK * 8);
        //if the new index value is less than the amount of bits that are already read, then return false because there are no more bits to be read
        if(index>already_read){
                return false;
        }
        //set the index value equal to its value plus one because the byte before has been read already. Mod this value by 8 times the block size because it needs to be the correct index in the right byte multiplied by the 8 for the total number of bits
        //index = (index+one)%(BLOCK * 8);

        //return true because a new bit could be returned
        return true;
}

//notes from doc:
//same bit-buffering logic as read_bit(). Use static buffer and an index. each bit in the code c will be buffered into buffer the bits will be
//buffered starting from 0th bit and when buffer of BLOCK bytes is filled, write it into the outfile
//static uint32_t write_index = 0;
//static value to count the number of bits already read use the static from read_bit
//static uint32_t already_read_write = 0;
//the buffer that will be used to read in the bytes
//uint8_t buffer_write[BLOCK];
void write_code(int outfile, Code *c){
        //go through the values in the code
        uint32_t x = 0;
        while(x<c->top){
                //if the first value of the code is a 0, clear the bit in the buffer
                if(code_get_bit(c, x)==true){
                        unsigned long one = 1;
                        buf[index / 8] &= ~(one << index % 8);
                }
                //if it's a 1, set the bit in the buffer
                else{
                        unsigned long one = 1;
                        buf[index / 8] |= (one << index % 8);
                }
                //increment the index because the current bit has been set
                index++;
                //if the index value has reached the end of the buffer, write out all the bytes and set the index back to 0
                if(index==BLOCK*8){
                        write_bytes(outfile, buf, BLOCK);
                        index = 0;
                }
                //keep going through the code
                x++;
        }
}

//notes from doc:
//bits left over in the buffer used to write by write_code()
//extra bits are zeroed before flushing the codes
void flush_codes(int outfile){
        //initiate an unsigned long value to shift for the bit mask
        unsigned long one = 1;
        uint32_t eight = 8;
        //the byte to get to is the initial index divided by 8 because the array is made of 8 bit integers
        uint32_t byte = index/eight;
        //left shift the one to get the leftover bits after the final index that was reached
        uint8_t mask = (one << (index % eight)) - 1;
        //if the mask isn't null, there are bits to be flushed
        if(mask){
                //mask out the leftover bits
                buf[byte+=one] &= mask;
        }
        //write out the leftover bits to the outfile
        write_bytes(outfile, buf, byte);
}
