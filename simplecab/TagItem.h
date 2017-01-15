#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include "Shelf.h"
using namespace std;
class Shelf;
class TagItem
{
private:
	string tagId;

	//Current Shelf Information
	int lastSeenTime;
	Shelf *lastSeenShelf;

	//Last shelf information
	bool lastOn;
	Shelf *lastShelf;

	//for knowing kinds of items. possible values would be: "door", "glass", "unknown", etc...
	string type;

	//See public constants below
	int status;
	
	//Large buffertime will stop the item from flickering between on and off, small time will decrease the time to realize an item no longer on the shelf.
	static const int bufferTime = 8;

public:
	TagItem(string tagId, string type);
	~TagItem(void);
	
	void update();
	void scanShelf(Shelf *shelf);
	Shelf *getCurrentShelf();
	string getTagId();
	string getType();
	int getStatus();

	static const int offShelf		= 0;//000
	static const int onShelf		= 1;//001
	static const int takenOffShelf	= 2;//010
	static const int putOnShelf		= 3;//011
	static const int changedShelf	= 4;//1xx  (>=4)
	//With this structure, all events are >=2
};