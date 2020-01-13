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
	afx_msg LPARAM OnReceive(UINT wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow(); 

public:
	/* ���� ���� �Լ� */
	
	void InitSocket(CSocCom* pSocCom);
	void SendGame(int iType, CString strTemp = "");

public:
	/* �̹��� ���� �Լ� */
	void InitPicCtrl();

	void ChangeCardImage(const USER_ID& eID, const CARD_STATUS& eStatus = THROWN, const CARD& tCard = {FRUIT_BACK, 1});
	
	afx_msg void OnClieckedImgPlayerOwn(); // ī�� �̹��� ��ü �׽�Ʈ
	

	afx_msg void OnBnClickedButtonSub();

public:
	/* ���� ���� ���� �Լ� */
	void InitGame();
	BOOL ReceiveCard(const char* pCardInfo);
	void addMyThrownCard(const CARD sCard);
	void deleteAllMyThrownCard();
	void addOtherThrownCard(const CARD sCard);
	void deleteAllOtherThrownCard();
	void CheckFive();
	void Win();

public:
	virtual void OnCancel();

	afx_msg void OnStnClickedStaticGain();

public:
	/* ���� ���� ���� */
	CSocCom* m_pSocCom = nullptr;

	BOOL m_bConnect = FALSE;

public:
	CListBox m_list;

public:
	/* �̹��� ���� ���� */
	CStatic m_BellPicCtrl; // �� �̹��� ��Ʈ��
	CStatic m_CardPicCtrl[USER_END][CARD_END]; 	// ī�� �̹��� ��Ʈ��

	CImageMgr* m_pImgMgr = nullptr; // �̹��� �Ŵ��� ������ ����

public:
	CString m_strCardCount;
	CString m_strGain;
	CStatic m_strMe;
	CString m_strSend;


public:
	list<CARD> m_lstMyCard;
	list<CARD> m_lstMyThrownCard;
	list<CARD> m_lstOtherThrownCard;

public:
	/* ���� ������� ���� */
	BOOL m_bStartSvr = FALSE;
	BOOL m_bWin;

	
	afx_msg void OnStnClickedImgBell();
};