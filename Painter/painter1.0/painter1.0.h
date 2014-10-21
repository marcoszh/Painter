
// painter1.0.h : main header file for the painter1.0 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// Cpainter10App:
// See painter1.0.cpp for the implementation of this class
//

class Cpainter10App : public CWinAppEx
{
public:
	Cpainter10App();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
//	int m_ShapeIndex;
};

extern Cpainter10App theApp;
