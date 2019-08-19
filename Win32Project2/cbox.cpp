#include "cbox.h"


cbox::cbox(HWND hParent, int control_id) : control(hParent, control_id)
{
	text = NULL;
}

cbox::~cbox()
{
}


void cbox::AddStr(char* str){ SendMessage(m_hwnd, CB_ADDSTRING,NULL, (LPARAM)str); }//add's the string in the list

int cbox::getItem(){ return SendMessage(m_hwnd, CB_GETCURSEL, NULL, NULL); }//return's the numer of the chosen element

char* cbox::getItemText(int index){
	if (index == -1){ return ""; }
	if (text){ delete text; }
	text = NULL;
	//sozdanie byfera
	int size = SendMessage(m_hwnd, CB_GETLBTEXTLEN, index, NULL);
	text = new char[size + 1];
	text[size] = 0;
	//sohranenie teksta vibrannogo elementa v byfer
	SendMessage(m_hwnd, CB_GETLBTEXT, index, (LPARAM)text);
	return text;
}

void cbox::setStr(int index){ SendMessage(m_hwnd, CB_SETCURSEL, (WPARAM)index, NULL); }//setting the new current element