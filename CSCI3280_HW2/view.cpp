#include "bmp.h"		//	Simple .bmp library
#include<iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

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


int main(int argc, char** argv)
{
	if (argc != 6)
	{
		cout << "Arguments prompt: viewSynthesis.exe <LF_dir> <X Y Z> <focal_length>" << endl;
		return 0;
	}
	string LFDir = argv[1];
	double Vx = stod(argv[2]), Vy = stod(argv[3]), Vz = stod(argv[4]);
	double targetFocalLen = stod(argv[5]);

	vector<Bitmap> viewImageList;
	//! loading light field views
	for (int i = 0; i < View_Grid_Col*View_Grid_Row; i++)
	{
		char name[128];
		sprintf(name, "/cam%03d.bmp", i+1);
		string filePath = LFDir + name;
		Bitmap view_i(filePath.c_str());
		viewImageList.push_back(view_i);
	}

	Bitmap targetView(Resolution_Col, Resolution_Row);
	cout << "Synthesizing image from viewpoint: (" << Vx << "," << Vy << "," << Vz << ") with focal length: " << targetFocalLen << endl;
	//! resample pixels of the target view one by one
	// double grid_w = Image_Width / (double)Resolution_Row;
	// double grid_h = Image_Height / (double)Resolution_Col;
	for (int r = 0; r < Resolution_Row; r++)
	{
		for (int c = 0; c < Resolution_Col; c++)
		{
			// Point3d rayRGB(0, 0, 0);
			//! resample the pixel value of this ray: TODO
			// Point3d pix(0, 0, 0);
			// pix.x = grid_w * r - Image_Width / 2 + grid_w / 2;
			// pix.y = grid_h * c - Image_Height / 2 + grid_h / 2;
			// pix.z = -targetFocalLen;

			Point2d plane(0, 0);
			// plane.x = (targetFocalLen + Focal_Length) / pix.x;
			// plane.y = (targetFocalLen + Focal_Length) / pix.y;
			plane.x = Vx;
			plane.y = Vy;

			plane.x += Baseline * 4;
			plane.y += Baseline * 4;
			int ceil_x = ceil(plane.x / Baseline);
			int floor_x = floor(plane.x / Baseline);
			int ceil_y = 8 - ceil(plane.y / Baseline);
			int floor_y = 8 - floor(plane.y / Baseline);
			//cout << "px: " << plane.x << " " << "py: " << plane.y << " ";
			//cout << floor_x << " " << ceil_x << " " << ceil_y << " " << floor_y << " "  << endl;

			// Point2d offset(, plane.y % Baseline);
			// offset.x = plane.x - ;
				
			// }
			double alpha = (plane.x - double(floor_x) * Baseline) / Baseline;
			double beta = - (((240 - plane.y) - double(floor_y) * Baseline) / Baseline);
			//cout << alpha << " " << beta << endl;
			Color pt, pta, ptb, ptab;
			// viewImageList[floor_x * View_Grid_Col + floor_y].getColor(c, r, pt.R, pt.G, pt.B);
			// viewImageList[floor_x * View_Grid_Col + ceil_y].getColor(c, r, pta.R, pta.G, pta.B);
			// viewImageList[ceil_x * View_Grid_Col + floor_y].getColor(c, r, ptb.R, ptb.G, ptb.B);
			// viewImageList[ceil_x * View_Grid_Col + ceil_y].getColor(c, r, ptab.R, ptab.G, ptab.B);
			viewImageList[floor_y * View_Grid_Row + floor_x].getColor(c, r, pt.R, pt.G, pt.B);
			viewImageList[ceil_y * View_Grid_Row + floor_x].getColor(c, r, pta.R, pta.G, pta.B);
			viewImageList[floor_y * View_Grid_Row + ceil_x].getColor(c, r, ptb.R, ptb.G, ptb.B);
			viewImageList[ceil_y * View_Grid_Row + ceil_x].getColor(c, r, ptab.R, ptab.G, ptab.B);
			//cout << ceil_x * View_Grid_Col + ceil_y << " " << ceil_x * View_Grid_Col + floor_y << " " << floor_x * View_Grid_Col + ceil_y << " " << floor_x * View_Grid_Col + floor_y << endl;
			Color p0, p1, ptarget;
			p0.R = (1 - alpha) * pt.R + alpha * pta.R;
			p1.R = (1 - alpha) * ptb.R + alpha * ptab.R;
			ptarget.R = (1 - beta) * p0.R + beta * p1.R;

			p0.G = (1 - alpha) * pt.G + alpha * pta.G;
			p1.G = (1 - alpha) * ptb.G + alpha * ptab.G;
			ptarget.G = (1 - beta) * p0.G + beta * p1.G;

			p0.B = (1 - alpha) * pt.B + alpha * pta.B;
			p1.B = (1 - alpha) * ptb.B + alpha * ptab.B;
			ptarget.B = (1 - beta) * p0.B + beta * p1.B;

			// ptarget.R = 0.25 * (pt.R + pta.R + ptb.R + ptab.R);
			// ptarget.G = 0.25 * (pt.G + pta.G + ptb.G + ptab.G);
			// ptarget.B = 0.25 * (pt.B + pta.B + ptb.B + ptab.B);


			//! record the resampled pixel value
			targetView.setColor(c, r, ptarget.R, ptarget.G, ptarget.B);
		}
	}
	string savePath = "newView.bmp";
	targetView.save(savePath.c_str());
	cout << "Result saved!" << endl;
	return 0;
}