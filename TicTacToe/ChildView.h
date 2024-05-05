#pragma once

#include "GameLogic.h"
#include "UIBoard.h"

class CChildView : public CWnd
{
public:
	CChildView();
	virtual ~CChildView();

protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

private:
	void AnnonceGameResult();

private:
	UIBoard m_board;
	TicTacToe::TicTacToe m_game;
};
