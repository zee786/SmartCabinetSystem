#pragma once
#ifdef _DEBUG
#pragma comment( lib, "opencv_imgproc245d.lib" )
#pragma comment( lib, "opencv_core245d.lib" )
#pragma comment( lib, "opencv_highgui245d.lib" )
#pragma comment( lib, "opencv_calib3d245d.lib" )
#pragma comment( lib, "opencv_features2d245d.lib" )
#pragma comment( lib, "opencv_flann245d.lib" )
#else
#pragma comment( lib, "opencv_imgproc245.lib" )
#pragma comment( lib, "opencv_core245.lib" )
#pragma comment( lib, "opencv_highgui245.lib" )
#pragma comment( lib, "opencv_calib3d245.lib" )
#pragma comment( lib, "opencv_features2d245.lib" )
#pragma comment( lib, "opencv_flann245.lib" )

#pragma comment (lib, "Ws2_32.lib")
#endif

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv\cv.h>
#include "opencv2\opencv.hpp"
using namespace cv;


class Cupboard
{
private:
	float width;
	cv::Mat homography;
	vector<Point2f> points;
	int id;

public:
	Cupboard(int id, float aspectRatio);
	~Cupboard(void);
	void init(vector<Point2f>);
	Mat getHomography();
	float getAspectRatio();
	vector<Point2f> getPoints();
	int getId();
};

