#pragma once
#include "MainPaint.h"
#include <string>
#include <vector>
#include "PaintController.h"

HMENU popUpMenu;
PaintManager *paintManager = new PaintManager();
Figure *ellipse, *triangle;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	HWND _hwnd;
	MSG _msg;
	WNDCLASSEX _wc;
	HMENU commandMenu, sysMenu;

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

	while (GetMessage(&_msg, NULL, 0, 0))
	{
		TranslateMessage(&_msg);
		DispatchMessage(&_msg);
	}
	return (int)_msg.wParam;
}

int movingFigureIndex;
HDC memory, secondMemory, hdc, hMemDc;
POINT prevMouseCoords, currMouseCoords;
PAINTSTRUCT ps;
HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
HPEN printPen;
POINT prevRelCoords, currRelCoords;
RECT windowPosCoords, currentMovingFigure, clientRect;
std::vector<RECT> figures;
HBITMAP memoryBitmap, secondMemoryBitmap, firstTargetBmp, SecondTargetBmp;
MENUITEMINFO menuInfo = { sizeof(MENUITEMINFO) };

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	menuInfo.fMask = MIIM_STATE;
	switch (message)
	{
	case WM_CREATE:
		CreateLayer(hwnd, &memory, &memoryBitmap, WND_WIDTH, WND_HEIGHT);
		CreateLayer(hwnd, &secondMemory, &secondMemoryBitmap, WND_WIDTH, WND_HEIGHT);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_CIRCLE:
			GetMenuItemInfo(popUpMenu, ID_CIRCLE, FALSE, &menuInfo);
			menuInfo.fState ^= MFS_CHECKED;
			SetMenuItemInfo(popUpMenu, ID_CIRCLE, FALSE, &menuInfo);
			paintManager->SetCurrentCommand(CIRCLE_DRAW_COMMAND);
			ellipse = new EllipseFigure();
			break;
		case ID_TRIANGLE:
			GetMenuItemInfo(popUpMenu, ID_TRIANGLE, FALSE, &menuInfo);
			menuInfo.fState ^= MFS_CHECKED;
			SetMenuItemInfo(popUpMenu, ID_TRIANGLE, FALSE, &menuInfo);
			paintManager->SetCurrentCommand(TRIANGLE_DRAW_COMMAND);
			triangle = new Triangle();
			break;
		case ID_RECTANGLE:
			GetMenuItemInfo(popUpMenu, ID_TRIANGLE, FALSE, &menuInfo);
			menuInfo.fState ^= MFS_CHECKED;
			SetMenuItemInfo(popUpMenu, ID_TRIANGLE, FALSE, &menuInfo);
			break;
		case ID_MOVE_OBJECTS:
			paintManager->SetCurrentCommand(MOVE_FIGURES_COMMAND);
			break;
		case ID_SAVE_FILE:
			SaveFile(secondMemoryBitmap);
			break;
		case ID_OPEN_FILE:
			memoryBitmap = LoadBitmapFromFile();
			secondMemoryBitmap = LoadBitmapFromFile();

			GetClientRect(hwnd, &clientRect);
			FillRect(hdc, &clientRect, (HBRUSH)(COLOR_WINDOW + 1));
			figures.clear();

			hdc = GetDC(hwnd);
			memory = CreateCompatibleDC(hdc);
			SelectObject(memory, memoryBitmap);
			ReleaseDC(hwnd, hdc);

			hdc = GetDC(hwnd);
			secondMemory = CreateCompatibleDC(hdc);
			SelectObject(secondMemory, secondMemoryBitmap);
			ReleaseDC(hwnd, hdc);

			InvalidateRect(hwnd, NULL, FALSE);
			UpdateWindow(hwnd);

 			break;
		}
		break;
	case WM_LBUTTONDOWN:
		if ((paintManager->GetCurrentCommand() == CIRCLE_DRAW_COMMAND) || (paintManager->GetCurrentCommand() == TRIANGLE_DRAW_COMMAND))
		{
			BitBlt(secondMemory, 0, 0, WND_WIDTH, WND_HEIGHT, memory, 0, 0, SRCCOPY);
			SelectObject(memory, pen);
			prevRelCoords = GetMouseCoords(hwnd);
			if (paintManager->GetCurrentCommand() != 0)
			{
				paintManager->SetDrawingState(true);
			}
		}
		else if (paintManager->GetCurrentCommand() == MOVE_FIGURES_COMMAND)
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
					paintManager->SetMovingState(true);
					break;
				}
			}
		}
		break;
	case WM_LBUTTONUP:
		if (paintManager->GetDrawingState())
		{
			paintManager->SetDrawingState(false);
			currRelCoords = GetMouseCoords(hwnd);
			ellipse->SetFigurePoints(prevRelCoords.x, prevRelCoords.y, currRelCoords.x, currRelCoords.y);
			ellipse->Draw(memory);
			figures.push_back(ellipse->GetFigureRect());
			InvalidateRect(hwnd, NULL, FALSE);
			UpdateWindow(hwnd);
		}
		else if (paintManager->GetMovingState())
		{
			paintManager->SetMovingState(false);
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
		if (paintManager->GetDrawingState())
		{
			BitBlt(memory, 0, 0, WND_WIDTH, WND_HEIGHT, secondMemory, 0, 0, SRCCOPY);
			currRelCoords = GetMouseCoords(hwnd);
			if (paintManager->GetCurrentCommand() == CIRCLE_DRAW_COMMAND)
			{
				triangle->SetFigurePoints(prevRelCoords.x, prevRelCoords.y, currRelCoords.x, currRelCoords.y);
				triangle->Draw(memory);
				//ellipse->SetFigurePoints(prevRelCoords.x, prevRelCoords.y, currRelCoords.x, currRelCoords.y);
				//ellipse->Draw(memory);
				InvalidateRect(hwnd, 0, FALSE);
				UpdateWindow(hwnd);
			}
		}
		else if ((paintManager->GetCurrentCommand() == MOVE_FIGURES_COMMAND) && (paintManager->GetMovingState() == true))
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
		delete ellipse;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
}

HBITMAP LoadBitmapFromFile()
{
	HANDLE hFile;
	BITMAPFILEHEADER bf;
	KHMZ_BITMAPINFOEX bmi;
	DWORD cb, cbImage;
	LPVOID pvBits1, pvBits2;
	HDC hDC;
	HBITMAP hbm;

	hbm = (HBITMAP)LoadImage(NULL, "custom.bmp", IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hbm)
		return hbm;

	hFile = CreateFile("custom.bmp", GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return NULL;
	}

	if (!ReadFile(hFile, &bf, sizeof(BITMAPFILEHEADER), &cb, NULL))
	{
		CloseHandle(NULL);
		return NULL;
	}

	pvBits1 = NULL;
	if (bf.bfType == 0x4D42 && bf.bfReserved1 == 0 && bf.bfReserved2 == 0 &&
		bf.bfSize > bf.bfOffBits && bf.bfOffBits > sizeof(BITMAPFILEHEADER) &&
		bf.bfOffBits <= sizeof(BITMAPFILEHEADER) + sizeof(KHMZ_BITMAPINFOEX))
	{
		cbImage = bf.bfSize - bf.bfOffBits;
		pvBits1 = HeapAlloc(GetProcessHeap(), 0, cbImage);
		if (pvBits1)
		{
			if (!ReadFile(hFile, &bmi, bf.bfOffBits -
				sizeof(BITMAPFILEHEADER), &cb, NULL) ||
				!ReadFile(hFile, pvBits1, cbImage, &cb, NULL))
			{
				HeapFree(GetProcessHeap(), 0, pvBits1);
				pvBits1 = NULL;
			}
		}
	}
	CloseHandle(hFile);
	if (pvBits1 == NULL)
	{
		return NULL;
	}
	hbm = CreateDIBSection(NULL, (BITMAPINFO*)&bmi, DIB_RGB_COLORS,
		&pvBits2, NULL, 0);
	if (hbm)
	{
		hDC = CreateCompatibleDC(NULL);
		if (!SetDIBits(hDC, hbm, 0, (UINT)labs(bmi.bmiHeader.biHeight), pvBits1, (BITMAPINFO*)&bmi, DIB_RGB_COLORS))
		{
			DeleteObject(hbm);
			hbm = NULL;
		}
		DeleteDC(hDC);
	}
	HeapFree(GetProcessHeap(), 0, pvBits1);
	return hbm;
}


void SaveFile(HBITMAP bmpToSave)
{
	BOOL fOK;
	KHMZ_BITMAPINFOEX bmi;
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER *bmpInfoHdr;
	BITMAP bm;
	DWORD cb, cbColors;
	HDC hDC;
	HANDLE hFile;
	LPVOID pBits;

	GetObject(bmpToSave, sizeof(BITMAP), &bm);
	bmpInfoHdr = &bmi.bmiHeader;
	ZeroMemory(bmpInfoHdr, sizeof(BITMAPINFOHEADER));
	bmpInfoHdr->biSize = sizeof(BITMAPINFOHEADER);
	bmpInfoHdr->biWidth = bm.bmWidth;
	bmpInfoHdr->biHeight = bm.bmHeight;
	bmpInfoHdr->biPlanes = 1;
	bmpInfoHdr->biBitCount = bm.bmBitsPixel;
	bmpInfoHdr->biCompression = BI_RGB;
	bmpInfoHdr->biSizeImage = (DWORD)(bm.bmWidthBytes * bm.bmHeight);

	if (bm.bmBitsPixel <= 8)
		cbColors = (DWORD)((1ULL << bm.bmBitsPixel) * sizeof(RGBQUAD));
	else
		cbColors = 0;

	bf.bfType = 0x4D42;
	bf.bfReserved1 = 0;
	bf.bfReserved2 = 0;
	cb = sizeof(BITMAPFILEHEADER) + bmpInfoHdr->biSize + cbColors;
	bf.bfOffBits = cb;
	bf.bfSize = cb + bmpInfoHdr->biSizeImage;

	pBits = HeapAlloc(GetProcessHeap(), 0, bmpInfoHdr->biSizeImage);
	if (pBits == NULL)
	{
		return;   /* allocation failure */
	}

	hDC = CreateCompatibleDC(NULL);

	fOK = FALSE;
	GetDIBits(hDC, bmpToSave, 0, (UINT)bm.bmHeight, pBits, (BITMAPINFO *)&bmi, DIB_RGB_COLORS);
	hFile = CreateFile("custom.bmp", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		fOK = WriteFile(hFile, &bf, sizeof(bf), &cb, NULL) &&
			WriteFile(hFile, &bmi, sizeof(BITMAPINFOHEADER) + cbColors, &cb, NULL) &&
			WriteFile(hFile, pBits, bmpInfoHdr->biSizeImage, &cb, NULL);

		CloseHandle(hFile);

		if (!fOK)
		{
			DeleteFile("custom.bmp");
		}
	}
	DeleteDC(hDC);
	HeapFree(GetProcessHeap(), 0, pBits);
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