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
	ON_MESSAGE(UM_RECEIVE, OnReceive)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CGameDlg::OnBnClickedButtonSend)
	ON_STN_CLICKED(IDC_IMG_PLAYER_OWN, &CGameDlg::OnClickedImgPlayerOwn)
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
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	/* �̹��� �Ŵ��� ���� */
	CImageMgr::DestroyInstance();
	m_pImgMgr = nullptr;

	return CDialogEx::DestroyWindow();
}

LPARAM CGameDlg::OnReceive(UINT wParam, LPARAM lParam)
{
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


//void CGameDlg::OnClickedImgOtherOwn()
//{
//	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//	static bool bTemp2 = false;
//	CARD tCard;
//	if (bTemp2)
//	{
//		tCard.iFruitID = FRUIT_PEAR;
//		tCard.iFruitCnt = 3;
//		ChangeCardImage(USER_OTHER, THROWN, tCard);
//	}
//	else
//	{
//		tCard.iFruitID = FRUIT_CHEERY;
//		tCard.iFruitCnt = 2;
//		ChangeCardImage(USER_OTHER, THROWN, tCard);
//	}
//
//	bTemp2 = !bTemp2;
//}


void CGameDlg::OnCancel()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	CDialogEx::OnCancel();
	DestroyWindow();

	/* ��Ʈ�� ���̾�α� ���� */
	g_pIntroDlg->DestroyWindow();
}

void CGameDlg::OnClickedImgPlayerOwn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	static bool bTemp = false;

	SetDlgItemInt(IDC_EDIT_WHOLECOUNTNUM, wcnt);
	SetDlgItemInt(IDC_EDIT_CARDCOUNTNUM, ccnt);

	CARD tCard;
	if (bTemp)
	{
		tCard.iFruitID = FRUIT_CHEERY;
		tCard.iFruitCnt = 1;
		ChangeCardImage(USER_PLAYER, THROWN, tCard);
	}
	else
	{
		tCard.iFruitID = FRUIT_PEAR;
		tCard.iFruitCnt = 5;
		ChangeCardImage(USER_PLAYER, THROWN, tCard);
	}

	bTemp = !bTemp;
	wcnt--;
	ccnt--;
}

void CGameDlg::OnBnClickedButtonSend()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	SendChat(SOC_TEXT, m_strSend);
	m_list.AddString(">>" + m_strSend);
	m_strSend = (_T(""));
	UpdateData(FALSE);
}

void CGameDlg::SendChat(int iType, CString strTmp) {
	UpdateData(TRUE);
	char pTmp[256];

	memset(pTmp, '\0', 256);
	
	sprintf_s(pTmp, "%d%s", iType, strTmp);

	m_pSocCom->Send(pTmp, 256);
}
