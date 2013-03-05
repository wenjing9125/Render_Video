#include "stdafx.h"
#ifdef GTEST
#include "stdafx.h"
#include <string>
#include <sstream>
#include <iostream>
using namespace std;
#include <stdio.h>



#define FILE_WIDTH   720
#define FILE_HEIGHT  576
#define IMG_NAME "D:\\Video team\\VideoSDK\\Test\\resources\\YV12_720_576.yuv"
/*"D:\\Video team\\VideoSDK\\Test\\resources\\YV12_176_144\\carphone080.yuv"*/

HANDLE m_moduleHandle=NULL;
PBYTE  m_buffer=NULL;
ULONG  m_bufLen=0;
LONG ret=-1;
HWND hwnd = 0;
void CoutRet(LONG data)
{

	cout<<"ErrCode=0x"<<hex<<ret<<endl;

}

void openImage()
{
	char str[100]=IMG_NAME;
	FILE * f = fopen(str, "rb");
	if ( f == NULL )
	{
		AfxMessageBox(L"Can't open file");
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
}

void openwindow()
{
	WCHAR wndClass[] = L"VideoRender"; 
	WCHAR title[] = L"Advantech Video Render";
	WNDCLASSEX wcx = {0}; 
	wcx.cbSize = sizeof(wcx);          
	wcx.style = CS_HREDRAW | CS_VREDRAW; 
	wcx.lpfnWndProc = (WNDPROC)DefWindowProc;      
	wcx.cbClsExtra = 0;               
	wcx.cbWndExtra = 0;               
	wcx.hInstance = NULL;        
	wcx.hIcon = NULL;
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);   
	wcx.hbrBackground = (HBRUSH)COLOR_WINDOWFRAME;    
	wcx.lpszMenuName = NULL;    
	wcx.lpszClassName = wndClass;
	RegisterClassEx(&wcx); 
	ULONG imageWidth = FILE_WIDTH;
	ULONG imageHeight = FILE_HEIGHT;

	hwnd = CreateWindow( wndClass, title, WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, imageWidth, imageHeight, NULL, NULL, NULL, NULL );
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
	EXPECT_EQ(Success,ret);
	ret = AvModuleClose(m_moduleHandle);
	ret = AvModuleClose(m_moduleHandle);
	EXPECT_EQ(ErrorDeviceUnrecognized,ret);
	//EXPECT_EQ(NULL,m_moduleHandle);
	CoutRet(ret);
}

/////////AvProperyRead Test///////////////////////////////
ULONG bufSizeCur;
ULONG bufSizeNeed;
ULONG notifyNow = 0;
ULONG attr;
PBYTE buffer;
ULONG renderMethod;

class T_AvPropertyRead: public testing::Test
{
protected:
	static void SetUpTestCase(){
		m_moduleHandle=NULL;
		ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
	}
	static void TearDownTestCase(){

	}
};


TEST_F (T_AvPropertyRead,NormalTest)
{
	//m_moduleHandle=NULL;
	//ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
	renderMethod = 1;
	ret = AvPropertyRead(m_moduleHandle, Para_RenderMethod, sizeof(renderMethod), &renderMethod, &bufSizeNeed, &attr );
	EXPECT_EQ(Success,ret);cout<<"renderMethod="<<renderMethod<<endl;
	CoutRet(ret);
}
TEST_F (T_AvPropertyRead,abNormalParaTest)
{
	//m_moduleHandle=NULL;
	//ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
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
TEST_F (T_AvPropertyRead,abNormalReCallTest)
{
	//m_moduleHandle=NULL;
	//ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
	ULONG renderMethod1 = 1,renderMethod2 = 2;
	ret = AvPropertyRead(m_moduleHandle, Para_RenderMethod, sizeof(renderMethod1), &renderMethod1, &bufSizeNeed, &attr );
	ret = AvPropertyRead(m_moduleHandle, Para_RenderMethod, sizeof(renderMethod2), &renderMethod2, &bufSizeNeed, &attr );
	EXPECT_EQ(Success,ret);
	EXPECT_EQ(renderMethod1,renderMethod2);
	CoutRet(ret);

}

/////////AvProperyWrite Test///////////////////////////////
class T_AvPropertyWrite: public testing::Test
{
protected:
	static void SetUpTestCase(){
		m_moduleHandle=NULL;
		ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
	}
	static void TearDownTestCase(){

	}
};

TEST_F (T_AvPropertyWrite,NormalTest)
{
	//m_moduleHandle=NULL;
	//ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
	ULONG renderMethod1 = VR_DirectDraw,renderMethod2 = 1;
	ret = AvPropertyWrite(m_moduleHandle, Para_RenderMethod, sizeof(renderMethod1), &renderMethod1, notifyNow );
	EXPECT_EQ(Success,ret);cout<<"renderMethod1="<<renderMethod1<<endl;
	CoutRet(ret);
	ret = AvPropertyRead(m_moduleHandle, Para_RenderMethod, sizeof(renderMethod2), &renderMethod2, &bufSizeNeed, &attr );
	EXPECT_EQ(renderMethod1,renderMethod2);

}
TEST_F (T_AvPropertyWrite,abNormalParaTest)
{
	//m_moduleHandle=NULL;
	//ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
	renderMethod = VR_DirectDraw;
	ret = AvPropertyWrite(NULL, Para_RenderMethod, sizeof(renderMethod), &renderMethod, notifyNow );
	EXPECT_EQ(ErrorHandleNotValid,ret);cout<<"handle=NULL";
	CoutRet(ret);
	ret = AvPropertyWrite(m_moduleHandle, 90, sizeof(renderMethod), &renderMethod, notifyNow );
	EXPECT_EQ(ErrorPropNotSpted,ret);cout<<"PropertyId=90 (0-77);";
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
TEST_F (T_AvPropertyWrite,abNormalReCallTest)
{
	//m_moduleHandle=NULL;
	//ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
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
	for ( int i = 0; i < bufSizeCur; i++ )
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
/////////Feature_FourCCs Test///////////////////////////////
ULONG fourCC = 0;
TEST (Feature_FourCCs,NormalReadTest)
{
	ret = -1;
	bufSizeCur = 0;
	bufSizeNeed = 0;
	notifyNow = 0;
	buffer = NULL;
	renderMethod = 0;
	m_moduleHandle=NULL;
	ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);

	ret = AvPropertyRead(m_moduleHandle, Feature_FourCCs, bufSizeCur, NULL, &bufSizeNeed, &attr );
	buffer = new BYTE[bufSizeNeed];
	bufSizeCur = bufSizeNeed;
	ret = AvPropertyRead(m_moduleHandle, Feature_FourCCs, bufSizeCur, buffer, &bufSizeNeed, &attr );
	EXPECT_EQ(Success,ret);
	cout<<"MAKEFOURCC('Y', 'V', '1', '2')="<<MAKEFOURCC('Y', 'V', '1', '2')<<endl;
	cout<<"Feature_FourCCs:";
	int bufValue=0;
	int j=0;
	for ( int i = 0; i < bufSizeCur; i++)
	{
		bufValue=bufValue|((ULONG)buffer[i] << j);
		j=i*8+8;
	}
	cout<<bufValue<<endl;
	EXPECT_EQ(MAKEFOURCC('Y', 'V', '1', '2'),bufValue);
	CoutRet(ret);

}
TEST (Feature_FourCCs,abNormalWriteTest)
{
	m_moduleHandle=NULL;
	ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
	fourCC = MAKEFOURCC('Y', 'V', '1', '2');
	ret = AvPropertyWrite(m_moduleHandle, Feature_FourCCs, sizeof(fourCC), &fourCC, notifyNow );
	EXPECT_EQ(ErrorPropReadOnly,ret)<<"Feature_FourCCs write value!";
	CoutRet(ret);
}

/////////Para_ RenderMethod Test///////////////////////////////
TEST (Para_RenderMethod,NormalReadTest)
{
	m_moduleHandle=NULL;
	ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
	renderMethod = 1;
	ret = AvPropertyRead(m_moduleHandle, Para_RenderMethod, sizeof(renderMethod), &renderMethod, &bufSizeNeed, &attr );
	EXPECT_EQ(Success,ret);
	CoutRet(ret);
	EXPECT_EQ(VR_DirectDraw,renderMethod);
	cout<<" Default renderMethod="<<renderMethod<<endl;

}
TEST (Para_RenderMethod,NormalWriteTest)
{
	m_moduleHandle=NULL;
	ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
	ULONG renderMethod1 = VR_DirectDraw,renderMethod2 = 1;
	ret = AvPropertyWrite(m_moduleHandle, Para_RenderMethod, sizeof(renderMethod1), &renderMethod1, notifyNow );
	EXPECT_EQ(Success,ret);
	CoutRet(ret);
	ret = AvPropertyRead(m_moduleHandle, Para_RenderMethod, sizeof(renderMethod2), &renderMethod2, &bufSizeNeed, &attr );
	EXPECT_EQ(renderMethod1,renderMethod2);
}
TEST (Para_RenderMethod,abNormalWriteTest)
{
	m_moduleHandle=NULL;
	ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
	ULONG renderMethod1 = 2,renderMethod2 = 2;
	ret = AvPropertyWrite(m_moduleHandle, Para_RenderMethod, sizeof(renderMethod1), &renderMethod1, notifyNow );
	EXPECT_EQ(ErrorPropValueNotSpted,ret);
	CoutRet(ret);
	ret = AvPropertyRead(m_moduleHandle, Para_RenderMethod, sizeof(renderMethod2), &renderMethod2, &bufSizeNeed, &attr );
	EXPECT_NE(renderMethod1,renderMethod2);cout<<"renderMethod2= "<<renderMethod2<<endl;
}

/////////Para_FourCC Test///////////////////////////////
TEST (Para_FourCC,NormalReadTest)
{
	m_moduleHandle=NULL;
	ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
	fourCC = 1;
	ret = AvPropertyRead(m_moduleHandle, Para_FourCC, sizeof(fourCC), &fourCC, &bufSizeNeed, &attr );
	EXPECT_EQ(Success,ret);
	CoutRet(ret);
	EXPECT_EQ(MAKEFOURCC('Y', 'V', '1', '2'),fourCC);
	cout<<" Default fourCC="<<fourCC<<endl;
	cout<<"MAKEFOURCC('Y', 'V', '1', '2')="<<MAKEFOURCC('Y', 'V', '1', '2')<<endl;

}
TEST (Para_FourCC,NormalWriteTest)
{
	m_moduleHandle=NULL;
	ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
	ULONG fourCC1 = MAKEFOURCC('Y', 'V', '1', '2'), fourCC2 = 1;
	ret = AvPropertyWrite(m_moduleHandle, Para_FourCC, sizeof(fourCC1), &fourCC1, notifyNow );
	EXPECT_EQ(Success,ret);
	CoutRet(ret);
	ret = AvPropertyRead(m_moduleHandle, Para_FourCC, sizeof(fourCC2), &fourCC2, &bufSizeNeed, &attr );
	EXPECT_EQ(MAKEFOURCC('Y', 'V', '1', '2'),fourCC2);


}
TEST (Para_FourCC,abNormalWriteTest)
{
	m_moduleHandle=NULL;
	ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
	EXPECT_EQ(Success,ret);
	ULONG fourCC1 = 2, fourCC2 = 2;
	ret = AvPropertyWrite(m_moduleHandle, Para_FourCC, sizeof(fourCC1), &fourCC1, notifyNow );
	EXPECT_EQ(ErrorPropValueNotSpted,ret);
	CoutRet(ret);
	ret = AvPropertyRead(m_moduleHandle, Para_RenderMethod, sizeof(fourCC2), &fourCC2, &bufSizeNeed, &attr );
	EXPECT_NE(fourCC1,fourCC2)<<"fourCC2= "<<fourCC2<<endl;
}
/////////Para_WindowHandle Test///////////////////////////////
TEST (Para_WindowHandle,NormalReadTest)
{
	m_moduleHandle=NULL;
	ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
	EXPECT_EQ(Success,ret);
	//openwindow();
	ULONG wndHandle= ULONG(hwnd);
	ret = AvPropertyRead(m_moduleHandle, Para_WindowHandle, sizeof(wndHandle), &wndHandle,NULL,0);
	EXPECT_EQ(Success,ret);
	CoutRet(ret);
	EXPECT_EQ(0,wndHandle);cout<<"Default wndHandle= "<<wndHandle<<endl;
}
TEST (Para_WindowHandle,NormalWriteTest)
{
	m_moduleHandle=NULL;
	ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
	EXPECT_EQ(Success,ret);
	//GetDlgItem(IDC_DISP_RND, &hwnd);
	ULONG wndHandle1=1001,wndHandle2=0;//= ULONG(hwnd);
	ret = AvPropertyWrite(m_moduleHandle, Para_WindowHandle, sizeof(wndHandle1), &wndHandle1, 0 );
	EXPECT_EQ(Success,ret);
	CoutRet(ret);
	ret = AvPropertyRead(m_moduleHandle, Para_WindowHandle, sizeof(wndHandle2), &wndHandle2,NULL,0);
	EXPECT_EQ(Success,ret);
	EXPECT_EQ(1001,wndHandle2)<<"read wndHandle= "<<wndHandle2<<endl;
}
TEST (Para_WindowHandle,abNormalWriteTest)
{
	m_moduleHandle=NULL;
	ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
	EXPECT_EQ(Success,ret);
	//GetDlgItem(IDC_DISP_RND, &hwnd);
	ULONG wndHandle1=0,wndHandle2=1;//= ULONG(hwnd);
	ret = AvPropertyWrite(m_moduleHandle, Para_WindowHandle, sizeof(wndHandle1), &wndHandle1, 0 );
	EXPECT_EQ(Success,ret);
	CoutRet(ret);
	ret = AvPropertyRead(m_moduleHandle, Para_WindowHandle, sizeof(wndHandle2), &wndHandle2,NULL,0);
	EXPECT_EQ(Success,ret);
	EXPECT_EQ(wndHandle1,wndHandle2)<<"read wndHandle= "<<wndHandle2<<endl;
}
/////////Para_ImageWidth Test///////////////////////////////
TEST(Para_WindowWidth,NormalReadTest)
{
	m_moduleHandle=NULL;
	ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
	EXPECT_EQ(Success,ret);
	ULONG width = FILE_WIDTH;
	ret = AvPropertyRead(m_moduleHandle,  Para_WindowWidth, sizeof(width), &width, NULL,0);
	EXPECT_EQ(Success,ret);
	CoutRet(ret);
	EXPECT_EQ(0,width);cout<<"Default width= "<<width<<endl;

}
TEST(Para_WindowWidth,NormalWriteTest)
{
	m_moduleHandle=NULL;
	ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
	EXPECT_EQ(Success,ret);
	ULONG width1 = FILE_WIDTH, width2 = 1;
	ret = AvPropertyWrite(m_moduleHandle, Para_WindowWidth, sizeof(width1), &width1, 0 );
	EXPECT_EQ(Success,ret);
	CoutRet(ret);
	ret = AvPropertyRead(m_moduleHandle,  Para_WindowWidth, sizeof(width2), &width2, NULL,0);
	EXPECT_EQ(width1,width2);
}
/////////Para_WindowHeight Test///////////////////////////////
TEST(Para_WindowHeight,NormalReadTest)
{
	m_moduleHandle=NULL;
	ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
	EXPECT_EQ(Success,ret);
	ULONG height = FILE_HEIGHT;
	ret = AvPropertyRead(m_moduleHandle,  Para_WindowHeight, sizeof(height), &height, NULL,0);
	EXPECT_EQ(Success,ret);
	CoutRet(ret);
	EXPECT_EQ(0,height);cout<<"Default height= "<<height<<endl;
}
TEST(Para_WindowHeight,NormalWriteTest)
{
	m_moduleHandle=NULL;
	ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
	EXPECT_EQ(Success,ret);
	ULONG height1 = FILE_HEIGHT, height2 = 1;
	ret = AvPropertyWrite(m_moduleHandle, Para_WindowHeight, sizeof(height1), &height1, 0 );
	EXPECT_EQ(Success,ret);
	CoutRet(ret);
	ret = AvPropertyRead(m_moduleHandle,  Para_WindowHeight, sizeof(height2), &height2, NULL,0);
	EXPECT_EQ(height1,height2);
}
/////////AvVideoRenderPrepare Test///////////////////////////////

TEST(AvVideoRenderPrepare,NormalTest)
{
	m_moduleHandle=NULL;
	ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
	EXPECT_EQ(Success,ret);
	///set para///////////////
	ULONG wndHandle = ULONG(hwnd);

	ret = AvPropertyWrite(m_moduleHandle, Para_WindowHandle, sizeof(wndHandle), &wndHandle, 0 );
	EXPECT_EQ(Success,ret)<<"Para_WindowHandle write failed!";
	ULONG width = FILE_WIDTH;
	ret = AvPropertyWrite(m_moduleHandle, Para_WindowWidth, sizeof(width), &width, 0 );
	EXPECT_EQ(Success,ret)<<"Para_WindowWidth write failed!";
	ULONG height = FILE_HEIGHT;
	ret = AvPropertyWrite(m_moduleHandle, Para_WindowHeight, sizeof(height), &height, 0 );
	EXPECT_EQ(Success,ret)<<"Para_WindowHeight write failed!";
	/////////////////
	ret = 1;
	ret = AvVideoRenderPrepare(m_moduleHandle);
	EXPECT_EQ(Success,ret);
	CoutRet(ret);
}
TEST(AvVideoRenderPrepare,abNormalParaTest)
{
	m_moduleHandle=NULL;
	//ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
	//EXPECT_EQ(Success,ret);
	ret = 1;
	ret = AvVideoRenderPrepare(m_moduleHandle);
	EXPECT_EQ(Success,ret);
	CoutRet(ret);
}
TEST(AvVideoRenderPrepare,abNormalNosetParaTest)
{
	m_moduleHandle=NULL;
	ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
	EXPECT_EQ(Success,ret);
	//ULONG wndHandle = ULONG(hwnd);
	//ret = AvPropertyWrite(m_moduleHandle, Para_WindowHandle, sizeof(wndHandle), &wndHandle, 0 );
	//EXPECT_EQ(Success,ret)<<"Para_WindowHandle write failed!";
	//ULONG width = FILE_WIDTH;
	//ret = AvPropertyWrite(m_moduleHandle, Para_WindowWidth, sizeof(width), &width, 0 );
	//EXPECT_EQ(Success,ret)<<"Para_WindowWidth write failed!";
	//ULONG height = FILE_HEIGHT;
	//ret = AvPropertyWrite(m_moduleHandle, Para_WindowHeight, sizeof(height), &height, 0 );
	//EXPECT_EQ(Success,ret)<<"Para_WindowHeight write failed!";
	ret = 1;
	ret = AvVideoRenderPrepare(m_moduleHandle);
	EXPECT_EQ(Success,ret);
	CoutRet(ret);
}
//TEST(AvVideoRenderPrepare,abNormalOrderTest)
//{
//	m_moduleHandle=NULL;
//
//	ret = 1;
//	ret = AvVideoRenderPrepare(m_moduleHandle);
//	EXPECT_EQ(Success,ret);
//	CoutRet(ret);
//}
TEST(AvVideoRenderPrepare,abNormalAfterModuleCloseTest)
{
	m_moduleHandle=NULL;
	ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
	EXPECT_EQ(Success,ret);
	ret = AvModuleClose(m_moduleHandle);
	ret = 1;
	ret = AvVideoRenderPrepare(m_moduleHandle);
	EXPECT_EQ(ErrorHandleNotValid,ret);
	CoutRet(ret);

}
TEST(AvVideoRenderPrepare,abNormalReCallTest)
{
	m_moduleHandle=NULL;
	ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
	EXPECT_EQ(Success,ret);
	///////set para///////////////

	ULONG wndHandle = ULONG(hwnd);
	ret = AvPropertyWrite(m_moduleHandle, Para_WindowHandle, sizeof(wndHandle), &wndHandle, 0 );
	EXPECT_EQ(Success,ret)<<"Para_WindowHandle write failed!";
	ULONG width = FILE_WIDTH;
	ret = AvPropertyWrite(m_moduleHandle, Para_WindowWidth, sizeof(width), &width, 0 );
	EXPECT_EQ(Success,ret)<<"Para_WindowWidth write failed!";
	ULONG height = FILE_HEIGHT;
	ret = AvPropertyWrite(m_moduleHandle, Para_WindowHeight, sizeof(height), &height, 0 );
	EXPECT_EQ(Success,ret)<<"Para_WindowHeight write failed!";
	///////////////////
	ret = 1;
	ret = AvVideoRenderPrepare(m_moduleHandle);
	EXPECT_EQ(Success,ret);
	CoutRet(ret);
	ret = AvVideoRenderPrepare(m_moduleHandle);
	EXPECT_EQ(Success,ret);
	CoutRet(ret);
}
///////////AvVideoRender Test///////////////////////////////
class T_AvVideoRender: public testing::Test
{
protected:
	static void SetUpTestCase(){
		m_moduleHandle=NULL;
		ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
		EXPECT_EQ(Success,ret);
		EXPECT_NE((HANDLE)NULL,m_moduleHandle);

		///set para///////////////
		//////////////////////////
		openwindow();
		//////////////////////////
		ULONG wndHandle = ULONG(hwnd);
		ret = AvPropertyWrite(m_moduleHandle, Para_WindowHandle, sizeof(wndHandle), &wndHandle, 0 );
		EXPECT_EQ(Success,ret)<<"Para_WindowHandle write failed!";
		ULONG width = FILE_WIDTH;
		ret = AvPropertyWrite(m_moduleHandle, Para_WindowWidth, sizeof(width), &width, 0 );
		EXPECT_EQ(Success,ret)<<"Para_WindowWidth write failed!";
		ULONG height = FILE_HEIGHT;
		ret = AvPropertyWrite(m_moduleHandle, Para_WindowHeight, sizeof(height), &height, 0 );
		EXPECT_EQ(Success,ret)<<"Para_WindowHeight write failed!";

		ret = 1;
		ret = AvVideoRenderPrepare(m_moduleHandle);
		EXPECT_EQ(Success,ret)<<"AvVideoRenderPrepare failed!";CoutRet(ret);
		///////////////////
		openImage();
		/////////////////////
	}
	static void TearDownTestCase(){
		ret = 1;
		ret = AvVideoRenderRelease(m_moduleHandle);
	}
};
TEST_F (T_AvVideoRender,NormalTest)
{
	ret = 1;
	ret = AvVideoRender(m_moduleHandle, m_buffer, m_bufLen);
	EXPECT_EQ(Success,ret)<<"AvVideoRender failed!";
	CoutRet(ret);
}
TEST_F (T_AvVideoRender,abNormalAfterReleaseTest)
{
	ret = 1;
	ret = AvVideoRenderRelease(m_moduleHandle);
	EXPECT_EQ(Success,ret);
	ret = 1;
	ret = AvVideoRender(m_moduleHandle, m_buffer, m_bufLen);
	EXPECT_EQ(ErrorFuncNotInited,ret)<<"AvVideoRender failed!";
	CoutRet(ret);
}
TEST_F (T_AvVideoRender,abNormalHandleNULLTest)
{
	ret = 1;
	ret = AvVideoRender(NULL, m_buffer, m_bufLen);
	EXPECT_EQ(ErrorHandleNotValid,ret)<<"m_moduleHandle=NULL!";
}
TEST_F (T_AvVideoRender,abNormalBufferNULLTest)
{
	ret = 1;
	ret = AvVideoRender(m_moduleHandle,NULL,m_bufLen);
	EXPECT_EQ(ErrorBufferIsNull,ret)<<"m_buffer=NULL!";
	CoutRet(ret);
}
TEST_F (T_AvVideoRender,abNormalBufLenZeroTest)
{
	ret = 1;
	ret = AvVideoRender(m_moduleHandle,m_buffer,0);
	EXPECT_EQ(Success,ret)<<"m_bufLen=0!";
	CoutRet(ret);
}
TEST (AvVideoRender,abNormalNoPrepareTest)
{ 
	m_moduleHandle=NULL;
	ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
	EXPECT_EQ(Success,ret);
	EXPECT_NE((HANDLE)NULL,m_moduleHandle);

	//////////////////////////
	openwindow();
	//////////////////////////
	openImage();
	ret = 1;
	ret = AvVideoRender(m_moduleHandle,m_buffer,m_bufLen);
	EXPECT_EQ(ErrorFuncNotInited,ret)<<"no prepare!";
	CoutRet(ret);
}
TEST (AvVideoRender,abNormalReCallTest)
{
	m_moduleHandle=NULL;
	ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
	EXPECT_EQ(Success,ret);
	EXPECT_NE((HANDLE)NULL,m_moduleHandle);

	///set para///////////////
	//////////////////////////
	//openwindow();
	WCHAR wndClass[] = L"VideoRender"; 
	WCHAR title[] = L"Advantech Video Render";
	WNDCLASSEX wcx = {0}; 
	wcx.cbSize = sizeof(wcx);          
	wcx.style = CS_HREDRAW | CS_VREDRAW; 
	wcx.lpfnWndProc = (WNDPROC)DefWindowProc;      
	wcx.cbClsExtra = 0;               
	wcx.cbWndExtra = 0;               
	wcx.hInstance = NULL;        
	wcx.hIcon = NULL;
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);   
	wcx.hbrBackground = (HBRUSH)COLOR_WINDOWFRAME;    
	wcx.lpszMenuName = NULL;    
	wcx.lpszClassName = wndClass;
	RegisterClassEx(&wcx); 
	ULONG imageWidth = 176;
	ULONG imageHeight = 144;

	hwnd = CreateWindow( wndClass, title, WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, imageWidth, imageHeight, NULL, NULL, NULL, NULL );
	//////////////////////////
	ULONG wndHandle = ULONG(hwnd);
	ret = AvPropertyWrite(m_moduleHandle, Para_WindowHandle, sizeof(wndHandle), &wndHandle, 0 );
	EXPECT_EQ(Success,ret)<<"Para_WindowHandle write failed!";
	ULONG width = 176;
	ret = AvPropertyWrite(m_moduleHandle, Para_WindowWidth, sizeof(width), &width, 0 );
	EXPECT_EQ(Success,ret)<<"Para_WindowWidth write failed!";
	ULONG height = 144;
	ret = AvPropertyWrite(m_moduleHandle, Para_WindowHeight, sizeof(height), &height, 0 );
	EXPECT_EQ(Success,ret)<<"Para_WindowHeight write failed!";

	ret = 1;
	ret = AvVideoRenderPrepare(m_moduleHandle);
	EXPECT_EQ(Success,ret)<<"AvVideoRenderPrepare failed!";CoutRet(ret);
	///////////////////

	if (m_buffer)
	{
		delete [] m_buffer;
	}
	m_bufLen = 176 * 144 * 3 / 2;
	m_buffer = new BYTE[m_bufLen];
	memset(m_buffer, 0, m_bufLen);
    
	string str,str1,str3;
	stringstream ss;
	str1="D:\\Video team\\VideoSDK\\Test\\resources\\YV12_176_144\\carphone0";
	str3=".yuv";
	const char *fname;
	char str2[25];
	for (int i=10;i<=96;i++)
	{	

		sprintf (str2,"%d",i);
		str=str1+str2+str3;
		fname= str.c_str();
		
        //AfxMessageBox(L"NEXT");
	    Sleep(100);
	
		FILE * f = fopen(fname, "rb");
		if ( f == NULL )
		{
			AfxMessageBox(L"Can't open file");
			return;
		}
		UINT len = fread(m_buffer, 1, m_bufLen, f );

		fclose(f);
		ret = 1;
		ret = AvVideoRender(m_moduleHandle,m_buffer,m_bufLen);
		EXPECT_EQ(Success,ret);
	}//end for
}
///////////AvVideoRenderRelease Test///////////////////////////////
class T_AvVideoRenderRelease: public testing::Test
{
protected:
	static void SetUpTestCase(){
		m_moduleHandle=NULL;
		ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
		EXPECT_EQ(Success,ret);
		EXPECT_NE((HANDLE)NULL,m_moduleHandle);

		///set para///////////////
		//////////////////////////
		openwindow();
		//////////////////////////
		ULONG wndHandle = ULONG(hwnd);
		ret = AvPropertyWrite(m_moduleHandle, Para_WindowHandle, sizeof(wndHandle), &wndHandle, 0 );
		EXPECT_EQ(Success,ret)<<"Para_WindowHandle write failed!";
		ULONG width = FILE_WIDTH;
		ret = AvPropertyWrite(m_moduleHandle, Para_WindowWidth, sizeof(width), &width, 0 );
		EXPECT_EQ(Success,ret)<<"Para_WindowWidth write failed!";
		ULONG height = FILE_HEIGHT;
		ret = AvPropertyWrite(m_moduleHandle, Para_WindowHeight, sizeof(height), &height, 0 );
		EXPECT_EQ(Success,ret)<<"Para_WindowHeight write failed!";

		ret = 1;
		ret = AvVideoRenderPrepare(m_moduleHandle);
		EXPECT_EQ(Success,ret)<<"AvVideoRenderPrepare failed!";CoutRet(ret);
		///////////////////
		openImage();
		/////////////////////
		ret = 1;
		ret = AvVideoRender(m_moduleHandle,m_buffer,m_bufLen);
		EXPECT_EQ(Success,ret);
	}
	static void TearDownTestCase(){
	}
};
TEST_F (T_AvVideoRenderRelease,NormalTest)
{
	ret = 1;
	ret = AvVideoRenderRelease(m_moduleHandle);
	EXPECT_EQ(Success,ret)<<"AvVideoRenderRelease failed!";
	CoutRet(ret);
}
TEST_F (T_AvVideoRenderRelease,abNormalReCallTest)
{
	ret = 1;
	ret = AvVideoRenderRelease(m_moduleHandle);
	EXPECT_EQ(Success,ret)<<"first AvVideoRenderRelease failed!";
	ret = 1;
	ret = AvVideoRenderRelease(m_moduleHandle);
	EXPECT_EQ(Success,ret)<<"second AvVideoRenderRelease failed!";
	CoutRet(ret);
}
TEST (AvVideoRenderRelease,abNormalParaTest)
{
	ret = 1;
	ret = AvVideoRenderRelease(NULL);
	EXPECT_EQ(Success,ret)<<"handle is NULL";
	CoutRet(ret);
}
TEST (AvVideoRenderRelease,abNormalAfterModuleCloseTest)
{
	ret= AvModuleClose(m_moduleHandle);
	ret = 1;
	ret = AvVideoRenderRelease(m_moduleHandle);
	EXPECT_EQ(ErrorHandleNotValid,ret)<<"handle is NULL";
	CoutRet(ret);
}
TEST (AvVideoRenderRelease,abNormalNoprepareTest)
{
	m_moduleHandle = NULL;
	ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
	EXPECT_EQ(Success,ret)<<"Module open failed!";

	ret = 1;
	ret = AvVideoRenderRelease(m_moduleHandle);
	EXPECT_EQ(Success,ret)<<"no prepare!";
	CoutRet(ret);
}
TEST (AvVideoRenderRelease,abNormalNoRenderTest)
{
	m_moduleHandle=NULL;
	ret = AvModuleOpen(Av_VideoRender,&m_moduleHandle);
	EXPECT_EQ(Success,ret);
	///set para///////////////
	ULONG wndHandle = ULONG(hwnd);

	ret = AvPropertyWrite(m_moduleHandle, Para_WindowHandle, sizeof(wndHandle), &wndHandle, 0 );
	EXPECT_EQ(Success,ret)<<"Para_WindowHandle write failed!";
	ULONG width = FILE_WIDTH;
	ret = AvPropertyWrite(m_moduleHandle, Para_WindowWidth, sizeof(width), &width, 0 );
	EXPECT_EQ(Success,ret)<<"Para_WindowWidth write failed!";
	ULONG height = FILE_HEIGHT;
	ret = AvPropertyWrite(m_moduleHandle, Para_WindowHeight, sizeof(height), &height, 0 );
	EXPECT_EQ(Success,ret)<<"Para_WindowHeight write failed!";
	/////////////////
	ret = 1;
	ret = AvVideoRenderPrepare(m_moduleHandle);
	EXPECT_EQ(Success,ret)<<"AvVideoRenderPrepare failed!";
	ret = 1;
	ret = AvVideoRenderRelease(m_moduleHandle);
	EXPECT_EQ(Success,ret)<<"AvVideoRenderRelease failed!!";
	CoutRet(ret);
}
#endif  //test code end