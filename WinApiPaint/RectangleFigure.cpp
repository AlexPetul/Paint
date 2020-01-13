#include "RectangleFigure.h"

RectangleFigure::RectangleFigure()
{

}

void RectangleFigure::Draw(HDC hdc)
{
	Rectangle(hdc, x1, y1, x2, y2);
}

void RectangleFigure::SetFigurePoints(int x1, int y1, int x2, int y2)
{
	this->x1 = x1;
	this->x2 = x2;
	this->y1 = y1;
	this->y2 = y2;
}

RECT RectangleFigure::GetFigureRect()
{
	RECT rect;
	rect.left = x1;
	rect.top = y1;
	rect.right = x2;
	rect.bottom = y2;
	return rect;
}

RectangleFigure::~RectangleFigure()
{

}