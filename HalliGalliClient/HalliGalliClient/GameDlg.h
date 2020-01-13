#pragma once


// CGameDlg ��ȭ �����Դϴ�.

class CGameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGameDlg)

	/* Picture Ctrl �� ����� */
	enum USER_ID { USER_PLAYER, USER_OTHER, USER_END };
	enum CARD_STATUS { OWN, THROWN, CARD_END };

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
	LPARAM OnReceive(UINT wParam, LPARAM lParam);

	void InitSocket(CSocCom* pSocCom);

public:
	/* �̹��� ���� �Լ� */
	void InitPicCtrl();

	void ChangeCardImage(const USER_ID& eID, const CARD_STATUS& eStatus = THROWN, const CARD& tCard = {FRUIT_BACK, 1});
	
//	afx_msg void OnClieckedImgPlayerOwn(); // ī�� �̹��� ��ü �׽�Ʈ

public:
	virtual void OnCancel();

public:
	/* ���� ���� ���� */
	CSocCom* m_pSocCom = nullptr;

public:
	CListBox m_list;

public:
	/* �̹��� ���� ���� */
	CStatic m_BellPicCtrl; // �� �̹��� ��Ʈ��
	CStatic m_CardPicCtrl[USER_END][CARD_END]; 	// ī�� �̹��� ��Ʈ��

	CImageMgr* m_pImgMgr = nullptr; // �̹��� �Ŵ��� ������ ����

public:
	CString m_strGain;
	CString m_strSend;
	CEdit m_strWholeCountNum;
	CEdit m_strCardCountNum;
	CStatic m_strCardCount;
	CStatic m_strWholeCount;
	CString m_strMe;
	afx_msg void OnBnClickedButtonSend();
	void SendChat(int iType, CString strTmp);
	afx_msg void OnClickedImgPlayerOwn();


};
