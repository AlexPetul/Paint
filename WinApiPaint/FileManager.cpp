#include "FileManager.h"

FileManager::FileManager()
{

}

HBITMAP FileManager::LoadBitmapFromFile()
{
	HANDLE hFile;
	BITMAPFILEHEADER bf;
	KHMZ_BITMAPINFOEX bmi;
	DWORD cb, cbImage;
	LPVOID pvBits1, pvBits2;
	HDC hDC;
	HBITMAP hbm;

	hbm = (HBITMAP)LoadImage(NULL, CUSTOM_FILENAME, IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hbm)
		return hbm;

	hFile = CreateFile(CUSTOM_FILENAME, GENERIC_READ, FILE_SHARE_READ, NULL,
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

void FileManager::SaveFile(HBITMAP bmpToSave, HWND hwnd)
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
	hFile = CreateFile(CUSTOM_FILENAME, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		fOK = WriteFile(hFile, &bf, sizeof(bf), &cb, NULL) &&
			WriteFile(hFile, &bmi, sizeof(BITMAPINFOHEADER) + cbColors, &cb, NULL) &&
			WriteFile(hFile, pBits, bmpInfoHdr->biSizeImage, &cb, NULL);

		CloseHandle(hFile);

		if (!fOK)
		{
			DeleteFile(CUSTOM_FILENAME);
		}
	}
	DeleteDC(hDC);
	HeapFree(GetProcessHeap(), 0, pBits);
	MessageBox(hwnd, TEXT("File saved (custom.bmp)"), TEXT("File Manager"), NULL);
}

FileManager::~FileManager()
{

}