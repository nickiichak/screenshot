#pragma once
#include "control.h"
#include<Windows.h>


class textbox :
	public control
{
public:
	textbox(HWND hParent, int control_id);
	virtual ~textbox();

	char* getText();
	void setText(char* text);
private:
	char* txt;
};

