
// HalliGalliServer.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CHalliGalliServerApp:
// �� Ŭ������ ������ ���ؼ��� HalliGalliServer.cpp�� �����Ͻʽÿ�.
//

class CHalliGalliServerApp : public CWinApp
{
public:
	CHalliGalliServerApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CHalliGalliServerApp theApp;