#include "bmp.h"		//	Simple .bmp library
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

#define Baseline 30.0
#define Focal_Length 100
#define Image_Width 35.0
#define Image_Height 35.0
#define Resolution_Row 512
#define Resolution_Col 512
#define View_Grid_Row 9
#define View_Grid_Col 9

struct Point3d
{
	double x;
	double y;
	double z;
	Point3d(double x_, double y_, double z_) :x(x_), y(y_), z(z_) {}
};

struct Point2d
{
	double x;
	double y;
	Point2d(double x_, double y_) :x(x_), y(y_) {}
};

inline int gridToIndex(int s, int t) { return t * 9 + s; }

int floor(double a){
	int b;
	if(a<0){
		b=(int)a-1;
		return b;
	}else return a;
}
int ceil(double a){
	if (a-(int)a==0)
	return a;
	else return floor(a)+1;
}

int round(double x)
{
    if (x < 0.0)
        return (int)(x - 0.5);
    else
        return (int)(x + 0.5);
}

int main(int argc, char** argv)
{
	if(argc < 5 || argc > 6)
	{
		cout << "Arguments prompt: viewSynthesis.exe <LF_dir> <X Y Z> OR: viewSynthesis.exe <LF_dir> <X Y Z> <focal_length>" << endl;
		return 0;
	}
	string LFDir = argv[1];
	double Vx = stod(argv[2]), Vy = stod(argv[3]), Vz = stod(argv[4]);
	double targetFocalLen = 100; // default focal length for "basic requirement" part
	if(argc == 6){targetFocalLen = stod(argv[5]);}

	if ( Vx < -120 || Vx > 120.0 ||  Vy <-120 || Vy > 120.0) {
	    cout << "Input out of range" << endl;
	    return 0;
	}
	if(Vz != 0 && targetFocalLen ==0){
		cout<<"invalid value"<<endl;
		return {-1};
	}

	vector<Bitmap> viewImageList;


	//! loading light field views
	for (int i = 0; i < View_Grid_Col*View_Grid_Row; i++)
	{
		char name[128];
		sprintf(name, "/cam%03d.bmp", i);
		string filePath = LFDir + name;
		Bitmap view_i(filePath.c_str());
		viewImageList.push_back(view_i);
	}

	Bitmap targetView(Resolution_Col, Resolution_Row);
	cout << "Synthesizing image from viewpoint: (" << Vx << "," << Vy << "," << Vz << ") with focal length: " << targetFocalLen << endl;
	//! resample pixels of the target view one by one
	for (int r = 0; r < Resolution_Row; r++)
	{
		for (int c = 0; c < Resolution_Col; c++)
		{
			Point3d rayRGB(0, 0, 0);
			//! resample the pixel value of this ray: TODO

			Point2d plane((c-255.5)/256*17.5,(r-255.5)/256*17.5);
			Point2d intersect (Vx+ Vz/targetFocalLen*plane.x, Vy+ Vz/targetFocalLen*plane.y );
			int col = c;
			int row = r;
			
			col = round((c-255.5)/targetFocalLen*100+255.5);
			row = round((r-255.5)/targetFocalLen*100+255.5);

			/*if((r==0 && c==0 ) || (r ==Resolution_Row-1 && c==0) || (r==0 && c== Resolution_Col-1) || (r== Resolution_Row-1 && c== Resolution_Col-1) ){
				cout<<intersect.y<<" "<<intersect.x<<endl;
			}*/

			if (intersect.x > 120 || intersect.x < -120 || intersect.y > 120 || intersect.y < -120 || col>511 ||col< -0||row>511||row<0) {
                rayRGB.x = 0;rayRGB.y = 0;rayRGB.z = 0;
            }else{
			//get neightbor rays
			
			double x= (intersect.x+120)/Baseline;
			double y= (intersect.y+120)/Baseline-1;
			if(x<0) x+=1;
			else if(x>7)x-=1;
			if(y<0) y+=1;
			else if(y>7)y-=1;

			
			
			int left = floor(x);	int right = ceil(x);	int up = ceil(y);	int down =floor(y);

			/*if((r==0 && c==0 ) || (r ==Resolution_Row-1 && c==0) || (r==0 && c== Resolution_Col-1) || (r== Resolution_Row-1 && c== Resolution_Col-1) ){
				cout<< left<<" "<< right<<" "<<down<<" "<<up<<  endl;
			}*/

			int TLbit = gridToIndex(left,up);
			int TRbit = gridToIndex(right,up);
			int BLbit = gridToIndex(left,down);
			int BRbit = gridToIndex(right,down);
			/*if((r==0 && c==0 ) || (r ==Resolution_Row-1 && c==0) || (r==0 && c== Resolution_Col-1) || (r== Resolution_Row-1 && c== Resolution_Col-1) ){
				cout<< TLbit<<" "<< TRbit << " " << BLbit << " " << BRbit<< endl;
			}*/

			double alpha = x-(int)x;
			double beta  = y-(int)y;
			//Bilinear Interpolation
			unsigned char TL[3],TR[3],BL[3],BR[3], pv[3],pv1[3],ptarget[3];
						
			/*viewImageList[TLbit].getColor(col,row,TL[0],TL[1],TL[2]);
			viewImageList[TRbit].getColor(col,row,TR[0],TR[1],TR[2]);
			viewImageList[BLbit].getColor(col,row,BL[0],BL[1],BL[2]);
			viewImageList[BRbit].getColor(col,row,BR[0],BR[1],BR[2]);*/
			
			viewImageList[TLbit].getColor(c,r,TL[0],TL[1],TL[2]);
			viewImageList[TRbit].getColor(c,r,TR[0],TR[1],TR[2]);
			viewImageList[BLbit].getColor(c,r,BL[0],BL[1],BL[2]);
			viewImageList[BRbit].getColor(c,r,BR[0],BR[1],BR[2]);
			for(int i=0;i<3;i++){
				pv[i]  = (1.0-alpha)* TL[i] + alpha * TR[i];
				pv1[i] = (1.0-alpha)* BL[i] + alpha * BR[i];
				ptarget[i] = (1-beta)*pv[i] + beta*pv1[i];
			}
			
			rayRGB.x=ptarget[0];
			rayRGB.y=ptarget[1];
			rayRGB.z=ptarget[2];
			
			}
			
			//! record the resampled pixel value
			targetView.setColor(c, r, (unsigned char) rayRGB.x, (unsigned char) rayRGB.y, (unsigned char) rayRGB.z);
		}
	}
	string savePath = "newView.bmp";
	targetView.save(savePath.c_str());
	cout << "Result saved!" << endl;
	return 0;
}