#pragma once
#include "afxwin.h"


// CDlgMorphology dialog

class CDlgMorphology : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMorphology)

public:
	CDlgMorphology(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgMorphology();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MORPHOLOGY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CUISingleDocumentApp* m_pApp;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonProcess();
	CComboBox m_comboMorphType;
	CComboBox m_comboMorphShape;
	int m_iEditMorphWidth;
	int m_iEditMorphHeight;
};
