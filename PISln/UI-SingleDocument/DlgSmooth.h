#pragma once
#include "afxwin.h"


// CDlgSmooth dialog

class CDlgSmooth : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSmooth)

public:
	CDlgSmooth(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSmooth();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SMOOTH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_comboSmoothType;
	CUISingleDocumentApp* m_pApp;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonProcess();
};
