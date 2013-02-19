// Render_VideoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Render_Video.h"
#include "Render_VideoDlg.h"



#define FILE_WIDTH   176
#define FILE_HEIGHT  144

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CRender_VideoDlg dialog




CRender_VideoDlg::CRender_VideoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRender_VideoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRender_VideoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRender_VideoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CRender_VideoDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CRender_VideoDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CRender_VideoDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CRender_VideoDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CRender_VideoDlg message handlers

BOOL CRender_VideoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	//////////////////////////////////////////////////////////////////////////
	//AvSDK
	m_buffer = NULL;

	m_moduleHandle = NULL;
	LONG ret = Success;
	

	ret = AvModuleOpen(Av_VideoRender, &m_moduleHandle);

	if (ret != Success)
	{
		MessageBox(L"Failed to open the module");
	} else {
		HWND hwnd;
		GetDlgItem(IDC_DISP_RND, &hwnd);
		ULONG wndHandle = ULONG(hwnd);
		ret = AvPropertyWrite(m_moduleHandle, Para_WindowHandle, sizeof(wndHandle), &wndHandle, 0 );

		if(ret != Success)
		{
			MessageBox(L"Failed to set Para_WindowHandle");
		}

		ULONG width = FILE_WIDTH;
		ret = AvPropertyWrite(m_moduleHandle, Para_WindowWidth, sizeof(width), &width, 0 );
		if(ret != Success)
		{
			MessageBox(L"Failed to set Para_WindowWidth");
		}

		ULONG height = FILE_HEIGHT;
		ret = AvPropertyWrite(m_moduleHandle, Para_WindowHeight, sizeof(height), &height, 0 );
		if(ret != Success)
		{
			MessageBox(L"Failed to set Para_WindowHeight");
		}



		ret = AdditionalPropertyAccess();
	
		if (ret != Success)
		{
			MessageBox(L"Failed to access the property");
		}
	}
	//////////////////////////////////////////////////////////////////////////
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRender_VideoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRender_VideoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRender_VideoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CRender_VideoDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//exit button
	LONG ret;
	if (m_moduleHandle)
	{
		ret=AvModuleClose(m_moduleHandle);
	}
	if (m_buffer)
	{
		delete [] m_buffer;
	}
	OnOK();
}



LONG CRender_VideoDlg::AdditionalPropertyAccess()
{
	//HANDLE m_moduleHandle;
	//PBYTE  m_buffer;
	//ULONG  m_bufLen;

	LONG  ret = Success;
	ULONG bufSizeCur = 0;
	ULONG bufSizeNeed = 0;
	ULONG notifyNow = 0;
	ULONG attr;
	PBYTE buffer = NULL;
	do{
		//Feature
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//for the property terminated with "s", the length is unknown
		//one method is available to get the required length:
		//setting the buffer as NULL



		ret = AvPropertyRead(m_moduleHandle, Feature_RenderMethods, bufSizeCur, NULL, &bufSizeNeed, &attr );
		buffer = new BYTE[bufSizeNeed];
		bufSizeCur = bufSizeNeed;
		//get the feature


		ret = AvPropertyRead(m_moduleHandle, Feature_RenderMethods, bufSizeCur, buffer, &bufSizeNeed, &attr );


		if(ret != Success)
		{
			break;
		}

		if (buffer)
		{
			delete [] buffer;
			buffer = NULL;
		}


		//Property 
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		ULONG renderMethod = 0;
		//get the required length by setting the buffer as NULL
		ret = AvPropertyRead(m_moduleHandle, Para_RenderMethod, sizeof(renderMethod), &renderMethod, &bufSizeNeed, &attr );
		if(ret != Success)
		{
			break;
		}

		//modify the property
		renderMethod = VR_DirectDraw;
		ret = AvPropertyWrite(m_moduleHandle, Para_RenderMethod, sizeof(renderMethod), &renderMethod, notifyNow );
		if(ret != Success)
		{
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		ULONG fourCC = 0;
		//get the property
		ret = AvPropertyRead(m_moduleHandle, Para_FourCC, sizeof(fourCC), &fourCC, &bufSizeNeed, &attr );
		if(ret != Success)
		{
			break;
		}

		//modify the property
		fourCC = MAKEFOURCC('Y', 'V', '1', '2');
		ret = AvPropertyWrite(m_moduleHandle, Para_FourCC, sizeof(fourCC), &fourCC, notifyNow );
		if(ret != Success)
		{
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		ULONG osdPosX = 0;
		//get the property
		ret = AvPropertyRead(m_moduleHandle, Para_OsdPosX, sizeof(osdPosX), &osdPosX, &bufSizeNeed, &attr );
		if(ret != Success)
		{
			break;
		}

		//modify the property
		osdPosX = 10;
		ret = AvPropertyWrite(m_moduleHandle, Para_OsdPosX, sizeof(osdPosX), &osdPosX, notifyNow );
		if(ret != Success)
		{
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		ULONG osdPosY;
		//get the property
		ret = AvPropertyRead(m_moduleHandle, Para_OsdPosY, sizeof(osdPosY), &osdPosY, &bufSizeNeed, &attr );
		if(ret != Success)
		{
			break;
		}

		//modify the property
		osdPosY = 10;
		ret = AvPropertyWrite(m_moduleHandle, Para_OsdPosY, sizeof(osdPosY), &osdPosY, notifyNow );
		if(ret != Success)
		{
			break;
		}

		//////////////////////////////////////////////////////////////////////////
#define OSD_STRING_SIZE 32
		WCHAR osdStr[OSD_STRING_SIZE];
		//get the property
		ret = AvPropertyRead(m_moduleHandle, Para_OsdString, OSD_STRING_SIZE*sizeof(WCHAR), osdStr, &bufSizeNeed, &attr );

		//modify the property
#define OSD_STRING L"Advantech SDK"  
		wcscpy_s(osdStr, OSD_STRING);
		ret = AvPropertyWrite(m_moduleHandle, Para_OsdString, OSD_STRING_SIZE*sizeof(WCHAR), &osdStr, notifyNow );
		if(ret != Success)
		{
			break;
		}

	}while(0);

	if (buffer)
	{
		delete [] buffer;
	}

	return ret;
}

//TEST(Test_Property,Normal_test)
//{
//	AdditionalPropertyAccess();
//}
void CRender_VideoDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	//Render button
	LONG ret = Success;
	FILE * f = fopen("D:\\Video team\\VideoSDK\\Test\\resources\\YV12_176_144\\carphone080.yuv", "rb");
	if ( f == NULL )
	{
		MessageBox(L"Can't open file");
		return;
	}

	if (m_buffer)
	{
		delete [] m_buffer;
	}
	m_bufLen = FILE_WIDTH * FILE_HEIGHT * 3 / 2;
	m_buffer = new BYTE[m_bufLen];
	memset(m_buffer, 0, m_bufLen);
	UINT len = fread(m_buffer, 1, m_bufLen, f );

	fclose(f);

	if (m_moduleHandle)
	{
		ret = AvVideoRender(m_moduleHandle, m_buffer, m_bufLen);
		if (ret != Success)
		{
			MessageBox(L"VideoRender Failed");
		}
	}
}

void CRender_VideoDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	//Prepare button
	LONG ret = Success;
	if (m_moduleHandle)
	{
		ret = AvVideoRenderPrepare(m_moduleHandle);
		if (ret != Success)
		{
			MessageBox(L"VideoRenderPrepare Failed");
		}
	}
}
void CRender_VideoDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	//Release button
	LONG ret = Success;
	if (m_moduleHandle)
	{
		ret = AvVideoRenderRelease(m_moduleHandle);
		if (ret != Success)
		{
			MessageBox(L"VideoRenderRelease Failed");
		}
	}

}

///Test code ////////////
#ifdef GTEST
#include <iostream>
using namespace std;
#include <stdio.h>

HANDLE m_moduleHandle=NULL;
PBYTE  m_buffer=NULL;
ULONG  m_bufLen=0;
LONG ret=-1;
void CoutRet(LONG data)
{

	cout<<"ErrCode=0x"<<hex<<ret<<endl;

}

/////////AvModeOpen Test///////////////////////////////
TEST (AvModuleOpen,NormalTest)
{	
	
	ret = -1;
	m_moduleHandle = NULL;
	ret = AvModuleOpen(Av_VideoRender, &m_moduleHandle);
	EXPECT_EQ(Success,ret);
	CoutRet(ret);
	EXPECT_NE(NULL,(int)m_moduleHandle)<<m_moduleHandle;

}
TEST (AvModuleOpen,abNormalPara1Test)
{
	ret=-1;
	m_moduleHandle = NULL;
	ret=AvModuleOpen(0,&m_moduleHandle);
	EXPECT_EQ(ErrorDeviceUnrecognized,ret)<<"moduleType=0 (-1,1...11)";
	CoutRet(ret);
	EXPECT_EQ(NULL,(int)m_moduleHandle)<<m_moduleHandle;
}
TEST (AvModuleOpen,abNormalPara2Test)
{
	ret=-1;
	ret=AvModuleOpen(Av_VideoRender,NULL);
	EXPECT_EQ(Success,ret)<<"handle=NULL";
	CoutRet(ret);
}
TEST (AvModuleOpen,abNormalAllParaTest)
{
	ret=-1;
	ret=AvModuleOpen(12,NULL);
	EXPECT_EQ(ErrorDeviceUnrecognized,ret)<<"moduleType=12;Handle=NULL";
	CoutRet(ret);
}
TEST(AvModuleOpen,abNormalOrderTest)
{
	m_moduleHandle = NULL;
	ret = -1;
	ret = AvModuleOpen(Av_VideoRender, &m_moduleHandle);
	ret = AvModuleClose(m_moduleHandle);
	ret = AvModuleOpen(Av_VideoRender, &m_moduleHandle);
	EXPECT_EQ(Success,ret);
	CoutRet(ret);
}
TEST(AvModuleOpen,abNormalReCallTest)
{
	m_moduleHandle = NULL;
	ret = -1;
	ret = AvModuleOpen(Av_VideoRender, &m_moduleHandle);
	ret = AvModuleOpen(Av_VideoRender, &m_moduleHandle);
	EXPECT_EQ(Success,ret);
	CoutRet(ret);
}
/////////AvModuleClose Test///////////////////////////////
TEST (AvModuleClose,NormalTest)
{
	ret = -1;
	ret = AvModuleOpen(Av_VideoRender, &m_moduleHandle);
	ret = AvModuleClose(m_moduleHandle);
	EXPECT_EQ(Success,ret);
	CoutRet(ret);
}
TEST (AvModuleClose,abNormalParaTest)
{
	ret = -1;
	ret = AvModuleClose(NULL);
	EXPECT_NE(Success,ret);cout<<"handle=NULL";
	CoutRet(ret);
}
TEST (AvModuleClose,abNormalOrderTest)
{
	ret = -1;
	m_moduleHandle=NULL;
	ret = AvModuleClose(m_moduleHandle);
	EXPECT_NE(Success,ret);
	CoutRet(ret);
}
TEST (AvModuleClose,abNormalReCallTest)
{
	ret = -1;
	m_moduleHandle=NULL;
	ret = AvModuleOpen(Av_VideoRender, &m_moduleHandle);
	//EXPECT_EQ(Success,ret);
	ret = AvModuleClose(m_moduleHandle);
	ret = AvModuleClose(m_moduleHandle);
	EXPECT_EQ(ErrorDeviceUnrecognized,ret);
	CoutRet(ret);
}
/////////AvProperyRead Test///////////////////////////////
ULONG bufSizeCur;
ULONG bufSizeNeed;
ULONG notifyNow;
ULONG attr;
PBYTE buffer;
ULONG renderMethod;
TEST (AvPropertyRead,NormalTest)
{
	m_moduleHandle=NULL;
	ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
	renderMethod = 1;
	ret = AvPropertyRead(m_moduleHandle, Para_RenderMethod, sizeof(renderMethod), &renderMethod, &bufSizeNeed, &attr );
	EXPECT_EQ(Success,ret);cout<<"renderMethod="<<renderMethod<<endl;
	CoutRet(ret);
}
TEST (AvPropertyRead,abNormalParaTest)
{
	m_moduleHandle=NULL;
	ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
	renderMethod = 0;
	ret = AvPropertyRead(NULL, Para_RenderMethod, sizeof(renderMethod), &renderMethod, &bufSizeNeed, &attr );
	EXPECT_EQ(ErrorHandleNotValid,ret);cout<<"handle=NULL;";
	CoutRet(ret);
	ret = AvPropertyRead(m_moduleHandle, 78, sizeof(renderMethod), &renderMethod, &bufSizeNeed, &attr );
	EXPECT_EQ(ErrorPropNotSpted,ret);cout<<"PropertyId=78 (0-77);";
	CoutRet(ret);
	ret = AvPropertyRead(m_moduleHandle, Para_RenderMethod, 0, &renderMethod, &bufSizeNeed, &attr );
	EXPECT_EQ(ErrorBufferTooSmall,ret);cout<<"bufferSize=0;";
	CoutRet(ret);
	ret = AvPropertyRead(m_moduleHandle, Para_RenderMethod, sizeof(renderMethod), NULL, &bufSizeNeed, &attr );
	EXPECT_EQ(Success,ret);cout<<"buffer=NULL;";
	CoutRet(ret);
	ret = AvPropertyRead(m_moduleHandle, Para_RenderMethod, sizeof(renderMethod), &renderMethod, NULL, &attr );
	EXPECT_EQ(Success,ret);cout<<"dataLength=NULL;";
	CoutRet(ret);
	ret = AvPropertyRead(m_moduleHandle, Para_RenderMethod, sizeof(renderMethod), &renderMethod, &bufSizeNeed, NULL );
	EXPECT_EQ(Success,ret);cout<<"attribute=NULL;";
	CoutRet(ret);
	ret = AvPropertyRead(m_moduleHandle, Para_RenderMethod, sizeof(renderMethod), NULL, NULL, NULL );
	EXPECT_EQ(Success,ret);cout<<"Para456=NULL;";
	CoutRet(ret);
}
TEST (AvPropertyRead,abNormalOrderTest)
{
	m_moduleHandle=NULL;
	renderMethod = 0;
	ret = AvPropertyRead(m_moduleHandle, Para_RenderMethod, sizeof(renderMethod), &renderMethod, &bufSizeNeed, &attr );
	EXPECT_EQ(ErrorHandleNotValid,ret);
	CoutRet(ret);
}
TEST (AvPropertyRead,abNormalReCallTest)
{
	m_moduleHandle=NULL;
	ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
	ULONG renderMethod1 = 1,renderMethod2 = 2;
	ret = AvPropertyRead(m_moduleHandle, Para_RenderMethod, sizeof(renderMethod1), &renderMethod1, &bufSizeNeed, &attr );
	ret = AvPropertyRead(m_moduleHandle, Para_RenderMethod, sizeof(renderMethod2), &renderMethod2, &bufSizeNeed, &attr );
	EXPECT_EQ(Success,ret);
	EXPECT_EQ(renderMethod1,renderMethod2);
	CoutRet(ret);

}
/////////AvProperyWrite Test///////////////////////////////
TEST (AvPropertyWrite,NormalTest)
{
	m_moduleHandle=NULL;
	ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
	ULONG renderMethod1 = VR_DirectDraw,renderMethod2 = 1;
	ret = AvPropertyWrite(m_moduleHandle, Para_RenderMethod, sizeof(renderMethod1), &renderMethod1, notifyNow );
	EXPECT_EQ(Success,ret);cout<<"renderMethod1="<<renderMethod1<<endl;
	CoutRet(ret);
	ret = AvPropertyRead(m_moduleHandle, Para_RenderMethod, sizeof(renderMethod2), &renderMethod2, &bufSizeNeed, &attr );
	EXPECT_EQ(renderMethod1,renderMethod2);
	
}
TEST (AvPropertyWrite,abNormalParaTest)
{
	m_moduleHandle=NULL;
	ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
	renderMethod = VR_DirectDraw;
	ret = AvPropertyWrite(NULL, Para_RenderMethod, sizeof(renderMethod), &renderMethod, notifyNow );
	EXPECT_EQ(ErrorHandleNotValid,ret);cout<<"handle=NULL";
	CoutRet(ret);
	ret = AvPropertyWrite(m_moduleHandle, 78, sizeof(renderMethod), &renderMethod, notifyNow );
	EXPECT_EQ(ErrorPropNotSpted,ret);cout<<"PropertyId=78 (0-77);";
	CoutRet(ret);
	ret = AvPropertyWrite(m_moduleHandle, Para_RenderMethod, 0, &renderMethod, notifyNow );
	EXPECT_EQ(ErrorBufferTooSmall,ret);cout<<"dataLength=0;";
	CoutRet(ret);
	ret = AvPropertyWrite(m_moduleHandle, Para_RenderMethod, sizeof(renderMethod), NULL, notifyNow );
	EXPECT_EQ(ErrorBufferIsNull,ret);cout<<"buffer=NULL;";
	CoutRet(ret);
	ret = AvPropertyWrite(m_moduleHandle, Para_RenderMethod, sizeof(renderMethod), &renderMethod, 3);
	EXPECT_EQ(ErrorParamOutOfRange,ret);cout<<"notifyNow=3 (0,1);";
	CoutRet(ret);
	renderMethod=1;
	ret = AvPropertyWrite(m_moduleHandle, Para_RenderMethod,  sizeof(renderMethod), &renderMethod, notifyNow );
	EXPECT_EQ(ErrorPropValueNotSpted,ret);cout<<"buffer=&renderMethod;renderMethod=1(only 0);";
	CoutRet(ret);

}
TEST (AvPropertyWrite,abNormalOrderTest)
{
	m_moduleHandle=NULL;
	renderMethod = VR_DirectDraw;
	ret = AvPropertyWrite(m_moduleHandle, Para_RenderMethod, sizeof(renderMethod), &renderMethod, notifyNow );
	EXPECT_EQ(ErrorHandleNotValid,ret)<<"Do not call AvModuleOpen!  ";
	CoutRet(ret);
}
TEST (AvPropertyWrite,abNormalReCallTest)
{
	m_moduleHandle=NULL;
	ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
	renderMethod = VR_DirectDraw;
	ret = AvPropertyWrite(m_moduleHandle, Para_RenderMethod, sizeof(renderMethod), &renderMethod, notifyNow );
	EXPECT_EQ(Success,ret)<<"first call AvPropertyWrite!  ";
	ret = AvPropertyWrite(m_moduleHandle, Para_RenderMethod, sizeof(renderMethod), &renderMethod, notifyNow );
	EXPECT_EQ(Success,ret)<<"second call AvPropertyWrite!  ";
	CoutRet(ret);
}
/////////Feature_RenderMethods Test///////////////////////////////
TEST (Feature_RenderMethods,NormalReadTest)
{
	ret = -1;
	bufSizeCur = 0;
	bufSizeNeed = 0;
	notifyNow = 0;
	buffer = NULL;
	renderMethod = 0;
	m_moduleHandle=NULL;
	ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);

	ret = AvPropertyRead(m_moduleHandle, Feature_RenderMethods, bufSizeCur, NULL, &bufSizeNeed, &attr );
	buffer = new BYTE[bufSizeNeed];
	bufSizeCur = bufSizeNeed;
	ret = AvPropertyRead(m_moduleHandle, Feature_RenderMethods, bufSizeCur, buffer, &bufSizeNeed, &attr );
	EXPECT_EQ(Success,ret);cout<<"Feature_RenderMethods:";
	for (int i = 0;i < bufSizeCur;i++)
	{
		cout<<(ULONG)buffer[i];
	}
	cout<<endl;
	CoutRet(ret);
}
TEST (Feature_RenderMethods,abNormalWriteTest)
{
	m_moduleHandle=NULL;
	ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
	renderMethod = VR_DirectDraw;
	ret = AvPropertyWrite(m_moduleHandle, Feature_RenderMethods, sizeof(renderMethod), &renderMethod, notifyNow );
	EXPECT_EQ(ErrorPropReadOnly,ret)<<"Feature_RenderMethods write value!";
	CoutRet(ret);
}

#endif  //test code end