#include "pch.h"
#include "ChildView.h"
#include "framework.h"
#include "TicTacToe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CChildView::CChildView()
  : m_board(3)
{
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
    
  m_board.UpdateBoard(&dc, i_point, 'X');
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CChildView::OnPaint()
{
  CPaintDC dc(this); // device context for painting
  CRect client_rect;
  GetClientRect(&client_rect);
  // TODO: Add your message handler code here

  // Do not call CWnd::OnPaint() for painting messages
  
  m_board.SetClientRect(client_rect);
  m_board.DrawNewBoard(&dc);
}
