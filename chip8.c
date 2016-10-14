

unsigned short opcode; //Keypad
unsigned char memory[4096]; //Memory (4kb)
unsigned char V[16]; //Registers V0-V15 plus Carry Flag
unsigned short I; //Index register
unsigned short pc; //Program counter

unsigned char screen[64*32]; //64x32 display
unsigned char key[16]; //Keypad
unsigned char draw;

unsigned char delay_timer, sound_timer; // Timer registers

unsigned short stack[16]; //stack
unsigned short sp; //stack pointer

//Functions
void init();        //Init interpreter
void load_file();   //Load the program
void cycle();       //Process one cpu cycle
void update_screen();        //Update the screen
void input();       //Update keyboard status

int main(int argc, char **argv){

    //Setup

    init();
    load_file();

    for(;;){

        cycle();
        if(draw){
            update_screen();
        }
        
        input();

    }
    
    return 0;
}

void init(){        
    //Init interpreter


}

void load_file(){   
    //Load the program

}
    
void cycle(){       
    //Process one cpu cycle

}
void update_screen(){
    //Update the screen
    
}
void input(){
    //Update keyboard status

}


