
// HalliGalliServerDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "HalliGalliServer.h"
#include "HalliGalliServerDlg.h"
#include "afxdialogex.h"

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
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHalliGalliServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHalliGalliServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(UM_ACCEPT, OnAccept)
	ON_MESSAGE(UM_RECEIVE, OnReceive)
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

	/* ���� �ʱ�ȭ */
	InitSocket();
	InitCardDed();

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
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	/* �̹��� �Ŵ��� ���� */
	CImageMgr::DestroyInstance();
	m_pImgMgr = nullptr;

	return CDialogEx::DestroyWindow();
}

LPARAM CHalliGalliServerDlg::OnAccept(UINT wParam, LPARAM lParam)
{
	AfxMessageBox("Ŭ���̾�Ʈ ����");
	//m_strStatus = "���� ����";

	m_pSocCom = m_socServer.GetAcceptSocCom();
	m_pSocCom->Init(this->m_hWnd);

	//UpdateData(FALSE);
	return TRUE;
}

LPARAM CHalliGalliServerDlg::OnReceive(UINT wParam, LPARAM lParam)
{
	return TRUE;
}

void CHalliGalliServerDlg::InitSocket()
{
	/* SocCom, SocServer �ʱ�ȭ �� ���� �Լ� */
	m_pSocCom = NULL;
	m_socServer.Create(DEFAULT_PORT);
	m_socServer.Listen();
	m_socServer.Init(this->m_hWnd);
}

void CHalliGalliServerDlg::InitCardDed()
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
	for (int i = 0; i < 28; i++)
		m_shuffleCardIndex[i] = i;

	int nRandomIndex1 = 0;
	int nRandomIndex2 = 0;

	srand((unsigned int)time(NULL));

	for (int i = 0; i < 84; i++)
	{
		nRandomIndex1 = rand() % 28;
		nRandomIndex2 = rand() % 28;

		int nTemp = 0;
		nTemp = m_shuffleCardIndex[nRandomIndex1];
		m_shuffleCardIndex[nRandomIndex1] = m_shuffleCardIndex[nRandomIndex2];
		m_shuffleCardIndex[nRandomIndex2] = nTemp;
	}
}
