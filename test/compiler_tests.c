/**
 *
 **/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "aux.h"

unsigned char char2byte(char input){

	if((input >= '0') & (input <= '9'))
		return input-'0';
	if((input >= 'A') & (input <= 'F'))
		return input-'A'+10;
	printf("Warning! Processing illegal hexadecimal digit.\n");
	return 0;
}

int main(){
    char *ref_line = (char *)malloc(256*sizeof(char)); 
    char target_digit; 
    int r, i, line = 0;
    char error = 0;
    FILE *ref = fopen("test/reference.txt","r");
    if(ref == NULL){
        printf("Could not find file test/reference.txt!\n");
        return 1;
    }
    FILE *target = fopen("test/all_instructions.chip8","rb");
    if(target == NULL){
        printf("Could not find file test/all_instructions.chip8!\n");
        printf("Have you compiled test/all_instructions.src before running this program?\n");
        fclose(ref);
        return 1;
    }
    
    while(fgets(ref_line, 255, ref) != NULL){
        line = line + 1;
        for(i=0;i<2;i++){
            if(0==fread(&r, 1, 1, target)){
                printf("Error! Compiled file ended abruptly. Stopping at line %d\n", line);
                fclose(ref);
                fclose(target);
                free(ref_line);
            }
            target_digit = ((char)r & 0xF0) >>4;
            if(target_digit != (char2byte(ref_line[2*i]))){
                printf("Error! Byte %d at line %d does not match the reference\n", i, line);
                error = 1;
            }
            target_digit = ((char)r & 0x0F);
            if(target_digit != (char2byte(ref_line[2*i+1]))){
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
    free(ref_line);
    return 0;
}
