#include "Triangle.h"

Triangle::Triangle()
{

}

void Triangle::Draw(HDC hdc)
{
	int side = abs(x2 - x1);
	POINT vertexes[3] = { {x1,y1}, { x1 + (0.5*side), y1 - (0.87*side) }, { x1 + side, y1 } };
	Polygon(hdc, vertexes, 3);
}

void Triangle::SetFigurePoints(int x1, int y1, int x2, int y2)
{
	this->x1 = x1;
	this->x2 = x2;
	this->y1 = y1;
	this->y2 = y2;
}

RECT Triangle::GetFigureRect()
{
	RECT rect;
	rect.left = x1;
	rect.top = y1;
	rect.right = x2;
	rect.bottom = y2;
	return rect;
}

Triangle::~Triangle()
{

}