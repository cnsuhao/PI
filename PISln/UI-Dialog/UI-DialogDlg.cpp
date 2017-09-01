
// UI-DialogDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UI-Dialog.h"
#include "UI-DialogDlg.h"
#include "afxdialogex.h"
#include <IJZUIEngineProc.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CUIDialogDlg �Ի���



CUIDialogDlg::CUIDialogDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_UIDIALOG_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pApp = (CUIDialogApp*)AfxGetApp();
}

void CUIDialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUIDialogDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SMOOTH, &CUIDialogDlg::OnBnClickedButtonSmooth)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_PROCESS, &CUIDialogDlg::OnBnClickedButtonProcess)
	ON_COMMAND(ID_MENU_OPEN, &CUIDialogDlg::OnMenuOpen)
	ON_COMMAND(ID_MENU_SAVE, &CUIDialogDlg::OnMenuSave)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CUIDialogDlg ��Ϣ�������

BOOL CUIDialogDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	GetClientRect(m_rect);
	m_menu.LoadMenu(IDR_MENU);
	SetMenu(&m_menu);

	SetTimer(1, 10, NULL);	// ÿ��10ms����һ��OnTimer()����
	CWnd* cwnd = GetDlgItem(IDC_STATIC_PIC);
	g_JZUIEngineAPI->pfnGetInterface(&m_pApp->m_pUIEngine);
	m_pApp->m_pUIEngine->Init(cwnd->m_hWnd);

	return FALSE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CUIDialogDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CUIDialogDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();		
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CUIDialogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUIDialogDlg::ChangeSize(CWnd* pCWnd, int cx, int cy)
{
	if (NULL != pCWnd)
	{
		CRect rect;
		pCWnd->GetWindowRect(&rect);// ��ȡ�ؼ��仯ǰ��С
		ScreenToClient(&rect);	
		rect.left = rect.left * (cx / m_rect.Width());
		rect.right = rect.right * (cx / m_rect.Width());
		rect.top = rect.top * (cy / m_rect.Height());
		rect.bottom = rect.bottom * (cy / m_rect.Height());
		pCWnd->MoveWindow(&rect);
	}
}

void CUIDialogDlg::OnBnClickedButtonSmooth()	// ͨ���˰�ť��ͼ��ģ�������������
{
	// TODO: Add your control notification handler code here
	// ģ̬�Ի���
	if (NULL == m_pSmoothDialog)
	{
		m_pSmoothDialog = new CDialogSmooth();
	}
	m_pSmoothDialog->DoModal();

	// ��ģ̬�Ի���
	//if (NULL == m_pSmoothDialog)
	//{
	//	m_pSmoothDialog = new CDialogSmooth();
	//	m_pSmoothDialog->Create(IDD_DIALOG_SMOOTH, this);
	//}
	//m_pSmoothDialog->ShowWindow(SW_SHOW);

	m_pApp->m_pUIEngine->SetCurProcessType(JZ_IMAGE_SMOOTH);
}

void CUIDialogDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
	if (NULL != m_pSmoothDialog)
	{
		delete m_pSmoothDialog;
		m_pSmoothDialog = NULL;
	}
	m_pApp->m_pUIEngine->Release();
}

void CUIDialogDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	m_pApp->m_pUIEngine->Render();
	CDialogEx::OnTimer(nIDEvent);
}

void CUIDialogDlg::OnBnClickedButtonProcess()
{
	// TODO: Add your control notification handler code here
	m_pApp->m_pUIEngine->ProcessImage();
}


void CUIDialogDlg::OnMenuOpen()
{
	// TODO: Add your command handler code here
	LPCTSTR lpszFilter = _T("ͼ���ļ�(*.jpg)|*.jpg|�����ļ�(*.*)|*.*|");
	CFileDialog fileDlg(TRUE, _T("jpg"), 0, 0, lpszFilter, this);

	INT_PTR response = fileDlg.DoModal();
	if (IDOK == response)
	{
		CString cstrFilePath = fileDlg.GetPathName();
		TCHAR* tfilename = cstrFilePath.GetBuffer(0);
		char filename[MAX_PATH] = { 0 };
#ifdef UNICODE
		int len = WideCharToMultiByte(CP_ACP, 0, tfilename, -1, NULL, 0, NULL, NULL);
		WideCharToMultiByte(CP_ACP, 0, tfilename, -1, filename, len, NULL, NULL);
#else
		strcpy(filename, tfilename);
#endif // UNICODE
		m_pApp->m_pUIEngine->SetImageData(filename);
	}
	else if (IDCANCEL == response)
	{

	}
}


void CUIDialogDlg::OnMenuSave()
{
	// TODO: Add your command handler code here
	LPCTSTR lpszFilter = _T("ͼ���ļ�(*.jpg)|*.jpg|�����ļ�(*.*)|*.*|");
	CFileDialog fileDlg(FALSE, _T("jpg"), _T("SaveImage"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszFilter, this);

	INT_PTR response = fileDlg.DoModal();
	if (IDOK == response)
	{
		CString cstrFilePath = fileDlg.GetPathName();
		TCHAR* tfilename = cstrFilePath.GetBuffer(0);
		char filename[MAX_PATH] = { 0 };
#ifdef UNICODE
		int len = WideCharToMultiByte(CP_ACP, 0, tfilename, -1, NULL, 0, NULL, NULL);
		WideCharToMultiByte(CP_ACP, 0, tfilename, -1, filename, len, NULL, NULL);
#else
		strcpy(filename, tfilename);
#endif // UNICODE

		m_pApp->m_pUIEngine->SaveImageData(filename);
	}

	else if (IDCANCEL == response)
	{
		int z = 3;
	}
}


void CUIDialogDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (1 == nType)	// �������С����ʲô������
	{
		return;
	}

	CWnd* pCWnd;
	/*pCWnd = GetDlgItem(IDC_STATIC_PIC);
	ChangeSize(pCWnd, cx, cy);*/
	/*pCWnd = GetDlgItem(IDC_BUTTON_SMOOTH);
	ChangeSize(pCWnd, cx, cy);*/
	/*pCWnd = GetDlgItem(IDC_BUTTON_PROCESS);
	ChangeSize(pCWnd, cx, cy);*/

	GetClientRect(&m_rect);
}
