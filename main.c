//#include "address_map_nios2.h"
#include <stdbool.h>
#define PS2_BASE			0xFF200100
#define PS2_DUAL_BASE			0xFF200108
//#define LED_BASE			0xFF200000

#define RLEDs ((volatile long *) 0xFF200000)
//GLOBALS
bool LEFT_MOUSE_CLICK;
bool RIGHT_MOUSE_CLICK;
int X_SIGN_BIT;
int Y_SIGN_BIT;
int X_MOVEMENT;
int Y_MOVEMENT;

int LAST_RECEIVED_KEY;

int BYTE0 = 0;
int BYTE1 = 0;
int BYTE2 = 0;

char currentText[] = "";

int main()
{
    int current_bit = 0;
    while(1)
    {
        getMouseData(current_bit);
        getKeyBoardData(current_bit);
        current_bit++;
        if(current_bit == 3)
        {
            current_bit = 0;
        }
    }
}


struct PS2 {
    volatile unsigned char data;  // The control/status register
    volatile unsigned char RVALID;
    volatile short int RAVAIL;
    volatile int control; //ps2 control register, dont have to worry about it
};

struct PS2 *const keyboard = ((struct PS2 *) PS2_BASE);
struct PS2 *const mouse = ((struct PS2 *) PS2_DUAL_BASE);


void getKeyBoardData(current_bit)
{
    int read_valid = keyboard->RVALID & 0b10000000;
    if(read_valid)
    {
        //get the make code and determine which char to return
        int PS2_data = keyboard->data;
        BYTE0 = BYTE1;
        BYTE1 = BYTE2;
        BYTE2 = PS2_data;
    }
    
    if (BYTE1 == 0xF0) 
    {
        switch (BYTE2) 
        {
            case 0x16: // one pressed
                if(LAST_RECEIVED_KEY != 0x16)
                {
                    printf("one pressed");
                }
                LAST_RECEIVED_KEY = 0x16;
                break;
            case 0x1E: // two pressed
                if(LAST_RECEIVED_KEY != 0x1E)
                {
                    printf("two pressed");
                }
                LAST_RECEIVED_KEY = 0x1E;
                break;
            case 0x26: // three pressed
                if(LAST_RECEIVED_KEY != 0x26)
                {
                    printf("three pressed");
                }
                LAST_RECEIVED_KEY = 0x26;
                break;
            case 0x25: // four pressed
                if(LAST_RECEIVED_KEY != 0x25)
                {
                    printf("four pressed");
                }
                LAST_RECEIVED_KEY = 0x25;
                break;
            case 0x2E: // five pressed
                if(LAST_RECEIVED_KEY != 0x2E)
                {
                    printf("five pressed");
                }
                LAST_RECEIVED_KEY = 0x2E;
                break;
            case 0x36: // six pressed
                if(LAST_RECEIVED_KEY != 0x36)
                {
                    printf("six pressed");
                }
                LAST_RECEIVED_KEY = 0x36;
                
                break;
            case 0x3D: // seven pressed
                if(LAST_RECEIVED_KEY != 0x3D)
                {
                    printf("seven pressed");
                }
                LAST_RECEIVED_KEY = 0x3D;
                
                break;
            case 0x3E: // eight pressed
                if(LAST_RECEIVED_KEY != 0x3E)
                {
                    printf("eight pressed");
                }
                LAST_RECEIVED_KEY = 0x3E;
                
                break;
            case 0x46: // nine pressed
                if(LAST_RECEIVED_KEY != 0x46)
                {
                    printf("nine pressed");
                }
                LAST_RECEIVED_KEY = 0x46;
                
                break;
            case 0x49: // period ' . ' pressed
                if(LAST_RECEIVED_KEY != 0x49)
                {
                    printf("period pressed");
                }
                LAST_RECEIVED_KEY = 0x49;
                
                break;
            case 0x4E: // minues sign ' - ' pressed
                if(LAST_RECEIVED_KEY != 0x4E)
                {
                    printf("minus pressed");
                }
                LAST_RECEIVED_KEY = 0x4E;
                break;
            case 0x66: // backspace pressed
                if(LAST_RECEIVED_KEY != 0x66)
                {
                    printf("backspace pressed");
                }
                LAST_RECEIVED_KEY = 0x66;
                break;
            default:
                
                break;
        }
    }
    *RLEDs = BYTE2;
}



//get the last 3 bits always, 
void getMouseData(int current_bit)
{
    int PS2_data = mouse->data;
    if(current_bit == 0)
    {
        LEFT_MOUSE_CLICK = (PS2_data & 0b1);
        RIGHT_MOUSE_CLICK = (PS2_data & 0b10);
        X_SIGN_BIT = (PS2_data & 0b10000);
        Y_SIGN_BIT = (PS2_data & 0b100000);
    }
    else if(current_bit == 1)
    {
        X_MOVEMENT = PS2_data;
    }
    else if(current_bit == 2)
    {
        Y_MOVEMENT = PS2_data;
    }
}


