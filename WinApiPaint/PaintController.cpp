#include "PaintController.h"

PaintManager::PaintManager()
{
	this->isDrawing = false;
	this->isMoving = false;
	this->toolUsed = 0;
	this->currentCommand = 0;
}

bool PaintManager::GetDrawingState()
{
	return this->isDrawing;
}

void PaintManager::SetDrawingState(bool state)
{
	this->isDrawing = state;
}

bool PaintManager::GetMovingState()
{
	return this->isMoving;
}

void PaintManager::SetMovingState(bool state)
{
	this->isMoving = state;
}

int PaintManager::GetCurrentCommand()
{
	return this->currentCommand;
}

void PaintManager::SetCurrentCommand(int command)
{
	this->currentCommand = command;
}

int PaintManager::GetUsedToolId()
{
	return this->toolUsed;
}

void PaintManager::SetToolId(int currToolId)
{
	this->toolUsed = currToolId;
}

void PaintManager::SetDrawer(Figure* drawer)
{
	if (this->drawer != NULL)
	{
		delete this->drawer;
	}
	this->drawer = drawer;
}

void PaintManager::DrawFigure(HDC hdc)
{
	this->drawer->Draw(hdc);
};

void PaintManager::SetDrawerCoords(int x1, int y1, int x2, int y2)
{
	this->drawer->SetFigurePoints(x1, y1, x2, y2);
}

Figure* PaintManager::GetDrawer()
{
	return this->drawer;
}

void PaintManager::AddFigure()
{
	this->figures.push_back({ this->GetDrawer()->GetFigureRect(), this->toolUsed });
}

void PaintManager::EraseFigureByIndex()
{
	this->figures.erase(this->figures.begin() + movingFigureIndex);
}

POINT PaintManager::GetRectCenter(RECT rect)
{
	int centerX = rect.left + (rect.right - rect.left) / 2;
	int centerY = rect.top + (rect.bottom - rect.top) / 2;
	return { centerX, centerY };
}

void PaintManager::InitConnectionTable(std::vector<FigureHood> &figureHood, std::vector<DrawnFigure> figures)
{
	figureHood.clear();
	for (int index = 0; index < figures.size(); index++)
	{
		std::map<int, POINT> newPair;
		POINT selfCenterPoint = { 0,0 };
		figureHood.push_back({ figures[index].figureRect, selfCenterPoint, newPair });
	}
	for (int subIndex = 0; subIndex < figureHood.size(); subIndex++)
	{
		figureHood[subIndex].selfCenterPos = GetRectCenter(figureHood[subIndex].figureRect);
		for (int index = 0; index < figureHood.size(); index++)
		{
			if (index != subIndex)
			{
				if (figureHood[index].linkedFigures.find(subIndex) == figureHood[index].linkedFigures.end())
				{
					figureHood[index].linkedFigures[subIndex] = GetRectCenter(figureHood[subIndex].figureRect);
				}
			}
		}
	}
}

void PaintManager::DrawConnections(HDC &memory, std::vector<FigureHood> &figureHood)
{
	POINT tempPoint;
	for (int index = 0; index < figureHood.size(); index++)
	{
		for (std::map<int, POINT>::iterator it = figureHood[index].linkedFigures.begin();
			it != figureHood[index].linkedFigures.end(); it++)
		{
			MoveToEx(memory, figureHood[index].selfCenterPos.x, figureHood[index].selfCenterPos.y, &tempPoint);
			LineTo(memory, (int)it->second.x, (int)it->second.y);
		}
	}
	SelectObject(memory, CreatePen(PS_SOLID, 1, BLACK_PEN));
}

PaintManager::~PaintManager()
{

}