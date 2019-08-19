#pragma once
#include "control.h"
class cbox :
	public control
{
public:
	cbox(HWND hParent, int control_id);
	~cbox();

	void AddStr(char* str);
	int getItem();
	char* getItemText(int index);
	void setStr(int index);
private:
	char* text;
};

