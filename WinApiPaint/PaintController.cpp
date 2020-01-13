#include "PaintController.h"

PaintManager::PaintManager()
{
	this->isDrawing = false;
	this->isMoving = false;
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

void PaintManager::SetDrawer(Figure* drawer)
{
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

PaintManager::~PaintManager()
{

}