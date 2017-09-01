#pragma once
#include "afxwin.h"


// CDialogSmooth dialog

class CDialogSmooth : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogSmooth)

public:
	CDialogSmooth(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogSmooth();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SMOOTH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
	CUIDialogApp* m_pApp;
	CComboBox m_comboSmoothType;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
