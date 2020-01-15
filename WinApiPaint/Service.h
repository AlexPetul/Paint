#pragma once
#include "Figure.h"

class Service
{
private:
	HMENU popUpMenu;
	MENUITEMINFO menuInfo;
public:
	Service(HMENU hMenu);
	bool GetMenuItemState(int currState);
	POINT GetMouseCoords(HWND hwnd);
	void CreateMainLayer(HWND hWnd, HDC* newDC, HBITMAP* newBmp, int width, int hight);
	void CreateAfterLoadLayer(HWND hwnd, HDC &hdc, HDC &memory, HBITMAP bmp);
	~Service();
};