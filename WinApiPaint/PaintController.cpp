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
	drawer->Draw(hdc);
};

PaintManager::~PaintManager()
{

}