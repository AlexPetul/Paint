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
	~Service();
};