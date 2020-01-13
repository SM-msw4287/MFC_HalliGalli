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
	, m_strCardCount(_T(""))
	, m_strGain(_T(""))
	, m_strSend(_T(""))
	, m_bWin{}
{

}

CGameDlg::~CGameDlg()
{
}

void CGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_CARDCOUNT, m_strCardCount);
	DDX_Text(pDX, IDC_STATIC_GAIN, m_strGain);
	DDX_Control(pDX, IDC_STATIC_ME, m_strMe);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Text(pDX, IDC_EDIT_SEND, m_strSend);
	DDX_Control(pDX, IDC_IMG_BELL, m_BellPicCtrl);
	DDX_Control(pDX, IDC_IMG_OTHER_OWN, m_CardPicCtrl[USER_OTHER][OWN]);
	DDX_Control(pDX, IDC_IMG_OTHER_THROWN, m_CardPicCtrl[USER_OTHER][THROWN]);
	DDX_Control(pDX, IDC_IMG_PLAYER_OWN, m_CardPicCtrl[USER_PLAYER][OWN]);
	DDX_Control(pDX, IDC_IMG_PLAYER_THROWN, m_CardPicCtrl[USER_PLAYER][THROWN]);
}


BEGIN_MESSAGE_MAP(CGameDlg, CDialogEx)
	ON_MESSAGE(UM_RECEIVE, OnReceive)
	ON_STN_CLICKED(IDC_STATIC_GAIN, &CGameDlg::OnStnClickedStaticGain)
	ON_STN_CLICKED(IDC_IMG_PLAYER_OWN, &CGameDlg::OnClieckedImgPlayerOwn)
	ON_STN_CLICKED(IDC_IMG_OTHER_OWN, &CGameDlg::OnClickedImgOtherOwn)
	ON_BN_CLICKED(IDC_BUTTON_SUB, &CGameDlg::OnBnClickedButtonSub)
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

// CGameDlg �޽��� ó�����Դϴ�.
void CGameDlg::OnClieckedImgPlayerOwn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
	CARD tCard;
	
	tCard.iFruitID = FRUIT_CHEERY;
	tCard.iFruitCnt = 1;
	addMyThrownCard(tCard);

	ChangeCardImage(USER_PLAYER, THROWN, m_lstMyThrownCard.back());
	
}


void CGameDlg::OnClickedImgOtherOwn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	static bool bTemp2 = false;
	CARD tCard;
	if (bTemp2)
	{
		tCard.iFruitID = FRUIT_PEAR;
		tCard.iFruitCnt = 3;
		ChangeCardImage(USER_OTHER, THROWN, tCard);
	}
	else
	{
		tCard.iFruitID = FRUIT_CHEERY;
		tCard.iFruitCnt = 2;
		ChangeCardImage(USER_OTHER, THROWN, tCard);
	}

	bTemp2 = !bTemp2;
}

void CGameDlg::OnStnClickedStaticGain()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CGameDlg::OnCancel()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	CDialogEx::OnCancel();
	DestroyWindow();

	/* ��Ʈ�� ���̾�α� ���� */
	g_pIntroDlg->DestroyWindow();
}

void CGameDlg::OnBnClickedButtonSub()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	deleteAllMyThrownCard();


	ChangeCardImage(USER_OTHER, THROWN, m_lstMyThrownCard.front());
}

void CGameDlg::addMyThrownCard(const CARD sCard)
{
	m_lstMyThrownCard.push_back(sCard);
	
}

void CGameDlg::deleteAllMyThrownCard()
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

void CGameDlg::addOtherThrownCard(const CARD sCard)
{
	m_lstOtherThrownCard.push_back(sCard);
}

void CGameDlg::deleteAllOtherThrownCard()
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
