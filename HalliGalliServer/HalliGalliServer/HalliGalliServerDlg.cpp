
// HalliGalliServerDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "HalliGalliServer.h"
#include "HalliGalliServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int wcnt = 5;
int ccnt = 5;

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CHalliGalliServerDlg ��ȭ ����



CHalliGalliServerDlg::CHalliGalliServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_HALLIGALLISERVER_DIALOG, pParent)
	, m_strGain(_T(""))
	, m_strSend(_T(""))
	, m_bTakeCard{}
	, m_strMe(_T("Ŭ���̾�Ʈ ���� �����"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHalliGalliServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);


	DDX_Control(pDX, IDC_IMG_BELL, m_BellPicCtrl);
	DDX_Control(pDX, IDC_IMG_OTHER_OWN, m_CardPicCtrl[USER_OTHER][OWN]);
	DDX_Control(pDX, IDC_IMG_OTHER_THROWN, m_CardPicCtrl[USER_OTHER][THROWN]);
	DDX_Control(pDX, IDC_IMG_PLAYER_OWN, m_CardPicCtrl[USER_PLAYER][OWN]);
	DDX_Control(pDX, IDC_IMG_PLAYER_THROWN, m_CardPicCtrl[USER_PLAYER][THROWN]);
	DDX_Control(pDX, IDC_EDIT_CARDCOUNTNUM, m_strCardCountNum);
	DDX_Control(pDX, IDC_EDIT_WHOLECOUNTNUM, m_strWholeCountNum);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_STATIC_WHOLECOUNT, m_strWholeCount);
	DDX_Control(pDX, IDC_STATIC_CARDCOUNT, m_strCardCount);
	DDX_Text(pDX, IDC_STATIC_ME, m_strMe);
	DDX_Text(pDX, IDC_STATIC_GAIN, m_strGain);
	DDX_Text(pDX, IDC_EDIT_SEND, m_strSend);
}

BEGIN_MESSAGE_MAP(CHalliGalliServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(UM_ACCEPT, OnAccept)
	ON_MESSAGE(UM_RECEIVE, OnReceive)
	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
	ON_STN_CLICKED(IDC_IMG_PLAYER_OWN, &CHalliGalliServerDlg::OnClickedImgPlayerOwn)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CHalliGalliServerDlg::OnClickedButtonSend)
	ON_STN_CLICKED(IDC_IMG_BELL, &CHalliGalliServerDlg::OnClickedImgBell)
END_MESSAGE_MAP()


// CHalliGalliServerDlg �޽��� ó����

BOOL CHalliGalliServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	
	/* �̹��� ���� �Ŵ��� �ʱ�ȭ => Image���� ���� �̹��� �ε��� */
	m_pImgMgr = CImageMgr::GetInstance();
	m_pImgMgr->Initialize();

	/* Picture Ctrl �ʱ�ȭ */
	InitPicCtrl();

	/* ���� �ʱ�ȭ */
	InitSocket();

	/* Ÿ�̸� �� ������ ���� */
	m_pImmediateTimer = CTimer::Create();
	m_p60Timer = CTimer::Create();
	m_pFrame = CFrame::Create(60.f);

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CHalliGalliServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CHalliGalliServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CHalliGalliServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CHalliGalliServerDlg::DestroyWindow()
{
	/* �̹��� �Ŵ��� ���� */
	CImageMgr::DestroyInstance();
	m_pImgMgr = nullptr;

	/* Ÿ�̸� ���� */
	if (m_pImmediateTimer)
		delete m_pImmediateTimer;
	if (m_p60Timer)
		delete m_p60Timer;
	if (m_pFrame)
		delete m_pFrame;

	return CDialogEx::DestroyWindow();
}

LPARAM CHalliGalliServerDlg::OnAccept(UINT wParam, LPARAM lParam)
{
	AfxMessageBox("Ŭ���̾�Ʈ ����");
	//m_strStatus = "���� ����";

	m_pSocCom = m_socServer.GetAcceptSocCom();
	m_pSocCom->Init(this->m_hWnd);

	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(TRUE);
	UpdateData(FALSE);
	
	// Ŭ���̾�Ʈ ���� �� ���� �ʱ�ȭ
	m_bConnect = TRUE;
	InitGame();

	return TRUE;
}

LPARAM CHalliGalliServerDlg::OnReceive(UINT wParam, LPARAM lParam)
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
	case SOC_GAMESTART:
		m_bStartCnt = TRUE;
		break;
	case SOC_THROWNCARD:
		strFruitID.Format("%c", (pTemp + 1)[0]);
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

LRESULT CHalliGalliServerDlg::OnKickIdle(WPARAM wParam, LPARAM lParam)
{
	if (m_pImmediateTimer && m_p60Timer && m_pFrame)
	{
		m_pImmediateTimer->Update();

		float fDeltaTime = m_pImmediateTimer->Get_DeltaTime();

		if (m_pFrame->Is_PermitCall(fDeltaTime))
		{
			m_p60Timer->Update();
			float fTime60 = m_p60Timer->Get_DeltaTime();
			g_fDeltaTime = fTime60;
		}

	}

	return (LRESULT)1;
}

void CHalliGalliServerDlg::InitSocket()
{
	/* SocCom, SocServer �ʱ�ȭ �� ���� �Լ� */
	m_pSocCom = NULL;
	m_socServer.Create(DEFAULT_PORT);
	m_socServer.Listen();
	m_socServer.Init(this->m_hWnd);
}

void CHalliGalliServerDlg::OnClickedButtonSend()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	SendGame(SOC_TEXT, m_strSend);

	m_list.AddString("  (��) : " + m_strSend);
	m_strSend = (_T(""));
	UpdateData(FALSE);
}

void CHalliGalliServerDlg::SendGame(int iType, CString strTemp)
{
	char pTemp[MAX_STR];

	memset(pTemp, '\0', MAX_STR);
	sprintf_s(pTemp, "%d%s", iType, strTemp.GetString());

	m_pSocCom->Send(pTemp, MAX_STR);
}

void CHalliGalliServerDlg::InitPicCtrl()
{
	ChangeCardImage(USER_PLAYER, OWN);
	ChangeCardImage(USER_OTHER, OWN);

	ChangeCardImage(USER_PLAYER, THROWN, CARD(FRUIT_BACK, 2));
	ChangeCardImage(USER_OTHER, THROWN, CARD(FRUIT_BACK, 2));

	CImage* pImage = nullptr;

	pImage = m_pImgMgr->GetImage("Bell");
	if(pImage != nullptr)
		m_BellPicCtrl.SetBitmap(*pImage);
}

void CHalliGalliServerDlg::ChangeCardImage(const USER_ID & eID, const CARD_STATUS & eStatus, const CARD & tCard)
{
	CImage* pImage = m_pImgMgr->GetCardImage(tCard.iFruitID, tCard.iFruitCnt);

	if (pImage != nullptr)
	{
		m_CardPicCtrl[eID][eStatus].SetBitmap(*pImage);
		Invalidate(TRUE);
	}
		
}

void CHalliGalliServerDlg::InitCardDeck()
{
	// 1:ü�� 2:�� ī�� �� �ʱ�ȭ
	int nIndex = 0;
	for (int i = 1; i <= 2; i++)
	{
		for (int j = 1; j <= 5; j++)
		{
			if (j == 1)
			{
				for (int k = 0; k < 5; k++)
				{
					m_cardDec[nIndex].iFruitID = i;
					m_cardDec[nIndex].iFruitCnt = j;
					nIndex++;
				}
			}
			else if (j == 4)
			{
				for (int k = 0; k < 2; k++)
				{
					m_cardDec[nIndex].iFruitID = i;
					m_cardDec[nIndex].iFruitCnt = j;
					nIndex++;
				}
			}
			else if (j == 5)
			{
				m_cardDec[nIndex].iFruitID = i;
				m_cardDec[nIndex].iFruitCnt = j;
				nIndex++;
			}
			else
			{
				for (int k = 0; k < 3; k++)
				{
					m_cardDec[nIndex].iFruitID = i;
					m_cardDec[nIndex].iFruitCnt = j;
					nIndex++;
				}
			}

		}
	}

	// �ε��� ���� 
	for (int i = 0; i < CARD_CNT; i++)
		m_shuffleCardIndex[i] = i;

	int nRandomIndex1 = 0;
	int nRandomIndex2 = 0;

	srand((unsigned int)time(NULL));

	for (int i = 0; i < 84; i++)
	{
		nRandomIndex1 = rand() % CARD_CNT;
		nRandomIndex2 = rand() % CARD_CNT;

		int nTemp = 0;
		nTemp = m_shuffleCardIndex[nRandomIndex1];
		m_shuffleCardIndex[nRandomIndex1] = m_shuffleCardIndex[nRandomIndex2];
		m_shuffleCardIndex[nRandomIndex2] = nTemp;
	}
}

void CHalliGalliServerDlg::InitGame()
{
	if (!m_bConnect)
		return;

	/* ī�� �� �ʱ�ȭ �� ���� */
	InitCardDeck();
	/* Ŭ���̾�Ʈ�� ī�� ���� */
	SendCardToClient();

	/* ���� ���� �� */
	ChangeMyTurn(TRUE);
}

void CHalliGalliServerDlg::SendCardToClient()
{
	for (int i = 0; i < CARD_HALF_CNT; ++i)
	{
		int iSuffleIndex = m_shuffleCardIndex[i];
		CARD tCard = m_cardDec[iSuffleIndex];

		char pCardInfo[MID_STR] = "";

		sprintf_s(pCardInfo, "%d%d", tCard.iFruitID, tCard.iFruitCnt);

		SendGame(SOC_INITGAME, pCardInfo);
	}

	/* Ŭ�� ī�� ���� �� �� ī�� ���� */
	for (int i = CARD_HALF_CNT; i < CARD_CNT; ++i)
	{
		int iSuffleIndex = m_shuffleCardIndex[i];
		CARD tCard = m_cardDec[iSuffleIndex];

		m_lstMyCard.push_back(tCard);
	}

	/* ī�带 �� ������ ���� GAMESTART �޽����� Ŭ�� ���� */
	SendGame(SOC_GAMESTART);
}

void CHalliGalliServerDlg::CheckThrownCard()
{
	int iMyFruitCnt = 0;
	int iOtherFruitCnt = 0;

	if (!m_lstMyThrownCard.empty())
		iMyFruitCnt = m_lstMyThrownCard.back().iFruitCnt;
	if (!m_lstOtherThrownCard.empty())
		iOtherFruitCnt = m_lstOtherThrownCard.back().iFruitCnt;
	
	if (iMyFruitCnt + iOtherFruitCnt == 5)
		m_bTakeCard = TRUE;
	else
		m_bTakeCard = FALSE;
}

void CHalliGalliServerDlg::TakeThrownCard()
{
	DeleteAllMyThrownCard();
	DeleteAllOtherThrownCard();
}

void CHalliGalliServerDlg::AddMyThrownCard(const CARD sCard)
{
	m_lstMyThrownCard.push_back(sCard);
}

void CHalliGalliServerDlg::AddOtherThrownCard(const CARD sCard)
{
	m_lstOtherThrownCard.push_back(sCard);
}

void CHalliGalliServerDlg::DeleteAllMyThrownCard()
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

void CHalliGalliServerDlg::DeleteAllOtherThrownCard()
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

void CHalliGalliServerDlg::ChangeMyTurn(BOOL bMyTurn)
{
	m_bMyTurn = bMyTurn;
	if (m_bMyTurn)
		m_strMe = "����� �����Դϴ�";
	else
		m_strMe = "������ �����Դϴ�";

	GetDlgItem(IDC_IMG_PLAYER_OWN)->EnableWindow(m_bMyTurn);

	UpdateData(FALSE);
}

BOOL CHalliGalliServerDlg::IsGameEnd()
{
	/* ������ �������� �˻��ϴ� �Լ� */
	//if(m_lstMyCard.size() <= 0)


	return 0;
}

void CHalliGalliServerDlg::OnClickedImgPlayerOwn()
{
	if (!m_bConnect || !m_bStartCnt || !m_bMyTurn)
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

	/* Ŭ�� �� ī�� ���� */
	char pCardInfo[MID_STR] = "";
	sprintf_s(pCardInfo, "%d%d", tCard.iFruitID, tCard.iFruitCnt);
	SendGame(SOC_THROWNCARD, pCardInfo);

	/* �� ���� */
	ChangeMyTurn(FALSE);

	wcnt--;
	ccnt--;
}

void CHalliGalliServerDlg::OnClickedImgBell()
{
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
		SendGame(SOC_NOTAKECARD); // ��뿡�� ���� �߸� �������� �˸�
	}
}