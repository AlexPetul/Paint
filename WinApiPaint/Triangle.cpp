#include "Triangle.h"
#include <math.h>

Triangle::Triangle()
{

}

void Triangle::Draw(HDC hdc)
{
	POINT vertexes[3] = { {x1,y1}, { x1 + (x2-x1)/2, y2-5 }, { x2 - 5, y1 } };
	Polygon(hdc, vertexes, 3);
}

void Triangle::SetFigurePoints(int x1, int y1, int x2, int y2)
{
	this->x1 = x2 > x1 ? x1 : x2;
	this->x2 = x2 > x1 ? x2 : x1;
	this->y1 = y2 > y1 ? y1 : y2;
	this->y2 = y2 > y1 ? y2 : y1;
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