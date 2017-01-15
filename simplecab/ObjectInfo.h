/*
---------------------------------
 @file Object.h

 @brief Each object information calss

 @author Yuichiro Fujimoto
 @date 2013-10-29
---------------------------------
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

#pragma comment (lib, "Ws2_32.lib")
#endif

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
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
#include "opencv2\opencv.hpp"

using namespace std;

class ObjectInfo
{
	int cabinetNo;
	int shelfNo;
	int rfid;       
	int objectNo;
	
	bool texImageIsCreated;

	string ANTID;				  //Location of each item
	//string UID;					  //Tag information
	string filepath;              // file path for object image
	string sentences;             // sentences about each object
	cv::Mat texImage;             // Displaed information texture image
    cv::Mat objectImage;   
	sql::Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt;
	sql::ResultSet *res;
	sql::ResultSet *dTagres;
	sql::PreparedStatement  *prep_stmt;
	sql::PreparedStatement  *prep_stmt1;
	//variable for socket connections
	SOCKADDR_IN addr;
	SOCKET sListen;
	SOCKET sConnect;
	SOCKET* Connections;
	int x, y;
	int id;
	char antID[1024];	
	char UID[1024];	
	char buffer[2056];
	char memori[1024];
	char fpath[1024];
public:
	//ObjectInfo(char *info);
	//ObjectInfo(char *info,static int m);
	ObjectInfo();
	~ObjectInfo();

	void makeTextureImage(char* info);


//--------------------------------
//  Functions to get imformation
//--------------------------------
	int GetShelfNo(void); 
	cv::Mat GetTexImage(void);
    bool TexImageIsCreated(void);
	string returntext();
	int n;
	string GetFilePath();
	int GetCabinetNo();
	void setObjectInfo(const char *info,int m);
	string getUID();
	string getANTID();
	void db_Connection();
	string ret_memory(int Id);
};