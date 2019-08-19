#include "control.h"


control::control(HWND hParent, int control_id)
{
	m_hwnd = GetDlgItem(hParent, control_id);
	m_hparent = hParent;
}

control::~control(){}

HWND control::getHandle(){ return this->m_hwnd; }
HWND control::getParent(){ return this->m_hparent; }
int control::getID(){ return this->m_id; }
void control::setFont(HFONT hFont){ SendMessage(m_hwnd, WM_SETFONT, (WPARAM)hFont, true); }
HFONT control::getFont(){ HFONT hFont = (HFONT)SendMessage(m_hwnd, WM_GETFONT, 0, 0); return hFont; }
RECT control::getPositionRect(){

	RECT ctrlRect = { 0 };
	GetWindowRect(m_hwnd, &ctrlRect);
	POINT p1 = { ctrlRect.left, ctrlRect.top };
	POINT p2 = { ctrlRect.right, ctrlRect.bottom };
	ScreenToClient(m_hparent, &p1);
	ScreenToClient(m_hparent, &p2);
	ctrlRect.left = p1.x;
	ctrlRect.top = p1.y;
	ctrlRect.right = p2.x;
	ctrlRect.bottom = p2.y;

	return ctrlRect;
}