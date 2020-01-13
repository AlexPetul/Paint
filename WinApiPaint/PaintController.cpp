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

void PaintManager::SetMovingFigureIndex(int index)
{
	this->movingFigureIndex = index;
}

std::vector<DrawnFigure> PaintManager::GetPaintedFigures()
{
	return this->figures;
}

PaintManager::~PaintManager()
{

}