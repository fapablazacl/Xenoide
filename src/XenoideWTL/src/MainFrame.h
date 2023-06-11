

#pragma once

#include <map>

#include <resource.h>
#include <xeno/wtl/wtl.h>

#include "CodeEditor.h"
#include "IdeCommands.h"


class CMainFrame : 
	public CFrameWindowImpl<CMainFrame>, 
	public CUpdateUI<CMainFrame>,
	public CMessageFilter, 
	public CIdleHandler {

public:
    CMainFrame();

	DECLARE_FRAME_WND_CLASS(_T("Xenoide_MainFrame"), IDR_MAINFRAME)

public:
    BEGIN_UPDATE_UI_MAP(CMainFrame)

    END_UPDATE_UI_MAP()

    BEGIN_MSG_MAP(CMainFrame)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
        CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
    END_MSG_MAP()

public:
    virtual BOOL PreTranslateMessage(MSG* pMsg) override;

    virtual BOOL OnIdle() override;

    LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

private:
    HMENU CreateMenuBar() const;

    void CreateCommandBar(CCommandBarCtrl &, const HMENU hMenuBar) const;

    void CreateToolBar(CToolBarCtrl &, const CImageList &imageList, const std::vector<IdeCommands> &commands) const;

    void CreateToolBarImageList(CImageList &, const std::vector<IdeCommands> &commands) const;

    CBitmapHandle LoadImageBitmap(const LPWSTR filePath) const;

    CBitmap& GetOrLoadBitmap(const std::wstring& fullFilePath);

private:
    CImageList mToolBarImageList;
    CToolBarCtrl mToolBar;
    CCommandBarCtrl mCommandBar;

    CodeEditor mCodeView;

    std::map<IdeCommands, std::wstring> mCommandBitmapPathMap;
    std::map<std::wstring, CBitmap> mFullFilePathBitmapMap;
};
