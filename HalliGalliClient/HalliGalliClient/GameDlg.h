#pragma once


// CGameDlg ��ȭ �����Դϴ�.

class CGameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGameDlg)

public:
	CGameDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CGameDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAME_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow(); 

public:
	/* ���� ���� �Լ� */
	void InitSocket(CSocCom* pSocCom);

public:
	afx_msg void OnStnClickedStaticGain();

public:
	/* ���� ���� ���� */
	CSocCom* m_pSocCom = nullptr;

public:
	CListBox m_list;

public:
	CString m_strCardCount;
	CString m_strGain;
	CStatic m_strMe;
	CString m_strSend;
};
