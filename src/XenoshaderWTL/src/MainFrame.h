
#pragma once 

#include <optional>
#include <memory>
#include <xeno/wtl/wtl.h>
#include <xeno/ui/TreeManagerController.h>

#include "DocumentManager.h"
#include "FolderExplorer.h"
#include "CodeEditor.h"
#include "TreeManager.h"
#include "resource.h"

class MainFrame : public CFrameWindowImpl<MainFrame>, public CUpdateUI<MainFrame> {
public:
    DECLARE_FRAME_WND_CLASS("MainFrame", IDR_MAINFRAME)

    BEGIN_UPDATE_UI_MAP(MainFrame)
        // UPDATE_ELEMENT(ID_CLOCK_START, UPDUI_MENUPOPUP)
        // UPDATE_ELEMENT(ID_CLOCK_STOP, UPDUI_MENUPOPUP)
    END_UPDATE_UI_MAP()

    BEGIN_MSG_MAP(MainFrame)
        COMMAND_ID_HANDLER_EX(ID_FILE_OPEN, OnOpenFile)
        COMMAND_ID_HANDLER_EX(ID_FILE_SAVE, OnSaveFile)
        COMMAND_ID_HANDLER_EX(ID_FILE_EXIT, OnExitApp)

        COMMAND_ID_HANDLER_EX(ID_HELP_ABOUT, OnAboutMenu)
        
        MSG_WM_CREATE(OnCreate)
        MSG_WM_DESTROY(OnDestroy)

        CHAIN_MSG_MAP(CUpdateUI<MainFrame>)
        CHAIN_MSG_MAP(CFrameWindowImpl<MainFrame>)
    END_MSG_MAP()

public:
    MainFrame();

    void openFile(const boost::filesystem::path& filePath);

public:
    LRESULT OnCreate(LPCREATESTRUCT cs);

    void OnDestroy();

    void OnOpenFile(UINT uCode, int nID, HWND hwndCtrl);
    void OnOpenFolder(UINT uCode, int nID, HWND hwndCtrl);
    void OnSaveFile(UINT uCode, int nID, HWND hwndCtrl);
    void OnExitApp(UINT uCode, int nID, HWND hwndCtrl);

    void OnAboutMenu(UINT uCode, int nID, HWND hwndCtrl);

private:
    void doOpenFile(const boost::filesystem::path& filePath);

private:
    CodeEditor mCodeView;

    std::unique_ptr<Xenoide::CTreeManager> mFolderManager;
    std::unique_ptr<Xenoide::TreeManagerController> folderManagerController;

    CString mFolderPath;
    CCommandBarCtrl m_wndCmdBar;
    CSplitterWindow mSplitterWindow;
    std::optional<boost::filesystem::path> mFilePath;

    CImageList folderImageList;
};
