#pragma once
#include <iostream>
#include <string>
#include "Cupboard.h"
#include "Door.h"

using namespace std;
class Door;
class Cupboard;
class Shelf
{
private:
	string antennaID;
	Cupboard *cupboard;
	Door *door;
	cv::Point2f center;
	cv::Size2f size;
	int id;

public:
	Shelf(int id, string antennaID,Cupboard *cupboard,Door *door);
	~Shelf(void);
	void setCenter(cv::Point2f);
	void setSize(cv::Size2f);
	string getAntennaId();
	Door* getDoor();
	Cupboard * getCupboard();
	Point2f getCenter();
	Size2f getSize();
	int getId();
};

