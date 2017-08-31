
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
}

void CUIDialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUIDialogDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CUIDialogDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_SMOOTH, &CUIDialogDlg::OnBnClickedButtonSmooth)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CUIDialogDlg::OnBnClickedButtonSave)
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
	CWnd* cwnd = GetDlgItem(IDC_STATIC_PIC);
	m_pUIEngine = NULL;
	g_JZUIEngineAPI->pfnGetInterface(&m_pUIEngine);
	m_pUIEngine->Init(cwnd->m_hWnd);

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
		//CDialogEx::OnPaint();
		m_pUIEngine->Render();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CUIDialogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUIDialogDlg::OnBnClickedButtonOpen()
{
	// TODO: Add your control notification handler code here
	LPCTSTR lpszFilter = _T("ͼ���ļ�(*.jpg)|*.jpg|�����ļ�(*.*)|*.*|");
	CFileDialog fileDlg(TRUE, _T("jpg"), 0, 0, lpszFilter, this);

	CString cstrFilePath;
	if (IDOK == fileDlg.DoModal())
	{
		cstrFilePath = fileDlg.GetPathName();
		SetDlgItemText(IDC_STATIC_FILE, cstrFilePath);
	}

	TCHAR* tfilename = cstrFilePath.GetBuffer(0);
	char filename[MAX_PATH] = { 0 };
#ifdef UNICODE
	int len = WideCharToMultiByte(CP_ACP, 0, tfilename, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, tfilename, -1, filename, len, NULL, NULL);
#else
	strcpy(filename, tfilename);
#endif // UNICODE
	m_pUIEngine->SetImageData(filename);
}


void CUIDialogDlg::OnBnClickedButtonSmooth()
{
	// TODO: Add your control notification handler code here
	m_pUIEngine->ProcessImage(JZ_IMAGE_SMOOTH);

}

void CUIDialogDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
	m_pUIEngine->ReleaseImageData();
	m_pUIEngine->Release();
}


void CUIDialogDlg::OnBnClickedButtonSave()
{
	// TODO: Add your control notification handler code here
	LPCTSTR lpszFilter = _T("ͼ���ļ�(*.jpg)|*.jpg|�����ļ�(*.*)|*.*|");
	CFileDialog fileDlg(FALSE, _T("jpg"), _T("SaveImage"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszFilter, this);

	CString cstrFilePath;
	if (IDOK == fileDlg.DoModal())
	{
		cstrFilePath = fileDlg.GetPathName();
		SetDlgItemText(IDC_STATIC_SAVE, cstrFilePath);
	}

	TCHAR* tfilename = cstrFilePath.GetBuffer(0);
	char filename[MAX_PATH] = { 0 };
#ifdef UNICODE
	int len = WideCharToMultiByte(CP_ACP, 0, tfilename, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, tfilename, -1, filename, len, NULL, NULL);
#else
	strcpy(filename, tfilename);
#endif // UNICODE

	m_pUIEngine->SaveImageData(filename);
}
