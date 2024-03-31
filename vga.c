#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#define X_RES 320
#define Y_RES 240

#define VGA_BASE 0xFF203020
#define BLACK 0x0000
#define RED 0xF800

/*==================STRUCTS==================*/

typedef struct VGA {
    volatile int front_buffer;
    volatile int back_buffer;
    volatile short x_res;
    volatile short y_res;
    volatile int status;
    volatile int control;
} VGA;

/*==================FUNCTION PROTOTYPES==================*/

//waits for the v_sync signal to be high
void wait_for_v_sync(VGA *vga);
//initializes the VGA
void initializeVGA(VGA *vga);
//plots a pixel at the x, y coordinate with the color
void plotPixel(int x, int y, short int color);
//plots a letter at the x, y coordinate with the color
//uses the character buffer on the De1-SoC
void plotLetter(int x, int y, char letter, short int color);
//plots a string at the x, y coordinate with the color
void plotString(int x, int y, char *string, short int color);
//plots a box at the x, y coordinate with the width, height, outline color, and fill color
void plotBox(int x, int y, int width, int height, short int outline_color, short int fill_color);
//clears the screen with the color
void clearScreen(short int color);

/*==================GLOBALS==================*/
//using 320 x 240 vga resolution
volatile int pixel_buffer_start; 
short int Buffer1[240][512];
short int Buffer2[240][512];

int main() 
{
    //declare an instance of the VGA struct and initialize it
    //to the base address of the VGA
    VGA *vga = (VGA *) VGA_BASE;
    
    //initialize the VGA
    initializeVGA(vga);

    //main loop
    while (1)
    {   
        //print buffer address
        // printf("Writing buffer address: %x\n", pixel_buffer_start);
        //clear the screen
        clearScreen(BLACK);
        //draw a box
        plotBox(100, 100, 100, 100, RED, BLACK);
        //write a 1 to the vga front buffer to swap buffers
        vga->front_buffer = 1;
        //polling loop while waiting for the swap to happen
        while (vga->status & 0x01);
        pixel_buffer_start = vga->back_buffer;
    }

    return 0;
}

void initializeVGA(VGA *vga)
{
    //set the resolution to 320 x 240
    assert(vga->x_res == 320);
    assert(vga->y_res == 240);

    //set buffer 1 to the back buffer
    vga->back_buffer = (int) &Buffer1;
    //wait for buffer swap
    wait_for_v_sync(vga);
    //initialize the pointer to the pixel buffer
    pixel_buffer_start = vga->front_buffer;
    clearScreen(BLACK);

    //set buffer 2 to the back buffer
    vga->back_buffer = (int) &Buffer2;
    //set the pixel_buffer_start to the back (drawing) buffer
    pixel_buffer_start = vga->back_buffer;
    clearScreen(BLACK);
}

void wait_for_v_sync(VGA *vga)
{
    vga->front_buffer = 1; //start the synchronization process
    while (vga->status & 0x01);
}

void plotPixel(int x, int y, short int color)
{
    volatile short int *pixel_address = (short int *) (pixel_buffer_start + (y << 10) + (x << 1));
    *pixel_address = color;
}

void plotBox(int x, int y, int width, int height, short int outline_color, short int fill_color)
{
    //loop through the width and height of the box, plot pixel 
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            if(i == 0 || i == width - 1 || j == 0 || j == height - 1)
            {
                plotPixel(x + i, y + j, outline_color);
            }
            else
            {
                plotPixel(x + i, y + j, fill_color);
            }
        }
    }
}

void clearScreen(short int color)
{
    for (int x = 0; x < X_RES; x++)
        for (int y = 0; y < Y_RES; y++)
            plotPixel(x, y, color);
}

void plotLetter(int x, int y, char letter, short int color)
{
}
