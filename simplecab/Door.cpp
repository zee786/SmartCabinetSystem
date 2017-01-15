
#include "Door.h"

using namespace std;

Door::Door(TagItem* tagItem)
{
	this->shelf = NULL;
	this->tagItem = tagItem;
}


Door::~Door(void)
{
}

bool Door::getClosed() {
	if (this->tagItem->getStatus() & 1 == 1) {
		if (this->shelf == NULL) {
			cout << "Shelf is NULL";
			return true;
		}
		else if (this->tagItem->getCurrentShelf() == NULL)
		{
			cout << "TagItem Shelf is NULL even thoug state says its on a shelf" << endl;
			false;
		}
		else
		{
			return true;
			
		}
	}
	else
		return false;
}

void Door::setShelf(Shelf * shelf)
{
	this->shelf = shelf;
}

int Door::getStatus() {
	return tagItem->getStatus();
}