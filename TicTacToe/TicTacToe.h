
// TicTacToe.h : main header file for the TicTacToe application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CTicTacToeApp:
// See TicTacToe.cpp for the implementation of this class
//

class CTicTacToeApp : public CWinApp
{
public:
	CTicTacToeApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CTicTacToeApp theApp;
