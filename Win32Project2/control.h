#pragma once
#include<Windows.h>
class control
{
public:
	control(HWND hParent, int control_id);
	virtual ~control();

	void setFont(HFONT hFont);
	HFONT getFont();
	RECT getPositionRect();

	HWND getHandle();
	HWND getParent();
	int getID();

protected:
	HWND m_hwnd;
	HWND m_hparent;
	int m_id;

};

