# Huffman Encoding

## Building
Run the first command in order to make encode.c and decode.c executable
```
$ make
```

## Running
In order to run encode, use
```
$ ./encode [- ]
```

The value in the brackets should be substituted by:
<br>
-h: displays program synopsis and usage.
<br>
-i: specifies the input file to encrypt (default: stdin).
<br>
-o: specifies the output file to encrypt (default: stdout).
<br>
-v: prints out the compression statistics to stderr (includes uncompressed file size, compressed file size, and space saving).
<br>

In order to run decode, use
```
$ ./decode [- ]
```

The value in the brackets should be substituted by:
<br>
-h: displays program synopsis and usage.
<br>
-i: specifies the input file to encrypt (default: stdin).
<br>
-o: specifies the output file to encrypt (default: stdout).
<br>
-v: prints out the compression statistics to stderr (includes uncompressed file size, compressed file size, and space saving).
<br>

## Files

1. encode.c: This file will contain your implementation of the Huffman encoder.
2. decode.c: This file will contain your implementation of the Huffman decoder.
3. defines.h: This file will contain the macro definitions used throughout the assignment. You may
not modify this file.
4. header.h: This will will contain the struct definition for a file header. You may not modify this
file.
5. node.h: This file will contain the node ADT interface. This file will be provided. You may not
modify this file.
6. node.c: This file will contain your implementation of the node ADT.
7. pq.h: This file will contain the priority queue ADT interface. This file will be provided. You may
not modify this file.
8. pq.c: This file will contain your implementation of the priority queue ADT. You must define your
priority queue struct in this file.
9. code.h: This file will contain the code ADT interface. This file will be provided. You may not
modify this file.
10. code.c: This file will contain your implementation of the code ADT.
11. io.h: This file will contain the I/O module interface. This file will be provided. You may not modify
this file.
12. io.c: This file will contain your implementation of the I/O module.
13. stack.h: This file will contain the stack ADT interface. This file will be provided. You may not
modify this file.
14. stack.c: This file will contain your implementation of the stack ADT. You must define your stack
struct in this file.
15. huffman.h: This file will contain the Huffman coding module interface. This file will be provided.
You may not modify this file.
16. huffman.c: This file will contain your implementation of the Huffman coding module interface.
17. Makefile: This is a file that will allow the grader to type make to compile your program.
18. README.md: This must describe how to build and run your program.
19. DESIGN.pdf: describes design of code. 
20. WRITEUP.pdf: has descriptions on how my code was tested for accuracy
