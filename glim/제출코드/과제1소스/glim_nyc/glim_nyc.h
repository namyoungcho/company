
// glim_nyc.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// Cglim_nycApp:
// �� Ŭ������ ������ ���ؼ��� glim_nyc.cpp�� �����Ͻʽÿ�.
//

class Cglim_nycApp : public CWinApp
{
public:
	Cglim_nycApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern Cglim_nycApp theApp;