#pragma once
#include "Figure.h"

class PaintManager
{
private:
	bool isDrawing, isMoving;
	int currentCommand;
	Figure *drawer;
public:
	PaintManager();
	bool GetDrawingState();
	void SetDrawingState(bool state);
	bool GetMovingState();
	void SetMovingState(bool state);
	int GetCurrentCommand();
	void SetCurrentCommand(int command);
	void SetDrawer(Figure* drawer);
	Figure* GetDrawer();
	void SetDrawerCoords(int x1, int y1, int x2, int y2);
	void DrawFigure(HDC hdc);
	~PaintManager();
};