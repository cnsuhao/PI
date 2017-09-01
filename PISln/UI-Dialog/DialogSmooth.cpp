// DialogSmooth.cpp : implementation file
//

#include "stdafx.h"
#include "UI-Dialog.h"
#include "DialogSmooth.h"
#include "afxdialogex.h"


// CDialogSmooth dialog

IMPLEMENT_DYNAMIC(CDialogSmooth, CDialogEx)

CDialogSmooth::CDialogSmooth(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_SMOOTH, pParent)
{
	m_pApp = (CUIDialogApp*)AfxGetApp();
}

CDialogSmooth::~CDialogSmooth()
{
}

void CDialogSmooth::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SMOOTH_TYPE, m_comboSmoothType);
}


BEGIN_MESSAGE_MAP(CDialogSmooth, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDialogSmooth::OnBnClickedOk)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDialogSmooth message handlers


BOOL CDialogSmooth::OnInitDialog()
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


void CDialogSmooth::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void CDialogSmooth::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	JZSmoothParam smoothParam;
	smoothParam.processType = JZ_IMAGE_SMOOTH;
	smoothParam.smoothType = (JZ_SMOOTH_TYPE)m_comboSmoothType.GetCurSel();
	m_pApp->m_pUIEngine->SetProcessParam(&smoothParam);
	CDialogEx::OnTimer(nIDEvent);
}
