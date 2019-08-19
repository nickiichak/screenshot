# pragma once
#include <Windows.h>
#include <gdiplus.h>




class gdiscreen
{
public:
	gdiscreen();
	~gdiscreen();

	void screenshot(WCHAR* folder);
private:
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
};

