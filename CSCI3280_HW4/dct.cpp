#include "bmp.h"		//	Simple .bmp library
#include<iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <vector>

using namespace std;
#define PI 3.14159265358979
#define N 8
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
double c(int x){
	return x == 0? (1 / sqrt(2)) : 1;
}
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
			long double pixel_d=pixel-128;
			convert[i][j] = (long double)pixel_d;
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
			
			for(int u=0;u<8;u++){
				for(int v=0;v<8;v++){
					int cu=1,cv=1;
					if(u==0){cu= 1/sqrt(2); }
					if(v==0){cv= 1/sqrt(2); }
					double sum=0,pixel=0;
					
					for(int x=0;x<N;x++){
						for(int y=0;y<N;y++){
							sum+= 	cos((2 * x + 1)* u * PI / (2*N))*
									cos((2 * y + 1)* v * PI / (2*N))*
									convert[xpos+x][ypos+y];
						}
					}
					coeffArray[xpos+u][ypos+v] =2*c(u)*c(v)*sum/N;
				}
			}
			//! quantize the frequency coefficient of this block (basic requirement)
			//TODO
			for(int u=0;u<8;u++){
				for(int v=0;v<8;v++){
					coeffArray[xpos+u][ypos+v] = coeffArray[xpos+u][ypos+v] / QuantizationMatrix[u][v];
					
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
			fprintf(fp, "%3.3lf,", coeffArray[c][r]);
		}
		fprintf(fp, "\n");
	}
	cout << "Quantized coefficients saved!" << endl;

	if (need_idct)
	{
		Bitmap reconstructedImg(cols, rows);
		int out =0;
		//! apply IDCT on the quantized coefficients (enhancement part)
		for (int i = 0; i < blockRow; i++)
		{
			for (int j = 0; j < blockCol; j++)
			{
				int xpos = j*8, ypos = i*8;
				
				//! apply de-quantization on block_ij 
				//TODO
				double back[8][8];
				for(int m=0;m<8;m++){
					for(int n=0;n<8;n++){
						back[m][n] = coeffArray[xpos+m][ypos+n]*QuantizationMatrix[m][n];
					}
				}

				//! apply IDCT on this block
				//TODO
				double v[8][8],pixel[8][8];

				for(int x=0;x<8;x++){
					for(int y=0;y<8;y++){
						double sum=0;
						for(int v=0;v<8;v++){
							sum+= cos((2*y+1)*v*PI/16)*back[x][v];
							
							if (v==0) sum /= sqrt(2);
						}
						v[x][y] = sum/2;
					}
				}
				for(int y=0;y<8;y++){
					for(int x=0;x<8;x++){
						double sum=0;
						for(int u=0;u<8;u++){
							sum+= cos((2*x+1)*u*PI/16)*v[u][y];
							if (u==0) sum /= sqrt(2);
						}
						pixel[x][y] = sum/2;
					}
				}
				

				//! shiftting back the pixel value range to 0~255
				//TODO
				
				for(int m=0;m<8;m++){
					for(int n=0;n<8;n++){	
						convert[ypos+n][xpos+m] = round(pixel[m][n]+128);
						
					}
				}
			}
		}
		for(int i=0;i<rows;i++){
			for(int j=0;j<cols;j++){
				reconstructedImg.setPixel(j,i,(char)convert[i][j]);
				
			}
		}
		
		string savePath = "reconstructedImg.bmp";
		reconstructedImg.save(savePath.c_str());
		cout << "reconstructed image saved!" << endl;
	}

	return 0;
}