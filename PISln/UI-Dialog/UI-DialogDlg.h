
// UI-DialogDlg.h : 头文件
//

#pragma once

#include <IJZUIEngine.h>

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
	HICON m_hIcon;
	IJZUIEngine* m_pUIEngine;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonSmooth();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonSave();
};
