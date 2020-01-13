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
	, m_bWin{}
	, m_strMe(_T(""))
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
	ON_BN_CLICKED(IDC_BUTTON_SUB, &CGameDlg::OnBnClickedButtonSub)
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

	switch (iType)
	{
	case SOC_INITGAME:
		/* �����κ��� ī�� �ޱ� */
		if (ReceiveCard(pTemp + 1) == TRUE)
			SendGame(SOC_GAMESTART);
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
		
		GetDlgItem(IDC_IMG_PLAYER_OWN)->EnableWindow(TRUE);
		break;
	case SOC_BELL:
		break;
	case SOC_TAKECARD:
		break;
	case SOC_TEXT:
		str.Format("%s", pTemp + 1);
		m_list.AddString("(����) : " + str);
		break;
	case SOC_GAMEEND:
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
}

void CGameDlg::ChangeCardImage(const USER_ID & eID, const CARD_STATUS & eStatus, const CARD & tCard)
{
	CImage* pImage = m_pImgMgr->GetCardImage(tCard.iFruitID, tCard.iFruitCnt);

	if(pImage != nullptr)
		m_CardPicCtrl[eID][eStatus].SetBitmap(*pImage);
}


void CGameDlg::OnClickedImgPlayerOwn()
{
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
	GetDlgItem(IDC_IMG_PLAYER_OWN)->EnableWindow(FALSE);
	SendGame(SOC_THROWNCARD, pCardInfo);

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
	//���� �̰��� ��
	if (m_bWin)
	{
		nThrowCardCount = m_lstMyThrownCard.size();

		for (int i = 0; i < nThrowCardCount; i++)
		{
			m_lstMyCard.push_back(m_lstMyThrownCard.back());
			m_lstMyThrownCard.pop_back();
		}
	}
	//���� ��
	else
	{
		m_lstMyThrownCard.clear();
	}
	
}

void CGameDlg::AddOtherThrownCard(const CARD sCard)
{
	m_lstOtherThrownCard.push_back(sCard);
}

void CGameDlg::DeleteAllOtherThrownCard()
{
	int nThrowCardCount = 0;
	//���� �̰�����
	if (m_bWin)
	{
		nThrowCardCount = m_lstOtherThrownCard.size();

		for (int i = 0; i < nThrowCardCount; i++)
		{
			m_lstMyCard.push_back(m_lstOtherThrownCard.back());
			m_lstOtherThrownCard.pop_back();
		}
	}
	//���� ������
	else
	{
		m_lstOtherThrownCard.clear();
	}
}


void CGameDlg::OnClickedImgBell()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	SendGame(SOC_BELL);
	CheckFive();
	if (m_bWin)
	{
		Win();
		SendGame(SOC_BELL);
		//SOC_TAKECARD
	}
}

void CGameDlg::CheckFive()
{
	if ((m_lstMyThrownCard.back().iFruitCnt + m_lstOtherThrownCard.back().iFruitCnt) == 5)
		m_bWin = TRUE;

}

void CGameDlg::Win()
{
	DeleteAllMyThrownCard();
	DeleteAllOtherThrownCard();
	m_bWin = FALSE;
}