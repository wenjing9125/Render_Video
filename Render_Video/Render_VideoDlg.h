// Render_VideoDlg.h : header file
//

#pragma once


// CRender_VideoDlg dialog
class CRender_VideoDlg : public CDialog
{
// Construction
public:
	CRender_VideoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_RENDER_VIDEO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
////ADV_SDK////////////////////////////////////////////////////////
private:
	LONG AdditionalPropertyAccess();
private:
	HANDLE m_moduleHandle;
	PBYTE  m_buffer;
	ULONG  m_bufLen;
///////////////////////////////////////////////////////////////////
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
};
