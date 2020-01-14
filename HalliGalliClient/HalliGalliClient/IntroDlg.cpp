// IntroDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "HalliGalliClient.h"
#include "IntroDlg.h"
#include "afxdialogex.h"

#include "ConnectDlg.h"
#include "GameDlg.h"
#include "Rule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
public:
	virtual BOOL DestroyWindow();
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


// CIntroDlg ��ȭ ����



CIntroDlg::CIntroDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_INTRO_DLG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIntroDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_2P, m_Radio[RADIO_2P]);
	DDX_Control(pDX, IDC_RADIO_4P, m_Radio[RADIO_4P]);
}

BEGIN_MESSAGE_MAP(CIntroDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RADIO_2P, &CIntroDlg::OnBnClickedRadio2p)
	ON_BN_CLICKED(IDC_RADIO_4P, &CIntroDlg::OnBnClickedRadio4p)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CIntroDlg::OnBnClickedButtonConnect)
	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
	ON_BN_CLICKED(IDC_BUTTON_HELP, &CIntroDlg::OnClickedButtonHelp)
END_MESSAGE_MAP()


// CIntroDlg �޽��� ó����

BOOL CIntroDlg::OnInitDialog()
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
	/* ���� ��ư 2�ο��� �⺻���� ���õ��ְ� �ʱ�ȭ 
	   �� �� ���ÿ� üũ�� �� ����					*/
	m_Radio[RADIO_2P].SetCheck(TRUE);
	m_Radio[RADIO_4P].SetCheck(FALSE);

	/* Ÿ�̸� �� ������ ���� */
	m_pImmediateTimer = CTimer::Create();
	m_p60Timer = CTimer::Create();
	m_pFrame = CFrame::Create(60.f);

	
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CIntroDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CIntroDlg::OnPaint()
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
HCURSOR CIntroDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CAboutDlg::DestroyWindow()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	return CDialogEx::DestroyWindow();
}

LRESULT CIntroDlg::OnKickIdle(WPARAM wParam, LPARAM lParam)
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

void CIntroDlg::OnBnClickedRadio2p()
{
	/* 2�ο� üũ �� ������ �Լ� */
}

void CIntroDlg::OnBnClickedRadio4p()
{
	/* 4�ο� üũ �� ������ �Լ� */
	AfxMessageBox("���� ���� �����Դϴ�^_^");

	/* 4�ο� üũ�ص� �����ϵ��� ���� */
	m_Radio[RADIO_2P].SetCheck(TRUE);
	m_Radio[RADIO_4P].SetCheck(FALSE);
}

void CIntroDlg::OnBnClickedButtonConnect()
{
	/* ���� ��ư Ŭ�� �� ������ �Լ� */
	CConnectDlg* pConnectDlg = new CConnectDlg;

	if (pConnectDlg->DoModal() == IDOK)
	{
		//pConnectDlg->UpdateData(TRUE);

		CSocCom* pSocCom = new CSocCom;
		pSocCom->Create();

		BOOL bResult = pSocCom->Connect(pConnectDlg->m_strIP, DEFAULT_PORT);
		if (bResult)
		{
			/* ���� ������ GameDlg �� ��ȯ */
			CGameDlg* pGameDlg = new CGameDlg;
			pGameDlg->Create(IDD_GAME_DLG);

			/* ���� ������ ���� */
			pGameDlg->InitSocket(pSocCom);

			pGameDlg->ShowWindow(SW_SHOW);
			ShowWindow(SW_HIDE);
		}
		else
		{
			/* ���� ���� */
			AfxMessageBox("���� ����");
		}
	}
}

void CIntroDlg::GameDlgEnd()
{
	/* Ÿ�̸� ���� */
	if (m_pImmediateTimer)
		delete m_pImmediateTimer;
	if (m_p60Timer)
		delete m_p60Timer;
	if (m_pFrame)
		delete m_pFrame;

	SendMessage(WM_CLOSE);



}


void CIntroDlg::OnClickedButtonHelp()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	m_rulerule = new CRule();
	m_rulerule->Create(IDD_RULE,this);
	m_rulerule->ShowWindow(SW_SHOW);
}
