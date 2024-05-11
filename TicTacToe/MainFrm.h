#pragma once
#include "ChildView.h"

class CMainFrame : public CFrameWnd
{
public:
  CMainFrame() noexcept;
  virtual ~CMainFrame();

  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

protected:
  DECLARE_DYNAMIC(CMainFrame)
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnSetFocus(CWnd* pOldWnd);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnTwoPlayers();
  afx_msg void OnThreePlayers();
  afx_msg void OnFourPlayers();
  afx_msg void OnGameSizeThree();
  afx_msg void OnGameSizeFour();
  afx_msg void OnGameSizeFive();
  DECLARE_MESSAGE_MAP()

protected:
  CToolBar m_wndToolBar;
  CStatusBar m_wndStatusBar;
  CChildView m_wndView;

private:
  size_t m_number_of_player;
  size_t m_board_size;
};
