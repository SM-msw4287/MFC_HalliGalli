
// HalliGalliServerDlg.h : ��� ����
//

#pragma once


// CHalliGalliServerDlg ��ȭ ����
class CHalliGalliServerDlg : public CDialogEx
{
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
	DECLARE_MESSAGE_MAP()


public:
	HRESULT Load_Image();

public:
	list<CImage*> m_pImgLst[FRUIT_END]; // �̹��� ���� ����Ʈ
};
