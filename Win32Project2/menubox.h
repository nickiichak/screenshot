#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include<fstream>
#include<string.h>
#include<string>
#include<iostream>
#include<Shlwapi.h>
#include<stdlib.h>


#include"textbox.h"
#include"resource1.h"
#include"gdiscreen.h"
#include"cbox.h"


using namespace std;

class menubox
{
private:
	textbox* m_text1;
	textbox* m_text2;
	textbox* m_text3;
	cbox* m_cbox1;
	HWND m_hwnd;
	//current settings
	int Ctrl, funk;
	char* hotK;			UINT type; //for F1-F12 hotkeys
	char* newSFolder;	int SFolderSize;
	char* newFName;		int SFileSize;
	//default settings
	char* def_hotK = "F09 0";
	char* save_name = "screen";
	char* save_folder = "C:\\";
	char* data_folder = "ScreenShotData.txt";
public:
	menubox(HINSTANCE hInstance, LPCSTR dlg_name, HWND parent);
	virtual ~menubox();
	void dlg_show(int mCmdShow);
	LRESULT RealDlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	void createdlg(HINSTANCE hInstance, LPCSTR dlg_name, HWND parent);
	static LRESULT WINAPI stDlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);



	int readInf();
	void getInf(string line);
	char* getFolder(string line);
	char* getFileName(string line);
	void func();
	int save();
	void standart();
	void crDataFile();

	void prtscr();


	void hkReg();
	void hkRegCtrl();
	void hkRegAlt();
	void hkRegShift();

	void hkFuncReg();
	void hkFuncRegCtrl();
	void hkFuncRegAlt();
	void hkFuncRegShift();	
};

