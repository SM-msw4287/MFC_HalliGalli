// GameDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "HalliGalliClient.h"
#include "GameDlg.h"
#include "afxdialogex.h"

#include "IntroDlg.h"


// CGameDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CGameDlg, CDialogEx)

CGameDlg::CGameDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GAME_DLG, pParent)
	, m_strSend(_T(""))
	, m_bTakeCard{}
	, m_strMe(_T("������ �����Դϴ�"))
	, m_strWholeCountNum(_T(""))
	, m_strCardCountNum(_T("0"))
{
}

CGameDlg::~CGameDlg()
{
}

BEGIN_MESSAGE_MAP(CGameDlg, CDialogEx)
	ON_STN_CLICKED(IDC_IMG_PLAYER_OWN, &CGameDlg::OnClickedImgPlayerOwn)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CGameDlg::OnBnClickedButtonSend)	
	ON_MESSAGE(UM_RECEIVE, OnReceive)
	ON_STN_CLICKED(IDC_IMG_BELL, &CGameDlg::OnClickedImgBell)
END_MESSAGE_MAP()

BOOL CGameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	/* �̹��� ���� �Ŵ��� �ʱ�ȭ => Image���� ���� �̹��� �ε��� */
	m_pImgMgr = CImageMgr::GetInstance();
	m_pImgMgr->Initialize();

	/* Picture Ctrl �ʱ�ȭ */
	InitPicCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
BOOL CGameDlg::DestroyWindow()
{
	/* �̹��� �Ŵ��� ���� */
	CImageMgr::DestroyInstance();
	m_pImgMgr = nullptr;

	return CDialogEx::DestroyWindow();
}

void CGameDlg::OnCancel()
{
	CDialogEx::OnCancel();
	DestroyWindow();

	/* ��Ʈ�� ���̾�α� ���� */
	g_pIntroDlg->DestroyWindow();
}

LPARAM CGameDlg::OnReceive(UINT wParam, LPARAM lParam)
{
	char pTemp[MAX_STR];
	CString strTemp, str;

	memset(pTemp, '\0', MAX_STR);
	m_pSocCom->Receive(pTemp, MAX_STR);

	strTemp.Format("%c", pTemp[0]);
	int iType = atoi(strTemp.GetString());
	
	CString strFruitID;
	CString	strFruitCnt;
	CARD tCard;
	

	CString strOtherCardCnt;
	CString strCardCnt;
	CString strGameEvent;

	int iOtherCardCnt = 0;
	int iEvent = 0;

	m_strWholeCountNum.Format("%d", m_iTurnCnt);

	switch (iType)
	{
	case SOC_INITGAME:
		/* �����κ��� ī�� �ޱ� */
		if (ReceiveCard(pTemp + 1) == TRUE)
		{
			SendGame(SOC_GAMESTART);
			ChangeMyTurn(FALSE);
		}	
		break;
	case SOC_GAMESTART:
		m_iTurnCnt = TURN_CNT;
		m_bStartSvr = TRUE;
		break;
	case SOC_THROWNCARD:	
		strFruitID.Format("%c", (pTemp+1)[0]);
		strFruitCnt.Format("%c", (pTemp + 1)[1]);

		tCard.iFruitID = atoi(strFruitID.GetString());
		tCard.iFruitCnt = atoi(strFruitCnt.GetString());

		AddOtherThrownCard(tCard);
		ChangeCardImage(USER_OTHER, THROWN, tCard);
		ChangeMyTurn(TRUE);
		m_iTurnCnt--;
		m_strWholeCountNum.Format("%d", m_iTurnCnt);
		UpdateData(FALSE);

		break;
	case SOC_BELL:
		/* ��밡 ���� ������ ��� */
		m_bOtherBell = TRUE;
		break;
	case SOC_TAKECARD:
		/* ��밡 ī�带 �������� ��� */
		m_bOtherBell = FALSE;
		DeleteAllMyThrownCard();
		DeleteAllOtherThrownCard();
		AfxMessageBox("��밡 ī�带 ȹ���߽��ϴ�!");
		break;
	case SOC_NOTAKECARD:
		/* ��밡 ���� �߸�ģ ��� */
		m_bOtherBell = FALSE;
		strFruitID.Format("%c", (pTemp + 1)[0]);
		strFruitCnt.Format("%c", (pTemp + 1)[1]);

		tCard.iFruitID = atoi(strFruitID.GetString());
		tCard.iFruitCnt = atoi(strFruitCnt.GetString());

		m_lstMyCard.push_back(tCard);
		m_strCardCountNum.Format("%d", m_lstMyCard.size());
		UpdateData(FALSE);

		AfxMessageBox("������ ī�带 ȹ���߽��ϴ�!");
		break;
	case SOC_TEXT:
		str.Format("%s", pTemp + 1);
		m_list.AddString("(����) : " + str);
		break;
	case SOC_NOTURN:
		strOtherCardCnt.Format("%s", pTemp + 1);
		iOtherCardCnt = atoi(strOtherCardCnt.GetString());
		CheckWin(iOtherCardCnt);
		break;
	case SOC_GAMEEND:
		strGameEvent.Format("%s", pTemp + 1);
		iEvent = atoi(strGameEvent.GetString());
		switch (iEvent)
		{
		case GAME_WIN:
			/* ��밡 �̱� */
			AfxMessageBox("�й��߽��ϴ�");
			break;
		case GAME_LOSE:
			/* ��밡 �� */
			AfxMessageBox("�¸��߽��ϴ�");
			break;
		case GAME_DRAW:
			/* ���º� */
			AfxMessageBox("���º��Դϴ�");
			break;
		}
		m_bGameEnd = TRUE;
		break;
	}

	return TRUE;
}

void CGameDlg::InitSocket(CSocCom * pSocCom)
{
	if (pSocCom == nullptr)
	{
		AfxMessageBox("Socket�� NULL �Դϴ�");
		return;
	}

	m_pSocCom = pSocCom;
	m_pSocCom->Init(this->m_hWnd);

	m_bConnect = TRUE;
	//InitGame(); // ������ ���� �� ���� �ʱ�ȭ
}

void CGameDlg::SendGame(int iType, CString strTemp)
{
	UpdateData(TRUE);
	char pTemp[MAX_STR];

	memset(pTemp, '\0', MAX_STR);
	sprintf_s(pTemp, "%d%s", iType, strTemp.GetString());

	m_pSocCom->Send(pTemp, MAX_STR);
}

void CGameDlg::InitPicCtrl()
{
	ChangeCardImage(USER_PLAYER, OWN);
	ChangeCardImage(USER_OTHER, OWN);

	ChangeCardImage(USER_PLAYER, THROWN, CARD(FRUIT_BACK, 2));
	ChangeCardImage(USER_OTHER, THROWN, CARD(FRUIT_BACK, 2));

	CImage* pImage = nullptr;

	pImage = m_pImgMgr->GetImage("Bell");
	if (pImage != nullptr)
		m_BellPicCtrl.SetBitmap(*pImage);
}

void CGameDlg::ChangeCardImage(const USER_ID & eID, const CARD_STATUS & eStatus, const CARD & tCard)
{
	CImage* pImage = m_pImgMgr->GetCardImage(tCard.iFruitID, tCard.iFruitCnt);

	if (pImage != nullptr)
	{
		m_CardPicCtrl[eID][eStatus].SetBitmap(*pImage);
		Invalidate(TRUE);
	}
}


void CGameDlg::OnClickedImgPlayerOwn()
{
	if (!m_bConnect || !m_bStartSvr || !m_bMyTurn || m_bGameEnd)
		return;

	CARD tCard;

	tCard.iFruitID = m_lstMyCard.back().iFruitID;
	tCard.iFruitCnt = m_lstMyCard.back().iFruitCnt;
	
	AddMyThrownCard(tCard);
	ChangeCardImage(USER_PLAYER, THROWN, tCard); 
	
	m_lstMyCard.pop_back();

	/* ������ �� ī�� ���� */
	char pCardInfo[MID_STR] = "";
	sprintf_s(pCardInfo, "%d%d", tCard.iFruitID, tCard.iFruitCnt);
	SendGame(SOC_THROWNCARD, pCardInfo);

	/* ī�� �ܿ��� �� �ϼ� ���� */
	m_strCardCountNum.Format("%d", m_lstMyCard.size());
	m_iTurnCnt--;
	m_strWholeCountNum.Format("%d", m_iTurnCnt);

	UpdateData(FALSE);

	/* ���� �������� �˻� */
	if (IsGameEnd())
		return;

	/* �� ���� */
	ChangeMyTurn(FALSE);
}

void CGameDlg::OnBnClickedButtonSend()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	SendGame(SOC_TEXT, m_strSend);
	
	m_list.AddString("   (��) : " + m_strSend);
	m_strSend = (_T(""));
	
	UpdateData(FALSE);
	
}

void CGameDlg::InitGame()
{
}

BOOL CGameDlg::ReceiveCard(const char* pCardInfo)
{
	/* ī�� �� ������ ���� Start */
	CARD tCard; 
	CString strFruitID, strFruitCnt; 
	
	strFruitID.Format("%c", pCardInfo[0]);
	strFruitCnt.Format("%c", pCardInfo[1]);

	tCard.iFruitID = atoi(strFruitID.GetString());
	tCard.iFruitCnt = atoi(strFruitCnt.GetString());

	m_lstMyCard.push_back(tCard);

	/* ī�� ������ 14�̸� TRUE, �ƴϸ� FALSE ���� */
	if (m_lstMyCard.size() == CARD_HALF_CNT)
	{
		m_strCardCountNum.Format("%d", m_lstMyCard.size());
		UpdateData(FALSE);

		return TRUE;
	}
	else
		return FALSE;
}

void CGameDlg::AddMyThrownCard(const CARD sCard)
{
	m_lstMyThrownCard.push_back(sCard);	
}

void CGameDlg::DeleteAllMyThrownCard()
{
	int nThrowCardCount = 0;
	/* ���� ī�带 ������ ��� */
	if (m_bTakeCard)
	{
		nThrowCardCount = m_lstMyThrownCard.size();

		for (int i = 0; i < nThrowCardCount; i++)
		{
			m_lstMyCard.push_front(m_lstMyThrownCard.back());
			m_lstMyThrownCard.pop_back();
		}
	}
	/* ��밡 ī�带 ������ ��� */
	else
	{
		m_lstMyThrownCard.clear();
	}

	ChangeCardImage(USER_PLAYER, THROWN, CARD(FRUIT_BACK, 2));
}

void CGameDlg::AddOtherThrownCard(const CARD sCard)
{
	m_lstOtherThrownCard.push_back(sCard);
}

void CGameDlg::DeleteAllOtherThrownCard()
{
	int nThrowCardCount = 0;
	/* ���� ī�带 ������ ��� */
	if (m_bTakeCard)
	{
		nThrowCardCount = m_lstOtherThrownCard.size();

		for (int i = 0; i < nThrowCardCount; i++)
		{
			m_lstMyCard.push_front(m_lstOtherThrownCard.back());
			m_lstOtherThrownCard.pop_back();
		}
	}
	/* ��밡 ī�带 ������ ��� */
	else
	{
		m_lstOtherThrownCard.clear();
	}

	ChangeCardImage(USER_OTHER, THROWN, CARD(FRUIT_BACK, 2));
}

void CGameDlg::ChangeMyTurn(BOOL bMyTurn)
{
	m_bMyTurn = bMyTurn;
	if (m_bMyTurn)
		m_strMe = "����� �����Դϴ�";
	else
		m_strMe = "������ �����Դϴ�";

	GetDlgItem(IDC_IMG_PLAYER_OWN)->EnableWindow(m_bMyTurn);

	UpdateData(FALSE);
}

void CGameDlg::CheckWin(const int & iOtherCnt)
{
	CString strEvent;

	if (iOtherCnt > int(m_lstMyCard.size())) // ����� ī�尡 ������ ���� ��
	{
		m_bWin = FALSE;
		strEvent.Format("%d", GAME_LOSE);
		SendGame(SOC_GAMEEND, strEvent);
		AfxMessageBox("�й��߽��ϴ�");
	}
	else if (iOtherCnt < int(m_lstMyCard.size()))
	{
		m_bWin = TRUE;
		strEvent.Format("%d", GAME_WIN);
		SendGame(SOC_GAMEEND, strEvent);
		AfxMessageBox("�¸��߽��ϴ�");
	}
	else
	{
		m_bWin = FALSE;
		strEvent.Format("%d", GAME_DRAW);
		SendGame(SOC_GAMEEND, strEvent);
		AfxMessageBox("���º��Դϴ�");
	}

	m_bGameEnd = TRUE;
}

BOOL CGameDlg::IsGameEnd()
{
	/* ������ �������� �˻��ϴ� �Լ� */
	if (m_lstMyCard.size() <= 0) // ���� ���� ī�尡 ���� ��
	{
		m_bWin = FALSE;
		AfxMessageBox("�й��߽��ϴ�");

		CString strEvent;
		strEvent.Format("%d", GAME_LOSE);
		SendGame(SOC_GAMEEND, strEvent);

		m_bGameEnd = TRUE;

		return TRUE;
	}

	if (m_iTurnCnt <= 0) // ��ü �ϼ��� ���� �� 
	{
		CString strCardCnt;
		strCardCnt.Format("%d", m_lstMyCard.size());

		SendGame(SOC_NOTURN, strCardCnt);

		return TRUE;
	}

	return FALSE;
}

void CGameDlg::OnClickedImgBell()
{
	if (m_bGameEnd)
		return;

	/* ���� Ŭ�� ���� ��� */
	if (m_bOtherBell) // �̹� ������ ���� ���� �������� return
		return;

	SendGame(SOC_BELL);
	CheckThrownCard(); // ī�� �˻�

	if (m_bTakeCard)
	{
		// ���� �� ������ ���
		TakeThrownCard();
		SendGame(SOC_TAKECARD); // ī�带 ���������� ��뿡�� �˸�

		AfxMessageBox("ī�� ȹ��!");

		m_bTakeCard = FALSE;
	}
	else
	{
		// ���� �߸� ������ ���
		CARD tCard;

		tCard.iFruitID = m_lstMyCard.back().iFruitID;
		tCard.iFruitCnt = m_lstMyCard.back().iFruitCnt;

		m_lstMyCard.pop_back();
		AfxMessageBox("���� �߸��Ƴ׿�. ī�� �ϳ��� �Ҿ����ϴ�!");
		
		/* ī�� ���� */
		char pCardInfo[MID_STR] = "";
		sprintf_s(pCardInfo, "%d%d", tCard.iFruitID, tCard.iFruitCnt);
		SendGame(SOC_NOTAKECARD,pCardInfo); // ��뿡�� ���� �߸� �������� �˸�

		m_strCardCountNum.Format("%d", m_lstMyCard.size());
		UpdateData(FALSE);
	}
}

void CGameDlg::CheckThrownCard()
{
	int iMyFruitCnt = 0;
	int iOtherFruitCnt = 0;
	
	int iMyFruitID = FRUIT_END;
	int iOhtherFruitID = FRUIT_END;

	if (!m_lstMyThrownCard.empty())
	{
		iMyFruitCnt = m_lstMyThrownCard.back().iFruitCnt;
		iMyFruitID = m_lstMyThrownCard.back().iFruitID;
	}
		
	if (!m_lstOtherThrownCard.empty())
	{
		iOtherFruitCnt = m_lstOtherThrownCard.back().iFruitCnt;
		iOhtherFruitID = m_lstOtherThrownCard.back().iFruitID;
	}
		
	if (iMyFruitID != FRUIT_END && iOhtherFruitID != FRUIT_END)
	{
		/* ���� ������ ������ ���� �� */
		if (iMyFruitID == iOhtherFruitID)
		{
			if (iMyFruitCnt + iOtherFruitCnt == 5)
				m_bTakeCard = TRUE;
			else
				m_bTakeCard = FALSE;
		}
		else
		{
			/* ���� ������ ������ �ٸ� �� */
			if(iMyFruitCnt == 5 || iOtherFruitCnt == 5)
				m_bTakeCard = TRUE;
			else
				m_bTakeCard = FALSE;
		}
	}
	else
		m_bTakeCard = FALSE;
}

void CGameDlg::TakeThrownCard()
{
	DeleteAllMyThrownCard();
	DeleteAllOtherThrownCard();
	m_strCardCountNum.Format("%d", m_lstMyCard.size());
	UpdateData(FALSE);
}