
// HalliGalliClient.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CHalliGalliClientApp:
// �� Ŭ������ ������ ���ؼ��� HalliGalliClient.cpp�� �����Ͻʽÿ�.
//
//
class CHalliGalliClientApp : public CWinApp
{
public:
	CHalliGalliClientApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CHalliGalliClientApp theApp;