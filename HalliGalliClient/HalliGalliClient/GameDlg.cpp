// GameDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "HalliGalliClient.h"
#include "GameDlg.h"
#include "afxdialogex.h"

#include "IntroDlg.h"

int wcnt = 5;
int ccnt = 5;

// CGameDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CGameDlg, CDialogEx)

CGameDlg::CGameDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GAME_DLG, pParent)
	//, m_strWholeCount(_T(""))
	//, m_strCardCount(_T(""))
	//, m_strWholeCountNum(_T(""))
	//, m_strCardCountNum(_T(""))
	, m_strGain(_T(""))
	, m_strSend(_T(""))
	, m_bTakeCard{}
	, m_strMe(_T("������ �����Դϴ�"))
{
}

CGameDlg::~CGameDlg()
{
}

void CGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_GAIN, m_strGain);
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
	DDX_Control(pDX, IDC_EDIT_WHOLECOUNTNUM, m_strWholeCountNum);
	DDX_Control(pDX, IDC_EDIT_CARDCOUNTNUM, m_strCardCountNum);
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

	int iOtherCardCnt = 0;

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
		AfxMessageBox("������ ī�带 ȹ���߽��ϴ�!");
		break;
	case SOC_TEXT:
		str.Format("%s", pTemp + 1);
		m_list.AddString("(����) : " + str);
		break;
	case SOC_NOTURN:
		strCardCnt.Format("%d", m_lstMyCard.size());
		SendGame(SOC_NOTURN, strCardCnt);

		strOtherCardCnt.Format("%s", pTemp + 1);
		iOtherCardCnt = atoi(strOtherCardCnt.GetString());

		CheckWin(iOtherCardCnt);
		break;
	case SOC_GAMEEND:
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

	/* ī�� ���� */
	SetDlgItemInt(IDC_EDIT_WHOLECOUNTNUM, wcnt);
	SetDlgItemInt(IDC_EDIT_CARDCOUNTNUM, ccnt);

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

	/* ���� �������� �˻� */
	if (IsGameEnd())
		return;

	/* �� ���� */
	ChangeMyTurn(FALSE);

	wcnt--;
	ccnt--;
}

void CGameDlg::OnBnClickedButtonSend()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	SendGame(SOC_TEXT, m_strSend);
	
	m_list.AddString("  (��) : " + m_strSend);
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
		return TRUE;
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
	if (iOtherCnt > int(m_lstMyCard.size())) // ����� ī�尡 ������ ���� ��
	{
		m_bWin = FALSE;
		AfxMessageBox("�й��߽��ϴ�");
	}
	else if (iOtherCnt < int(m_lstMyCard.size()))
	{
		m_bWin = TRUE;
		AfxMessageBox("�¸��߽��ϴ�");
	}
	else
	{
		m_bWin = FALSE;
		AfxMessageBox("���º��Դϴ�");
	}

	SendGame(SOC_GAMEEND);
	m_bGameEnd = TRUE;
}

BOOL CGameDlg::IsGameEnd()
{
	/* ������ �������� �˻��ϴ� �Լ� */
	if (m_lstMyCard.size() <= 0) // ���� ���� ī�尡 ���� ��
	{
		m_bWin = FALSE;
		SendGame(SOC_GAMEEND);

		AfxMessageBox("�й��߽��ϴ�");

		m_bGameEnd = TRUE;

		return TRUE;
	}

	//if (m_iTurnCnt <= 0) // ��ü �ϼ��� ���� �� 
	//{
	//	CString strCardCnt;
	//	strCardCnt.Format("%d", m_lstMyCard.size());

	//	SendGame(SOC_NOTURN, strCardCnt);

	//	return TRUE;
	//}

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
		AfxMessageBox("���� �߸��Ƴ���. ī�� �ϳ��� �Ҿ����ϴ�!");
		/* ������ �� ī�� ���� */
		char pCardInfo[MID_STR] = "";
		sprintf_s(pCardInfo, "%d%d", tCard.iFruitID, tCard.iFruitCnt);
		SendGame(SOC_NOTAKECARD,pCardInfo); // ��뿡�� ���� �߸� �������� �˸�
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
}