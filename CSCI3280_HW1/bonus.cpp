/*

CSCI 3280, Introduction to Multimedia Systems
Spring 2022

Assignment 01 Skeleton

inverse.cpp

*/

#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <math.h>
#include "bmp.h"		//	Simple .bmp library

#define MAX_SHADES 8

char shades[MAX_SHADES] = {'@','#','%','*','|','-','.',' '};
char* space= "&nbsp;";

#define SAFE_FREE(p)  { if(p){ free(p);  (p)=NULL;} }



int main(int argc, char** argv)
{
    Bitmap image_data(argv[1]);
    unsigned int width = image_data.getWidth();
	unsigned int height = image_data.getHeight();
    char compresssize=1;
    if(width*height >=65536)
        compresssize= char(log10(width))*char(log10(height));
    printf("%d %d %d\n",width,height,compresssize);

    FILE *fout = fopen(argv[2], "w");
	if (!fout){
        printf("failed\n");
		return -1;}
    fprintf( fout, "<!DOCTYPE html>\n<html>\n<head>\n</head>\n" );
    fprintf(fout, "<style>\nbody\n{font-family:Courier New;\nfont-size:4px;\nletter-spacing: 1px;\nline-height:1.1em;\nbackground-color:black;\n}\n</style>\n");
    fprintf(fout,"<body>\n");
    unsigned int x,y;
    unsigned int gray[height/compresssize][width/compresssize];
    unsigned char red,green,blue;

    for(y=0;y<height;y++){

        fprintf(fout,"<p class=\"pixel\">\n");

        for(x=0;x<width;x++){
            if(x%compresssize==0 && y%compresssize==0){
            image_data.getColor(x, y, red, green, blue);
            gray[y/compresssize][x/compresssize] =int((0.299 * red + 0.587 * green + 0.114 * blue)/36);
            char picture=shades[MAX_SHADES-gray[y/compresssize][x/compresssize]-1];
            if((MAX_SHADES-gray[y/compresssize][x/compresssize])!= MAX_SHADES){
                fprintf(fout, "<a style=\"color:rgb(%d,%d,%d);\">%c", (int)red, (int)green, (int)blue, '@');
            }
            else{
                fprintf(fout, "<a style=\"color:rgb(%d,%d,%d);\">%s", (int)red, (int)green, (int)blue, space);
            }
            }
        }
        fprintf(fout,"</p>\n");
    }

    fprintf(fout,"</body>\n");
    fprintf(fout,"</html>\n");
    fclose(fout);
    return 0;
}