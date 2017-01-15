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



#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv\cv.h>
#include <fstream>
#include <sstream>
#include <cmath>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <time.h>

#include <windows.h>

#include "opencv2\opencv.hpp"
using namespace std;

class RFIDTag
{	
private:
	stringstream strANT;
	stringstream ss;
	stringstream tID;
	stringstream fID;
	//int doorstatus;
	//const char* leftdoorID;
	//const char* rightdoorID;
	char UID[1024];	
	char AID[1024];
	//int shelfno;
	//int leftdoor;
	//int rightdoor;
	int buffersize;
	string  tagBuffer[105];
	string  antBuffer[105];
	const char *data;
	int n;
	
public:
	RFIDTag();
	~RFIDTag(void);
	void setInfo(char* info);
	stringstream* getAntID(void);
	void setUID(string uID);
	void setAID(string aID);
	string getUID(void);
	string getAID();
	int Getdoorstatus();
	int getShelfno();
	void processInfo();
	//buffer for antennas
	void processAntInfo();

	//get Antenna Status
	int getAntStatus();
	void setAntStatus(int aStatus);
};
