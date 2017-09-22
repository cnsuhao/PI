// DlgPlateRecog.cpp : implementation file
//

#include "stdafx.h"
#include "UI-SingleDocument.h"
#include "DlgPlateRecog.h"
#include "afxdialogex.h"


// CDlgPlateRecog dialog

IMPLEMENT_DYNAMIC(CDlgPlateRecog, CDialogEx)

CDlgPlateRecog::CDlgPlateRecog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_PLATERECOG, pParent)
{
	m_pApp = (CUISingleDocumentApp*)AfxGetApp();
}

CDlgPlateRecog::~CDlgPlateRecog()
{
}

void CDlgPlateRecog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PLATENUM, m_editPlateNum);
}


BEGIN_MESSAGE_MAP(CDlgPlateRecog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_PLATERECOG, &CDlgPlateRecog::OnBnClickedButtonPlaterecog)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_PLATENUM, &CDlgPlateRecog::OnEnChangeEditPlatenum)
END_MESSAGE_MAP()


// CDlgPlateRecog message handlers


void CDlgPlateRecog::OnBnClickedButtonPlaterecog()
{
	// TODO: Add your control notification handler code here
	m_pApp->m_pUIEngine->ProcessImage();
	JZImageProcessExtraData* pExtraData = m_pApp->m_pUIEngine->GetExtraData();
	for (int i = 0; i < pExtraData->vecPlateStrs.size(); i++)
	{
		const char* plateStr = pExtraData->vecPlateStrs[i].c_str();
		TCHAR tPlatStr[MAX_PATH] = { 0 };
#ifdef UNICODE
		//int len = MultiByteToWideChar(CP_ACP, 0, plateStr, -1, NULL, 0, NULL, NULL);
		MultiByteToWideChar(CP_ACP, 0, plateStr, -1, tPlatStr, MAX_PATH);
#else
		strcpy(tPlatStr, plateStr);
#endif // UNICODE
		m_editPlateNum.SetWindowText(tPlatStr);
	}

	CView* pView = (CView*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveView();
	pView->Invalidate();
}


BOOL CDlgPlateRecog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	SetTimer(1, 10, NULL);	// 每隔10ms调用一次OnTimer()函数
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgPlateRecog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	JZPlateRecogParam plateRecogParam;
	m_pApp->m_pUIEngine->SetProcessParam(&plateRecogParam);
	CDialogEx::OnTimer(nIDEvent);
}


void CDlgPlateRecog::OnEnChangeEditPlatenum()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
