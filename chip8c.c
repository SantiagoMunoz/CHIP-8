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

unsigned short parse_instruction();

unsigned short parse_CLS();
unsigned short parse_RET();
unsigned short parse_SYS();
unsigned short parse_JP();
unsigned short parse_CALL();
unsigned short parse_SE();
unsigned short parse_SNE();
unsigned short parse_LD();
unsigned short parse_ADD();
unsigned short parse_OR();
unsigned short parse_AND();
unsigned short parse_XOR();
unsigned short parse_SUB();
unsigned short parse_SHR();
unsigned short parse_SUBN();
unsigned short parse_SHL();
unsigned short parse_RND();
unsigned short parse_DRW();
unsigned short parse_SKP();
unsigned short parse_SKPN();

int line;
char *linebuffer;

int main(int argc, char **argv){

char *word;
size_t size = 256;
unsigned short instruction;
FILE *out;

    if(argc < 2){
        printf("Usage: chip8c <input file> <output file>\n");
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
    if(argc == 3){
        out = fopen(argv[2],"wb");
    }else{
        out = fopen("program.out","wb");
        printf("Saving binary with name program.out\n");
    }
    if(out == NULL){
        printf("Unable to create or open output file\n");
        return 1;
    }
    linebuffer = (char*)malloc(256*sizeof(char)); //Just in case there are comments
    line = 0;

    while(fgets(linebuffer, 256,in) !=NULL){
        line += 1;
        printf("Processing line %d :", line);
        printf(linebuffer);
        printf("\n");
        instruction = parse_instruction();
        if(instruction != 0xFFFF){
        word =(char*) &instruction;
        fwrite(&word[1], 1, 1, out);// memory position of instructiom, 2 bytes long, 1 element and into output file
        fwrite(&word[0], 1, 1, out);// memory position of instructiom, 2 bytes long, 1 element and into output file

        }else{
            printf("Encountered irrecoverable error, aborting compilation...\n");
            fclose(in);
            fclose(out);
            free(linebuffer);
            return 1;
        }
    }
    
    free(linebuffer);
    fclose(in);
    fclose(out);

    return 0;
}

unsigned char char2byte(char input){

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

unsigned short parse_instruction(){
    char *buffer = strtok(linebuffer," ,"); //Only the first tokenize needs this. From now on strtok(NULL, " ,");
    
    if(0==strcmp(buffer, "CLK")){
        return parse_CLS();
    }else if(0==strcmp(buffer, "RET")){
        return parse_RET();
    }else if(0==strcmp(buffer, "SYS")){
        return parse_SYS();
    }else if(0==strcmp(buffer, "JP")){
        return parse_JP();
    }else if(0==strcmp(buffer, "CALL")){
        return parse_CALL();
    }else if(0==strcmp(buffer, "SE")){
        return parse_SE();
    }else if(0==strcmp(buffer, "SNE")){
        return parse_SNE();
    }else if(0==strcmp(buffer, "LD")){
        return parse_LD();
    }else if(0==strcmp(buffer, "ADD")){
        return parse_ADD();
    }else if(0==strcmp(buffer, "OR")){
        return parse_OR();
    }else if(0==strcmp(buffer, "AND")){
        return parse_AND();
    }else if(0==strcmp(buffer, "XOR")){
        return parse_XOR();
    }else if(0==strcmp(buffer, "SUB")){
        return parse_SUB();
    }else if(0==strcmp(buffer, "SHR")){
        return parse_SHR();
    }else if(0==strcmp(buffer, "SUBN")){
        return parse_SUBN();
    }else if(0==strcmp(buffer, "SHL")){
        return parse_SHL();
    }else if(0==strcmp(buffer, "RND")){
        return parse_RND();
    }else if(0==strcmp(buffer, "DRW")){
        return parse_DRW();
    }else if(0==strcmp(buffer, "SKP")){
        return parse_SKP();
    }else if(0==strcmp(buffer, "SKPN")){
        return parse_SKPN();
    }else{
        return 0xFFFF;
    }
}

unsigned short parse_CLS(){ 
    return 0x00E0;
}
unsigned short parse_RET(){
    return 0x00EE;
}
unsigned short parse_SYS(){
    unsigned short instruction = 0x0000;
    char *addr;

    addr = strtok(NULL, " ,");
    instruction |= (char2byte(addr[0]) & 0x0F) << 8;
    instruction |= (char2byte(addr[1]) & 0x0F) << 4;
    instruction |= (char2byte(addr[2]) & 0x0F) << 0;
    
    return instruction;
}
unsigned short parse_JP(){
    unsigned short instruction;
    char *arg;

    arg = strtok(NULL, " ,");

    if(arg[0] == 'V'){
        instruction = 0xB000;
        arg = strtok(NULL, " ,");
        instruction |= (char2byte(arg[0]) & 0x0F) << 8;
        instruction |= (char2byte(arg[1]) & 0x0F) << 4;
        instruction |= (char2byte(arg[2]) & 0x0F) << 0;
    }else{
        instruction = 0x1000;
        instruction |= (char2byte(arg[0]) & 0x0F) << 8;
        instruction |= (char2byte(arg[1]) & 0x0F) << 4;
        instruction |= (char2byte(arg[2]) & 0x0F) << 0;
    }

    return instruction;
}
unsigned short parse_CALL(){
    unsigned short instruction = 0x2000;
    char *addr;

    addr = strtok(NULL, " ,");
    instruction |= (char2byte(addr[0]) & 0x0F) << 8;
    instruction |= (char2byte(addr[1]) & 0x0F) << 4;
    instruction |= (char2byte(addr[2]) & 0x0F) << 0;
    
    return instruction;
}

unsigned short parse_SE(){
    unsigned short instruction = 0x0000;
    char *arg;

    arg = strtok(NULL, " ,");
    if(arg[0] != 'V'){
        //TODO: Return error
        return 0xFFFF;
    }
    instruction |= (char2byte(arg[1]) & 0x0F) << 8;
    arg = strtok(NULL, " ,");
    if(arg[0] == 'V'){
        instruction |= 0x5000;
        instruction |= (char2byte(arg[1]) & 0x0F) << 4;
    }else{
        instruction |= 0x3000;
        instruction |= (char2byte(arg[0]) & 0x0F) << 4;
        instruction |= (char2byte(arg[1]) & 0x0F) << 0;
    }

    return instruction;
}
unsigned short parse_SNE(){
    unsigned short instruction = 0x0000;
    char *arg;

    arg = strtok(NULL, " .");
    if(arg[0] != 'V'){
        //TODO: Return error
        return 0xFFFF;
    }
    instruction |= (char2byte(arg[1] & 0x0F)) << 8;
    arg = strtok(NULL, " ,");
    if(arg[0] == 'V'){
        instruction |= 0x9000;
        instruction |= (char2byte(arg[1]) & 0x0F) << 4;
    }else{
        instruction |= 0x4000;
        instruction |= (char2byte(arg[0]) & 0x0F) << 4;
        instruction |= (char2byte(arg[1]) & 0x0F) << 0;
    }
    return instruction;
}
unsigned short parse_LD(){
    unsigned short instruction = 0x0000;
    char *arg;

    arg = strtok(NULL, " ,");
    switch(arg[0]){
        case 'V':
            instruction |= (char2byte(arg[1]) & 0x0F) << 8;
            arg = strtok(NULL, " ,");
            switch(arg[0]){
                case 'V':
                    instruction |=  0x8000;
                    instruction |= (char2byte(arg[1]) & 0x0F) << 4;
                    break;
                case 'D':
                    instruction |= 0xF007;
                    break;
                case 'K':
                    instruction |= 0xF00A;
                    break;
                case '[':
                    instruction |= 0xF065;
                    break;
                default:
                    instruction |= 0x6000;
                    instruction |= (char2byte(arg[0]) & 0x0F) << 4;
                    instruction |= (char2byte(arg[1]) & 0x0F) << 0;
            }
            break;
        case 'I':
            instruction = 0xA000;
            arg = strtok(NULL, " ,");
            instruction |= (char2byte(arg[0]) & 0x0F) << 8;
            instruction |= (char2byte(arg[1]) & 0x0F) << 4;
            instruction |= (char2byte(arg[2]) & 0x0F) << 0;
            break;
        case 'D':
            instruction |= 0xF015;
            arg = strtok(NULL, " ,");
            instruction |= (char2byte(arg[1]) & 0x0F) << 8;
            break;
        case 'S':
            instruction |= 0xF018;
            arg = strtok(NULL, " ,");
            instruction |= (char2byte(arg[1]) & 0x0F) << 8;
            break;
        case 'F':
            instruction |= 0xF029;
            arg = strtok(NULL, " ,");
            instruction |= (char2byte(arg[1]) & 0x0F) << 8;
            break;
        case 'B':
            instruction |= 0xF033;
            arg = strtok(NULL, " ,");
            instruction |= (char2byte(arg[1]) & 0x0F) << 8;
            break;
        case '[':
            instruction |= 0xF065;
            arg = strtok(NULL, " ,");
            instruction |= (char2byte(arg[1]) & 0x0F) << 8;
            break;
        default:
            //TODO: Report error
            return 0xFFFF;
    }

    return instruction;
}
unsigned short parse_ADD(){
    unsigned short instruction = 0x0000;
    char *arg;

    arg = strtok(NULL, " ,");
    if(arg[0] == 'I'){
        instruction = 0xF01E;
        arg = strtok(NULL, " ,");
        instruction |= (char2byte(arg[1]) & 0x0F) << 8;
    }else{
        if(arg[0] != 'V'){
            //TODO: Report error
            return 0xFFFF;
        }
        instruction |= (char2byte(arg[1]) & 0x0F) << 8;
        arg = strtok(NULL, " ,");
        if(arg[0] == 'V'){
            instruction |= 0x8004;
            instruction |= (char2byte(arg[1]) & 0x0F) << 4;
        }else{
            instruction |= 0x7000;
            instruction |= (char2byte(arg[0]) & 0x0F) << 4;
            instruction |= (char2byte(arg[1]) & 0x0F) << 0;
        }
    }

    return instruction;
}
unsigned short parse_OR(){
    unsigned short instruction = 0x8001;
    char *arg;

    arg = strtok(NULL, " ,");
    
    if(arg[0] != 'V'){
        //TODO: Report error
        return 0xFFFF;
    }
    instruction |= (char2byte(arg[1])) << 8;
    arg = strtok(NULL, " ,");
    if(arg[0] != 'V'){
        //TODO: Report error
        return 0xFFFF;
    }
    instruction |= (char2byte(arg[1])) << 4;

    return instruction; 
}
unsigned short parse_AND(){
    unsigned short instruction = 0x8002;
    char *arg;

    arg = strtok(NULL, " ,");
    
    if(arg[0] != 'V'){
        //TODO: Report error
        return 0xFFFF;
    }
    instruction |= (char2byte(arg[1])) << 8;
    arg = strtok(NULL, " ,");
    if(arg[0] != 'V'){
        //TODO: Report error
        return 0xFFFF;
    }
    instruction |= (char2byte(arg[1])) << 4;

    return instruction; 
}
unsigned short parse_XOR(){
    unsigned short instruction = 0x8003;
    char *arg;

    arg = strtok(NULL, " ,");
    
    if(arg[0] != 'V'){
        //TODO: Report error
        return 0xFFFF;
    }
    instruction |= (char2byte(arg[1])) << 8;
    arg = strtok(NULL, " ,");
    if(arg[0] != 'V'){
        //TODO: Report error
        return 0xFFFF;
    }
    instruction |= (char2byte(arg[1])) << 4;

    return instruction; 

}
unsigned short parse_SUB(){
    unsigned short instruction = 0x8005;
    char *arg;

    arg = strtok(NULL, " ,");
    
    if(arg[0] != 'V'){
        //TODO: Report error
        return 0xFFFF;
    }
    instruction |= (char2byte(arg[1])) << 8;
    arg = strtok(NULL, " ,");
    if(arg[0] != 'V'){
        //TODO: Report error
        return 0xFFFF;
    }
    instruction |= (char2byte(arg[1])) << 4;

    return instruction; 
}
unsigned short parse_SHR(){
    unsigned short instruction = 0x8006;
    char *arg;

    arg = strtok(NULL, " ,");

    if(arg[0] != 'V'){
        //TODO: Report error
        return 0xFFFF;
    }
    instruction |= (char2byte(arg[1]) & 0x0F) << 8;
    arg = strtok(NULL, " ,");
    arg = strtok(NULL, " ,");
    if(arg[0] != 'V'){
        //TODO: Report error
        return 0xFFFF;
    }
    instruction |= (char2byte(arg[1]) & 0x0F) << 4;
     
    return instruction;
}
unsigned short parse_SUBN(){
    unsigned short instruction = 0x8007;
    char *arg;

    arg = strtok(NULL, " ,");
    
    if(arg[0] != 'V'){
        //TODO: Report error
        return 0xFFFF;
    }
    instruction |= (char2byte(arg[1])) << 8;
    arg = strtok(NULL, " ,");
    if(arg[0] != 'V'){
        //TODO: Report error
        return 0xFFFF;
    }
    instruction |= (char2byte(arg[1])) << 4;

    return instruction; 

}
unsigned short parse_SHL(){
    unsigned short instruction = 0x800E;
    char *arg;

    arg = strtok(NULL, " ,");

    if(arg[0] != 'V'){
        //TODO: Report error
        return 0xFFFF;
    }
    instruction |= (char2byte(arg[1]) & 0x0F) << 8;
    arg = strtok(NULL, " ,");
    arg = strtok(NULL, " ,");
    if(arg[0] != 'V'){
        //TODO: Report error
        return 0xFFFF;
    }
    instruction |= (char2byte(arg[1]) & 0x0F) << 4;
     
    return instruction;

}
unsigned short parse_RND(){
    unsigned short instruction = 0xC000;
    char *arg;

    arg = strtok(NULL, " ,");
    if(arg[0] != 'V'){
        //TODO: report error
        return 0xFFFF;
    }
    instruction |= (char2byte(arg[1]) & 0x0F) << 8;

    arg = strtok(NULL, " ,");
    instruction |= (char2byte(arg[0]) & 0x0F) << 4;
    instruction |= (char2byte(arg[1]) & 0x0F) << 0;

    return instruction;
}   
unsigned short parse_DRW(){
    unsigned short instruction = 0xD000;
    char *arg;

    arg = strtok(NULL, " ,");

    if(arg[0] != 'V'){
        //TODO: Report error
        return 0xFFFF;
    }
    instruction |= (char2byte(arg[1]) & 0x0F) << 8;

    arg = strtok(NULL, " ,");

    if(arg[0] != 'V'){
        //TODO: Report error
        return 0xFFFF;
    }
    instruction |= (char2byte(arg[1]) & 0x0F) << 4;

    if(arg[0] != 'V'){
        //TODO: Report error
        return 0xFFFF;
    }
    instruction |= (char2byte(arg[0]) & 0x0F) << 0;

    return instruction;
}
unsigned short parse_SKP(){
    unsigned short instruction = 0xE09E;
    char *arg;

    arg = strtok(NULL, " ,");
    
    if(arg[0] != 'V'){
        //TODO: Report error
        return 0xFFFF;
    }
    instruction |= (char2byte(arg[1]) & 0x0F) << 8;

    return instruction;
}
unsigned short parse_SKPN(){
    unsigned short instruction = 0xE0A1;
    char *arg;

    arg = strtok(NULL, " ,");
    
    if(arg[0] != 'V'){
        //TODO: Report error
        return 0xFFFF;
    }
    instruction |= (char2byte(arg[1])) << 8;

    return instruction;

}

