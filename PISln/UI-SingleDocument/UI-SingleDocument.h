
// UI-SingleDocument.h : UI-SingleDocument Ӧ�ó������ͷ�ļ�
//
#pragma once
#include <IJZUIEngine.h>
#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CUISingleDocumentApp:
// �йش����ʵ�֣������ UI-SingleDocument.cpp
//

class CUISingleDocumentApp : public CWinAppEx
{
public:
	CUISingleDocumentApp();
	~CUISingleDocumentApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;
	IJZUIEngine* m_pUIEngine;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CUISingleDocumentApp theApp;
