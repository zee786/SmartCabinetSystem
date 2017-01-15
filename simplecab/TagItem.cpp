
#include "TagItem.h"
using namespace std;

TagItem::TagItem(string tagId, string type)
{
	this->tagId = tagId;
	this->type = type;
	this->lastSeenShelf = NULL;
	this->lastSeenTime = 1000;
	this->status = offShelf;

	cout << "new TagItem(" << tagId << ", " << type << ");" << endl;
}


TagItem::~TagItem(void)
{
}

void TagItem::update() {
	if (this->lastSeenTime < 1000)
		this->lastSeenTime++;
	
	bool newOn = this->lastSeenTime < bufferTime;
	//cout << "updating tag " << tagId << " " << type << " ";
	
	status = 0;
	if (newOn)
		status = 1;
	if (newOn != lastOn)
		status |= 2;
	if (lastShelf != lastSeenShelf)
		status |= 4;
	
	//if (lastSeenShelf != NULL)
	//	cout << " " << lastSeenShelf->getAntennaId();
	//cout << endl;

	lastOn = newOn;
	lastShelf = lastSeenShelf;
}

void TagItem::scanShelf(Shelf *shelf)
{
	this->lastSeenShelf = shelf;
	this->lastSeenTime = 0;
}

Shelf* TagItem::getCurrentShelf()
{
	if (this->lastSeenTime < bufferTime)
		return this->lastSeenShelf;
	else
		return NULL;
}

string TagItem::getTagId() {
	return tagId;
}

string TagItem::getType() {
	return this->type;
}

int TagItem::getStatus() {
	return status;
}