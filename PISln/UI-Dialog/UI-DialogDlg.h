
// UI-DialogDlg.h : 头文件
//

#pragma once
#include "DialogSmooth.h"


// CUIDialogDlg 对话框
class CUIDialogDlg : public CDialogEx
{
// 构造
public:
	CUIDialogDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UIDIALOG_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	CRect m_rect; // 用于上一次对话框大小
	HICON m_hIcon;
	CMenu m_menu;
	CUIDialogApp* m_pApp;
	CDialogSmooth* m_pSmoothDialog;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void ChangeSize(CWnd* pCWnd, int cx, int cy);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSmooth();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonProcess();
	afx_msg void OnMenuOpen();
	afx_msg void OnMenuSave();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
