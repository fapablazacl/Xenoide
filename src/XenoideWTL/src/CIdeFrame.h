
#pragma once 

#include <map>

#include <xeno/wtl/wtl.h>

#include <xenoide/core/FolderService.h>
#include <xeno/ui/MenuPanel.h>
#include <xeno/ui/IDEFrame.h>

#include "CDocument.h"
#include "CDocumentManager.h"
#include "CFolderBrowser.h"
#include "CDialogManager.h"

#include "CustomTabView.h"
#include "IdeCommands.h"


namespace Xenoide {
    class CIdeFrame : 
        public CFrameWindowImpl<CMainFrame>, 
        public CUpdateUI<CMainFrame>,
		public CMessageFilter, 
        public CIdleHandler

    public:
        CIdeFrame(IDEFrame::Presenter *presenter);

        virtual ~CIdeFrame();

        virtual DocumentManager* getDocumentManager() override;

        virtual DialogManager* getDialogManager() override;
        
        virtual FolderBrowser* getFolderBrowser() override;

        virtual void close() override;

        virtual void show() override;

        virtual void showPanel(const Panel panel) override;

    public:
        void fillMenuItem(CMenuHandle parent, const MenuData &menu);

        std::string mapMenuTitle(const MenuData& menu) const;

        virtual void setupMenuBar(const MenuData &menu) override;

    public:
        DECLARE_WND_CLASS(_T("CIdeFrame"))

        BEGIN_MSG_MAP(CIdeFrame)
            COMMAND_CODE_HANDLER(0, OnCommand)

            MSG_WM_CREATE(OnCreate)
            MSG_WM_CLOSE(OnClose)
            MSG_WM_DESTROY(OnDestroy)
            MSG_WM_SIZE(OnSize)
        END_MSG_MAP()

    public:
        int OnCommand(WORD wNotifyCode  , WORD wID, HWND hWndCtrl, BOOL &bHandled);

        int OnCreate(LPCREATESTRUCT lpCreateStruct);

        void OnClose();

        void OnDestroy();

        void OnSize(UINT nType, CSize size);

    private:
        void SetupDocumentManager();

        void SetupFolderBrowser();

        void SetupMenuBar();

    private:
        CCustomTabView mTabView;

        CSplitterWindow splitterWindow;
        CMenu menuBar;
        
        std::unique_ptr<FolderService> folderService;

        std::unique_ptr<FolderBrowser::Model> folderBrowserModel;
        std::unique_ptr<DocumentManager::Model> documentManagerModel;

        std::unique_ptr<FolderBrowser::Presenter> folderBrowserPresenter;

        std::unique_ptr<CFolderBrowser> folderBrowser;
        std::unique_ptr<CDialogManager> dialogManager;
    };
}
