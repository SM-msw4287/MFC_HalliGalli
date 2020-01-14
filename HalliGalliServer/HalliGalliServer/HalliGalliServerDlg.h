
// HalliGalliServerDlg.h : ��� ����
//

#pragma once

#include "SocSvr.h"
#include "PictureEx.h"

#include "Timer.h"
#include "Frame.h"

// CHalliGalliServerDlg ��ȭ ����
class CHalliGalliServerDlg : public CDialogEx
{
	/* Picture Ctrl �� ����� */
	enum USER_ID { USER_PLAYER, USER_OTHER, USER_END };
	enum CARD_STATUS { OWN, THROWN, CARD_END };

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
	afx_msg LPARAM OnAccept(UINT wParam, LPARAM lParam);
	afx_msg LPARAM OnReceive(UINT wParam, LPARAM lParam);
	afx_msg LRESULT OnKickIdle(WPARAM wParam, LPARAM lParam); // ������
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL DestroyWindow();

public:
	/* ���� ���� �Լ� */
	void InitSocket();
	void SendGame(int iType, CString strTemp = "");

public:
	/* �̹��� ���� �Լ� */
	void InitPicCtrl();
	void ChangeCardImage(const USER_ID& eID, const CARD_STATUS& eStatus = THROWN, const CARD& tCard = { FRUIT_BACK, 1 });
	
	afx_msg void OnClickedImgPlayerOwn();
	afx_msg void OnClickedImgBell();

public:
	/* ���� ���� ���� �Լ� */
	void InitGame(); // ���� �ʱ�ȭ �Լ� (���� �� �ʱ�ȭ)
	void InitCardDeck(); // ī�� �ʱ�ȭ �� ���� �Լ�
	void SendCardToClient(); // ���� ī�� Ŭ�� ������ �Լ�

	void CheckThrownCard(); // THROWN CARD 5������ �˻�
	void TakeThrownCard(); // ���� ���� �ļ� ī�带 ��� ������
	
	void AddMyThrownCard(const CARD sCard);
	void AddOtherThrownCard(const CARD sCard);
	void DeleteAllMyThrownCard();
	void DeleteAllOtherThrownCard();

	void ChangeMyTurn(BOOL bMyTurn);

	void CheckWin(const int& iOtherCnt);
	BOOL IsGameEnd(); // ������ �������� �˻��ϴ� �Լ�

public:
	/* ä�� ���� �Լ� */
	afx_msg void OnClickedButtonSend();

public:
	/* ���� ���� ���� */
	CSocSvr m_socServer;
	CSocCom* m_pSocCom = nullptr;

	BOOL m_bConnect = FALSE;

public:
	/* �̹��� ���� ���� */
	CPictureEx m_BellPicCtrl; // �� �̹��� ��Ʈ��
	CPictureEx m_CardPicCtrl[USER_END][CARD_END]; 	// ī�� �̹��� ��Ʈ��

	CImageMgr* m_pImgMgr = nullptr; // �̹��� �Ŵ��� ������ ����
	
public:
	/* ī�� �ʱ� ���� */
	CARD m_cardDec[CARD_CNT];
	int  m_shuffleCardIndex[CARD_CNT];

	/* ī�� ���� �ڷᱸ�� */
	list<CARD> m_lstMyCard;
	list<CARD> m_lstMyThrownCard;
	list<CARD> m_lstOtherThrownCard;

public:
	/* ���� ������� ���� */
	BOOL m_bStartCnt = FALSE;
	BOOL m_bTakeCard = FALSE;
	BOOL m_bMyTurn = FALSE;
	BOOL m_bOtherBell = FALSE;
	
	BOOL m_bWin = FALSE;
	BOOL m_bGameEnd = FALSE;

	CEdit m_strCardCountNum;
	CEdit m_strWholeCountNum;
	
	CStatic m_strWholeCount;
	CStatic m_strCardCount;
	CString m_strMe;
	CString m_strGain;

public:
	/* ä�� ���� ���� */
	CListBox m_list;
	CString m_strSend;

public:
	/* Ÿ�̸� ���� ���� */
	CTimer* m_pImmediateTimer = nullptr;
	CTimer* m_p60Timer = nullptr;
	CFrame* m_pFrame = nullptr;
};