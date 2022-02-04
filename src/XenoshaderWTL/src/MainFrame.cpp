
#include "MainFrame.h"
#include "AboutDlg.h"

#include <Xenoide/Core/FileService.h>


MainFrame::MainFrame() {}


void MainFrame::openFile(const boost::filesystem::path &filePath) {
    doOpenFile(filePath);
}


LRESULT MainFrame::OnCreate(LPCREATESTRUCT cs) {
    CreateSimpleToolBar();
    CreateSimpleStatusBar();

    const DWORD dwClientStyle = WS_CHILD | /*WS_VISIBLE | */ WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
    const DWORD dwClientExStyle = WS_EX_CLIENTEDGE;

    mSplitterWindow.Create(*this, rcDefault);

    // mFolderView.Create(mSplitterWindow, rcDefault, NULL);
    mFolderManager.Create(mSplitterWindow, rcDefault, NULL);
    mCodeView.Create(mSplitterWindow, rcDefault, NULL, dwClientStyle, dwClientExStyle);
    mDocumentManager.Create(mSplitterWindow, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);

    // mSplitterWindow.SetSplitterPanes(mFolderView, mDocumentManager);
    mSplitterWindow.SetSplitterPanes(mFolderManager, mDocumentManager);
    
    m_hWndClient = mSplitterWindow;
    UpdateLayout();
    mSplitterWindow.SetSplitterPos(200);

    return 0;
}


void MainFrame::OnDestroy() {
    SetMsgHandled(false);
    PostQuitMessage(0);
}


void MainFrame::OnFileMenu(UINT uCode, int nID, HWND hwndCtrl) {
    if (nID == ID_FILE_NEW) {
        const DWORD dwClientStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
        const DWORD dwClientExStyle = WS_EX_CLIENTEDGE;

        auto codeView = new CodeView();
        codeView->Create(mDocumentManager, rcDefault, NULL, dwClientStyle, dwClientExStyle);

        mDocumentManager.AddPage(codeView->m_hWnd, _T("New 01"));
    }

    if (nID == ID_FILE_OPEN) {
        auto dialog = CFileDialog{TRUE, _T("All Files\0*.*")};

        if (dialog.DoModal() != IDOK) {
            return;
        }

        doOpenFile(boost::filesystem::path{dialog.m_szFileName});
    }
        
    if (nID == ID_FILE_OPENFOLDER) {
        auto folderDialog = CFolderDialog(m_hWnd, _T("Open Folder"));

        if (folderDialog.DoModal() != IDOK) {
            return;
        }

        const boost::filesystem::path folderPath = folderDialog.GetFolderPath();

        mFolderView.DisplayFolder(folderPath);
    }

    if (nID == ID_FILE_SAVE) {
        if (mFilePath) {
            Xenoide::FileService fileService;
            const std::string content = mCodeView.GetContent();

            fileService.save(mFilePath.value().string(), content);

            mCodeView.ClearSaveState();
        } else {
            OnFileMenu(uCode, ID_FILE_SAVE_AS, hwndCtrl);
        }
    }

    if (nID == ID_FILE_SAVE_AS) {
        int x = 0;
    }

    if (nID == ID_FILE_EXIT) {
        DestroyWindow();
    }
}


void MainFrame::OnEditMenu(UINT uCode, int nID, HWND hwndCtrl) {
    int x = 0;
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
    const auto codeLang = getCodeLanguage(*mFilePath);

    switch (codeLang) {
        case CodeLanguage::CPP: 
            mCodeView.SetLanguage(Lexilla::MakeLexer("cpp"), languageConfigC); 
            break;

        case CodeLanguage::GLSL: 
            mCodeView.SetLanguage(Lexilla::MakeLexer("cpp"), languageConfigGLSL); 
            break;

        default: 
            mCodeView.ClearLanguage();
    }
}
