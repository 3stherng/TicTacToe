#include "pch.h"

#include "UIBoard.h"

#include <string>

UIBoard::UIBoard(const size_t& i_game_size)
  : m_game_size(i_game_size)
  , m_client_rect(CRect(0, 0, 0, 0))
  , m_cell_size(50)
  , m_symbol_size(40)
  , m_border_size(10)
  , m_game_board({})
{
  m_game_board.reserve(9);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UIBoard::~UIBoard()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIBoard::SetClientRect(const CRect& i_client_rect)
{
  m_client_rect = i_client_rect;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIBoard::DrawNewBoard(CDC* pDC)
{
  CPen pen(PS_SOLID, m_border_size, RGB(0, 0, 0));
  CPen* p_old_pen = pDC->SelectObject(&pen);

  m_cell_size = m_client_rect.Height() / m_game_size;
  m_symbol_size = m_cell_size - 2 * m_border_size;

  for (auto i = 0; i < m_game_size; ++i)
  {
    for (auto j = 0; j < m_game_size; ++j)
    {
      m_game_board.push_back(CRect(m_cell_size * i, m_cell_size * j, m_cell_size * (i + 1), m_cell_size * (j + 1)));
    }
  }

  for (auto i = 1; i < m_game_size; ++i)
  {
    pDC->MoveTo(m_cell_size * i, 0);
    pDC->LineTo(m_cell_size * i, m_cell_size * m_game_size);
    pDC->MoveTo(0, m_cell_size * i);
    pDC->LineTo(m_cell_size * m_game_size, m_cell_size * i);
  }

  pDC->SelectObject(p_old_pen);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

size_t UIBoard::UpdateBoard(CDC* pDC, const CPoint& i_point, const char& i_char)
{

  CPen pen(PS_SOLID, m_border_size, RGB(0, 0, 0));
  const auto p_old_pen = pDC->SelectObject(&pen);

  CFont font;
  font.CreatePointFont(720, L"Comic Sans MS");
  CFont* p_old_font = pDC->SelectObject(&font);

  const auto cell_position = _GetCellPosition(i_point);
  if (cell_position < 0)
  {
    return 10000;
  }
  
  const auto row = cell_position / m_game_size;
  const auto column = cell_position % m_game_size;

  CRect rect(
    row * m_cell_size + m_border_size,
    column * m_cell_size + m_border_size,
    row * m_cell_size + m_border_size + m_symbol_size,
    column * m_cell_size + m_border_size + m_symbol_size);

  pDC->SetTextColor(RGB(0, 0, 255));
  pDC->SetBkMode(TRANSPARENT);
  pDC->DrawText(std::wstring(1, i_char).c_str(), m_game_board[cell_position], DT_SINGLELINE | DT_CENTER | DT_VCENTER);
  pDC->SelectObject(p_old_font);

  return cell_position;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int UIBoard::_GetCellPosition(const CPoint& i_point)
{
  for (int i = 0; i < m_game_board.size(); ++i) 
  {
    if (m_game_board[i].PtInRect(i_point))
    {
      return i;
    }
  }
  return -1;
}