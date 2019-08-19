#include "menubox.h"


menubox::menubox(HINSTANCE hInstance, LPCSTR dlg_name, HWND parent)
{
	createdlg(hInstance, dlg_name, parent);
	readInf();
}

menubox::~menubox()
{
	delete m_text1;
	delete m_text2;
	delete m_text3;
	delete m_cbox1;
	delete hotK;
	delete newSFolder;	
	delete newFName;
	delete save_name;
	delete save_folder;
	delete data_folder;
}


void menubox::createdlg(HINSTANCE hInstance, LPCSTR dlg_name, HWND parent){
	m_hwnd = CreateDialog(hInstance, dlg_name, parent, (DLGPROC)stDlgProc);
	SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);

	m_text1 = new textbox(m_hwnd, IDC_EDIT1);
	m_text2 = new textbox(m_hwnd, IDC_EDIT2);
	m_text3 = new textbox(m_hwnd, IDC_EDIT3);

	m_cbox1 = new cbox(m_hwnd, IDC_COMBO3);
	m_cbox1->AddStr("NO");
	m_cbox1->AddStr("Ctrl");
	m_cbox1->AddStr("Alt");
	m_cbox1->AddStr("Shift");

	hotK = NULL;
	newFName = NULL;
	newSFolder = NULL;
}

LRESULT CALLBACK menubox::stDlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	menubox* dlg_ptr = NULL;
	dlg_ptr = (menubox*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	if (dlg_ptr != 0){
		return dlg_ptr->RealDlgProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

void menubox::dlg_show(int mCmdShow){
	ShowWindow(m_hwnd, mCmdShow);
}


//reading settings from the file
int menubox::readInf(){
	//check if the settings file exists
	crDataFile();

	string line;
	ifstream inf(data_folder);
	if (inf.is_open()){
		//reading and checking hotkey data
		getline(inf, line);
		if (!line.size()){
			MessageBox(m_hwnd, "The data is corrupted", "DATA ERROR", MB_OK);
			standart();
			return 0;
		}
		getInf(line);
		//reading and checking screenshot name
		getline(inf, line);
		if (!line.size()){
			MessageBox(m_hwnd, "The data is corrupted", "DATA ERROR", MB_OK);
			standart();
			return 0;
		}
		if (newFName){ delete newFName; newFName = NULL; }
		newFName = getFileName(line);
		m_text2->setText(newFName);
		//reading and cheking saving path for a screenshot
		getline(inf, line);
		if (!line.size()){
			MessageBox(m_hwnd, "The data is corrupted", "DATA ERROR", MB_OK);
			standart();
			return 0;
		}
		if (newSFolder){ delete newSFolder; newSFolder = NULL; }
		newSFolder = getFolder(line);
		m_text3->setText(newSFolder);
	}
	inf.close();
	//setting hotkey if it is not F1-F12
	if (funk == 0){
		m_text1->setText(hotK);
		//hotkey registration (not F1-F12)
		switch (Ctrl){
		case 48:
			hkReg();
			break;
		case 49:
			hkRegCtrl();
			break;
		case 50:
			hkRegAlt();
			break;
		case 51:
			hkRegShift();
			break;
		}
	}
	else{
		//setting hotkey if it is F1-F12
		string st1;
		//if F1-F9
		if (*hotK == '0'){ 
			char* text = new char[3]; 
			text[0] = 'F';
			text[1] = hotK[1];
			text[2] = 0;
			m_text1->setText(text);
			delete[] text;
		}
		//if F10-F12
		else{
			char* text = new char[4];
			text[0] = 'F';
			text[1] = *hotK;
			text[2] = hotK[1];
			text[3] = 0;
			m_text1->setText(text);
			delete[] text;
		}
		//hotkey registration (F1-F12)
		func(); 
		switch (Ctrl){
		case 48:
			hkFuncReg();
			break;
		case 49:
			hkFuncRegCtrl();
			break;
		case 50:
			hkFuncRegAlt();
			break;
		case 51:
			hkFuncRegShift();
			break;
		}
	}
	//setting hotkey modifier
	m_cbox1->setStr(Ctrl-48);
	return 0;
}

//rading hotkey data
void menubox::getInf(string line){
	int size = line.size();
	if (size == 3){
		//if the hotkey is not F1-F12
		if (hotK){ delete hotK; }
		hotK = new char[2];
		*hotK = line[0];
		hotK[1] = 0;
		if (*hotK > 64 && *hotK < 91)
			*hotK += 32;
		Ctrl = line[2];
		funk = 0;
	}
	else{
		//if the hotkey is F1-F12
		if (hotK){ delete hotK; }
		hotK = new char[3];
		*hotK = line[1];
		hotK[1] = line[2];
		hotK[2] = 0;
		Ctrl = line[4];
		funk = 1;
	}
}

//reading screenshots name
char* menubox::getFileName(string line){
	char* text;
	int size = line.size();
	int i = 0;
	//reading screenshots name until '.' or the end of line
	while (line[i] != '.' && i < size){ i++; }
	size = i;
	text = new char[size + 1];
	text[size] = 0;
	for (i = 0; i < size; i++){
		text[i] = line[i];
	}
	SFileSize = size;
	return text;
}

//reading screenshot saving path
char* menubox::getFolder(string line){
	char* text;
	int size = line.size();
	int a = 0;
	//if the path ends with '/'
	if (line[size - 1] != 92){
		a++;
	}
	size += a;
	text = new char[size+1];
	text[size] = 0;
	for (int i = 0; i < size-a; i++){
		text[i] = line[i];
	}
	//setting '/' at the end if it need
	if (a){ text[size - 1] = 92; }
	SFolderSize = size;
	return text;
}

//matching hotkey and system variable
void menubox::func(){
	if (*hotK == '0'){
		switch (hotK[1]){
		case '1':
			type = VK_F1;
			break;
		case '2':
			type = VK_F2;
			break;
		case '3':
			type = VK_F3;
			break;
		case '4':
			type = VK_F4;
			break;
		case '5':
			type = VK_F5;
			break;
		case '6':
			type = VK_F6;
			break;
		case '7':
			type = VK_F7;
			break;
		case '8':
			type = VK_F8;
			break;
		case '9':
			type = VK_F9;
			break;
		}
	}
	else{
		switch (hotK[1]){
		case '0':
			type = VK_F10;
			break;
		case '1':
			type = VK_F11;
			break;
		case '2':
			type = VK_F12;
			break;
		}
	}
}

//applying and saving new settings
int menubox::save(){
	//proverka na rysskie bykvi v pole HotKey
	char* Hkey = m_text1->getText();
	if (Hkey[0]<0){
		MessageBox(m_hwnd, "Wrong hot key button", "Kyrsach message", MB_OK);
		return 0;
	}
	//proverka na nalichie imeni screenshota
	string name = m_text2->getText();
	if (!name.size()){
		MessageBox(m_hwnd, "Enter screenshot name", "Kyrsach message", MB_OK);
		return 0;
	}
	//proverca na nalichie papki po ykazannomy adresy
	char* path = m_text3->getText();
	if (!PathFileExists(path)){
		MessageBox(m_hwnd, "The folder doesn't exist", "Kyrsach message", MB_OK);
		return 0;
	}

	string text = m_text1->getText();
	int size = text.length();
	if (size){
		if (hotK)
		{
			delete[] hotK;
		}
		//saving hotkey (F1-F9)
		if ((text[0] == 'f' || text[0] == 'F') && (text[1]>48 && text[1]<58) && (size == 2)){
			hotK = new char[3];
			*hotK = '0';
			hotK[1] = text[1];
			hotK[2] = 0;
			funk = 1;
		}
		//saving hotkey (F10-F12)
		else if ((text[0] == 'f' || text[0] == 'F') && (text[1] > 48 && text[1] < 58) && (text[2] > 47 && text[2] < 51) && (size == 3)) {
				hotK = new char[3];
				*hotK = text[1];
				hotK[1] = text[2];
				hotK[2] = 0;
				funk = 1;
			}
			else{
				//saving hotkey (NOT F1-F12)
				hotK = new char[2];
				*hotK = text[0];
				hotK[1] = 0;
				funk = 0;
			}
		//writing data to the settings file
		ofstream inf(data_folder);
		if (funk == 0)
		{
			inf << hotK << ' ' << Ctrl - 48 << '\n';
		}
		else 
		{
			inf << "F" << hotK[0] << hotK[1] << ' ' << Ctrl - 48 << '\n';
		}
		char* text = m_text2->getText();
		inf << text << '\n';
		text = m_text3->getText();
		inf << text;
		inf.close();

		UnregisterHotKey(m_hwnd, 1);
		readInf();
	}
	return 1;
}

//restore standart settings
void menubox::standart(){
	ofstream inf(data_folder);
	if (inf)
	{
		inf << def_hotK << '\n';
		inf << save_name << '\n';
		inf << save_folder;
		inf.close();
		MessageBox(m_hwnd, "Changes successfully  saved", "Kyrsach message", MB_OK);
		UnregisterHotKey(m_hwnd, 1);
		readInf();
	}
	else MessageBox(m_hwnd, "Fail to create settings file", "Kyrsach message", MB_OK);
}

//chek if the setting file exists, create a new file if there is no one
void menubox::crDataFile(){
	if (!PathFileExists(data_folder)){
		ofstream inf(data_folder);
		if (inf)
		{
			inf << def_hotK << '\n';
			inf << save_name << '\n';
			inf << save_folder;
			inf.close();
		}
	}
}


//hotkey registration if it is NOT F1-F12
void menubox::hkReg(){
	if (!RegisterHotKey(m_hwnd, 1, MOD_NOREPEAT, VkKeyScanEx(*hotK, GetKeyboardLayout(0)))){
		MessageBox(m_hwnd, "Hotkey registration failure", "Hotkey error", MB_OK);
	}
}

void menubox::hkRegCtrl(){
	if (!RegisterHotKey(m_hwnd, 1, MOD_CONTROL | MOD_NOREPEAT, VkKeyScanEx(*hotK, GetKeyboardLayout(0)))){
		MessageBox(m_hwnd, "Hotkey registration failure", "Hotkey error", MB_OK);
	}
}

void menubox::hkRegAlt(){
	if (!RegisterHotKey(m_hwnd, 1, MOD_ALT | MOD_NOREPEAT, VkKeyScanEx(*hotK, GetKeyboardLayout(0)))){
		MessageBox(m_hwnd, "Hotkey registration failure", "Hotkey error", MB_OK);
	}
}

void menubox::hkRegShift(){
	if (!RegisterHotKey(m_hwnd, 1, MOD_SHIFT | MOD_NOREPEAT, VkKeyScanEx(*hotK, GetKeyboardLayout(0)))){
		MessageBox(m_hwnd, "Hotkey registration failure", "Hotkey error", MB_OK);
	}
}

//hotkey registration if it is F1-F12
void menubox::hkFuncReg(){
	if (!RegisterHotKey(m_hwnd, 1, NULL, type)){
		MessageBox(m_hwnd, "Hotkey registration failure", "Hotkey error", MB_OK);
	}
}

void menubox::hkFuncRegCtrl(){
	if (!RegisterHotKey(m_hwnd, 1, MOD_CONTROL , type)){
		MessageBox(m_hwnd, "Hotkey registration failure", "Hotkey error", MB_OK);
	}
}

void menubox::hkFuncRegAlt(){
	if (!RegisterHotKey(m_hwnd, 1, MOD_ALT, type)){
		MessageBox(m_hwnd, "Hotkey registration failure", "Hotkey error", MB_OK);
	}
}

void menubox::hkFuncRegShift(){
	if (!RegisterHotKey(m_hwnd, 1, MOD_SHIFT, type)){
		MessageBox(m_hwnd, "Hotkey registration failure", "Hotkey error", MB_OK);
	}
}

//creating screenshot
void menubox::prtscr(){
	//creating a full name for a new screenshot
	string pathstr = newSFolder, namestr = newFName;
	string tempstr = pathstr + namestr + ".jpeg";
	int tempnumb = 1;
	while (PathFileExists(tempstr.c_str())){
		char a[10];
		_itoa(tempnumb, a, 10);
		tempstr = pathstr + namestr + a + ".jpeg";
		tempnumb++;
	}
	int size = tempstr.size();								
	wchar_t result[128];
	result[size] = 0;
	for (int i = 0; i < size; i++) 
		result[i] = tempstr.c_str()[i];
	//creating screenshot using GDI+
	gdiscreen scr;
	scr.screenshot(result);
}


LRESULT menubox::RealDlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	switch (uMsg)
	{
	case WM_INITDIALOG:
		break;
	case WM_CLOSE:
		//close button
		EndDialog(hwnd, wParam);
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
		//create screenshot button
		if (wParam == IDOK){
			prtscr();
		}
		//save settings button
		if (wParam == IDCANCEL){
			if (save()){ MessageBox(m_hwnd, "Changes successfully  saved", "Kyrsach message", MB_OK); }
		}
		//default settings button
		if (wParam == IDNO){
			standart();
		}
		switch (HIWORD(wParam)){
			//hotkey modifier list
			case CBN_SELCHANGE:	
				Ctrl = m_cbox1->getItem() + 48;
		}
		break;
	//hotkey
	case WM_HOTKEY:
		if (uMsg == WM_HOTKEY){
			prtscr();
		}
		break;
	}
	return 0;
}