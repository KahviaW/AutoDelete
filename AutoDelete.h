
// AutoDelete.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CAutoDeleteApp: 
// �йش����ʵ�֣������ AutoDelete.cpp
//

class CAutoDeleteApp : public CWinApp
{
public:
	CAutoDeleteApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CAutoDeleteApp theApp;