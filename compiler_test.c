/**
 *
 **/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "aux.h"

int main(){
    char *ref_line = (char *)malloc(4*sizeof(char)); 
    char target_char; 
    int r, i, line = 0;
    char error = 0;
    FILE *ref = fopen("test/reference.txt","r");
    if(ref == NULL){
        printf("Could not find file test/reference.txt!\n");
        return 1;
    }
    FILE *target = fopen("test/all_instructions.chip8","r");
    if(target == NULL){
        printf("Could not find file test/all_instructions.chip8!\n");
        printf("Have you compiled test/all_instructions.src before running this program?\n");
        fclose(ref);
        return 1;
    }
    
    while(fgets(ref_line, 4, ref) != NULL){
        line++;
        for(i=0;i<4;i++){
            r = fgetc(target);
            if(feof(target)){
                printf("Error! Compiled file ended abruptly. Stopping at line %d\n", line);
                fclose(ref);
                fclose(target);
            }
            target_char = (char)r;
            if(target_char != char2byte(ref_line[i])){
                printf("Error! Byte %d at line %d does not match the reference\n", i, line);
                error = 1;
            }
        }
    } 
    if(error){
        printf("Compiler does NOT work correctly\n");
    }else{
        printf("Compiler works correctly!\n");
    }
    fclose(ref);
    fclose(target);

    return 0;
}
