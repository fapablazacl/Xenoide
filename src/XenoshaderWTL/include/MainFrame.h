
#pragma once 

#include <optional>
#include <memory>

#include "framework.h"
#include "resource.h"
#include "AppController.h"
#include "DocumentManager.h"
#include "FolderView.h"

#include <xeno/wtl/CodeEditor.h>
#include <xeno/wtl/TreeManager.h>
#include <xeno/wtl/TreeManagerController.h>


// Frame Windows are defined with CFrameWindowImpl
class MainFrame :   public CFrameWindowImpl<MainFrame>,
                    public CUpdateUI<MainFrame>/*,
                    public CMessageFilter,
                    public CIdleHandler*/, 
                    public AppController {
public:
    DECLARE_FRAME_WND_CLASS("MainFrame", IDR_MAINFRAME)

    BEGIN_UPDATE_UI_MAP(MainFrame)
        // UPDATE_ELEMENT(ID_CLOCK_START, UPDUI_MENUPOPUP)
        // UPDATE_ELEMENT(ID_CLOCK_STOP, UPDUI_MENUPOPUP)
    END_UPDATE_UI_MAP()

    BEGIN_MSG_MAP(MainFrame)
        COMMAND_ID_HANDLER_EX(ID_FILE_NEW, OnFileMenu)
        COMMAND_ID_HANDLER_EX(ID_FILE_OPEN, OnFileMenu)
        COMMAND_ID_HANDLER_EX(ID_FILE_OPENFOLDER, OnFileMenu)
        COMMAND_ID_HANDLER_EX(ID_FILE_SAVE, OnFileMenu)
        COMMAND_ID_HANDLER_EX(ID_FILE_SAVE_AS, OnFileMenu)
        COMMAND_ID_HANDLER_EX(ID_FILE_EXIT, OnFileMenu)

        COMMAND_ID_HANDLER_EX(ID_EDIT_UNDO, OnEditMenu)
        COMMAND_ID_HANDLER_EX(ID_EDIT_REDO, OnEditMenu)
        COMMAND_ID_HANDLER_EX(ID_EDIT_CUT, OnEditMenu)
        COMMAND_ID_HANDLER_EX(ID_EDIT_COPY, OnEditMenu)
        COMMAND_ID_HANDLER_EX(ID_EDIT_PASTE, OnEditMenu)
        
        COMMAND_ID_HANDLER_EX(ID_HELP_ABOUT, OnAboutMenu)
        
        MSG_WM_CREATE(OnCreate)
        MSG_WM_DESTROY(OnDestroy)

        CHAIN_MSG_MAP(CUpdateUI<MainFrame>)
        CHAIN_MSG_MAP(CFrameWindowImpl<MainFrame>)
    END_MSG_MAP()

public:
    MainFrame();

    void openFile(const boost::filesystem::path& filePath) override;

public:
    LRESULT OnCreate(LPCREATESTRUCT cs);

    void OnDestroy();

    void OnFileMenu(UINT uCode, int nID, HWND hwndCtrl);

    void OnEditMenu(UINT uCode, int nID, HWND hwndCtrl);

    void OnAboutMenu(UINT uCode, int nID, HWND hwndCtrl);

private:
    void doOpenFile(const boost::filesystem::path& filePath);

private:
    CodeEditor mCodeView;
    // CXenoFolderView mFolderView;

    std::unique_ptr<Xenoide::CTreeManager> mFolderManager;
    std::unique_ptr<Xenoide::TreeManagerController> folderManagerController;

    DocumentManager mDocumentManager;

    CString mFolderPath;
    CSplitterWindow mSplitterWindow;
    std::optional<boost::filesystem::path> mFilePath;

    CImageList folderImageList;
};
