/**
 *  Chip8 Compiler
 *  --------------
 *  Because writing your programs directly in binary is a bit inconvenient
 *  As always, use this as you wish.
 **/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv){
char *linebuffer;
char *word;
unsigned short instruction;
    if(argc < 2){
        printf("Usage: chip8c <filename>\n");
        return 1;
    }
    FILE *in = fopen(argv[1], "r");
    if(in == NULL){
        printf("File not found!\n");
        return 1;
    }
    printf("Compiling file ");
    printf(argv[1]);
    printf("\n");
    FILE *out = fopen("program.out","wb");
    if(out == NULL){
        printf("Unable to create or open output file\n");
        return 1;
    }
    printf("Saving binary with name program.out\n");
    linebuffer = (char*)malloc(256*sizeof(char)); //Just in case there are comments
    while(fgets(linebuffer, 256,in) !=NULL){
        //Get first word
        word = strtok(linebuffer," ,"); //Only the first tokenize needs this. From now on strtok(NULL, " ,");
        if(strcmp(word,"CLS")){
           instruction = 0x00E0; 
        }else if(strcmp(word,"RET")){
            instruction = 0x0EE;
        /* Other instructions...
         
        }else if(strcmp(word,"XXX")){ 
         ...
         ...
         */
        }else{
            printf("Found an error in the program. Exiting...\n");    
            free(linebuffer);
            fclose(in);
            fclose(out);
            return 1;
        } 
        fwrite(&instruction, 2, 1, out);// memory position of instructiom, 2 bytes long, 1 element and into output file
    } //end while

    free(linebuffer);
    fclose(in);
    fclose(out);
    return 0;
}
