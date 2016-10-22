/**
 *  Chip8 Compiler
 *  --------------
 *  Because writing your programs directly in binary is a bit inconvenient
 *  As always, use this as you wish.
 **/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

unsigned short char2us(char input);

int main(int argc, char **argv){

char *linebuffer;
char *word;
char *word2;
unsigned short instruction;
int line;
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
    line = 0;
    while(fgets(linebuffer, 256,in) !=NULL){
        line += 1;
        //Get first word
        word = strtok(linebuffer," ,"); //Only the first tokenize needs this. From now on strtok(NULL, " ,");
        if(strcmp(word,"CLS")){
           instruction = 0x00E0; 
        }else if(strcmp(word,"RET")){
            instruction = 0x0EE;
        }else if(strcmp(word,"SYS")){
            instruction = 0x0000;
            word = strtok(NULL," ,"); //Word now contains addr
            instruction |= (char2us(word[0])*256) + (char2us(word[1])*16) + (char2us(word[0]));
        }else if(strcmp(word,"JP")){
            instruction = 0x1000;
            word = strtok(NULL," ,"); //Word now contains addr
            instruction |= (char2us(word[0])*256) + (char2us(word[1])*16) + (char2us(word[0]));
        }else if(strcmp(word,"CALL")){
            instruction = 0x2000;
            word = strtok(NULL," ,"); //Word now contains addr
            instruction |= (char2us(word[0])*256) + (char2us(word[1])*16) + (char2us(word[0]));
        }else if(strcmp(word,"SE")){
            word = strtok(NULL," ,"); //Word now contains Vx
            if (word[0] != 'V'){
                printf("Error at line %d, expected register argument\n",line);
                free(linebuffer);
                fclose(in);
                fclose(out);
                return 1;
            }
            word2 = strtok(NULL," ,"); //Word2 now contains byte or Vy
            if(word2[0] =='V'){
                instruction = 0x5000; //0x5XY0
                instruction |= char2us(word[1])* 256 + char2us(word2[1])*16;
            }else{
                instruction = 0x3000; //0x3XKK
                instruction |= (char2us(word[1])*256) + (char2us(word2[1])*16) + (char2us(word2[0]));
            }
        }else if(strcmp(word,"SNE")){
            word = strtok(NULL," ,"); //Word now contains Vx
            if(word[0] != 'V'){
                printf("Error at line %d, expected register argument\n",line);
                free(linebuffer);
                fclose(in);
                fclose(out);
                return 1;
            }
            word2 = strtok(NULL," ,");
            if(word2[0] == 'V'){
                instruction = 0x9000; //0x9XY0
                instruction |= char2us(word[1])*256 + char2us(word2[1])*16;
            }else{
                instruction = 0x4000; //0x4XKK
                instruction |= (char2us(word[0])*256) +(char2us(word[1])*16) + (char2us(word[0])); 
            }
        }else if(strcmp(word,"LD")){
         

        }else if(strcmp(word,"OR")){


    
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

unsigned short char2us(char input){

    switch(input){
        case '0':
            return 0;
        case '1':
            return 1;
        case '2':
            return 2;
        case '3':
            return 3;
        case '4':
            return 4;
        case '5':
            return 5;
        case '6':
            return 6;
        case '7':
            return 7;
        case '8':
            return 8;
        case '9':
            return 9;
        case 'A':
            return 10;
        case 'B':
            return 11;
        case 'C':
            return 12;
        case 'D':
            return 13;
        case 'E':
            return 14;
        case 'F':
            return 15;
    }
    return 0;
}

