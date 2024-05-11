#include "pch.h"

#include "UIBoard.h"

UIBoard::UIBoard(size_t i_game_size)
  : m_game_size(i_game_size)
  , m_client_rect(CRect(0, 0, 0, 0))
  , m_cell_size(50)
  , m_symbol_size(40)
  , m_border_size(10)
  , m_game_board({})
{
  m_colors.push(RGB(0, 0, 0));
  m_colors.push(RGB(255, 0, 0));
  m_colors.push(RGB(0, 255, 0));
  m_colors.push(RGB(0, 0, 255));

  for (int i = 0; i < m_game_size * m_game_size; ++i)
    m_game_board.emplace_back(std::make_pair(CRect(0, 0, 0, 0), ' '));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UIBoard::~UIBoard()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIBoard::SetClientRect(const CRect& i_client_rect)
{
  m_client_rect = i_client_rect;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIBoard::DrawBoard(CDC* pDC)
{
  CPen pen(PS_SOLID, static_cast<int>(m_border_size), RGB(0, 0, 0));
  CPen* p_old_pen = pDC->SelectObject(&pen);

  m_cell_size = m_client_rect.Height() / m_game_size;
  m_symbol_size = m_cell_size - 2 * m_border_size;

  for (auto i = 0; i < m_game_size; ++i)
  {
    for (auto j = 0; j < m_game_size; ++j)
    {
      m_game_board[i * m_game_size + j].first = CRect(
        static_cast<int>(m_cell_size * i),
        static_cast<int>(m_cell_size * j),
        static_cast<int>(m_cell_size * (i + 1)),
        static_cast<int>(m_cell_size * (j + 1)));
    }
  }

  for (auto i = 1; i < m_game_size; ++i)
  {
    pDC->MoveTo(static_cast<int>(m_cell_size) * i, 0);
    pDC->LineTo(static_cast<int>(m_cell_size * i), static_cast<int>(m_cell_size * m_game_size));
    pDC->MoveTo(0, static_cast<int>(m_cell_size) * i);
    pDC->LineTo(static_cast<int>(m_cell_size * m_game_size), static_cast<int>(m_cell_size * i));
  }

  for (auto i = 0; i < m_game_size * m_game_size; ++i)
  {
    if (m_game_board[i].second != ' ')
    {
      _DrawSymbol(pDC, m_game_board[i].second, i);
    }
  }

  pDC->SelectObject(p_old_pen);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

size_t UIBoard::DrawMarkerOnBoard(CDC* pDC, const CPoint& i_point, const char& i_char)
{
  if (!m_symbol_colors.count(i_char) && m_symbol_colors.size() < 4)
  {
    m_symbol_colors[i_char] = m_colors.top();
    m_colors.pop();
  }

  auto cell = _GetCellPosition(i_point);

  if (m_game_board[cell].second == ' ')
  {
    _DrawSymbol(pDC, i_char, cell);
    m_game_board[cell].second = i_char;
  }

  return cell;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UIBoard::_DrawSymbol(CDC* pDC, const char& i_char, const size_t& i_cell)
{
  CFont font;
  font.CreatePointFont(720, L"Comic Sans MS");
  CFont* p_old_font = pDC->SelectObject(&font);

  pDC->SetTextColor(m_symbol_colors[i_char]);

  pDC->SetBkMode(TRANSPARENT);
  pDC->DrawText(std::wstring(1, i_char).c_str(), m_game_board[i_cell].first,
    DT_SINGLELINE | DT_CENTER | DT_VCENTER);
  pDC->SelectObject(p_old_font);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

size_t UIBoard::_GetCellPosition(const CPoint& i_point)
{
  for (int i = 0; i < m_game_board.size(); ++i)
  {
    if (m_game_board[i].first.PtInRect(i_point))
    {
      return i;
    }
  }
  return sizeof(size_t);
}
