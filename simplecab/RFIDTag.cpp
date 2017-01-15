#include "StdAfx.h"
#include "RFIDTag.h"

using namespace std;

RFIDTag::RFIDTag()
{
	UID[1024]=NULL;
	AID[1024]=NULL;
	buffersize=0;
	
}
RFIDTag::~RFIDTag(void)
{
}


void RFIDTag::setInfo(char *info)
{
	clock_t t;
	strANT.str("");
	ss.str("");
	fID.str("");
	memset(UID,0,sizeof(UID));
	memset(AID,0,sizeof(AID));
	std::vector< std::vector< string > > a;
	
	for(int y = 0; y < 3; y++ )
	{
	  strANT << setfill('0') << setw(2) << hex << (int)((unsigned char)info[y]);
	}	
	for(int z = 11; z > 3; z-- ) 
	{
	  ss << setfill('0') << setw(2) << hex << (int)((unsigned char)info[z]);
    }
	
	fID>>ss.str()>>strANT.str();
	data=ss.str().c_str();
	string strUID(ss.str());
	setUID(strUID.c_str());
	setAID(strANT.str());

}

void RFIDTag::processInfo()
{
}


void RFIDTag::setUID(string uID)
{
	strcpy(UID,uID.c_str());
}
string RFIDTag::getUID()
{
	return UID;
}
void RFIDTag::setAID(string aID)
{
	strcpy(AID,aID.c_str());

}
string RFIDTag::getAID()
{
	return AID;
}
stringstream* RFIDTag::getAntID()
{
	return &strANT;
}

int RFIDTag::Getdoorstatus()
{
	return 0;//doorstatus;
}


 