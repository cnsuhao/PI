
// UI-DialogDlg.h : ͷ�ļ�
//

#pragma once
#include "DialogSmooth.h"


// CUIDialogDlg �Ի���
class CUIDialogDlg : public CDialogEx
{
// ����
public:
	CUIDialogDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UIDIALOG_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	CRect m_rect; // ������һ�ζԻ����С
	HICON m_hIcon;
	CMenu m_menu;
	CUIDialogApp* m_pApp;
	CDialogSmooth* m_pSmoothDialog;

	// ���ɵ���Ϣӳ�亯��
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
