#include "StdAfx.h"
#include "Cupboard.h"
#include <vector>

Cupboard::Cupboard(int id, float aspectRatio)
{
	this->id = id;
	this->width = aspectRatio;
	this->points = vector<Point2f>(4);
}


Cupboard::~Cupboard(void)
{
}


void Cupboard::init(vector<Point2f> destPoints)
{
	std::vector<cv::Vec2f> srcPoints(4);
	srcPoints[0][0] = 0;
	srcPoints[0][1] = 0;
	srcPoints[1][0] = this->width;
	srcPoints[1][1] = 0;
	srcPoints[2][0] = 0;
	srcPoints[2][1] = 1;
	srcPoints[3][0] = this->width;
	srcPoints[3][1] = 1;
	points = destPoints;
	this->homography = cv::findHomography(srcPoints, destPoints, CV_RANSAC);
}

Mat Cupboard::getHomography()
{
	return homography;
}

float Cupboard::getAspectRatio() 
{
	return width;
}

vector<Point2f> Cupboard::getPoints()
{
	return points;
}

int Cupboard::getId() {
	return id;
}