#include "pch.h"

#include "MainFrm.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
  ON_WM_CREATE()
  ON_WM_SETFOCUS()
  ON_WM_SIZE()
  ON_COMMAND(ID_2_PLAYERS, &CMainFrame::OnTwoPlayers)
  ON_COMMAND(ID_3_PLAYERS, &CMainFrame::OnThreePlayers)
  ON_COMMAND(ID_4_PLAYERS, &CMainFrame::OnFourPlayers)
  ON_COMMAND(ID_GAME_SIZE_3, &CMainFrame::OnGameSizeThree)
  ON_COMMAND(ID_GAME_SIZE_4, &CMainFrame::OnGameSizeFour)
  ON_COMMAND(ID_GAME_SIZE_5, &CMainFrame::OnGameSizeFive)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static UINT indicators[] =
{
  ID_SEPARATOR,           // status line indicator
  ID_INDICATOR_CAPS,
  ID_INDICATOR_NUM,
  ID_INDICATOR_SCRL,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CMainFrame::CMainFrame() noexcept
  : m_number_of_player(2)
  , m_board_size(3)
  , m_wndView(CChildView(2, 3))
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CMainFrame::~CMainFrame()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
    return -1;

  // create a view to occupy the client area of the frame
  if (!m_wndView.Create(nullptr, nullptr, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, nullptr))
  {
    TRACE0("Failed to create view window\n");
    return -1;
  }

  if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) /* ||
    !m_wndToolBar.LoadToolBar(IDR_MAINFRAME)*/)
  {
    TRACE0("Failed to create toolbar\n");
    return -1;      // fail to create
  }

  if (!m_wndStatusBar.Create(this))
  {
    TRACE0("Failed to create status bar\n");
    return -1;      // fail to create
  }
  m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));

  // TODO: Delete these three lines if you don't want the toolbar to be dockable
  m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
  EnableDocking(CBRS_ALIGN_ANY);
  DockControlBar(&m_wndToolBar);

  return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
  if (!CFrameWnd::PreCreateWindow(cs))
    return FALSE;
  // TODO: Modify the Window class or styles here by modifying
  //  the CREATESTRUCT cs

  cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
  cs.lpszClass = AfxRegisterWndClass(0);

  cs.cy = 600; // width
  cs.cx = 500; // height
  cs.y = 0; // top position
  cs.x = 0; // left position
  return CWnd::PreCreateWindow(cs);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
  CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
  CFrameWnd::Dump(dc);
}
#endif //_DEBUG

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
  m_wndView.SetFocus(); // forward focus to the view window
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
  // let the view have first crack at the command
  if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
    return TRUE;

  // otherwise, do default handling
  return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
  CFrameWnd::OnSize(nType, cx, cy);

  // Ignore resize attempts (optional: set desired size here)
  if (nType == SIZE_MAX) {
    SetWindowPos(NULL, 0, 0, 600, 500, SWP_NOMOVE | SWP_NOREPOSITION);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnTwoPlayers()
{
  m_number_of_player = 2;
  m_wndView.ResetGame(m_number_of_player, m_board_size);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnThreePlayers()
{
  m_number_of_player = 3;
  m_wndView.ResetGame(m_number_of_player, m_board_size);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnFourPlayers()
{
  m_number_of_player = 4;
  m_wndView.ResetGame(m_number_of_player, m_board_size);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnGameSizeThree()
{
  m_board_size = 3;
  m_wndView.ResetGame(m_number_of_player, m_board_size);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnGameSizeFour()
{
  m_board_size = 4;
  m_wndView.ResetGame(m_number_of_player, m_board_size);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnGameSizeFive()
{
  m_board_size = 5;
  m_wndView.ResetGame(m_number_of_player, m_board_size);
}
