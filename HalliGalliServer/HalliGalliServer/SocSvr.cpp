#include "stdafx.h"
#include "SocSvr.h"

CSocSvr::CSocSvr()
{
}

CSocSvr::~CSocSvr()
{
}

void CSocSvr::Init(HWND hWnd)
{
	m_hWnd = hWnd;
}

CSocCom * CSocSvr::GetAcceptSocCom()
{
	return &m_socCom;
}

void CSocSvr::OnAccept(int nErrorCode)
{
	// Ŭ�� �����ϸ� ���� �����쿡 UM_ACCEPT �޽��� ����
	Accept(m_socCom);
	SendMessage(m_hWnd, UM_ACCEPT, 0, 0);

	CSocket::OnAccept(nErrorCode);
}