// simplecab.cpp : Defines the entry point for the console application.
//
//setup adhoc network on windows 8
//netsh wlan start hosted network
//http://www.users.pjwstk.edu.pl/~jms/qnx/help/tcpip_4.25_en/prog_guide/sock_advanced_tut.html

#include "stdafx.h"
/*
#define BOTTOM_LEFT "02004c"
#define BOTTOM_RIGHT "02014c"
#define TOP_LEFT "02024c"
#define TOP_RIGHT "02034c"
*/
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
#pragma comment( lib, "opencv_nonfree245.lib" )
//#pragma comment (lib, "Ws2_32.lib")
#endif



#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/nonfree/nonfree.hpp>

#include <opencv\cv.h>
#include <conio.h>

#include "mysql_connection.h"
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <driver.h>
#include <exception.h>
#include <resultset.h>
#include <statement.h>
#include <prepared_statement.h>
#include <winsock.h>
#include <io.h>
#include <mysql.h>
#include "ObjectInfo.h"
#include "RFIDTag.h"
#include <unordered_map>

#include "Cupboard.h"
#include "Shelf.h"
#include "Door.h"
#include "TagItem.h"
#include <atlstr.h>
//#pragma comment (lib, "Ws2_32.lib")

using namespace std;
using namespace cv;

CvPoint point;
double point4[8];

#define PRO_WIDTH 1280
#define PRO_HEIGHT 960
#define DEFAULT_PORT "10777"
#define REMOTE_PORT "8001"
#define DEFAULT_BUFLEN 1024
//Canvas Variables
cv::Mat_< cv::Vec3b > canvas;
cv::Mat_< cv::Vec3b > image;



//SetupInfo
vector<Cupboard*> cupboards;
vector<TagItem*> tagItems;
vector<Shelf*> shelfs;

void InitServer2();
int read_tagInfo();
int bytes=0;
//highlight the window
void display2(double x,double y,double width,double height,cv::Mat_< float >  homography);
string tagBuff[30];

ObjectInfo *obj;
	
RFIDTag *rfidObj;

int antStatus;
int counter=0;
int n;
int d;
int tId=0;
//variables for database
sql::Driver *driver;
sql::Connection *con;
sql::Statement *stmt;
sql::ResultSet *res;
sql::PreparedStatement  *prep_stmt;
sql::PreparedStatement  *prep_stmt1;
//variable for socket connections
SOCKADDR_IN addr;
SOCKET sListen;
SOCKET sConnect;
SOCKET* Connections;
int ConCounter = 0;
//conncetion to the database
void db_connection();
//save the last location of the item
void save_Location();

//Socket IP Address
int addrlen = sizeof(addr);

//Useful for socket programming
struct Buffer
{
	int ID;
	char Message[2056];
};


//Winsocket Parameter Initialization
SOCKET s; //Socket handle
SOCKET* newclient;
SOCKET s2;
WSADATA wsadata;
SOCKET client;
int iResult;
int Id=1;
static char* Re1= new char[2056];
static char* Recv2 = new char[2056];
	
bool disp=true;
bool hlight=true;
void highlight();

void db_connection();
void remote_socket_connection();
char * accept_clientcon();
//Display Info on the cabinet 
void displayInformation();
void display(double x,double y,double width,double height,cv::Mat_< float >  homography);
void mymouse(int event, int x, int y, int flags, void* param);
Shelf* findShelf(string antennaId);
TagItem* findTagItem(string tagId);
void clearProjection();
void display(TagItem * item);
void highlight(TagItem * item);
SOCKET* client2;
SOCKET Client;
DWORD thread;
char* Recv = new char[2056];
char* Recvbytes = new char[2056];

DWORD WINAPI secondthread(LPVOID lpParam) 
{
	//ZeroMemory(Recv2, 2056);
	client = INVALID_SOCKET;
	static struct timeval recv_tv;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	WSADATA WsaDat;
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsadata);
	if (iResult != 0) 
	{
		printf("WSAStartup failed with error: %d\n", iResult);
		exit(1);
	}
	// set address info
	ZeroMemory( &hints, sizeof(hints) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;  //TCP connection!!!
	//resolve server address and port 
	iResult = getaddrinfo("10.20.212.103",REMOTE_PORT, &hints, &result);
	if( iResult != 0 )
	{
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		exit(1);
	}
	// Attempt to connect to an address until one succeeds
	for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) 
	{
		// Create a SOCKET for connecting to server
		client= socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (client == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			exit(1);
		}
		// Connect to server.
		int maxAttempts = 5;
		int attempts = 0;
		iResult = SOCKET_ERROR;
		while (iResult == SOCKET_ERROR) {
			attempts++;
			cout << "Attempting connection [" << attempts << "/" << maxAttempts << "]" << endl;
			iResult = connect( client, ptr->ai_addr, (int)ptr->ai_addrlen);
			if (iResult == SOCKET_ERROR && attempts >= maxAttempts)
			{
				closesocket(client);
				client= INVALID_SOCKET;
				//printf ("The server is down... did not connect");
				MessageBoxA(NULL, "Could not connect to server", "Error", MB_OK | MB_ICONERROR);
				exit(0);
			}
		}
	}
	u_long iMode=1;
	ioctlsocket(client,FIONBIO,&iMode);
	
	Recv2=accept_clientcon();	

	return 0;


}


//
DWORD WINAPI receive_cmds(LPVOID lpParam) 
{

	SOCKET s; //Socket handle
	s = INVALID_SOCKET;
	//ZeroMemory(Recv, 2056);
	static struct timeval recv_tv;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	WSADATA WsaDat;
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsadata);
	if (iResult != 0) 
	{
		printf("WSAStartup failed with error: %d\n", iResult);
		exit(1);
	}
	// set address info
	ZeroMemory( &hints, sizeof(hints) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;  //TCP connection!!!
	//resolve server address and port 
	iResult = getaddrinfo("10.16.77.170",DEFAULT_PORT, &hints, &result);
	
	if( iResult != 0 )
	{
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		exit(1);
	}
	// Attempt to connect to an address until one succeeds
	for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) 
	{
		// Create a SOCKET for connecting to server
		s= socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (s == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			exit(1);
		}
		// Connect to server.
		int maxAttempts = 5;
		int attempts = 0;
		iResult = SOCKET_ERROR;
			while (iResult == SOCKET_ERROR) {
				attempts++;
				cout << "Attempting connection [" << attempts << "/" << maxAttempts << "]" << endl;
				iResult = connect( s, ptr->ai_addr, (int)ptr->ai_addrlen);
			
				if (iResult == SOCKET_ERROR && attempts >= maxAttempts)
				{
					closesocket(s);
					s = INVALID_SOCKET;
					//printf ("The server is down... did not connect");
					MessageBoxA(NULL, "Could not connect to server", "Error", MB_OK | MB_ICONERROR);
					exit(0);
				}
			//_beginthreadex(0,0,ServClient, (void*)s, 0,0);	
			}
		
		}
		u_long iMode=0;
		ioctlsocket(s,FIONBIO,&iMode);
	
	//ZeroMemory(Recv, 2056);
	printf("thread created\r\n");
    cout<<"Client Connected"<<endl;
	while(true)
	{
		bytes=recv(s, Recv, 2056,0);
		Sleep(10);
      
		 if(bytes < 0)
		 {
			 int nError=WSAGetLastError();
			 if(nError!=WSAEWOULDBLOCK&&nError!=0)
			 {
				 cout<<"bytes "<<nError<<endl;
				 shutdown(s,SD_SEND);
				 closesocket(s);
				 ExitThread(0);
			 }
		  }
		 if(bytes==0)
		 {
			 int nError=WSAGetLastError();
			 std::cout<<"Winsock error code: "<<nError<<"\r\n";
	      	std::cout<<"Server disconnected!\r\n";
		 }
		 if(bytes>0)
		 {
			
		 
		 }
      
	}

	return 0;


}

unsigned int __stdcall ServClient(void *data)
{
	newclient=(SOCKET*)data;
	Client=*newclient;
	cout<<"client is connected"<<endl;
	while(true)
	{
		bytes=recv(Client, Recv, 2056,0);
		Sleep(10);
      
		 if(bytes == 0)
		 {
			 
			 closesocket(Client);
			 ExitThread(0);
		  }
      
		cout<<"Recv Tag"<<Recv<<endl;
		cout<<"Bytes Received"<<bytes<<endl;
	}
	return 0;
}
int read_tagInfo()
{
	cout << "Starting LOOP" << endl;

	//Variable to decide display mode
	int m=0;

	//Copy tag information to buffer
	Buffer sbuffer;

	//Variable to recieve tag information
	char* Recv = new char[2056];
	ZeroMemory(Recv, 2056);

	//Variable to recieve information from client
	char* nRecv2 = new char[2056];
	
	ZeroMemory(Recv2, 2056);

	//Set 100ms socket timeout
	
	//char* nRecv2 = new char[2056];
	//ZeroMemory(nRecv2, 2056);
	//
	rfidObj=new RFIDTag();
	//
	obj=new ObjectInfo();
	
	char* tValue;

	bool hlight=false;
	//set the timer
	//int id = 0;
	TagItem* theItem = NULL;
	TagItem* lastItem = NULL;
	remote_socket_connection();
	//InitServer2();
	cout<<"Connected with Remote computer"<<endl;
	
	while(true)
	{
		//receive data from Remote Computer
		Recv2=accept_clientcon();
		//find compatible items from the database.
		
		if (theItem == NULL || theItem->getTagId() != Recv2)
		{
			
			if (strlen(Recv2) == 16) {
				theItem = findTagItem(Recv2);
				if (theItem == NULL) {
					if (Recv2[0] == 'e') {
						cout << "Requested item " << Recv2 << " does not exist in the database." << endl;
						theItem = new TagItem(Recv2, "unknown");
						tagItems.push_back(theItem);
					}
				}
			 }
		 }

	bytes=recv(s, Recv, 2056,0);
		int nbytes;
		nbytes=bytes;
			
		if (bytes > 0 ) 
		{
			n=bytes/19;
			hlight=true;
			cout << bytes << " read, looking for " << n << " tags" << endl;
			cout<<"String from other computer"<<Recv2<<endl;
		
			if((bytes % 19) !=0)
			{
					cout<<"bytes "<<bytes << endl;
					n=bytes/19;
					cout << nbytes << " read, looking for " << n << " tags" << endl;
					cout<<"String from other computer"<<Recv2<<endl;
		
			}
			for(int x =0;x<n;x++)
			{	
				//seperate packets from the signal/
				char* Recvbytes = new char[2056];
				Recvbytes=Recv;
				tValue=Recv+(19*x);
				rfidObj->setInfo(tValue);
				Shelf * shelf = findShelf(rfidObj->getAID());
				TagItem * item = NULL;
				string uid = rfidObj->getUID();
				if (uid[0] == 'e') 
				{
					item = findTagItem(rfidObj->getUID());
					if (shelf == NULL) 
					{
						cout << "invalid antenna ID \"" << rfidObj->getAID() << "\"" << endl;
					}
					if (item == NULL) {
						cout << "Found an item that we've never seen before oh my goodness! D:" << endl;
						item = new TagItem(rfidObj->getUID(), "unknown");
						tagItems.push_back(item);
					} else if (shelf != NULL) {
						//cout << item->getTagId() << " scanShelf( " << shelf->getAntennaId() << " );" << endl;
						item->scanShelf(shelf);
					}
				}
			}//end of for loop
		}//end of if bytes >0
		
		int lim = tagItems.size();
		for (int i = 0; i < lim; i++) {
			tagItems[i]->update();
		}
		
		bool forceUpdate = false;
		if (theItem != lastItem)
		{
			cout << "Updating due to new item" << endl;
			forceUpdate = true;
		}
		lastItem = theItem;

		if (theItem == NULL) {
			//cout << "No item of interest." << endl;
			clearProjection();
		}
		else 
		{
			int status = theItem->getStatus();
			if (!forceUpdate) {
				if (status >= 4)
				{
					cout << "Updating due to changed shelf" << endl;
					forceUpdate = true;
				}
				else if (status >= 2)
				{
					cout << "Updating due to entering/exiting shelf" << endl;
					forceUpdate = true;
				}
			}

			if (status & 1 == 1)
				if (theItem->getCurrentShelf()->getDoor()->getStatus() >= 2) {
					cout << "Updating due to door status change" << endl;
					forceUpdate = true;
				}
			//cout << theItem->getStatus() << endl;

			if (forceUpdate)
			{
				if (status & 1 == 1)
				{
					//On shelf
					Shelf * curShelf = theItem->getCurrentShelf();

					cout << theItem->getTagId();
					if (curShelf == NULL)
					{
						cout << " 404 shelf not found, clearing projection" << endl;
						clearProjection();
					}
					else
					{
						cout << " on Shelf " << curShelf->getId() << " " << endl;

						Door * curDoor = curShelf->getDoor();
						cout << "Door is ";
						
						if (curDoor == NULL)
						{
							cout << "NULL, highlighting the item";
							highlight(theItem);
						}
						else if (curDoor->getClosed())
						{
							cout << "closed, displaying item";
							display(theItem);
						}
						else
						{
							cout << "open, highlighting the item";
						//highlight(theItem);
						}
						cout << endl;
					}
				}
				else
				{
					cout << "Not on shelf, clearing projection" << endl;
					//clearProjection();
				}
			}
		}
	}//end of while loop
	ZeroMemory(Recv, 2056);
	ZeroMemory(Recv2, 2056);

}

bool cleaned = false;
//recv data from another computer/application
char * accept_clientcon()
{
	if (!cleaned) {
		ZeroMemory(Re1, 2056);
		cleaned = true;
	}

	char* Re= new char[2056];
	ZeroMemory(Re, 2056);
	char* Send = new char[sizeof(Buffer)];
	ZeroMemory(Send, sizeof(Buffer));
	Buffer sbuffer;
	int ID=0;
	int m;
	//client socket for remote socket connection
	//sConnect for local host
	if(recv(client, Re, 2056, NULL))
	{
		if(Re && Re[0] == '\0' || Re[0] != 'e')
		{

			return Re1;

		}
		else
		{
			sbuffer.ID = ID;
			memcpy(sbuffer.Message, Re, 2056);
			memcpy(Send, &sbuffer, sizeof(Buffer));
			//Re1=Re;
			disp=false;
//			tmr=10;
			strcpy(Re1,Re);
		}

	}
	return Re;
}

TagItem * findTagItem(string tagId) {
	int lim = tagItems.size();

	for (int i = 0; i < lim; i++)
	{
		if (tagItems[i]->getTagId() == tagId) {
			return tagItems[i];
		}
	}
	return NULL;
}

Shelf * findShelf(string antennaId) {
	int lim = shelfs.size();

	for (int i = 0; i < lim; i++)
	{
		if (shelfs[i]->getAntennaId() == antennaId) {
			return shelfs[i];
		}
	}
	return NULL;
}

//----------------------------------------
//Read Homography Point Data from File
int read_fromfile()
{
	cout << "Beginning to read predef.dat" << endl;
	ifstream file("predefnew.dat");
	string line;
	int x=0;
	if(file.fail()) {
		cout << "Could not load file." << endl;
		return 0;
	}
	std::string token;
	stringstream ss;
	//(!file.eof()) // To get you all the lines.
	try
	{
		getline(file,line); // Saves the line in STRING.S

		int cupboardCount = atoi(line.c_str());
		std::cout << cupboardCount << " cupboards listed." << endl;
		for (int i = 0; i < cupboardCount; i++) {
			std::cout << "Reading cupboard " << i << endl;
			//First read the size of the cupboard to get the aspect ratio
			Point2f size;
			vector<Point2f> corners(4);

			for (int j = 0; j < 5; j++) {
				getline(file, line);
				stringstream ss(line);
				int y=0;
				while(std::getline(ss, token, ','))
				{
					if (y == 0)
						((j == 0) ? size : corners[j - 1]).x = atof(token.c_str());
					else 
						((j == 0) ? size : corners[j - 1]).y = atof(token.c_str());
					y++;
				}
			}

			std::cout << "  size: " << size << endl;
			std::cout << "  4 corners read" << endl;

			Cupboard * cup = new Cupboard(cupboards.size(), size.x / size.y);
			cup->init(corners);
			cupboards.push_back(cup);

			
			getline(file,line);
			cout << "  " << line << endl;
			int shelfCount = atoi(line.c_str());
			std::cout << "  " << shelfCount << " shelves listed" << endl;
			for (int j = 0; j < shelfCount; j++) {
				std::cout << "  reading shelf " << j << endl;
				//Read antenna ID
				getline(file,line);
				string antenna = line;
				std::cout << "    antenna tag " << antenna << endl;
				//Read doorTag ID
				getline(file, line);
				string doorTagID = line;
				std::cout << "    door tag " << doorTagID << endl;

				//Create door object
				TagItem* doorTag = findTagItem(doorTagID);
				if (doorTag == NULL)
				{
					std::cout << "    creating new door ItemTag" << endl;
					doorTag = new TagItem(doorTagID, "door");
					tagItems.push_back(doorTag);
				}
				Door* dooor =new Door(doorTag);

				Vec2f center;
				Vec2f siz;
				//Read center and size
				for (int k = 0; k < 2; k++) {
					getline(file, line);
					stringstream ss(line);
					int y = 0;
					while(std::getline(ss, token, ','))
					{
						if (k == 0) //read center position
							center[y] = atof(token.c_str());
						else // and size
							siz[y] = atof(token.c_str());
						y++;
					}
				}
				cout << "    center: " << center << " size: " << siz << endl;

				//Construct and save
				Shelf * s = new Shelf(shelfs.size(), antenna, cup, dooor);
				s->setCenter(center);
				s->setSize(Size2f(siz[0], siz[1]));
				dooor->setShelf(s);
				shelfs.push_back(s);
				cout << "  finished reading shelf " << j << endl;
			}
			cout << "finished reading cupboard " << i << endl;
		}
	} catch (...) {
		cout << "Failed to read config file." << endl;
		return 0;
	}
	cout << "Finished reading file" << endl;
	file.close();
	return 1;
}



void display(TagItem* item) {
	if (item == NULL) {
		cout << "Called display with NULL" << endl;
		return;
	}

	//Load the image and put some nice text there for goodness' sake
	obj->setObjectInfo(item->getTagId().c_str(), 2);

	Shelf * shelf = item->getCurrentShelf();
	if (shelf == NULL) {
		cout << "Called display with item not on a shelf" << endl;
		return;
	}
	
	Cupboard * cupb = shelf->getCupboard();
	if (cupb == NULL) {
		cout << "Called display with item on a shelf that has no cupboard, check configuration." << endl;
		return;
	}

	Mat homography = cupb->getHomography();
	
	Mat inner = obj->GetTexImage();
	int innerWidth = inner.size().width;
	int innerHeight = inner.size().height;
	float innerAR = ((float)innerWidth) / innerHeight;
	//cout << "innerAR " << innerAR << endl;

	int outerWidth;
	int outerHeight;
	float outerAR = cupb->getAspectRatio() * shelf->getSize().width / shelf->getSize().height;
	//cout << shelf->getSize() << endl;
	//cout << "Shelf aspect ratio " << outerAR << endl;
	if (outerAR > innerAR) {
		outerHeight = innerHeight;
		outerWidth = outerAR * outerHeight;
	}
	else
	{
		outerWidth = innerWidth;
		outerHeight = outerWidth / outerAR;
	}
	//cout << "Inner size (" << innerWidth << ", " << innerHeight << ")" << endl;
	//cout << "Outer size (" << outerWidth << ", " << outerHeight << ")" << endl;
	Mat outer = Mat::zeros(outerHeight, outerWidth, inner.type());
	outer.setTo(Scalar(0, 0, 0));
	int left = (outerWidth - innerWidth) / 2;
	int top = (outerHeight - innerHeight) / 2;
	//cout << "left: " << left << " top: " << top << endl;

	inner.copyTo(outer(Rect(left, top, innerWidth, innerHeight)));
	//imshow("hewfwef", outer);
	//Find little t

	vector<Point2f> srcTri(4);
	vector<Point2f> dstTri(4);

	srcTri[0] = Point2f(0, 0);
	srcTri[1] = Point2f(outerWidth, 0);
	srcTri[2] = Point2f(outerWidth, outerHeight);
	srcTri[3] = Point2f(0, outerHeight);

	double w = cupb->getAspectRatio() * shelf->getSize().width / 2;
	double h = shelf->getSize().height / 2;
	Point2f center = shelf->getCenter();
	center.x = center.x * cupb->getAspectRatio();
	dstTri[0] = center + Point2f(-w, -h);
	dstTri[1] = center + Point2f(w, -h);
	dstTri[2] = center + Point2f(w, h);
	dstTri[3] = center + Point2f(-w, h);

	Mat t = findHomography(srcTri, dstTri, CV_RANSAC);
	Mat T = homography * t;
	warpPerspective(outer, canvas, T, canvas.size());
	imshow("canvas", canvas);
	waitKey(20);
}

void highlight(TagItem * item) {
	if (item == NULL) {
		cout << "Called highlight with NULL item" << endl;
		return;
	}

	//Load the image and put some nice text there for goodness' sake
	obj->setObjectInfo(item->getTagId().c_str(), 2);

	Shelf * shelf = item->getCurrentShelf();
	if (shelf == NULL) {
		cout << "Called highlight with item that has no shelf" << endl;
		return;
	}
	Cupboard * cupb = shelf->getCupboard();
	if (cupb == NULL) {
		cout << "Called highlight with item on a shelf that has no cupboard. Check configuration." << endl;
		return;
	}

	clearProjection();

	Mat homography = cupb->getHomography();
	
	//Magic numbers:
	int innerWidth = 200;
	int innerHeight = 200;
	float innerAR = 1;
	
	int outerWidth;
	int outerHeight;
	float outerAR = cupb->getAspectRatio() * shelf->getSize().width / shelf->getSize().height;
	
	if (outerAR > innerAR) {
		outerHeight = innerHeight;
		outerWidth = outerAR * outerHeight;
	}
	else
	{
		outerWidth = innerWidth;
		outerHeight = outerWidth / outerAR;
	}
	Mat outer = Mat::zeros(outerHeight, outerWidth, obj->GetTexImage().type());
	outer.setTo(Scalar(100, 100, 100));
	
	vector<Point2f> srcTri(4);
	vector<Point2f> dstTri(4);

	srcTri[0] = Point2f(0, 0);
	srcTri[1] = Point2f(outerWidth, 0);
	srcTri[2] = Point2f(outerWidth, outerHeight);
	srcTri[3] = Point2f(0, outerHeight);

	double w = cupb->getAspectRatio() * shelf->getSize().width / 2;
	double h = shelf->getSize().height / 2;
	Point2f center = shelf->getCenter();
	center.x = center.x * cupb->getAspectRatio();
	dstTri[0] = center + Point2f(-w, -h);
	dstTri[1] = center + Point2f(w, -h);
	dstTri[2] = center + Point2f(w, h);
	dstTri[3] = center + Point2f(-w, h);

	Mat t = findHomography(srcTri, dstTri, CV_RANSAC);
	Mat T = homography * t;
	warpPerspective(outer, canvas, T, canvas.size());
	imshow("canvas", canvas);
	waitKey(20);
}

void clearProjection() {
	canvas.setTo(Scalar(0, 0, 0));
	imshow("canvas", canvas);
	waitKey(20);
}
//get project points 
void mymouse(int event, int x, int y, int flags, void* param)
{
	
}

void InitServer2(void)
{
	int RetVal = 0;
	WSADATA wsaData;
	static struct timeval recv_tv;
	if(RetVal != 0)
	{
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	Connections = (SOCKET*)calloc(64, sizeof(SOCKET));

	sListen = socket(AF_INET, SOCK_STREAM, NULL);
	sConnect = socket(AF_INET, SOCK_STREAM, NULL);
	//addr.sin_addr.s_addr = inet_addr("163.221.126.43");
	addr.sin_addr.s_addr = inet_addr("10.20.208.107");
	addr.sin_port        = htons(8001);
	addr.sin_family      = AF_INET;

	::bind(sListen, (SOCKADDR*)&addr, sizeof(addr));

	listen(sListen, 64);
	sConnect = accept(sListen, NULL, NULL);
	if(sConnect == INVALID_SOCKET)
	{
		MessageBoxA(NULL,"accept failed","Error",MB_OK | MB_ICONERROR);
	}
	closesocket(sListen);
}
//connection with database
void db_connection()
{
	try {
		/* Create a connection */
		driver = get_driver_instance();
		con = driver->connect("", "root", "");
		/* Connect to the MySQL test database */
		con->setSchema("test");

	}
	catch (sql::SQLException &e) {
		cout<<"Sql Error" << &e;
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;

	}

}

//connection to remote computer
void remote_socket_connection()
{

	client = INVALID_SOCKET;
	static struct timeval recv_tv;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	WSADATA WsaDat;
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsadata);
	if (iResult != 0) 
	{
		printf("WSAStartup failed with error: %d\n", iResult);
		exit(1);
	}
	// set address info
	ZeroMemory( &hints, sizeof(hints) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;  //TCP connection!!!
	//resolve server address and port 
	iResult = getaddrinfo("10.20.203.210",REMOTE_PORT, &hints, &result);
	if( iResult != 0 )
	{
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		exit(1);
	}
	// Attempt to connect to an address until one succeeds
	for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) 
	{
		// Create a SOCKET for connecting to server
		client= socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (client == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			exit(1);
		}
		// Connect to server.
		int maxAttempts = 5;
		int attempts = 0;
		iResult = SOCKET_ERROR;
		while (iResult == SOCKET_ERROR) {
			attempts++;
			cout << "Attempting connection [" << attempts << "/" << maxAttempts << "]" << endl;
			iResult = connect( client, ptr->ai_addr, (int)ptr->ai_addrlen);
			if (iResult == SOCKET_ERROR && attempts >= maxAttempts)
			{
				closesocket(client);
				client= INVALID_SOCKET;
				//printf ("The server is down... did not connect");
				MessageBoxA(NULL, "Could not connect to server", "Error", MB_OK | MB_ICONERROR);
				exit(0);
			}
		}
		u_long iMode=1;
		ioctlsocket(client,FIONBIO,&iMode);	
	}
	
	

}
//connection with RFID reader
void socket_connection()
{

	s = INVALID_SOCKET;
	static struct timeval recv_tv;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	WSADATA WsaDat;
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsadata);
	if (iResult != 0) 
	{
		printf("WSAStartup failed with error: %d\n", iResult);
		exit(1);
	}
	// set address info
	ZeroMemory( &hints, sizeof(hints) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;  //TCP connection!!!
	//resolve server address and port 
	//10.16.77.170
	iResult = getaddrinfo("10.16.77.170",DEFAULT_PORT, &hints, &result);
	if( iResult != 0 )
	{
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		exit(1);
	}
	// Attempt to connect to an address until one succeeds
	for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) 
	{
		// Create a SOCKET for connecting to server
		s= socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (s == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			exit(1);
		}
		// Connect to server.
		int maxAttempts = 5;
		int attempts = 0;
		iResult = SOCKET_ERROR;
		while (iResult == SOCKET_ERROR) {
			attempts++;
			cout << "Attempting connection [" << attempts << "/" << maxAttempts << "]" << endl;
			iResult = connect( s, ptr->ai_addr, (int)ptr->ai_addrlen);
			if (iResult == SOCKET_ERROR && attempts >= maxAttempts)
			{
				closesocket(s);
				s = INVALID_SOCKET;
				//printf ("The server is down... did not connect");
				MessageBoxA(NULL, "Could not connect to server", "Error", MB_OK | MB_ICONERROR);
				exit(0);
			}
		}
	}
	u_long iMode=0;
	ioctlsocket(s,FIONBIO,&iMode);
	

}
void readItemsFromDb();
int _tmain()
{

	int ID;
	int n;
	int m;
	char *strPath;
	db_connection();
//	closesocket(s);
	//closesocket(sConnect);
	socket_connection();
	cout<<"RFID system is connected"<<endl;
	canvas = cv::Mat(PRO_HEIGHT, PRO_WIDTH, CV_8UC3);
	canvas.setTo(Scalar(0, 0, 0));
	cv::namedWindow("canvas", CV_WINDOW_FULLSCREEN);
	cv::imshow("canvas", canvas);
	read_fromfile();

	Scalar colours[] = {
		Scalar(255, 100, 100),
		Scalar(100, 255, 100),
		Scalar(100, 100, 255),
		Scalar(255, 255, 100),
		Scalar(100, 255, 255),
		Scalar(255, 100, 255),
	};
	//Draw the points of each cupboard
	for (int i = 0; i < cupboards.size(); i++) {
		for (int j = 0; j < 4; j++) {
			circle(canvas, cupboards[i]->getPoints()[j], 5, colours[i % 6], -1);
		}
	}

	imshow("canvas", canvas);

	readItemsFromDb();

	ID=	read_tagInfo();
	shutdown(sConnect,SD_SEND);
	closesocket(sConnect);
	WSACleanup();
	system("PAUSE");
	return 0;
}

void readItemsFromDb() {
	//number of items in db
	int lim = 0;
	for (int i = 0; i < lim; i++) {
		//Get i'th id from DB
		string itemTagId;
		//Get i'th type from DB
		string itemType;

		TagItem* item = new TagItem(itemTagId, itemType);
		tagItems.push_back(item);
	}
}