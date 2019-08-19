#include "textbox.h"


textbox::textbox(HWND hParent, int control_id) : control(hParent, control_id)
{
	txt = NULL;
}


textbox::~textbox()
{
	if (txt)
		delete txt;
}

char* textbox::getText(){
	if (txt){
		delete txt;
		txt = NULL;
	}
	int len = SendMessage(m_hwnd, EM_LINELENGTH, 0, 0);
	if (len){
		txt = new char[len + 1];
		txt[len] = 0;
		SendMessage(m_hwnd, EM_GETLINE, 0, (LPARAM)txt);
		return txt;
	}

	return "";	 
}

void textbox::setText(char* text){ SendMessage(m_hwnd, WM_SETTEXT, 0, (LPARAM)text); }
	