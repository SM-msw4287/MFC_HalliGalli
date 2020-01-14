
// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.

// MFC�� ���� �κа� ���� ������ ��� �޽����� ���� ����⸦ �����մϴ�.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ����Դϴ�.
#include <afxext.h>         // MFC Ȯ���Դϴ�.


#include <afxdisp.h>        // MFC �ڵ�ȭ Ŭ�����Դϴ�.



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC�� ���� �� ��Ʈ�� ���� ����

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

/* STL ���� ��Ŭ��� */
#include <vector>
#include <list>
#include <map>
#include <algorithm>

using namespace std;

/* ���� ���� ��Ŭ��� �� Define */
#include <afxsock.h>

#include "SocSvr.h"

#define UM_ACCEPT	WM_USER+1
#define UM_RECEIVE	WM_USER+2

// �̺�Ʈ ó�� 
#define SOC_INITGAME		0	// ���� ������: ������ ī�� ������(CARD ����ü ������)
#define SOC_GAMESTART		1	// ���� ������: X
#define SOC_GAMEEND			2	// ���� ������: ����� ���� ����
#define SOC_TEXT			3	// ���� ������: ������ ���ڿ�(m_strSend)
#define SOC_THROWNCARD		4	// ���� ������: ���� ī��
#define SOC_BELL			5	// ���� ������: X
#define SOC_TAKECARD		6	// ���� ������: X
#define SOC_NOTAKECARD		7	// ���� ������: �г�Ƽ ī�� ���� ����
#define SOC_NOTURN			8	// ���� ������: �� ī�� ����

#define DEFAULT_PORT	5000
#define DEFAULT_IP		"127.0.0.1"

/* ����*/
#include "fmod.h"
#pragma comment(lib, "fmodex_vc.lib")

class CSoundMgr;
extern CSoundMgr* g_pSoundMgr;

/* ��Ÿ */
#include <afxpriv.h>

#include "Struct.h"
#include "Macro.h"

#include "ImageMgr.h" // �̹��� ���� Ŭ����

#define CARD_CNT 28
#define CARD_HALF_CNT 14

#define TURN_CNT	50

#define GAME_WIN	0	// ��밡 �̱�	
#define GAME_LOSE	1	// ��밡 ��
#define GAME_DRAW	2	// ���º�

#define MID_STR		128
#define MAX_STR		256
	
extern float g_fDeltaTime;

#pragma warning(disable : 4985)