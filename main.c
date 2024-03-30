//#include "address_map_nios2.h"
#include <stdbool.h>
#include <stdlib.h>
#define PS2_BASE			0xFF200100
#define PS2_DUAL_BASE		0xFF200108
//#define LED_BASE			0xFF200000

#define RLEDs ((volatile long *) 0xFF200000)
//GLOBALS
bool LEFT_MOUSE_CLICK;
bool RIGHT_MOUSE_CLICK;
int X_SIGN_BIT;
int Y_SIGN_BIT;

bool READY_TO_READ;

int BYTE0 = 0;
int BYTE1 = 0;
int BYTE2 = 0;

int current_byte = 0;

//half of 640 x 320
int X_POSITION= 320;
int Y_POSITION = 240;

int NEW_X_POSITION= 320;
int NEW_Y_POSITION = 320;

int CURRENT_TEXT_IDX = 0;


struct PS2 *const keyboard = ((struct PS2 *) PS2_BASE);
struct PS2 *const mouse = ((struct PS2 *) PS2_DUAL_BASE);

int main()
{
    int TEXT_LENGTH = 20; // size of the array
    char *CURRENT_TEXT = (char *)malloc(TEXT_LENGTH * sizeof(char)); // allocate memory
    while(1)
    {
        getMouseData();
        getKeyBoardData(CURRENT_TEXT);

        if(LEFT_MOUSE_CLICK)
        {
            printf("left mouse clicked ");
        }
        if(RIGHT_MOUSE_CLICK)
        {
            printf("right mouse clicked ");
        }
        
        //printf("x: %d",X_POSITION);
        //printf("y: %d",Y_POSITION);
    }
    free(CURRENT_TEXT);
}

struct PS2 {
    volatile unsigned char data;  // The control/status register
    volatile unsigned char RVALID;
    volatile short int RAVAIL;
    volatile int control; //ps2 control register, dont have to worry about it
};




void getKeyBoardData(char *CURRENT_TEXT)
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
    //this means that a key has been released
    if (BYTE1 == 0xF0) 
    {
        switch (BYTE2) 
        {
            case 0x16: // one pressed
                if(READY_TO_READ)
                {
                    printf(CURRENT_TEXT);
                    READY_TO_READ = false;
                    CURRENT_TEXT[CURRENT_TEXT_IDX] = '1';
                    CURRENT_TEXT_IDX++;
                }
                
                break;
            case 0x1E: // two pressed
                if(READY_TO_READ)
                {
                    printf(CURRENT_TEXT);
                    READY_TO_READ = false;
                    CURRENT_TEXT[CURRENT_TEXT_IDX] = '2';
                    CURRENT_TEXT_IDX++;
                }
                
                break;
            case 0x26: // three pressed
                if(READY_TO_READ)
                {
                    printf(CURRENT_TEXT);
                    READY_TO_READ = false;
                    CURRENT_TEXT[CURRENT_TEXT_IDX] = '3';
                    CURRENT_TEXT_IDX++;
                }
                
                break;
            case 0x25: // four pressed
                if(READY_TO_READ)
                {
                    printf(CURRENT_TEXT);
                    READY_TO_READ = false;
                    CURRENT_TEXT[CURRENT_TEXT_IDX] = '4';
                    CURRENT_TEXT_IDX++;
                }
                
                break;
            case 0x2E: // five pressed
                if(READY_TO_READ)
                {
                    printf(CURRENT_TEXT);
                    READY_TO_READ = false;
                    CURRENT_TEXT[CURRENT_TEXT_IDX] = '5';
                    CURRENT_TEXT_IDX++;
                }
                
                break;
            case 0x36: // six pressed
                if(READY_TO_READ)
                {
                    printf(CURRENT_TEXT);
                    READY_TO_READ = false;
                    CURRENT_TEXT[CURRENT_TEXT_IDX] = '6';
                    CURRENT_TEXT_IDX++;
                }
                
                
                break;
            case 0x3D: // seven pressed
                if(READY_TO_READ)
                {
                    printf(CURRENT_TEXT);
                    READY_TO_READ = false;
                    CURRENT_TEXT[CURRENT_TEXT_IDX] = '7';
                    CURRENT_TEXT_IDX++;
                }
                
                
                break;
            case 0x3E: // eight pressed
                if(READY_TO_READ)
                {
                    printf(CURRENT_TEXT);
                    READY_TO_READ = false;
                    CURRENT_TEXT[CURRENT_TEXT_IDX] = '8';
                    CURRENT_TEXT_IDX++;
                }
                
                
                break;
            case 0x46: // nine pressed
                if(READY_TO_READ)
                {
                    printf(CURRENT_TEXT);
                    READY_TO_READ = false;
                    CURRENT_TEXT[CURRENT_TEXT_IDX] = '9';
                    CURRENT_TEXT_IDX++;
                }
                
                
                break;
            case 0x49: // period ' . ' pressed
                if(READY_TO_READ)
                {
                    printf(CURRENT_TEXT);
                    READY_TO_READ = false;
                    CURRENT_TEXT[CURRENT_TEXT_IDX] = '.';
                    CURRENT_TEXT_IDX++;
                }
                
                
                break;
            case 0x4E: // minues sign ' - ' pressed
                if(READY_TO_READ)
                {
                    printf(CURRENT_TEXT);
                    READY_TO_READ = false;
                    CURRENT_TEXT[CURRENT_TEXT_IDX] = '-';
                    CURRENT_TEXT_IDX++;
                }
                
                break;
            case 0x66: // backspace pressed
                if(READY_TO_READ)
                {
                    printf(CURRENT_TEXT);
                    READY_TO_READ = false;
                    CURRENT_TEXT_IDX = CURRENT_TEXT_IDX - 1;
                    CURRENT_TEXT[CURRENT_TEXT_IDX] = ' ';
                }
                
                break;
            default:
                break;
            
        }
    }
    else
    {
        READY_TO_READ = true;
    }
    
    *RLEDs = BYTE2;
}



//get the last 3 bits always, 
void getMouseData()
{
    int read_valid = mouse->RVALID & 0b10000000;
    if(read_valid)
    {
        int PS2_data = mouse->data;
        if(current_byte == 0)
        {
            LEFT_MOUSE_CLICK = (PS2_data & 0b1);
            RIGHT_MOUSE_CLICK = (PS2_data & 0b10);
            X_SIGN_BIT = (PS2_data & 0b10000);
            Y_SIGN_BIT = (PS2_data & 0b100000);
        }
        else if(current_byte == 1)
        {
            //if the sign is negative
            if(X_SIGN_BIT)
            {
                if(X_POSITION < ((PS2_data ^ 0b11111111) + 1))
                {
                    NEW_X_POSITION = 0;
                }
                else{
                    NEW_X_POSITION = X_POSITION - ((PS2_data ^ 0b11111111) + 1);
                }
            }
            else
            {
                if(X_POSITION + PS2_data > 640)
                {
                    NEW_X_POSITION = 640;
                }
                else
                {
                    NEW_X_POSITION = X_POSITION+ PS2_data;
                }
            }
           X_POSITION = NEW_X_POSITION;
        }
        else if(current_byte == 2)
        {
            if(Y_SIGN_BIT)
            {
                if(Y_POSITION < ((PS2_data ^ 0b11111111) + 1))
                {
                    NEW_Y_POSITION = 0;
                }
                else{
                    NEW_Y_POSITION = Y_POSITION - ((PS2_data ^ 0b11111111) + 1);
                }
            }
            else
            {
                if(Y_POSITION + PS2_data > 480)
                {
                    NEW_Y_POSITION = 480;
                }
                else
                {
                    NEW_Y_POSITION = Y_POSITION + PS2_data;
                }
            }
            Y_POSITION = NEW_Y_POSITION;
        }
        
        current_byte++;
        if(current_byte == 3)
        {
            current_byte = 0;
        }
    }
   
}

