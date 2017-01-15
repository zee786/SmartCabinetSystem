#include "StdAfx.h"
#include "Shelf.h"


Shelf::Shelf(int id, string antennaID, Cupboard *cupboard, Door *door)
{
	cout << "new Shelf(" << id << ", " << antennaID << ", " << cupboard << ", " << door << ");" << endl;
	this->id = id;
	this->antennaID = antennaID;
	this->cupboard = cupboard;
	this->door = door;
}


Shelf::~Shelf(void)
{
}

void Shelf::setCenter(cv::Point2f center)
{
	this->center = center;
}

void Shelf::setSize(cv::Size2f size)
{
	this->size = size;
}

string Shelf::getAntennaId() {
	return this->antennaID;
}

Door * Shelf::getDoor() {
	return door;
}

Cupboard * Shelf::getCupboard()
{
	return cupboard;
}

Point2f Shelf::getCenter() {
	return center;
}

Size2f Shelf::getSize() {
	return size;
}

int Shelf::getId() {
	return id;
}