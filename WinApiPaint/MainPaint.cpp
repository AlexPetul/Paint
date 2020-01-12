#pragma once
#include "MainPaint.h"
#include <string>
#include <vector>

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	HWND _hwnd;
	MSG _msg;
	WNDCLASSEX _wc;
	HMENU commandMenu;

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
		WND_START_X, WND_START_Y, WND_WIDTH, WND_HEIGHT, NULL, NULL, hInstance, NULL);
	ShowWindow(_hwnd, nCmdShow);
	UpdateWindow(_hwnd);

	commandMenu = CreateMenu();
	AppendMenu(commandMenu, MF_STRING, ID_CIRCLE, "Ellipse");
	AppendMenu(commandMenu, MF_STRING, ID_RECTANGLE, "Rectangle");
	AppendMenu(commandMenu, MF_STRING, ID_TRIANGLE, "Triangle");
	AppendMenu(commandMenu, MF_STRING, ID_CONNECTOR_LINE, "Connector line");
	AppendMenu(commandMenu, MF_STRING, ID_MOVE_OBJECTS, "Move");
	AppendMenu(commandMenu, MF_STRING, ID_SAVE_FILE, "Save");
	AppendMenu(commandMenu, MF_STRING, ID_OPEN_FILE, "Open");
	SetMenu(_hwnd, commandMenu);

	while (GetMessage(&_msg, NULL, 0, 0))
	{
		TranslateMessage(&_msg);
		DispatchMessage(&_msg);
	}
	return (int)_msg.wParam;
}

int currentCommand = 0, movingFigureIndex;
HDC memory, secondMemory, hdc;
POINT prevMouseCoords, currMouseCoords;
Figure *ellipse;
PAINTSTRUCT ps;
bool isDrawing = false, isMoving = false;
HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
std::string st;
POINT prevRelCoords, currRelCoords;
RECT windowPosCoords, currentMovingFigure;
std::vector<RECT> figures;


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		HBITMAP memoryBitmap, secondMemoryBitmap;
		CreateLayer(hwnd, &memory, &memoryBitmap, WND_WIDTH, WND_HEIGHT);
		CreateLayer(hwnd, &secondMemory, &secondMemoryBitmap, WND_WIDTH, WND_HEIGHT);
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == ID_CIRCLE)
		{
			currentCommand = CIRCLE_DRAW_COMMAND;
			ellipse = new EllipseFigure();
		}
		else if (LOWORD(wParam) == ID_MOVE_OBJECTS)
		{
			currentCommand = MOVE_FIGURES_COMMAND;
		}
		break;
	case WM_LBUTTONDOWN:
		if (currentCommand == CIRCLE_DRAW_COMMAND)
		{
			BitBlt(secondMemory, 0, 0, WND_WIDTH, WND_HEIGHT, memory, 0, 0, SRCCOPY);
			SelectObject(memory, pen);
			prevRelCoords = GetMouseCoords(hwnd);
			if (currentCommand != 0)
			{
				isDrawing = true;
			}
		}
		else if (currentCommand == MOVE_FIGURES_COMMAND)
		{
			currRelCoords = GetMouseCoords(hwnd);
			for (int index = 0; index < figures.size(); index++)
			{
				if (PtInRect(&(figures[index]), currRelCoords))
				{
					BitBlt(secondMemory, 0, 0, WND_WIDTH, WND_HEIGHT, memory, 0, 0, SRCCOPY);
					BitBlt(memory, 0, 0, WND_WIDTH, WND_HEIGHT, secondMemory, 0, 0, SRCCOPY);
					FillRect(memory, &(figures[index]), (HBRUSH)(COLOR_WINDOW + 1));
					InvalidateRect(hwnd, 0, FALSE);
					UpdateWindow(hwnd);
					currentMovingFigure = figures[index];
					movingFigureIndex = index;
					isMoving = true;
					break;
				}
			}
		}
		break;
	case WM_LBUTTONUP:
		if (isDrawing)
		{
			isDrawing = false;
			currRelCoords = GetMouseCoords(hwnd);
			ellipse->SetFigurePoints(prevRelCoords.x, prevRelCoords.y, currRelCoords.x, currRelCoords.y);
			ellipse->Draw(memory);
			figures.push_back(ellipse->GetFigureRect());
			InvalidateRect(hwnd, NULL, FALSE);
			UpdateWindow(hwnd);
		}
		else if (isMoving)
		{
			isMoving = false;
			currRelCoords = GetMouseCoords(hwnd);
			int ellipseWidth = currentMovingFigure.right - currentMovingFigure.left;
			int ellipseHeight = currentMovingFigure.bottom - currentMovingFigure.top;
			ellipse->SetFigurePoints(currRelCoords.x, currRelCoords.y,
				currRelCoords.x + ellipseWidth, currRelCoords.y + ellipseHeight);
			ellipse->Draw(memory);
			figures.erase(figures.begin() + movingFigureIndex);
			figures.push_back(ellipse->GetFigureRect());
			InvalidateRect(hwnd, NULL, FALSE);
			UpdateWindow(hwnd);
		}
		break;
	case WM_MOUSEMOVE:
		if (isDrawing)
		{
			BitBlt(memory, 0, 0, WND_WIDTH, WND_HEIGHT, secondMemory, 0, 0, SRCCOPY);
			currRelCoords = GetMouseCoords(hwnd);
			if (currentCommand == CIRCLE_DRAW_COMMAND)
			{
				ellipse->SetFigurePoints(prevRelCoords.x, prevRelCoords.y, currRelCoords.x, currRelCoords.y);
				ellipse->Draw(memory);
				InvalidateRect(hwnd, 0, FALSE);
				UpdateWindow(hwnd);
			}
		}
		else if ((currentCommand == MOVE_FIGURES_COMMAND) && (isMoving == true))
		{
			BitBlt(memory, 0, 0, WND_WIDTH, WND_HEIGHT, secondMemory, 0, 0, SRCCOPY);
			FillRect(memory, &currentMovingFigure, (HBRUSH)(COLOR_WINDOW + 1));
			currRelCoords = GetMouseCoords(hwnd);
			int ellipseWidth = currentMovingFigure.right - currentMovingFigure.left;
			int ellipseHeight = currentMovingFigure.bottom - currentMovingFigure.top;
			ellipse->SetFigurePoints(currRelCoords.x, currRelCoords.y,
				currRelCoords.x + ellipseWidth, currRelCoords.y + ellipseHeight);
			ellipse->Draw(memory);
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
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
}

POINT GetMouseCoords(HWND hwnd)
{
	POINT absCursorPos, resultCoords;
	RECT windowPosCoords;
	GetCursorPos(&absCursorPos);
	GetWindowRect(hwnd, &windowPosCoords);
	resultCoords.x = absCursorPos.x - windowPosCoords.left;
	resultCoords.y = absCursorPos.y - windowPosCoords.top;
	return resultCoords;
}

void CreateLayer(HWND hWnd, HDC* newDC, HBITMAP* newBmp, int width, int hight)
{
	HDC hdc = GetDC(hWnd);
	*newDC = CreateCompatibleDC(hdc);
	*newBmp = CreateCompatibleBitmap(hdc, width, hight);
	SelectObject(*newDC, *newBmp);
	HPEN printPen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
	SelectObject(*newDC, printPen);
	Rectangle(*newDC, -1, -1, width + 1, hight + 1);
	ReleaseDC(hWnd, hdc);
}