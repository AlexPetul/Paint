#include "Service.h"
#include "MainPaint.h"

Service::Service(HMENU hMenu)
{
	this->popUpMenu = hMenu;
	menuInfo = { sizeof(MENUITEMINFO) };
	menuInfo.fMask = MIIM_STATE;
}

bool Service::GetMenuItemState(int currMenuItem)
{
	for (int index = ID_CIRCLE; index <= ID_TRIANGLE; index++)
	{
		if (index != currMenuItem)
		{
			GetMenuItemInfo(popUpMenu, index, FALSE, &menuInfo);
			if (menuInfo.fState == MFS_CHECKED)
			{
				menuInfo.fState ^= MFS_CHECKED;
				SetMenuItemInfo(popUpMenu, index, FALSE, &menuInfo);
			}
		}
	}

	if (currMenuItem == ID_MOVE_OBJECTS)
	{
		return true;
	}

	GetMenuItemInfo(popUpMenu, currMenuItem, FALSE, &menuInfo);
	menuInfo.fState ^= MFS_CHECKED;
	SetMenuItemInfo(popUpMenu, currMenuItem, FALSE, &menuInfo);
	if (menuInfo.fState == MFS_CHECKED)
	{
		return true;
	}
	else
	{
		return false;
	}
}

POINT Service::GetMouseCoords(HWND hwnd)
{
	POINT absCursorPos, resultCoords;
	RECT windowPosCoords;
	GetCursorPos(&absCursorPos);
	GetWindowRect(hwnd, &windowPosCoords);
	resultCoords.x = absCursorPos.x - windowPosCoords.left;
	resultCoords.y = absCursorPos.y - windowPosCoords.top;
	return resultCoords;
}

Service::~Service()
{

}