#pragma once


// CIntroDlg ��ȭ �����Դϴ�.

class CIntroDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CIntroDlg)

public:
	CIntroDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CIntroDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_INTRO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
