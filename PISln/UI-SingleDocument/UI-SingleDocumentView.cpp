
// UI-SingleDocumentView.cpp : CUISingleDocumentView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "UI-SingleDocument.h"
#endif

#include "UI-SingleDocumentDoc.h"
#include "UI-SingleDocumentView.h"
#include <IJZLogProc.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define UI_SDV_LOG_FILENAME _T("UI-SingleDocumentView.cpp")

// CUISingleDocumentView

IMPLEMENT_DYNCREATE(CUISingleDocumentView, CView)

BEGIN_MESSAGE_MAP(CUISingleDocumentView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CUISingleDocumentView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_OPEN, &CUISingleDocumentView::OnFileOpen)
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_COMMAND(ID_IMAGE_SMOOTH, &CUISingleDocumentView::OnImageSmooth)
	ON_WM_DESTROY()
	ON_COMMAND(ID_FILE_SAVE, &CUISingleDocumentView::OnFileSave)
	ON_COMMAND(ID_IMAGE_MORPH, &CUISingleDocumentView::OnImageMorph)
	ON_COMMAND(ID_32781, &CUISingleDocumentView::OnPlateRecog)
	ON_COMMAND(ID_IMAGE_HIST, &CUISingleDocumentView::OnImageHist)
END_MESSAGE_MAP()

// CUISingleDocumentView 构造/析构

CUISingleDocumentView::CUISingleDocumentView()
{
	// TODO: 在此处添加构造代码
	m_pApp = (CUISingleDocumentApp*)AfxGetApp();
	m_pSmoothDialog = NULL;
	m_pMorphologyDialog = NULL;
	m_pPlateRecogDialog = NULL;
}

CUISingleDocumentView::~CUISingleDocumentView()
{
}

BOOL CUISingleDocumentView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	return CView::PreCreateWindow(cs);
}

// CUISingleDocumentView 绘制

void CUISingleDocumentView::OnDraw(CDC* /*pDC*/)
{
	CUISingleDocumentDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	m_pApp->m_pUIEngine->Render();
}


// CUISingleDocumentView 打印


void CUISingleDocumentView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CUISingleDocumentView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CUISingleDocumentView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CUISingleDocumentView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CUISingleDocumentView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CUISingleDocumentView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CUISingleDocumentView 诊断

#ifdef _DEBUG
void CUISingleDocumentView::AssertValid() const
{
	CView::AssertValid();
}

void CUISingleDocumentView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CUISingleDocumentDoc* CUISingleDocumentView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUISingleDocumentDoc)));
	return (CUISingleDocumentDoc*)m_pDocument;
}
#endif //_DEBUG


// CUISingleDocumentView 消息处理程序


int CUISingleDocumentView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	SetTimer(1, 10, NULL);	// 每隔10ms调用一次OnTimer()函数
	return 0;
}

void CUISingleDocumentView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
	if (NULL != m_pSmoothDialog)
	{
		delete m_pSmoothDialog;
		m_pSmoothDialog = NULL;
	}

	if (NULL != m_pMorphologyDialog)
	{
		delete m_pMorphologyDialog;
		m_pMorphologyDialog = NULL;
	}
}

void CUISingleDocumentView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	//m_pApp->m_pUIEngine->Render();
	CView::OnTimer(nIDEvent);
}


void CUISingleDocumentView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}

void CUISingleDocumentView::OnFileOpen()
{
	// TODO: Add your command handler code here
	LPCTSTR lpszFilter = _T("图像文件(*.jpg)|*.jpg|所有文件(*.*)|*.*|");
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

	m_pApp->m_pUIEngine->Render();

}

void CUISingleDocumentView::OnFileSave()
{
	// TODO: Add your command handler code here
	LPCTSTR lpszFilter = _T("图像文件(*.jpg)|*.jpg|所有文件(*.*)|*.*|");
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
}

void CUISingleDocumentView::OnImageSmooth()
{
	// TODO: Add your command handler code here
	if (!m_pApp->m_pUIEngine->IsSetSrcImage())
	{
		// 显示消息对话框   
		 INT_PTR nRes = MessageBox(_T("请设置要处理的图像"), _T("错误"), MB_OK);
		 g_JZLogAPI->pfnWriteLog(JZ_LOG_TYPE_WARNING, UI_SDV_LOG_FILENAME, _T("CUISingleDocumentView::OnImageSmooth"), _T("%s"), _T("没有设置要处理的图像"));
		 return;
	}

	//m_pApp->m_pUIEngine->SetCurProcessType(JZ_IMAGE_SMOOTH);

	/*if (NULL == m_pSmoothDialog)
	{
		m_pSmoothDialog = new CDlgSmooth();
	}
	m_pSmoothDialog->DoModal();*/

	// 非模态对话框
	if (NULL == m_pSmoothDialog)
	{
		m_pSmoothDialog = new CDlgSmooth();
		m_pSmoothDialog->Create(IDD_DIALOG_SMOOTH, this);
	}
	m_pSmoothDialog->ShowWindow(SW_SHOW);

	
}

void CUISingleDocumentView::OnImageMorph()
{
	// TODO: Add your command handler code here
	if (!m_pApp->m_pUIEngine->IsSetSrcImage())
	{
		// 显示消息对话框   
		INT_PTR nRes = MessageBox(_T("请设置要处理的图像"), _T("错误"), MB_OK);
		g_JZLogAPI->pfnWriteLog(JZ_LOG_TYPE_WARNING, UI_SDV_LOG_FILENAME, _T("CUISingleDocumentView::OnImageMorph"), _T("%s"), _T("没有设置要处理的图像"));
		return;
	}

	//m_pApp->m_pUIEngine->SetCurProcessType(JZ_IMAGE_MORPHOLOGY);

	/*if (NULL == m_pMorphologyDialog)
	{
	m_pMorphologyDialog = new CDlgMorphology();
	}
	m_pMorphologyDialog->DoModal();*/

	// 非模态对话框
	if (NULL == m_pMorphologyDialog)
	{
		m_pMorphologyDialog = new CDlgMorphology();
		m_pMorphologyDialog->Create(IDD_DIALOG_MORPHOLOGY, this);
	}
	m_pMorphologyDialog->ShowWindow(SW_SHOW);
}

void CUISingleDocumentView::OnPlateRecog()
{
	// TODO: Add your command handler code here
	if (!m_pApp->m_pUIEngine->IsSetSrcImage())
	{
		// 显示消息对话框   
		INT_PTR nRes = MessageBox(_T("请设置要处理的图像"), _T("错误"), MB_OK);
		g_JZLogAPI->pfnWriteLog(JZ_LOG_TYPE_WARNING, UI_SDV_LOG_FILENAME, _T("CUISingleDocumentView::OnPlateRecog"), _T("%s"), _T("没有设置要处理的图像"));
		return;
	}

	//m_pApp->m_pUIEngine->SetCurProcessType(JZ_IMAGE_PLATERECOG);

	/*if (NULL == m_pPlateRecogDialog)
	{
	m_pPlateRecogDialog = new CDlgPlateRecog();
	}
	m_pPlateRecogDialog->DoModal();*/

	// 非模态对话框
	if (NULL == m_pPlateRecogDialog)
	{
		m_pPlateRecogDialog = new CDlgPlateRecog();
		m_pPlateRecogDialog->Create(IDD_DIALOG_PLATERECOG, this);
	}
	m_pPlateRecogDialog->ShowWindow(SW_SHOW);
}


void CUISingleDocumentView::OnImageHist()
{
	// TODO: Add your command handler code here
	if (!m_pApp->m_pUIEngine->IsSetSrcImage())
	{
		// 显示消息对话框   
		INT_PTR nRes = MessageBox(_T("请设置要处理的图像"), _T("错误"), MB_OK);
		g_JZLogAPI->pfnWriteLog(JZ_LOG_TYPE_WARNING, UI_SDV_LOG_FILENAME, _T("CUISingleDocumentView::OnImageHist"), _T("%s"), _T("没有设置要处理的图像"));
		return;
	}

	JZHistogramParam histogramParam;
	m_pApp->m_pUIEngine->SetProcessParam(&histogramParam);
	m_pApp->m_pUIEngine->ProcessImage(JZ_IMAGE_BASEPROCESS, JZ_BASEPROCESS_HISTOGRAM);
}
