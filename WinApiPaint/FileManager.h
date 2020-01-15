#pragma once
#include "PaintController.h"
#include <fstream>

#define CUSTOM_FILENAME "custom.bmp"
#define FIGURES_FILENAME "figures.dat"

typedef struct tagKHMZ_BITMAPINFOEX
{
	BITMAPINFOHEADER bmiHeader;
	RGBQUAD          bmiColors[256];
} KHMZ_BITMAPINFOEX, FAR *LPKHMZ_BITMAPINFOEX;

class FileManager
{
private:
	std::ofstream fout;
	std::ifstream fin;
	struct stat filestatus;
public:
	FileManager();
	HBITMAP LoadBitmapFromFile();
	void SaveFile(HBITMAP bmpToSave, HWND hwnd);
	void SaveFiguresToFile(std::vector<DrawnFigure> figs);
	bool LoadFigures(std::vector<DrawnFigure> &figs);
	~FileManager();
};