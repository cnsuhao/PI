
// UI-DialogDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UI-Dialog.h"
#include "UI-DialogDlg.h"
#include "afxdialogex.h"
#include <IJZUIEngineProc.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CUIDialogDlg 对话框



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


// CUIDialogDlg 消息处理程序

BOOL CUIDialogDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CWnd* cwnd = GetDlgItem(IDC_STATIC_PIC);
	m_pUIEngine = NULL;
	g_JZUIEngineAPI->pfnGetInterface(&m_pUIEngine);
	m_pUIEngine->Init(cwnd->m_hWnd);

	return FALSE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUIDialogDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//CDialogEx::OnPaint();
		m_pUIEngine->Render();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CUIDialogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUIDialogDlg::OnBnClickedButtonOpen()
{
	// TODO: Add your control notification handler code here
	LPCTSTR lpszFilter = _T("图像文件(*.jpg)|*.jpg|所有文件(*.*)|*.*|");
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
	LPCTSTR lpszFilter = _T("图像文件(*.jpg)|*.jpg|所有文件(*.*)|*.*|");
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
