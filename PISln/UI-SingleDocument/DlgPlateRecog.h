#pragma once
#include "afxwin.h"


// CDlgPlateRecog dialog

class CDlgPlateRecog : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPlateRecog)

public:
	CDlgPlateRecog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPlateRecog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PLATERECOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CUISingleDocumentApp* m_pApp;
	afx_msg void OnBnClickedButtonPlaterecog();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEnChangeEditPlatenum();
	CEdit m_editPlateNum;
};
