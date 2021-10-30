
#pragma once 

#include <atlbase.h>
#include <atlapp.h>
#include <atlframe.h>
#include <atlctrls.h>
#include <atlctrlx.h>
#include <atluser.h>
#include <atlcrack.h>
#include <atlsplit.h>

#include <Xenoide/Gui/FolderBrowser.h>

namespace Xenoide {
    class CFolderBrowser : public CWindowImpl<CFolderBrowser, CWindow, CControlWinTraits>, public FolderBrowser {
    public:
        DECLARE_WND_CLASS(_T("Xenoide::CFolderBrowser"))

        BEGIN_MSG_MAP(CFolderBrowser)
            MSG_WM_CREATE(OnCreate)
            MSG_WM_NOTIFY(OnNotify)
        END_MSG_MAP()

    public:
        CFolderBrowser(FolderBrowser::Presenter *presenter);

        ~CFolderBrowser();

        void ChangeFolderPath(LPCTSTR folderPath, LPCTSTR title);

        int OnCreate(LPCREATESTRUCT lpCreateStruct);

        LRESULT OnNotify(int idCtrl, LPNMHDR pnmh);

    public:
        virtual void displayFolder(const std::string &folder) override;

        virtual boost::optional<std::string> getSelectedPath() const override;

        virtual void displayContextualMenu(const Point &point, const Menu &menu) override;

    private:
        void PopulateItems(LPCTSTR folderPath, LPCTSTR title);

    private:
        CTreeViewCtrl treeView;
    };
}
