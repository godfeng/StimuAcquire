
// StimuAcquire.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CStimuAcquireApp: 
// �йش����ʵ�֣������ StimuAcquire.cpp
//

class CStimuAcquireApp : public CWinApp
{
public:
	CStimuAcquireApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CStimuAcquireApp theApp;