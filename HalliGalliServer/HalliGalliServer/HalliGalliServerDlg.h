
// HalliGalliServerDlg.h : ��� ����
//

#pragma once

#include "SocSvr.h"

// CHalliGalliServerDlg ��ȭ ����
class CHalliGalliServerDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CHalliGalliServerDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HALLIGALLISERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL DestroyWindow();

public:
	/* ���� ���� �Լ� */
	afx_msg LPARAM OnAccept(UINT wParam, LPARAM lParam);
	afx_msg LPARAM OnReceive(UINT wParam, LPARAM lParam);

	void InitSocket();

public:
	/* ���� ���� ���� */
	CSocSvr m_socServer;
	CSocCom* m_pSocCom = nullptr;
};
