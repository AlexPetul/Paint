#pragma once
#include <Windows.h>

#define CUSTOM_FILENAME "custom.bmp"

typedef struct tagKHMZ_BITMAPINFOEX
{
	BITMAPINFOHEADER bmiHeader;
	RGBQUAD          bmiColors[256];
} KHMZ_BITMAPINFOEX, FAR *LPKHMZ_BITMAPINFOEX;

class FileManager
{
public:
	FileManager();
	HBITMAP LoadBitmapFromFile();
	void SaveFile(HBITMAP bmpToSave, HWND hwnd);
	~FileManager();
};