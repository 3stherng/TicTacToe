#pragma once

class UIBoard
{
public:
  explicit UIBoard(size_t i_game_size);
  ~UIBoard();

  void SetClientRect(const CRect& i_client_rect);
  void DrawBoard(CDC* pDC);
  size_t DrawMarkerOnBoard(CDC* pDC, const CPoint& i_point, const char& i_char);

private:
  void _DrawSymbol(CDC* pDC, const char& i_char, const size_t& i_cell);
  size_t _GetCellPosition(const CPoint& i_point);

private:
  size_t m_game_size;
  CRect m_client_rect;
  size_t m_cell_size;
  size_t m_symbol_size;
  size_t m_border_size;
  std::vector<std::pair<CRect, char>> m_game_board;
  std::unordered_map<char, COLORREF> m_symbol_colors;
  std::stack<COLORREF> m_colors;
};
