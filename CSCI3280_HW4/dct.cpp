#include "bmp.h"		//	Simple .bmp library
#include<iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <vector>

using namespace std;
#define PI 3.14159265358979
int QuantizationMatrix[8][8] = {
	{3, 5, 7, 9, 11, 13, 15, 17}, 
	{5, 7, 9, 11, 13, 15, 17, 19},
	{7, 9, 11, 13, 15, 17, 19, 21},
	{9, 11,	13,	15,	17,	19,	21,	23},
	{11, 13, 15, 17, 19, 21, 23, 25},
	{13, 15, 17, 19, 21, 23, 25, 27},
	{15, 17, 19, 21, 23, 25, 27, 29},
	{17, 19, 21, 23, 25, 27, 29, 31}
	};

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		cout << "Arguments prompt: dct.exe <img_path> <apply_idct>" << endl;
		return 0;
	}
	string imgPath = argv[1];
	bool need_idct = stoi(argv[2]);

	//! read input image
	Bitmap s_img(imgPath.c_str());
	int rows = s_img.getHeight(), cols = s_img.getWidth();
	cout << "Apply DCT on image ("<<rows<<", "<<cols<< ")." << endl;
	
	//! preprocess by shifting pixel values by 128
	//TODO
	double convert[rows][cols];
	for(int i=0;i<rows;i++){
		for(int j=0;j< cols;j++){
			unsigned char pixel ='\0';
			s_img.getPixel(i,j,pixel);
			
			convert[i][j] = (double)((int)pixel-128);
		}
	}
	//! 2D DCT for every 8x8 block (assume that the input image resolution is fixed to 256)	
	// The quantized coefficients should be stored into 'coeffArray'
	double coeffArray[256][256]={0};
	int blockRow = rows / 8, blockCol = cols / 8;
	for (int i = 0; i < blockRow; i++)
	{
		for (int j = 0; j < blockCol; j++)
		{
			int xpos = j*8, ypos = i*8;
			//! apply DCT on block_ij (basic requirement)
			//TODO
			int cu=1,cv=1;
			for(int u=0;u<8;u++){
				for(int v=0;v<8;v++){
					if(u==0)cu= 1/sqrt(2); else cu=1;
					if(v==0)cv= 1/sqrt(2); else cv=1;
					double sum=0,pixel=0;
					for(int y=0;y<8;y++){
						for(int x=0;x<8;x++){
							//pixel = convert[xpos+k][ypos+l]*cos((2*k+1)*PI*i/16);
						}
					}
					coeffArray[xpos+u][ypos+v] = cu*cv*sum;
				}
			}
			//! quantize the frequency coefficient of this block (basic requirement)
			//TODO
			for(int y=0;y<8;y++){
				for(int x=0;x<8;x++){
					double quan = coeffArray[xpos+x][ypos+y]/QuantizationMatrix[y][x];
					coeffArray[xpos+x][ypos+y] = quan;
				}
			}
		}
	}
	
	//! output the computed coefficient array
	FILE *fp = fopen("coeffs.txt", "w");
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			fprintf(fp, "%3.3lf ", coeffArray[c][r]);
		}
		fprintf(fp, "\n");
	}
	cout << "Quantized coefficients saved!" << endl;

	if (need_idct)
	{
		Bitmap reconstructedImg(cols, rows);
		//! apply IDCT on the quantized coefficients (enhancement part)
		for (int i = 0; i < blockRow; i++)
		{
			for (int j = 0; j < blockCol; j++)
			{
				int xpos = j*8, ypos = i*8;
				//! apply de-quantization on block_ij 
				//TODO

				//! apply IDCT on this block
				//TODO

				//! shiftting back the pixel value range to 0~255
				//TODO
				for(int y=0;y<8;y++){
					for(int x=0;x<8;x++){
						coeffArray[xpos+y][ypos+x] +=128;
					}
				}
			}
		}
		string savePath = "reconstructedImg.bmp";
		reconstructedImg.save(savePath.c_str());
		cout << "reconstructed image saved!" << endl;
	}

	return 0;
}