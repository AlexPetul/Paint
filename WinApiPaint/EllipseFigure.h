#pragma once
#include "Figure.h"

class EllipseFigure : public Figure
{
public:
	EllipseFigure();
	void Draw(HDC hdc) override;
	void SetFigurePoints(int x1, int x2, int y1, int y2) override;
	RECT GetFigureRect() override;
	~EllipseFigure();
};