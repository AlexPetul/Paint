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
	void DrawFigure(HDC hdc);
	~PaintManager();
};