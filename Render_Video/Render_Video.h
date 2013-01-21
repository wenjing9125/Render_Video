// Render_Video.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CRender_VideoApp:
// See Render_Video.cpp for the implementation of this class
//

class CRender_VideoApp : public CWinApp
{
public:
	CRender_VideoApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CRender_VideoApp theApp;