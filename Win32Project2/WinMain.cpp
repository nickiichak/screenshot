#include"menubox.h"
#include <CommCtrl.h>

#pragma comment (lib,"Comctl32.lib")
#pragma comment (lib,"Gdiplus.lib")
#pragma comment(lib, "Shlwapi.lib")



#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow){
	//UI initializing
	INITCOMMONCONTROLSEX init = { sizeof(INITCOMMONCONTROLSEX), ICC_WIN95_CLASSES };
	if (!InitCommonControlsEx(&init)){
		MessageBox(NULL, "Initialization of controls has been failed", "FATAL ERROR", MB_OK | MB_ICONERROR);
	}
	//creating main window
	MSG msg;
	menubox* mdlg = new menubox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL);
	mdlg->dlg_show(SW_SHOW);
	
	while (FALSE != GetMessage(&msg, NULL, NULL, NULL)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}