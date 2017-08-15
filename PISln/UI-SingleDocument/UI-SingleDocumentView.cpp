
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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUISingleDocumentView

IMPLEMENT_DYNCREATE(CUISingleDocumentView, CView)

BEGIN_MESSAGE_MAP(CUISingleDocumentView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CUISingleDocumentView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CUISingleDocumentView 构造/析构

CUISingleDocumentView::CUISingleDocumentView()
{
	// TODO: 在此处添加构造代码

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
