#pragma once

#include "GameLogic.h"
#include "UIBoard.h"

class CChildView : public CWnd
{
public:
	CChildView(size_t i_number_of_players, size_t i_board_size);
	virtual ~CChildView();

	void ResetGame(const size_t& i_player_number, const size_t& m_board_size);

protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs) override;

private:
	void _AnnounceGameResult();

private:
	UIBoard m_board;
	TicTacToe::TicTacToe m_game;
	size_t m_player_number;
	size_t m_board_size;
};
