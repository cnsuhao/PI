
// UI-SingleDocumentView.h : CUISingleDocumentView 类的接口
//

#pragma once
#include "DlgSmooth.h"

class CUISingleDocumentView : public CView
{
protected: // 仅从序列化创建
	CUISingleDocumentView();
	DECLARE_DYNCREATE(CUISingleDocumentView)

// 特性
public:
	CUISingleDocumentDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CUISingleDocumentView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CUISingleDocumentApp* m_pApp;
	CDlgSmooth* m_pSmoothDialog;
// 生成的消息映射函数
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
};

#ifndef _DEBUG  // UI-SingleDocumentView.cpp 中的调试版本
inline CUISingleDocumentDoc* CUISingleDocumentView::GetDocument() const
   { return reinterpret_cast<CUISingleDocumentDoc*>(m_pDocument); }
#endif

