
// UI-DialogDlg.h : ͷ�ļ�
//

#pragma once

#include <IJZUIEngine.h>

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
	HICON m_hIcon;
	IJZUIEngine* m_pUIEngine;

	// ���ɵ���Ϣӳ�亯��
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
