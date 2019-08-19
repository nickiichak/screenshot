#include "gdiscreen.h"


gdiscreen::gdiscreen()
{
}
gdiscreen::~gdiscreen()
{
}

void gdiscreen::screenshot(WCHAR* folder)
{
	using namespace Gdiplus;
	//GDI+ inicialize
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Status st = GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	if (st != Status::Ok)
		MessageBox(NULL, "Initialization of GDI+ has been failed", "Fatal Error", MB_OK | MB_ICONERROR);

	{
		HDC scrdc, memdc;
		HBITMAP membit;
		//getting main screen HDC
		scrdc = GetDC(0);
		//getting main screen resolution
		DEVMODE lpDevMode;
		lpDevMode.dmSize = sizeof(DEVMODE);
		EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, &lpDevMode);
		int Width = lpDevMode.dmPelsWidth;
		int Height = lpDevMode.dmPelsHeight;

		memdc = CreateCompatibleDC(scrdc);
		membit = CreateCompatibleBitmap(scrdc, Width, Height);
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(memdc, membit);
		BitBlt(memdc, 0, 0, Width, Height, scrdc, 0, 0, SRCCOPY);



		

		//saving the screenshot
		Bitmap bitmap(membit, NULL);
		CLSID clsid;
		GetEncoderClsid(L"image/jpeg", &clsid);
		bitmap.Save(folder, &clsid);

		SelectObject(memdc, hOldBitmap);
		DeleteObject(memdc);		 
		DeleteObject(membit);
		::ReleaseDC(0, scrdc);
	}

	GdiplusShutdown(gdiplusToken);
}

int gdiscreen::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	using namespace Gdiplus;
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}

	free(pImageCodecInfo);
	return 0;
}