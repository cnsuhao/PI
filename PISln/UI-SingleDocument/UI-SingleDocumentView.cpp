
// UI-SingleDocumentView.cpp : CUISingleDocumentView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CUISingleDocumentView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CUISingleDocumentView ����/����

CUISingleDocumentView::CUISingleDocumentView()
{
	// TODO: �ڴ˴���ӹ������

}

CUISingleDocumentView::~CUISingleDocumentView()
{
}

BOOL CUISingleDocumentView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CUISingleDocumentView ����

void CUISingleDocumentView::OnDraw(CDC* /*pDC*/)
{
	CUISingleDocumentDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CUISingleDocumentView ��ӡ


void CUISingleDocumentView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CUISingleDocumentView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CUISingleDocumentView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CUISingleDocumentView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
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


// CUISingleDocumentView ���

#ifdef _DEBUG
void CUISingleDocumentView::AssertValid() const
{
	CView::AssertValid();
}

void CUISingleDocumentView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CUISingleDocumentDoc* CUISingleDocumentView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUISingleDocumentDoc)));
	return (CUISingleDocumentDoc*)m_pDocument;
}
#endif //_DEBUG


// CUISingleDocumentView ��Ϣ�������
