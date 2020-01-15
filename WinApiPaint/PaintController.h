#pragma once
#include <vector>
#include <map>
#include "Figure.h"

typedef struct figureHood
{
	RECT figureRect;
	POINT selfCenterPos;
	std::map<int, POINT> linkedFigures;
}FigureHood;

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
	POINT GetRectCenter(RECT rect);
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
	void InitConnectionTable(std::vector<FigureHood> &figureHood, std::vector<DrawnFigure> figures);
	void DrawConnections(HDC &memory, std::vector<FigureHood> &figureHood);
	~PaintManager();
};