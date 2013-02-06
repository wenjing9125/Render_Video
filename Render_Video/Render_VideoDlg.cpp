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
	ON_BN_CLICKED(IDC_BUTTON4, &CRender_VideoDlg::OnBnClickedButton4)
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
	
	//TEST(Test_AvPropertyRead,NormalTest)
	//{
		ret = AvModuleOpen(Av_VideoRender, &m_moduleHandle);
		//EXPECT_EQ(0,ret);
	//}
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
	if (m_moduleHandle)
	{
		AvModuleClose(m_moduleHandle);
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

		//TEST(Test_AvPropertyRead,NormalTest)
		//{
			ret = AvPropertyRead(m_moduleHandle, Feature_RenderMethods, bufSizeCur, buffer, &bufSizeNeed, &attr );
			//EXPECT_EQ(0,0);
		//}//test

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

void CRender_VideoDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	//Open Source button
}
