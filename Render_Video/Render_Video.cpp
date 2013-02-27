// Render_Video.cpp : Defines the class behaviors for the application.
//
class CConsole  
{  
public:  
	CConsole(void);  
	virtual ~CConsole(void);  
private:  
}; 

#include "stdafx.h"
#include "Render_Video.h"
#include "Render_VideoDlg.h"
#include "Console.h"
#include <conio.h> 
#include <fcntl.h> 
#include <io.h> 


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CConsole::CConsole(void)  
{  
	AllocConsole();  
	int hCrun;      
	hCrun = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);  
	FILE* hFile  = _fdopen(hCrun, "w");  
	// use default stream buffer
	setvbuf(hFile, NULL, _IONBF, 0);  
	*stdout = *hFile;  
	//test 
	//_cprintf("test console by _cprintf/n", 0);
	//std::cout < testconsolebystdoutnspan>
}  
CConsole::~CConsole(void)  
{  
	FreeConsole();  
}  

// CRender_VideoApp

BEGIN_MESSAGE_MAP(CRender_VideoApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CRender_VideoApp construction

CRender_VideoApp::CRender_VideoApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CRender_VideoApp object

CRender_VideoApp theApp;


// CRender_VideoApp initialization

BOOL CRender_VideoApp::InitInstance()
{	
#ifdef GTEST
	CConsole cc;  
	int argc = 0;  
	TCHAR* argv = _T("");
	//testing::GTEST_FLAG(output) = "xml:"; //加上这条则测试结果以xml格式保存下来。保存路径可以修改，如"xml:d/"表示保存到d:下 
	testing::InitGoogleTest(&argc, &argv);  
	RUN_ALL_TESTS();  
	HWND hwnd = GetConsoleWindow();  
	SetWindowPos(hwnd, HWND_TOP,500,250,0,0, /*SWP_NOMOVE|*/SWP_NOSIZE);  
	AfxMessageBox(_T("about to exit!"));  
	//std::cin.get();
	return false;

#else


	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CRender_VideoDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
#endif //GTEST
}
