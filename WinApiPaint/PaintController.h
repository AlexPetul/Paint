#pragma once
#include <vector>
#include "Figure.h"

typedef struct drawnFigure
{
	RECT figureRect;
	int toolId;
}DrawnFigure;

class PaintManager
{
private:
	bool isDrawing, isMoving;
	int currentCommand;
	int toolUsed;
	int movingFigureIndex;
	Figure *drawer;
	std::vector<DrawnFigure> figures;
public:
	PaintManager();
	bool GetDrawingState();
	void SetDrawingState(bool state);
	bool GetMovingState();
	void SetMovingState(bool state);
	int GetCurrentCommand();
	void SetCurrentCommand(int command);
	int GetUsedToolId();
	void SetToolId(int currToolId);
	void SetDrawer(Figure* drawer);
	Figure* GetDrawer();
	void SetDrawerCoords(int x1, int y1, int x2, int y2);
	void DrawFigure(HDC hdc);
	void AddFigure();
	void EraseFigureByIndex();
	void SetMovingFigureIndex(int index);
	std::vector<DrawnFigure> GetPaintedFigures();
	~PaintManager();
};