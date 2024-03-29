/*

#include "CIdeFrame.h"

#include <boost/algorithm/string/join.hpp>
// #include <xenoide/core/FileService.h"
// #include "FolderService.h"
#include "CFolderBrowser.h"

#include <atldlgs.h>
#include <resource.h>


namespace Xenoide {
    CIdeFrame::CIdeFrame(IDEFrame::Presenter *presenter) : IDEFrame(presenter) {
        folderBrowserModel = FolderBrowser::Model::create(folderService.get());
        documentManagerModel = DocumentManager::Model::create();

        folderBrowserPresenter = std::make_unique<FolderBrowser::Presenter>(folderBrowserModel.get());

        folderBrowser = std::make_unique<CFolderBrowser>(folderBrowserPresenter.get());
        dialogManager = std::make_unique<CDialogManager>();
    }


    CIdeFrame::~CIdeFrame() {

    }


    DocumentManager* CIdeFrame::getDocumentManager() {
        return nullptr;
    }


    DialogManager* CIdeFrame::getDialogManager() {
        return dialogManager.get();
    }
    

    FolderBrowser* CIdeFrame::getFolderBrowser() {
        return folderBrowser.get();
    }


    void CIdeFrame::close() {
        // TODO: Add implementation
    }


    void CIdeFrame::show() {
        // TODO: Add implementation
    }


    void CIdeFrame::showPanel(const Panel panel) {
        // TODO: Add implementation
    }
}


namespace Xenoide {
    const int commandIdBase = 1000;

    void CIdeFrame::fillMenuItem(CMenuHandle parent, const MenuData &menu) {

    }


    std::string CIdeFrame::mapMenuTitle(const MenuData &menu) const {
        if (menu.shortcut.isNone()) {
            return menu.text;
        }

        // base text
        std::string title = menu.text;
        
        // modifiers
        std::vector<std::string> modifiers;
        if (menu.shortcut.modifier & Modifier::Ctrl) {
            modifiers.push_back("Ctrl");
        }

        if (menu.shortcut.modifier & Modifier::Shift) {
            modifiers.push_back("Shift");
        }

        if (menu.shortcut.modifier & Modifier::Alt) {
            modifiers.push_back("Alt");
        }

        // key
        std::string key;
        switch (menu.shortcut.key) {
            case Key::A: key = "A"; break;
            case Key::B: key = "B"; break;
            case Key::C: key = "C"; break;
            case Key::D: key = "D"; break;
            case Key::E: key = "E"; break;
            case Key::F: key = "F"; break;
            case Key::G: key = "G"; break;
            case Key::H: key = "H"; break;
            case Key::I: key = "I"; break;
            case Key::J: key = "J"; break;
            case Key::K: key = "K"; break;
            case Key::L: key = "L"; break;
            case Key::M: key = "M"; break;
            case Key::N: key = "N"; break;
            case Key::O: key = "O"; break;
            case Key::P: key = "P"; break;
            case Key::Q: key = "Q"; break;
            case Key::R: key = "R"; break;
            case Key::S: key = "S"; break;
            case Key::T: key = "T"; break;
            case Key::U: key = "U"; break;
            case Key::V: key = "V"; break;
            case Key::W: key = "W"; break;
            case Key::X: key = "X"; break;
            case Key::Y: key = "Y"; break;
            case Key::Z: key = "Z"; break;
            case Key::F1: key = "F1"; break;
            case Key::F2: key = "F2"; break;
            case Key::F3: key = "F3"; break;
            case Key::F4: key = "F4"; break;
            case Key::F5: key = "F5"; break;
            case Key::F6: key = "F6"; break;
            case Key::F7: key = "F7"; break;
            case Key::F8: key = "F8"; break;
            case Key::F9: key = "F9"; break;
            case Key::F10: key = "F10"; break;
            case Key::F11: key = "F11"; break;
            case Key::F12: key = "F12"; break;
        }

        if (modifiers.size () == 0) {
            return title + "\t" + key;
        } else {
            return title + "\t" + boost::join(modifiers, "+") + "+" + key;
        }
    }


    void CIdeFrame::setupMenuBar(const MenuData &menu) {
        // create the menu 
        menuBar.CreateMenu();

        for (const MenuData &menuItem : menu.childs) {
            this->fillMenuItem(menuBar.m_hMenu, menuItem);
        }

        this->SetMenu(menuBar);
    }
}


namespace Xenoide {
    /*
    int CIdeFrame::OnFileOpen(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL &bHandled) {
        CFileDialog dialog(TRUE, _T("All Files\0*.*"));

        if (dialog.DoModal() == IDOK) {
            auto fileService = FileService::create();

            CString filename = dialog.m_szFileName;

            auto content2 = fileService->load(dialog.m_szFileName).c_str();

            CString content = fileService->load((LPCSTR)filename).c_str();

            m_editor.SetText(content);
        }
        
        return 0;
    }


    int CIdeFrame::OnCommand(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL &bHandled) {
        const IdeCommands commandId = static_cast<IdeCommands>(wID);

        switch (commandId)
        {
        case ID_XENOIDE_FILE_NEW:
            break;

        case ID_XENOIDE_FILE_OPEN:
            break;

        case ID_XENOIDE_FILE_SAVE:
            break;

        case ID_XENOIDE_FILE_SAVEAS:
            break;

        case ID_XENOIDE_FILE_EXIT:
            break;

        case ID_XENOIDE_EDIT_UNDO:
            break;

        case ID_XENOIDE_EDIT_REDO:
            break;

        case ID_XENOIDE_EDIT_CUT:
            break;

        case ID_XENOIDE_EDIT_COPY:
            break;

        case ID_XENOIDE_EDIT_PASTE:
            break;

        case ID_WINDOW_SHOW_VIEWS:
            break;

        case ID_WINDOW_CLOSE:
            break;

        default:
            break;
        }
        return 0;
    }


    int CIdeFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) {
        CRect clientRect;
        this->GetClientRect(&clientRect);

        // TODO: Initialize tab control here 
        m_hWndClient = m_view.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);
		m_view.SetTitleBarWindow(m_hWnd);


        /*
        splitterWindow.Create(m_hWnd, &clientRect, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

        this->SetupFolderBrowser();
        this->SetupDocumentManager();

        
        splitterWindow.SetSplitterPane(SPLIT_PANE_LEFT, folderBrowser->m_hWnd);
        splitterWindow.SetSplitterPane(SPLIT_PANE_RIGHT, documentManager->m_hWnd);
        
        // set the vertical splitter parameters
        splitterWindow.m_cxyMin = 35; // minimum size
        splitterWindow.SetSplitterPos(85); // from left

        // TODO: Notify initialization
        mPresenter->onInitialized(this, dialogManager.get(), this);

        return 0;
    }


    void CIdeFrame::SetupDocumentManager() {
        // set the main view
        RECT rcClient;
        splitterWindow.GetClientRect(&rcClient);

        // documentManager->Create(splitterWindow, rcClient, "", WS_CHILD | WS_VISIBLE);
    }

    void CIdeFrame::SetupFolderBrowser() {
        RECT clientRect;
        splitterWindow.GetClientRect(&clientRect);
        
        folderBrowser->Create(splitterWindow, clientRect, "", WS_VISIBLE | WS_CHILD);
    }


    void CIdeFrame::SetupMenuBar() {
        /*
        IDR_MAINFRAME MENU
        BEGIN
            POPUP "&File"
            BEGIN
                MENUITEM "New",                         ID_FILE_NEW
                MENUITEM SEPARATOR
                POPUP "Open"
                BEGIN
                    MENUITEM "&File ...",                   ID_OPEN_FILE
                    MENUITEM "F&older ...",                 ID_OPEN_FOLDER
                END
                MENUITEM SEPARATOR
                MENUITEM "Save",                        ID_FILE_SAVE
                MENUITEM "Save As",                     ID_FILE_SAVEAS
                MENUITEM SEPARATOR
                MENUITEM "Exit",                        ID_FILE_EXIT
            END
            POPUP "&Edit"
            BEGIN
                MENUITEM "&Undo",                       ID_EDIT_UNDO
                MENUITEM "&Redo",                       ID_EDIT_REDO
                MENUITEM SEPARATOR
                MENUITEM "&Cut",                        ID_EDIT_CUT
                MENUITEM "C&opy",                       ID_EDIT_COPY
                MENUITEM "&Paste",                      ID_EDIT_PASTE
            END
            MENUITEM "&Help",                       0
        END
    }


    void CIdeFrame::OnClose() {
        DestroyWindow();
    }

    void CIdeFrame::OnDestroy() {
        PostQuitMessage(0);
    }

    void CIdeFrame::OnSize(UINT nType, CSize size) {
        // m_editor.ResizeClient(size.cx, size.cy, TRUE);
        // splitterWindow.ResizeClient(size.cx, size.cy, TRUE);
    }
}

*/
