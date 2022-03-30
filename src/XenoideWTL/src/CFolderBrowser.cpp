
#include "CFolderBrowser.h"

namespace Xenoide {
    CFolderBrowser::CFolderBrowser(FolderBrowser::Presenter *presenter) : FolderBrowser(presenter) {

    }


    CFolderBrowser::~CFolderBrowser() {

    }

    void CFolderBrowser::displayFolder(const std::string &folder) {
    }

    boost::optional<std::string> CFolderBrowser::getSelectedPath() const {
        return {};
    }

    void CFolderBrowser::displayContextualMenu(const Point &point, const MenuData &menu) {

    }
}


namespace Xenoide {
    int CFolderBrowser::OnCreate(LPCREATESTRUCT lpCreateStruct) {
        RECT clientRect;
        this->GetClientRect(&clientRect);

        treeView.Create(m_hWnd, &clientRect, "",  TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT | WS_VISIBLE | WS_CHILD);

        this->ChangeFolderPath("", "Test");

        return 0;
    }


    LRESULT CFolderBrowser::OnNotify(int idCtrl, LPNMHDR pnmh) {
        const auto notification = LPNMTREEVIEW(pnmh);
        const UINT code = notification->hdr.code;

        switch (code) {
            case TVN_ITEMEXPANDED: {

                return 0;
            }
        }

        return 0;
    }


    void CFolderBrowser::ChangeFolderPath(LPCTSTR folderPath, LPCTSTR title) {
        treeView.DeleteAllItems();
        this->PopulateItems(folderPath, title);
    }


    void CFolderBrowser::PopulateItems(LPCTSTR folderPath, LPCTSTR title) {
        const HTREEITEM rootItem = treeView.InsertItem(title, 0, 0, nullptr, nullptr);
        const DWORD style = TVIF_TEXT /*| TVIF_IMAGE | TVIF_SELECTEDIMAGE*/;

        // TODO: Fix those ugly const_cast's ...

        TVINSERTSTRUCT insertStruct = {};

        insertStruct.hParent = rootItem;
        insertStruct.item.mask = style;
        insertStruct.item.pszText = const_cast<char*>("Class1");
        insertStruct.item.iImage = 1;
        insertStruct.item.iSelectedImage = 1;
        treeView.InsertItem(&insertStruct);

        insertStruct.hParent = rootItem;
        insertStruct.item.mask = style;
        insertStruct.item.pszText = const_cast<char*>("Class2");
        insertStruct.item.iImage = 1;
        insertStruct.item.iSelectedImage = 1;
        treeView.InsertItem(&insertStruct);

        insertStruct.hParent = rootItem;
        insertStruct.item.mask = style;
        insertStruct.item.pszText = const_cast<char*>("Function1");
        insertStruct.item.iImage = 2;
        insertStruct.item.iSelectedImage = 2;
        treeView.InsertItem(&insertStruct);
    }
}
