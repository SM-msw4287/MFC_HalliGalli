#pragma once


#include "PictureEx.h"
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
	virtual void DoDataExchange(CDataExchange* pDX)
	{
		CDialogEx::DoDataExchange(pDX);
		DDX_Text(pDX, IDC_EDIT_SEND, m_strSend);
		DDX_Text(pDX, IDC_STATIC_ME, m_strMe);
		DDX_Control(pDX, IDC_LIST1, m_list);
		DDX_Control(pDX, IDC_IMG_BELL, m_BellPicCtrl);
		DDX_Control(pDX, IDC_IMG_OTHER_OWN, m_CardPicCtrl[USER_OTHER][OWN]);
		DDX_Control(pDX, IDC_IMG_OTHER_THROWN, m_CardPicCtrl[USER_OTHER][THROWN]);
		DDX_Control(pDX, IDC_IMG_PLAYER_OWN, m_CardPicCtrl[USER_PLAYER][OWN]);
		DDX_Control(pDX, IDC_IMG_PLAYER_THROWN, m_CardPicCtrl[USER_PLAYER][THROWN]);
		DDX_Control(pDX, IDC_STATIC_CARDCOUNT, m_strCardCount);
		DDX_Control(pDX, IDC_STATIC_WHOLECOUNT, m_strWholeCount);
		DDX_Text(pDX, IDC_WHOLECOUNTNUM, m_strWholeCountNum);
		DDX_Text(pDX, IDC_CARDCOUNTNUM, m_strCardCountNum);
	}    // DDX/DDV �����Դϴ�.
	afx_msg LPARAM OnReceive(UINT wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow(); 
	virtual void OnCancel();

public:
	/* ���� ���� �Լ� */
	void InitSocket(CSocCom* pSocCom);
	void SendGame(int iType, CString strTemp = "");

public:
	/* �̹��� ���� �Լ� */
	void InitPicCtrl();
	void ChangeCardImage(const USER_ID& eID, const CARD_STATUS& eStatus = THROWN, const CARD& tCard = {FRUIT_BACK, 1});
	
	afx_msg void OnClickedImgPlayerOwn();
	afx_msg void OnClickedImgBell();

public:
	/* ���� ���� ���� �Լ� */
	void InitGame();
	BOOL ReceiveCard(const char* pCardInfo);

	void CheckThrownCard(); // THROWN CARD 5������ �˻�
	void TakeThrownCard(); // ���� ���� �ļ� ī�带 ��� ������

	void AddMyThrownCard(const CARD sCard);
	void AddOtherThrownCard(const CARD sCard);
	void DeleteAllMyThrownCard();
	void DeleteAllOtherThrownCard();

	void ChangeMyTurn(BOOL bMyTurn);

public:
	/* ä�� ���� �Լ� */
	afx_msg void OnBnClickedButtonSend();

public:
	/* ���� ���� ���� */
	CSocCom* m_pSocCom = nullptr;

	BOOL m_bConnect = FALSE;

public:
	/* �̹��� ���� ���� */
	CPictureEx m_BellPicCtrl; // �� �̹��� ��Ʈ��
	CPictureEx m_CardPicCtrl[USER_END][CARD_END]; 	// ī�� �̹��� ��Ʈ��

	CImageMgr* m_pImgMgr = nullptr; // �̹��� �Ŵ��� ������ ����
	
public:
	/* ī�� ���� �ڷᱸ�� */
	list<CARD> m_lstMyCard;			// 14
	list<CARD> m_lstMyThrownCard;
	list<CARD> m_lstOtherThrownCard;
	int m_lstWholeCard;		// 50

public:
	/* ���� ������� ���� */
	BOOL m_bStartSvr = FALSE;
	BOOL m_bTakeCard = FALSE;
	BOOL m_bMyTurn = FALSE;
	BOOL m_bOtherBell = FALSE;

	CString m_strCardCountNum;
	CString m_strWholeCountNum;
	CStatic m_strCardCount;
	CStatic m_strWholeCount;
	CString m_strMe;

public:
	/* ä�� ���� ���� */
	CListBox m_list;
	CString m_strSend;
};