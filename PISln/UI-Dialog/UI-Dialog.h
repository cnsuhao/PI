
// UI-Dialog.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once
#include <IJZUIEngine.h>
#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CUIDialogApp: 
// �йش����ʵ�֣������ UI-Dialog.cpp
//

class CUIDialogApp : public CWinApp
{
public:
	CUIDialogApp();
	IJZUIEngine* m_pUIEngine;

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CUIDialogApp theApp;