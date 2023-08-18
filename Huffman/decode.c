#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv){
        int opt = 0;
        while ((opt = getopt(argc, argv, "ih"))!=-1){
                switch(opt){
                        case 'i':
                                break;
			case 'h':
                                fprintf(stderr,
"SYNOPSIS\n"
"  A Huffman decoder.\n"
"  Decompresses a file using the Huffman coding algorithm.\n\n"
"USAGE\n"
"  ./decode [-h] [-i infile] [-o outfile]\n\n"
"OPTIONS\n"
"  -h             Program usage and help.\n"
"  -v             Print compression statistics.\n"
"  -i infile      Input file to decompress.\n"
"  -o outfile     Output of decompressed data.\n");
                                return 0;
                        default:
				fprintf(stderr,
"SYNOPSIS\n"
"  A Huffman decoder.\n"
"  Decompresses a file using the Huffman coding algorithm.\n\n"
"USAGE\n"
"  ./decode [-h] [-i infile] [-o outfile]\n\n"
"OPTIONS\n"
"  -h             Program usage and help.\n"
"  -v             Print compression statistics.\n"
"  -i infile      Input file to decompress.\n"
"  -o outfile     Output of decompressed data.\n");
                                return 1;
                }       
        }       
        return 0;
}       
