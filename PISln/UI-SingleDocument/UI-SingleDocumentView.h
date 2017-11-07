
// UI-SingleDocumentView.h : CUISingleDocumentView ��Ľӿ�
//

#pragma once
#include "DlgSmooth.h"
#include "DlgMorphology.h"
#include "DlgPlateRecog.h"

class CUISingleDocumentView : public CView
{
protected: // �������л�����
	CUISingleDocumentView();
	DECLARE_DYNCREATE(CUISingleDocumentView)

// ����
public:
	CUISingleDocumentDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CUISingleDocumentView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CUISingleDocumentApp* m_pApp;
	CDlgSmooth* m_pSmoothDialog;
	CDlgMorphology* m_pMorphologyDialog;
	CDlgPlateRecog* m_pPlateRecogDialog;
	
// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileOpen();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnImageSmooth();
	afx_msg void OnDestroy();
	afx_msg void OnFileSave();
	afx_msg void OnImageMorph();
	afx_msg void OnPlateRecog();
	afx_msg void OnImageHist();
};

#ifndef _DEBUG  // UI-SingleDocumentView.cpp �еĵ��԰汾
inline CUISingleDocumentDoc* CUISingleDocumentView::GetDocument() const
   { return reinterpret_cast<CUISingleDocumentDoc*>(m_pDocument); }
#endif

