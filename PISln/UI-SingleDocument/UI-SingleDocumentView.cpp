
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
#include <IJZLogProc.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define UI_SDV_LOG_FILENAME _T("UI-SingleDocumentView.cpp")

// CUISingleDocumentView

IMPLEMENT_DYNCREATE(CUISingleDocumentView, CView)

BEGIN_MESSAGE_MAP(CUISingleDocumentView, CView)
	// ��׼��ӡ����
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

// CUISingleDocumentView ����/����

CUISingleDocumentView::CUISingleDocumentView()
{
	// TODO: �ڴ˴���ӹ������
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
	m_pApp->m_pUIEngine->Render();
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


int CUISingleDocumentView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	SetTimer(1, 10, NULL);	// ÿ��10ms����һ��OnTimer()����
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

	m_pApp->m_pUIEngine->Render();

}

void CUISingleDocumentView::OnFileSave()
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
}

void CUISingleDocumentView::OnImageSmooth()
{
	// TODO: Add your command handler code here
	if (!m_pApp->m_pUIEngine->IsSetSrcImage())
	{
		// ��ʾ��Ϣ�Ի���   
		 INT_PTR nRes = MessageBox(_T("������Ҫ�����ͼ��"), _T("����"), MB_OK);
		 g_JZLogAPI->pfnWriteLog(JZ_LOG_TYPE_WARNING, UI_SDV_LOG_FILENAME, _T("CUISingleDocumentView::OnImageSmooth"), _T("%s"), _T("û������Ҫ�����ͼ��"));
		 return;
	}

	//m_pApp->m_pUIEngine->SetCurProcessType(JZ_IMAGE_SMOOTH);

	/*if (NULL == m_pSmoothDialog)
	{
		m_pSmoothDialog = new CDlgSmooth();
	}
	m_pSmoothDialog->DoModal();*/

	// ��ģ̬�Ի���
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
		// ��ʾ��Ϣ�Ի���   
		INT_PTR nRes = MessageBox(_T("������Ҫ�����ͼ��"), _T("����"), MB_OK);
		g_JZLogAPI->pfnWriteLog(JZ_LOG_TYPE_WARNING, UI_SDV_LOG_FILENAME, _T("CUISingleDocumentView::OnImageMorph"), _T("%s"), _T("û������Ҫ�����ͼ��"));
		return;
	}

	//m_pApp->m_pUIEngine->SetCurProcessType(JZ_IMAGE_MORPHOLOGY);

	/*if (NULL == m_pMorphologyDialog)
	{
	m_pMorphologyDialog = new CDlgMorphology();
	}
	m_pMorphologyDialog->DoModal();*/

	// ��ģ̬�Ի���
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
		// ��ʾ��Ϣ�Ի���   
		INT_PTR nRes = MessageBox(_T("������Ҫ�����ͼ��"), _T("����"), MB_OK);
		g_JZLogAPI->pfnWriteLog(JZ_LOG_TYPE_WARNING, UI_SDV_LOG_FILENAME, _T("CUISingleDocumentView::OnPlateRecog"), _T("%s"), _T("û������Ҫ�����ͼ��"));
		return;
	}

	//m_pApp->m_pUIEngine->SetCurProcessType(JZ_IMAGE_PLATERECOG);

	/*if (NULL == m_pPlateRecogDialog)
	{
	m_pPlateRecogDialog = new CDlgPlateRecog();
	}
	m_pPlateRecogDialog->DoModal();*/

	// ��ģ̬�Ի���
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
		// ��ʾ��Ϣ�Ի���   
		INT_PTR nRes = MessageBox(_T("������Ҫ�����ͼ��"), _T("����"), MB_OK);
		g_JZLogAPI->pfnWriteLog(JZ_LOG_TYPE_WARNING, UI_SDV_LOG_FILENAME, _T("CUISingleDocumentView::OnImageHist"), _T("%s"), _T("û������Ҫ�����ͼ��"));
		return;
	}

	JZHistogramParam histogramParam;
	m_pApp->m_pUIEngine->SetProcessParam(&histogramParam);
	m_pApp->m_pUIEngine->ProcessImage(JZ_IMAGE_BASEPROCESS, JZ_BASEPROCESS_HISTOGRAM);
}
