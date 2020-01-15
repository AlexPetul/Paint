#pragma once
#include <Windows.h>

class Figure
{
protected:
	int x1, x2, y1, y2;
public:
	Figure();
	virtual void Draw(HDC hdc) = 0;
	virtual void SetFigurePoints(int x1, int y1, int x2, int y2) = 0;
	virtual RECT GetFigureRect() = 0;
	virtual ~Figure();
};