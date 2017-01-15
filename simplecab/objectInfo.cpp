/*
---------------------------------
 @file ObjectInformation.cpp

---------------------------------
*/

#include "ObjectInfo.h"

using namespace std;
using namespace cv;


ObjectInfo::ObjectInfo()

{
		
		texImage =cv::Mat(cv::Size(300, 300), CV_8UC3);
		db_Connection();
}
void ObjectInfo::db_Connection()
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
void ObjectInfo::setObjectInfo(const char *info,int m)
{
	n=m;
	//projection image with text
	
	if(info && info[0] == '\0')
	{
		return;
	}
	if(n==2)
	{
		try
		{
			stmt=con->createStatement();
			prep_stmt=con->prepareStatement("SELECT * from imageonlyp5 where UID=?");
			//prep_stmt=con->prepareStatement("SELECT * from itemsrelatedinfop4 where UID=?");
			//prep_stmt=con->prepareStatement("SELECT * from itemsnrelatedinfop11 where UID=?");
			
			prep_stmt->setString(1,info);
			res=prep_stmt->executeQuery();
			
			while (res->next())
			{
				//update current locaiton of the tag from dTaginfo
				prep_stmt=con->prepareStatement("Select ANTID from dtaginfo where UID=?");
				prep_stmt->setString(1,res->getString("UID").c_str());
				dTagres=prep_stmt->executeQuery();
				if(dTagres->next())
				{
					if(dTagres->rowsCount()>0)
					{
						strcpy(antID,dTagres->getString("ANTID").c_str());	
					}
					else
					{
						strcpy(antID,res->getString("ANTID").c_str());
				
					}
				}
				strcpy(UID, res->getString("UID").c_str());
				strcpy(fpath, res->getString("imgpath").c_str());
				ANTID=antID;
				id = atoi(res->getString("TAGID").c_str());
				string	memori=ret_memory(id);
		
				int nError=WSAGetLastError();
				if(nError!=WSAEWOULDBLOCK&&nError!=0)
				{
					std::cout<<"Winsock error code: "<<nError<<"\r\n";
					std::cout<<"Client disconnected!\r\n";
						
					break;
				}
			}
			texImage =cv::Mat(cv::Size(200, 200), CV_8UC3);
			texImage.setTo(Scalar(100, 100, 100));
			texImageIsCreated = false;
			cv::Point loc;
			objectImage = cv::imread(fpath);
			if(objectImage.data == NULL)
			{
					return;
			}
				
			float arrrr = ((float)objectImage.size().width) / objectImage.size().height;
			float nw, nh;
				
			if (arrrr > 2) {
				nw = 150;
				nh = nw / arrrr;
			} else {
				nh = 150;
				nw = nh * arrrr;
			}

			cv::Mat imageROI(texImage(cv::Rect((texImage.size().width - nw) / 2, (texImage.size().height - nh) / 2 + 20, nw, nh)));
			cv::resize(objectImage, imageROI, imageROI.size(), 0, 0, cv::INTER_LINEAR);
			texImageIsCreated = true;
				
		} 
		catch (sql::SQLException &e) 
		{
			cout<<"Sql Error" << &e;
			cout << "# ERR: SQLException in " << __FILE__;
			cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
			exit(1);
		}
		//cout << "end case n==2" << endl;
	}
	if(n==1)
	{
				strcpy(fpath,"./kitchenimages/rect.jpg");
				texImage =cv::Mat(cv::Size(512, 512), CV_8UC3);
				texImage.setTo(cv::Scalar(0, 0, 0));
				texImageIsCreated = true;
			
	} 
	
	
	
	
	
	
}
	


string ObjectInfo::ret_memory(int Id)
{	
	
	prep_stmt=con->prepareStatement("select distinct(memories.memory_Id),memories.memory_desc from memories,item_memory where item_memory.TAGID=? ORDER BY RAND() LIMIT 1" );
	prep_stmt->setInt(1,Id);
	res=prep_stmt->executeQuery();
	while (res->next()) 
	{
			cout << res->getString(2).c_str() << endl;
			strcpy(memori, res->getString(2).c_str());
			
			
	}
	return memori;


}





ObjectInfo::~ObjectInfo()
{
}

void ObjectInfo::makeTextureImage(char* info)
{
}

int ObjectInfo::GetShelfNo(void)
{
	return objectNo;
}
int ObjectInfo::GetCabinetNo(void)
{
	return cabinetNo;
}

cv::Mat ObjectInfo::GetTexImage(void)
{
	return texImage;
}

bool ObjectInfo::TexImageIsCreated(void)
{
	return texImageIsCreated;
}
string ObjectInfo:: returntext()
{
	return sentences;


}
string ObjectInfo::GetFilePath()
{
	return filepath;
}
string ObjectInfo::getANTID()
{
	return ANTID;

}
string ObjectInfo::getUID()
{
	return UID;
}