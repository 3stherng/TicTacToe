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
  DECLARE_MESSAGE_MAP()

protected:
  CToolBar m_wndToolBar;
  CStatusBar m_wndStatusBar;
  CChildView m_wndView;
};
