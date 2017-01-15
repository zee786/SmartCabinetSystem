#pragma once
#include <iostream>
#include <string>
#include "TagItem.h"
#include "Shelf.h"
using namespace std;
class Shelf;
class TagItem;

class Door
{
private:
	Shelf * shelf;
	TagItem *tagItem;

public:
	Door(TagItem*);
	~Door(void);
	bool getClosed();
	void setShelf(Shelf * shelf);
	int getStatus();
};

