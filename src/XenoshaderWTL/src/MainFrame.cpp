
#include "MainFrame.h"
#include "AboutDlg.h"

#include <xeno/core/FileService.h>
#include <xeno/ui/TreeManagerControllerFileSystem.h>

MainFrame::MainFrame() {
    folderImageList = Xenoide::CreateImageList(48, 48, ILC_COLOR32, {
        LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FILE)),
        LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FOLDER_2)),
        LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FOLDER_OPEN_5))
    });

    mFolderManager = std::make_unique<Xenoide::CTreeManager>(folderImageList);
}


void MainFrame::openFile(const boost::filesystem::path &filePath) {
    doOpenFile(filePath);
}


LRESULT MainFrame::OnCreate(LPCREATESTRUCT cs) {
    // create command bar window
    {
        RECT rcCmdBar = { 0, 0, 100, 100 };
        m_wndCmdBar.Create(m_hWnd, rcCmdBar, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
        // atach menu
        m_wndCmdBar.AttachMenu(GetMenu());
        // load command bar images
        m_wndCmdBar.LoadImages(IDR_MAINFRAME);
        // remove old menu
        SetMenu(NULL);
    }
    
    // CreateSimpleToolBar();
    CreateSimpleStatusBar();

    mSplitterWindow.Create(*this, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
    mFolderManager->Create(mSplitterWindow, rcDefault, NULL);
    mCodeView.Create(mSplitterWindow, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);
    
    CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
    AddSimpleReBarBand(m_wndCmdBar);

    mSplitterWindow.SetSplitterPanes(*mFolderManager, mCodeView);
    
    m_hWndClient = mSplitterWindow;
    UpdateLayout();
    mSplitterWindow.SetSplitterPos(200);

    return 0;
}


void MainFrame::OnDestroy() {
    SetMsgHandled(false);
    PostQuitMessage(0);
}


void MainFrame::OnOpenFile(UINT uCode, int nID, HWND hwndCtrl) {
    auto dialog = CFileDialog{TRUE, _T("All Files\0*.*")};

    if (dialog.DoModal() != IDOK) {
        return;
    }

    doOpenFile(boost::filesystem::path{dialog.m_szFileName});
}

void MainFrame::OnOpenFolder(UINT uCode, int nID, HWND hwndCtrl) {
    auto folderDialog = CFolderDialog(m_hWnd, _T("Open Folder"));

    if (folderDialog.DoModal() != IDOK) {
        return;
    }

    const boost::filesystem::path folderPath = folderDialog.GetFolderPath();

    folderManagerController = std::make_unique<Xenoide::TreeManagerControllerFileSystem>(folderPath);
    mFolderManager->SetController(folderManagerController.get());
    mFolderManager->ReloadContent();
}


void MainFrame::OnSaveFile(UINT uCode, int nID, HWND hwndCtrl) {
    if (!mFilePath) {
        return;
    }

    Xenoide::FileService fileService;
    const std::string content = mCodeView.GetContent();

    fileService.save(mFilePath.value().string(), content);

    mCodeView.ClearSaveState();
}


void MainFrame::OnExitApp(UINT uCode, int nID, HWND hwndCtrl) {
    DestroyWindow();
}


void MainFrame::OnAboutMenu(UINT uCode, int nID, HWND hwndCtrl) {
    if (nID == ID_HELP_ABOUT) {
        AboutDlg dlg;
        dlg.DoModal();
    }
}


void MainFrame::doOpenFile(const boost::filesystem::path &filePath) {
    mFilePath = filePath;

    // load the file and put the content into the 
    Xenoide::FileService fileService;

    const std::string fileContent = fileService.load(mFilePath->string());
    mCodeView.SetInitialContent(fileContent.c_str());

    // 
    const auto config = CodeEditorConfiguration::detect(filePath);

    if (config) {
        mCodeView.Configure(config.value());
    } else {
        mCodeView.ClearLanguage();
    }
}
