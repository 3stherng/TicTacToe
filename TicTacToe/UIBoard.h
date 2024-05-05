#pragma once

#include <vector>

class UIBoard
{
public:
  UIBoard(const size_t& i_game_size);
  ~UIBoard();

  void SetClientRect(const CRect& i_client_rect);
  void DrawNewBoard(CDC* pDC);
  size_t UpdateBoard(CDC* pDC, const CPoint& i_point, const char& i_char);
  
private:
  int _GetCellPosition(const CPoint& i_point);

private:
  size_t m_game_size;
  CRect m_client_rect;
  size_t m_cell_size;
  size_t m_symbol_size;
  size_t m_border_size;
  std::vector<CRect> m_game_board;
};
