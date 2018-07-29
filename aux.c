#include "aux.h"
unsigned char char2byte(char input){

	if((input >= '0') & (input <= '9'))
		return input-'0';
	if((input >= 'A') & (input <= 'F'))
		return input-'A'+10;
	printf("Warning! Processing illegal hexadecimal digit.\n");
	return 0;
}
