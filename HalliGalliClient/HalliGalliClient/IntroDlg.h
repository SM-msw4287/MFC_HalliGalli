
// IntroDlg.h : 헤더 파일
//

#pragma once

// CIntroDlg 대화 상자
class CIntroDlg : public CDialogEx
{
	enum RADIO_INDEX { RADIO_2P, RADIO_4P, RADIO_END };

// 생성입니다.
public:
	CIntroDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INTRO_DLG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadio2p();
	afx_msg void OnBnClickedRadio4p();

	afx_msg void OnBnClickedButtonConnect(); 

public:
	CButton m_Radio[RADIO_END];
};
