#include "pch.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CChildView::CChildView(size_t i_number_of_players, size_t i_board_size)
  : m_player_number(i_number_of_players)
  , m_board_size(i_board_size)
  , m_board(UIBoard(i_board_size))
  , m_game(TicTacToe::TicTacToe(i_board_size))
{
  m_game.AddPlayer(m_player_number);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CChildView::~CChildView()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CChildView, CWnd)
  ON_WM_PAINT()
  ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CChildView message handlers
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
  if (!CWnd::PreCreateWindow(cs))
    return FALSE;

  cs.dwExStyle |= WS_EX_CLIENTEDGE;
  cs.style &= ~WS_BORDER;
  cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
    ::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), nullptr);

  return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CChildView::OnLButtonDown(UINT nFlags, CPoint i_point)
{
  CClientDC dc(this);

  auto cell_position = m_board.DrawMarkerOnBoard(&dc, i_point, m_game.GetCurrentPlayerMarker());
  m_game.ContinueGame(cell_position);
  if (m_game.HasGameEnded())
    _AnnounceGameResult();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CChildView::_AnnounceGameResult()
{
  CString annoucement;
  if (m_game.GetGameStatus() == TicTacToe::GameStatus::DRAW)
  {
    annoucement = L"It's a DRAW game!";
  }
  else
  {
    annoucement = (m_game.GetWinnerName() + " has WON the game!").c_str();
  }
  MessageBox(annoucement, _T("Game Over"), MB_ICONEXCLAMATION | MB_OK);
  ResetGame(m_player_number, m_board_size);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CChildView::ResetGame(const size_t& i_player_number, const size_t& i_board_size)
{
  m_game = TicTacToe::TicTacToe(i_board_size);
  m_board = UIBoard(i_board_size);
  m_game.AddPlayer(i_player_number);
  Invalidate();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CChildView::OnPaint()
{
  CPaintDC dc(this);
  CRect client_rect;
  GetClientRect(&client_rect);

  m_board.SetClientRect(client_rect);

  m_board.DrawBoard(&dc);
}
