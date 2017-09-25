// DlgSmooth.cpp : implementation file
//

#include "stdafx.h"
#include "UI-SingleDocument.h"
#include "DlgSmooth.h"
#include "afxdialogex.h"


// CDlgSmooth dialog

IMPLEMENT_DYNAMIC(CDlgSmooth, CDialogEx)

CDlgSmooth::CDlgSmooth(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_SMOOTH, pParent)
{
	m_pApp = (CUISingleDocumentApp*)AfxGetApp();
}

CDlgSmooth::~CDlgSmooth()
{
}

void CDlgSmooth::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SMOOTH, m_comboSmoothType);
}


BEGIN_MESSAGE_MAP(CDlgSmooth, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CDlgSmooth::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_PROCESS, &CDlgSmooth::OnBnClickedButtonProcess)
END_MESSAGE_MAP()


// CDlgSmooth message handlers


BOOL CDlgSmooth::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_comboSmoothType.AddString(_T("高斯滤波"));
	m_comboSmoothType.AddString(_T("均值滤波"));
	m_comboSmoothType.AddString(_T("中值滤波"));
	m_comboSmoothType.AddString(_T("双边滤波"));
	m_comboSmoothType.SetCurSel(0);

	SetTimer(1, 10, NULL);	// 每隔10ms调用一次OnTimer()函数
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgSmooth::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	JZSmoothParam smoothParam;
	smoothParam.processType = JZ_IMAGE_BASEPROCESS;
	smoothParam.baseProcessType = JZ_BASEPROCESS_SMOOTH;
	smoothParam.smoothType = (JZ_SMOOTH_TYPE)m_comboSmoothType.GetCurSel();
	m_pApp->m_pUIEngine->SetProcessParam(&smoothParam);
	CDialogEx::OnTimer(nIDEvent);
}


void CDlgSmooth::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void CDlgSmooth::OnBnClickedButtonProcess()
{
	// TODO: Add your control notification handler code here
	m_pApp->m_pUIEngine->ProcessImage(JZ_IMAGE_BASEPROCESS, JZ_BASEPROCESS_SMOOTH);
	CView* pView = (CView*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveView();
	pView->Invalidate();
}
