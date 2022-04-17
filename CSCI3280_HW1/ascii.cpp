/*

CSCI 3280, Introduction to Multimedia Systems
Spring 2022

Assignment 01 Skeleton

ascii.cpp

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
	//	1. Open BMP file
	//
	Bitmap image_data(argv[1]);

	if(image_data.getData() == NULL)
	{
		printf("unable to load bmp image!\n");
		return -1;
	}
	FILE *fp = fopen(argv[2], "w");
	if (!fp)
		return -1;

	int width=image_data.getWidth();
	int height = image_data.getHeight();
	fprintf( fp, "%d %d", width, height );
	//
	//	Your code goes here ....
	//
	//	2. Obtain Luminance
	//
	int x;
	int y;
	unsigned int grey[height][width];
	unsigned char green,red,blue;


	for(int n=0; n<height*width;n++){
		x=n%width;
		y=n/width;
		image_data.getColor(x, y, red, green, blue);
		grey[y][x]=int((0.299 * red + 0.587 * green + 0.114 * blue)/36);
	}

	
	
	for(int n=0; n<height*width;n++){
		x=n%width;
		y=n/width;
		if(x==0)fputc('\n', fp);
		fputc(shades[MAX_SHADES-grey[y][x]-1], fp);

	}


	fclose(fp);
	return 0;
} 
