
#include "MainFrame.h"
#include "AboutDlg.h"

#include <shobjidl.h> 

#include <xenoide/core/FileService.h>
#include <xeno/ui/Menu.h>
#include <xeno/ui/TreeManagerControllerFileSystem.h>


MainFrame::MainFrame() {
    //folderImageList = Xenoide::CreateImageList(48, 48, ILC_COLOR32, {
    //    LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FILE)),
    //    LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FOLDER_2)),
    //    LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FOLDER_OPEN_5))
    //});
    //
    //mFolderManager = std::make_unique<Xenoide::CTreeManager>(folderImageList);
}


void MainFrame::openFile(const boost::filesystem::path &filePath) {
    doOpenFile(filePath);
}


void MainFrame::InitializeCommandBar() {
    /*
    using namespace Xenoide;

    const std::vector<Xenoide::MenuData> menus {
        MenuData::menu("&File", {
            MenuData::action(IDX_FILE_NEW, "&New", Shortcut{Modifier::Ctrl, Key::N})
        })
    };

    menuBar.CreateMenu();
    CMenuFactory2{}.fillMenu(menuBar, menus);
    
    SetMenu(menuBar);
    */

    RECT rcCmdBar = { 0, 0, 100, 100 };
    m_wndCmdBar.Create(m_hWnd, rcCmdBar, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
    m_wndCmdBar.AttachMenu(GetMenu());
    m_wndCmdBar.LoadImages(IDR_MAINFRAME);
    SetMenu(NULL);
}


LRESULT MainFrame::OnCreate(LPCREATESTRUCT cs) {
    // create command bar window
    InitializeCommandBar();

    CreateSimpleToolBar();
    CreateSimpleStatusBar();

    //mSplitterWindow.Create(*this, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
    //mFolderManager->Create(mSplitterWindow, rcDefault, NULL);
    // mCodeView.Create(mSplitterWindow, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);
    mCodeView.Create(*this, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);
    
    CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
    AddSimpleReBarBand(m_wndCmdBar);

    // mSplitterWindow.SetSplitterPanes(*mFolderManager, mCodeView);
    
    // m_hWndClient = mSplitterWindow;
    m_hWndClient = mCodeView;
    UpdateLayout();
    // mSplitterWindow.SetSplitterPos(200);

    CMessageLoop* pLoop = _Module.GetMessageLoop();
    pLoop->AddMessageFilter(this);
    pLoop->AddIdleHandler(this);

    return 0;
}


BOOL MainFrame::PreTranslateMessage(MSG* pMsg)  {
    return CFrameWindowImpl<MainFrame>::PreTranslateMessage(pMsg);
}


BOOL MainFrame::OnIdle()  {
    UIUpdateToolBar();

    return FALSE;
}


void MainFrame::OnDestroy() {
    SetMsgHandled(false);
    PostQuitMessage(0);
}

void MainFrame::OnNewFile(UINT uCode, int nID, HWND hwndCtrl) {
    
}


void MainFrame::OnOpenFile(UINT uCode, int nID, HWND hwndCtrl) {
    IFileOpenDialog *pFileOpen = nullptr;

    // Create the FileOpenDialog object.
    auto hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, 
        IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

    if (FAILED(hr)) {
        return;
    }

    // Show the Open dialog box.
    const COMDLG_FILTERSPEC rgSpec[] = {
        { L"GLSL Shader", L"*.glsl;*.vert;*.frag" },
        { L"C/C++ Code", L"*.c;*.cpp;*.cxx;*.c++;*.h;*.hpp;*.hxx;*.h++" },
        { L"All Files", L"*.*" },
    };

    pFileOpen->SetFileTypes(3, rgSpec);
    pFileOpen->SetTitle(L"Open File");

    hr = pFileOpen->Show(*this);
    if (FAILED(hr)) {
        return;
    }

    // Get the file name from the dialog box.
    IShellItem *pItem = nullptr;
    hr = pFileOpen->GetResult(&pItem);

    if (FAILED(hr)) {
        return;
    }

    PWSTR pszFilePath;
    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

    // Display the file name to the user.
    if (SUCCEEDED(hr)) {
        doOpenFile(boost::filesystem::path{pszFilePath});
        CoTaskMemFree(pszFilePath);
    }
    pItem->Release();
    
    pFileOpen->Release();
}


void MainFrame::OnOpenFolder(UINT uCode, int nID, HWND hwndCtrl) {
    //auto folderDialog = CFolderDialog(m_hWnd, _T("Open Folder"));

    //if (folderDialog.DoModal() != IDOK) {
    //    return;
    //}

    //const boost::filesystem::path folderPath = folderDialog.GetFolderPath();

    //folderManagerController = std::make_unique<Xenoide::TreeManagerControllerFileSystem>(folderPath);
    //mFolderManager->SetController(folderManagerController.get());
    //mFolderManager->ReloadContent();
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


void MainFrame::OnSaveAsFile(UINT uCode, int nID, HWND hwndCtrl) {

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
