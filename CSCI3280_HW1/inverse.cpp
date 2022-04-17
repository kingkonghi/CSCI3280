/*

CSCI 3280, Introduction to Multimedia Systems
Spring 2022

Assignment 01 Skeleton

inverse.cpp

*/

#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include "bmp.h"		//	Simple .bmp library

#define MAX_SHADES 8

char shades[MAX_SHADES] = {'@','#','%','*','|','-','.',' '};

#define SAFE_FREE(p)  { if(p){ free(p);  (p)=NULL;} }

int main(int argc, char** argv)
{
    //
    // 1. Open the txt file
    //
    int width,height;
    FILE *fp = fopen(argv[1], "r");
    if (!fp)
        return -1;
    fscanf( fp,"%d %d",&width,&height);
    Bitmap final(width,height);

    //
    // Your code goes here ////
    //
    //
    // 2. read the ascii art file.
    //

    char gray;
    //
    // 3. Get RGB values for the resultant bitmap
    //
    for(int y=0;y<height;y++){
        gray = fgetc(fp);
        for(int x=0;x<width;x++){
            gray = fgetc(fp);
            for(int m=0;m<MAX_SHADES;m++){
                if(shades[m]==gray){
                    unsigned char color = (MAX_SHADES-1-m)*36;
                    final.setColor(x,y,color,color,color);
                    break;
                }
            }
        }
    }


    //
    // 4. Store the BMP file to the given place
    //
    final.save(argv[2]);
    fclose(fp);

    // free memory if any


    return 0;
}