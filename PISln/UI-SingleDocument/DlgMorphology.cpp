// DlgMorphology.cpp : implementation file
//

#include "stdafx.h"
#include "UI-SingleDocument.h"
#include "DlgMorphology.h"
#include "afxdialogex.h"
#include <IJZLogProc.h>
#define UI_DM_LOG_FILENAME _T("DlgMorphology.cpp")
// CDlgMorphology dialog

IMPLEMENT_DYNAMIC(CDlgMorphology, CDialogEx)

CDlgMorphology::CDlgMorphology(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_MORPHOLOGY, pParent)
	, m_iEditMorphWidth(0)
	, m_iEditMorphHeight(0)
{
	m_pApp = (CUISingleDocumentApp*)AfxGetApp();
}

CDlgMorphology::~CDlgMorphology()
{
}

void CDlgMorphology::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_MORPHTYPE, m_comboMorphType);
	DDX_Control(pDX, IDC_COMBO_MORPHSHAPE, m_comboMorphShape);
	DDX_Text(pDX, IDC_EDIT_MORPH_W, m_iEditMorphWidth);
	DDX_Text(pDX, IDC_EDIT_MORPH_H, m_iEditMorphHeight);
}


BEGIN_MESSAGE_MAP(CDlgMorphology, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgMorphology::OnBnClickedOk)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_PROCESS, &CDlgMorphology::OnBnClickedButtonProcess)
END_MESSAGE_MAP()


// CDlgMorphology message handlers


void CDlgMorphology::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


BOOL CDlgMorphology::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	// ��̬ѧ�������ͳ�ʼ��
	m_comboMorphType.AddString(_T("��ʴ")); 
	m_comboMorphType.AddString(_T("����"));
	m_comboMorphType.AddString(_T("������")); 
	m_comboMorphType.AddString(_T("������")); 
	m_comboMorphType.AddString(_T("��̬ѧ�ݶ�"));
	m_comboMorphType.AddString(_T("��ñ")); 
	m_comboMorphType.AddString(_T("��ñ")); 
	m_comboMorphType.AddString(_T("���л�����")); 
	m_comboMorphType.SetCurSel(0);

	// ��̬ѧ�ṹԪ����״�������ͳ�ʼ��
	m_comboMorphShape.AddString(_T("����"));
	m_comboMorphShape.AddString(_T("ʮ�ֽ�����"));
	m_comboMorphShape.AddString(_T("��Բ��"));
	m_comboMorphShape.SetCurSel(0);
	SetTimer(1, 10, NULL);	// ÿ��10ms����һ��OnTimer()����
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgMorphology::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	JZMorphologyParam morphologyParam;
	morphologyParam.processType		= JZ_IMAGE_BASEPROCESS;
	morphologyParam.baseProcessType = JZ_BASEPROCESS_MORPHOLOGY;
	morphologyParam.morphologyType	= (JZ_MORPHOLOGY_TYPE)m_comboMorphType.GetCurSel();
	morphologyParam.morphologyShape = (JZ_MORPHOLOGY_SHAPE)m_comboMorphShape.GetCurSel();
	UpdateData(TRUE);
	morphologyParam.width			= m_iEditMorphWidth;
	morphologyParam.height			= m_iEditMorphHeight;

	m_pApp->m_pUIEngine->SetProcessParam(&morphologyParam);
	CDialogEx::OnTimer(nIDEvent);
}


void CDlgMorphology::OnBnClickedButtonProcess()
{
	// TODO: Add your control notification handler code here
	JZ_RESULT res = m_pApp->m_pUIEngine->ProcessImage(JZ_IMAGE_BASEPROCESS, JZ_BASEPROCESS_MORPHOLOGY);
	if (JZ_INVAILD_PARAM == res)
	{
		// ��ʾ��Ϣ�Ի���   
		INT_PTR nRes = MessageBox(_T("��Ч�����������ߣ����������룡"), _T("����"), MB_OK);
		g_JZLogAPI->pfnWriteLog(JZ_LOG_TYPE_WARNING, UI_DM_LOG_FILENAME, _T("CDlgMorphology::OnBnClickedButtonProcess"), _T("%s"), _T("��̬ѧ������Ч�ĽṹԪ�ؿ��"));
		return;
	}
	CView* pView = (CView*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveView();
	pView->Invalidate();
}
