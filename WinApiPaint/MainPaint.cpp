#pragma once
#include "MainPaint.h"
#include <string>
#include "PaintController.h"

PaintManager *paintManager = new PaintManager();
FileManager *fileManager = new FileManager();
Service *service;

POINT prevRelCoords, currRelCoords, temp;
HDC memory, secondMemory, hdc;
PAINTSTRUCT ps;
int movingFigureIndex;
int xOffset, yOffset;
RECT currentMovingFigure, clientRect;
std::vector<DrawnFigure> figures;
HBITMAP memoryBitmap, secondMemoryBitmap, tempBitMap;
bool isConnectionDrawn = false;

std::vector<FigureHood> figureHood;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	HWND _hwnd;
	MSG _msg;
	WNDCLASSEX _wc;
	HMENU commandMenu, sysMenu, popUpMenu;

	_wc.cbSize = sizeof(WNDCLASSEX);
	_wc.style = CS_DBLCLKS;
	_wc.lpfnWndProc = WndProc;
	_wc.cbClsExtra = 0;
	_wc.cbWndExtra = 0;
	_wc.hInstance = hInstance;
	_wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	_wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	_wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	_wc.lpszMenuName = NULL;
	_wc.lpszClassName = WND_CLASSNAME;
	_wc.hIconSm = _wc.hIcon;

	RegisterClassEx(&_wc);
	_hwnd = CreateWindow(WND_CLASSNAME, WND_NAME, WS_OVERLAPPEDWINDOW^WS_THICKFRAME,
		300, 300, WND_WIDTH, WND_HEIGHT, NULL, NULL, hInstance, NULL);
	ShowWindow(_hwnd, nCmdShow);
	UpdateWindow(_hwnd);

	sysMenu = GetSystemMenu(_hwnd, FALSE);
	DeleteMenu(sysMenu, SC_MAXIMIZE, MF_BYCOMMAND);
	DeleteMenu(sysMenu, SC_SIZE, MF_BYCOMMAND);

	commandMenu = CreateMenu();
	popUpMenu = CreatePopupMenu();
	AppendMenu(commandMenu, MFT_STRING | MF_POPUP, (UINT)popUpMenu, "Figures");
	AppendMenu(popUpMenu, MF_STRING, ID_CIRCLE, "Ellipse");
	AppendMenu(popUpMenu, MF_STRING, ID_RECTANGLE, "Rectangle");
	AppendMenu(popUpMenu, MF_STRING, ID_TRIANGLE, "Triangle");
	AppendMenu(commandMenu, MF_STRING, ID_CONNECTOR_LINE, "Connector line");
	AppendMenu(commandMenu, MF_STRING, ID_MOVE_OBJECTS, "Move");
	AppendMenu(commandMenu, MF_STRING, ID_SAVE_FILE, "Save");
	AppendMenu(commandMenu, MF_STRING, ID_OPEN_FILE, "Open");
	SetMenu(_hwnd, commandMenu);
	SetMenu(_hwnd, popUpMenu);
	service = new Service(popUpMenu);

	while (GetMessage(&_msg, NULL, 0, 0))
	{
		TranslateMessage(&_msg);
		DispatchMessage(&_msg);
	}
	return (int)_msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		service->CreateMainLayer(hwnd, &memory, &memoryBitmap, WND_WIDTH, WND_HEIGHT);
		service->CreateMainLayer(hwnd, &secondMemory, &secondMemoryBitmap, WND_WIDTH, WND_HEIGHT);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_CIRCLE:
			if (service->GetMenuItemState(ID_CIRCLE))
			{
				paintManager->SetCurrentCommand(CIRCLE_DRAW_COMMAND);
				paintManager->SetDrawer(new EllipseFigure());
				paintManager->SetToolId(ID_CIRCLE);
			}
			else
			{
				paintManager->SetCurrentCommand(NO_COMMAND);
			}
			break;
		case ID_TRIANGLE:
			if (service->GetMenuItemState(ID_TRIANGLE))
			{
				paintManager->SetCurrentCommand(TRIANGLE_DRAW_COMMAND);
				paintManager->SetDrawer(new Triangle());
				paintManager->SetToolId(ID_TRIANGLE);
			}
			else
			{
				paintManager->SetCurrentCommand(NO_COMMAND);
			}
			break;
		case ID_RECTANGLE:
			if (service->GetMenuItemState(ID_RECTANGLE))
			{
				paintManager->SetCurrentCommand(RECTANGLE_DRAW_COMMAND);
				paintManager->SetDrawer(new RectangleFigure());
				paintManager->SetToolId(ID_RECTANGLE);
				break;
			}
			else
			{
				paintManager->SetCurrentCommand(NO_COMMAND);
			}
		case ID_MOVE_OBJECTS:
			service->GetMenuItemState(ID_MOVE_OBJECTS);
			paintManager->SetCurrentCommand(MOVE_FIGURES_COMMAND);
			break;
		case ID_SAVE_FILE:
			if (paintManager->GetCurrentCommand() == CONNECTOR_LINE_COMMAND)
			{
				SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(ID_CONNECTOR_LINE, HIWORD(wParam)), lParam);
			}
			fileManager->SaveFile(memoryBitmap, hwnd);
			fileManager->SaveFiguresToFile(figures);
			break;
		case ID_CONNECTOR_LINE:
			if (isConnectionDrawn)
			{
				SelectObject(memory, CreatePen(PS_SOLID, 1, WHITE_PEN));
				paintManager->SetCurrentCommand(NO_COMMAND);
				isConnectionDrawn = false;
			}
			else
			{
				SelectObject(memory, CreatePen(PS_DASH, 1, GRAY_PEN));
				paintManager->SetCurrentCommand(CONNECTOR_LINE_COMMAND);
				isConnectionDrawn = true;
			}

			service->GetMenuItemState(ID_CONNECTOR_LINE);
			paintManager->InitConnectionTable(figureHood, figures);
			paintManager->DrawConnections(memory, figureHood);
			InvalidateRect(hwnd, NULL, FALSE);
			UpdateWindow(hwnd);
			break;
		case ID_OPEN_FILE:
			tempBitMap = fileManager->LoadBitmapFromFile();
			if (tempBitMap == NULL)
			{
				MessageBox(hwnd, TEXT("Error opening custom.bmp file."), TEXT("File Manager"), NULL);
			}
			else
			{
				if (fileManager->LoadFigures(figures) == false)
				{
					MessageBox(hwnd, TEXT("Error opening figures.dat file."), TEXT("File Manager"), NULL);
				}
				else
				{
					memoryBitmap = (HBITMAP)CopyImage(tempBitMap, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
					secondMemoryBitmap = (HBITMAP)CopyImage(memoryBitmap, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
					GetClientRect(hwnd, &clientRect);
					FillRect(hdc, &clientRect, (HBRUSH)(COLOR_WINDOW + 1));
					service->CreateAfterLoadLayer(hwnd, hdc, memory, memoryBitmap);
					service->CreateAfterLoadLayer(hwnd, hdc, secondMemory, secondMemoryBitmap);
					isConnectionDrawn = false;
					paintManager->SetMovingState(false);
					paintManager->SetDrawingState(false);
					InvalidateRect(hwnd, NULL, FALSE);
					UpdateWindow(hwnd);
				}
			}
 			break;
		}
		break;
	case WM_LBUTTONDOWN:
		if ((paintManager->GetCurrentCommand() >= 1) && (paintManager->GetCurrentCommand() <= 3))
		{
			BitBlt(secondMemory, 0, 0, WND_WIDTH, WND_HEIGHT, memory, 0, 0, SRCCOPY);
			SelectObject(memory, CreatePen(PS_SOLID, 1, BLACK_PEN));
			prevRelCoords = service->GetMouseCoords(hwnd);
			if (paintManager->GetCurrentCommand() != 0)
			{
				paintManager->SetDrawingState(true);
			}
		}
		else if (paintManager->GetCurrentCommand() == MOVE_FIGURES_COMMAND)
		{
			currRelCoords = service->GetMouseCoords(hwnd);
			for (int index = 0; index < figures.size(); index++)
			{
				if (PtInRect(&(figures[index].figureRect), currRelCoords))
				{
					if (figures[index].toolId == ID_CIRCLE)
					{
						paintManager->SetDrawer(new EllipseFigure());
					}
					else if (figures[index].toolId == ID_RECTANGLE)
					{
						paintManager->SetDrawer(new RectangleFigure());
					}
					else if (figures[index].toolId == ID_TRIANGLE)
					{
						paintManager->SetDrawer(new Triangle());
					}
					BitBlt(secondMemory, 0, 0, WND_WIDTH, WND_HEIGHT, memory, 0, 0, SRCCOPY);
					BitBlt(memory, 0, 0, WND_WIDTH, WND_HEIGHT, secondMemory, 0, 0, SRCCOPY);
					FillRect(memory, &(figures[index].figureRect), (HBRUSH)(COLOR_WINDOW + 1));
					yOffset = currRelCoords.y - figures[index].figureRect.top;
					xOffset = currRelCoords.x - figures[index].figureRect.left;
					currentMovingFigure = figures[index].figureRect;
					movingFigureIndex = index;
					paintManager->SetMovingState(true);

					if (isConnectionDrawn)
					{
						SelectObject(memory, CreatePen(PS_SOLID, 1, WHITE_PEN));
						for (std::map<int, POINT>::iterator it = figureHood[movingFigureIndex].linkedFigures.begin();
							it != figureHood[movingFigureIndex].linkedFigures.end(); it++)
						{
							MoveToEx(memory, it->second.x, it->second.y, &temp);
							int nextCenterX = figureHood[movingFigureIndex].figureRect.left + (figureHood[movingFigureIndex].figureRect.right - figureHood[movingFigureIndex].figureRect.left) / 2;
							int nextCenterY = figureHood[movingFigureIndex].figureRect.top + (figureHood[movingFigureIndex].figureRect.bottom - figureHood[movingFigureIndex].figureRect.top) / 2;
							LineTo(memory, nextCenterX, nextCenterY);
						}
						SelectObject(memory, CreatePen(PS_SOLID, 1, BLACK_PEN));
					}

					BitBlt(secondMemory, 0, 0, WND_WIDTH, WND_HEIGHT, memory, 0, 0, SRCCOPY);
					InvalidateRect(hwnd, 0, FALSE);
					UpdateWindow(hwnd);
					break;
				}
			}
		}
		break;
	case WM_LBUTTONUP:
		if (paintManager->GetDrawingState())
		{
			paintManager->SetDrawingState(false);
			currRelCoords = service->GetMouseCoords(hwnd);
			paintManager->SetDrawerCoords(prevRelCoords.x, prevRelCoords.y, currRelCoords.x, currRelCoords.y);
			paintManager->DrawFigure(memory);
			figures.push_back({ paintManager->GetDrawer()->GetFigureRect(), paintManager->GetUsedToolId() });

			if (isConnectionDrawn)
			{
				SelectObject(memory, CreatePen(PS_DASH, 1, GRAY_PEN));
				paintManager->InitConnectionTable(figureHood, figures);
				paintManager->DrawConnections(memory, figureHood);
			}

			InvalidateRect(hwnd, NULL, FALSE);
			UpdateWindow(hwnd);
		}
		else if (paintManager->GetMovingState())
		{
			paintManager->SetMovingState(false);
			currRelCoords = service->GetMouseCoords(hwnd);
			int figureWidth = currentMovingFigure.right - currentMovingFigure.left;
			int figureHeight = currentMovingFigure.bottom - currentMovingFigure.top;
			paintManager->SetDrawerCoords(currRelCoords.x - xOffset, currRelCoords.y - yOffset, currRelCoords.x + figureWidth - xOffset, currRelCoords.y + figureHeight - yOffset);
			paintManager->DrawFigure(memory);
			figures[movingFigureIndex] = { paintManager->GetDrawer()->GetFigureRect(), figures[movingFigureIndex].toolId };
			

			if (!figureHood.empty())
			{
				paintManager->InitConnectionTable(figureHood, figures);
			}

			if (isConnectionDrawn)
			{
				SelectObject(memory, CreatePen(PS_DASH, 1, GRAY_PEN));
				for (int index = 0; index < figureHood.size(); index++) 
				{
					int nextCenterX = figures[movingFigureIndex].figureRect.left + (figures[movingFigureIndex].figureRect.right - figures[movingFigureIndex].figureRect.left) / 2;
					int nextCenterY = figures[movingFigureIndex].figureRect.top + (figures[movingFigureIndex].figureRect.bottom - figures[movingFigureIndex].figureRect.top) / 2;

					if (index != movingFigureIndex)
					{
						for (std::map<int, POINT>::iterator it = figureHood[index].linkedFigures.begin();
							it != figureHood[index].linkedFigures.end(); it++)
						{
							if ((int)it->first == movingFigureIndex)
							{
								figureHood[index].linkedFigures[movingFigureIndex] = { nextCenterX, nextCenterY };
							}
						}
					}
					else
					{
						for (std::map<int, POINT>::iterator it = figureHood[index].linkedFigures.begin();
							it != figureHood[index].linkedFigures.end(); it++)
						{
							MoveToEx(memory, it->second.x, it->second.y, &temp);
							LineTo(memory, nextCenterX, nextCenterY);
						}
					}
				}
				SelectObject(memory, CreatePen(PS_SOLID, 1, BLACK_PEN));
			}

			BitBlt(secondMemory, 0, 0, WND_WIDTH, WND_HEIGHT, memory, 0, 0, SRCCOPY);
			BitBlt(memory, 0, 0, WND_WIDTH, WND_HEIGHT, secondMemory, 0, 0, SRCCOPY);


			InvalidateRect(hwnd, NULL, FALSE);
			UpdateWindow(hwnd);
		}
		break;
	case WM_MOUSEMOVE:
		if (paintManager->GetDrawingState())
		{
			BitBlt(memory, 0, 0, WND_WIDTH, WND_HEIGHT, secondMemory, 0, 0, SRCCOPY);
			currRelCoords = service->GetMouseCoords(hwnd);
			if (paintManager->GetCurrentCommand() >= 1)
			{
				paintManager->SetDrawerCoords(prevRelCoords.x, prevRelCoords.y, currRelCoords.x, currRelCoords.y);
				paintManager->DrawFigure(memory);
				InvalidateRect(hwnd, 0, FALSE);
				UpdateWindow(hwnd);
			}
		}
		else if ((paintManager->GetCurrentCommand() == MOVE_FIGURES_COMMAND) && (paintManager->GetMovingState() == true))
		{
			BitBlt(memory, 0, 0, WND_WIDTH, WND_HEIGHT, secondMemory, 0, 0, SRCCOPY);
			BitBlt(secondMemory, 0, 0, WND_WIDTH, WND_HEIGHT, memory, 0, 0, SRCCOPY);
			FillRect(memory, &currentMovingFigure, (HBRUSH)(COLOR_WINDOW + 1));
			currRelCoords = service->GetMouseCoords(hwnd);
			int figureWidth = currentMovingFigure.right - currentMovingFigure.left;
			int figureHeight = currentMovingFigure.bottom - currentMovingFigure.top;
			paintManager->SetDrawerCoords(currRelCoords.x - xOffset, currRelCoords.y - yOffset, currRelCoords.x + figureWidth - xOffset, currRelCoords.y + figureHeight- yOffset);
			paintManager->DrawFigure(memory);

			if (isConnectionDrawn)
			{
				SelectObject(memory, CreatePen(PS_DASH, 1, GRAY_PEN));
				for (std::map<int, POINT>::iterator it = figureHood[movingFigureIndex].linkedFigures.begin();
					it != figureHood[movingFigureIndex].linkedFigures.end(); it++)
				{
					MoveToEx(memory, it->second.x, it->second.y, &temp);
					int nextCenterX = paintManager->GetDrawer()->GetFigureRect().left + (paintManager->GetDrawer()->GetFigureRect().right - paintManager->GetDrawer()->GetFigureRect().left) / 2;
					int nextCenterY = paintManager->GetDrawer()->GetFigureRect().top + (paintManager->GetDrawer()->GetFigureRect().bottom - paintManager->GetDrawer()->GetFigureRect().top) / 2;
					LineTo(memory, nextCenterX, nextCenterY);
				}
				SelectObject(memory, CreatePen(PS_SOLID, 1, BLACK_PEN));
			}
			
			InvalidateRect(hwnd, 0, FALSE);
			UpdateWindow(hwnd);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		BitBlt(hdc, 0, 0, WND_WIDTH, WND_HEIGHT, memory, 0, 0, SRCCOPY);
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		delete service;
		delete fileManager;
		delete paintManager;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
}