#include "EllipseFigure.h"

EllipseFigure::EllipseFigure()
{

}

void EllipseFigure::Draw(HDC hdc)
{
	Ellipse(hdc, x1, y1, x2, y2);
}

void EllipseFigure::SetFigurePoints(int x1, int y1, int x2, int y2)
{
	this->x1 = x2 > x1 ? x1 : x2;
	this->x2 = x2 > x1 ? x2 : x1;
	this->y1 = y2 > y1 ? y1 : y2;
	this->y2 = y2 > y1 ? y2 : y1;
}

RECT EllipseFigure::GetFigureRect()
{
	RECT rect;
	rect.left = x1;
	rect.top = y1;
	rect.right = x2;
	rect.bottom = y2;
	return rect;
}

EllipseFigure::~EllipseFigure()
{

}