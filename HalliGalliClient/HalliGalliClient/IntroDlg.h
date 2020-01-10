
// IntroDlg.h : ��� ����
//

#pragma once

// CIntroDlg ��ȭ ����
class CIntroDlg : public CDialogEx
{
	enum RADIO_INDEX { RADIO_2P, RADIO_4P, RADIO_END };

// �����Դϴ�.
public:
	CIntroDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INTRO_DLG };
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
	afx_msg void OnBnClickedRadio2p();
	afx_msg void OnBnClickedRadio4p();

	afx_msg void OnBnClickedButtonConnect(); 

public:
	CButton m_Radio[RADIO_END];
};
