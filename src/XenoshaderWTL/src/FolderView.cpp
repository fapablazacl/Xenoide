
#include "FolderView.h"


int CALLBACK FolderView_CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) {
    auto &cmp = *reinterpret_cast<std::function<int (int, int)>*>(lParamSort);

    return cmp(static_cast<int>(lParam1), static_cast<int>(lParam2));
}


FolderView::FolderView (AppController *controller) : mPresenter(controller) {
    m_bMsgHandled = false;
}


void FolderView::clear() {
    mTreeView.DeleteAllItems();
}


int FolderView::insert(const std::string &title, const std::optional<int> parentItemId, const bool hasChildren) {
    const int itemId = ++mItemCount;

    HTREEITEM hItem = NULL;

    if (! parentItemId) {
        hItem = InsertTreeItem(mTreeView, title.c_str(), itemId);
    } else {
        const HTREEITEM hParentItem = mTreeItemBimap.right.find(*parentItemId)->second;

        hItem = InsertTreeItem(mTreeView, title.c_str(), itemId, hParentItem, hasChildren);
    }

    mTreeItemBimap.insert({hItem, itemId});

    return itemId;
}


void FolderView::sort(const int itemId) {
    const HTREEITEM hItem = mTreeItemBimap.right.find(itemId)->second;
    mTreeView.SortChildren(hItem);
}


void FolderView::sort(const int itemId, std::function<int (int, int)> cmp) {
    TVSORTCB sort = {};

    sort.hParent = mTreeItemBimap.right.find(itemId)->second;
    sort.lpfnCompare = FolderView_CompareFunc;
    sort.lParam = reinterpret_cast<LPARAM>(&cmp);

    mTreeView.SortChildrenCB(&sort, FALSE);
}


LRESULT FolderView::OnCreate(LPCREATESTRUCT cs) {
    SetMsgHandled(true);

    const DWORD dwStyle = TVS_FULLROWSELECT | TVS_SHOWSELALWAYS | TVS_HASBUTTONS | TVS_LINESATROOT | WS_VISIBLE | WS_CHILD;
    
    mTreeView.Create(m_hWnd, rcDefault, "", dwStyle, 0L, ID_FOLDERVIEW_TREEVIEW);
    mTreeView.SetExtendedStyle(TVS_EX_DOUBLEBUFFER, TVS_EX_DOUBLEBUFFER);

    mPresenter.attachView(this);

    return 0;
}


void FolderView::OnDestroy() {
    SetMsgHandled(false);
}


void FolderView::OnSize(UINT nType, CSize size) {
    const CRect rect = { 0, 0, size.cx, size.cy };

    mTreeView.SetWindowPos(NULL, rect, 0);
}


LRESULT FolderView::OnNotify(int idCtrl, LPNMHDR pnmh) {
    SetMsgHandled(true);

    switch (pnmh->code) {
    case NM_DBLCLK: {
        const HTREEITEM selectedItem = mTreeView.GetSelectedItem();

        if (selectedItem) {
            const int itemId = mTreeItemBimap.left.find(selectedItem)->second;
            mPresenter.onItemActivated(itemId);
        }

        break;
    }

    case TVN_ITEMEXPANDING: {
        const auto &pnmtv = *reinterpret_cast<LPNMTREEVIEW>(pnmh);

        if (pnmtv.action == TVE_EXPAND) {
            mPresenter.onItemExpanded(static_cast<int>(pnmtv.itemNew.lParam));
        }

        break;
    }

    default:
        SetMsgHandled(false);
    }

    return 0;
}


LRESULT FolderView::OnEraseBkgnd(HDC hDC) {
    return TRUE;
}


void FolderView::DisplayFolder(const boost::filesystem::path &folderPath) {
    mPresenter.displayFolder(folderPath);
}


HTREEITEM FolderView::InsertTreeItem(CTreeViewCtrl &treeView, const char *text, const int itemId) {
    const DWORD style = TVIF_PARAM | TVIF_TEXT | TVIF_CHILDREN /*| TVIF_IMAGE | TVIF_SELECTEDIMAGE*/;

    TVINSERTSTRUCT insertStruct = {};

    insertStruct.item.lParam = static_cast<LPARAM>(itemId);
    insertStruct.item.mask = style;
    insertStruct.item.pszText = const_cast<char*>(text);
    insertStruct.item.cChildren = 1;
    // insertStruct.item.iImage = 1;
    // insertStruct.item.iSelectedImage = 1;
    return treeView.InsertItem(&insertStruct);
}


HTREEITEM FolderView::InsertTreeItem(CTreeViewCtrl &treeView, const char *text, const int itemId, const HTREEITEM parentItem, const bool hasChildren) {
    const DWORD style = TVIF_PARAM | TVIF_TEXT | (hasChildren ? TVIF_CHILDREN : 0) /*| TVIF_IMAGE | TVIF_SELECTEDIMAGE*/;

    TVINSERTSTRUCT insertStruct = {};

    insertStruct.hParent = parentItem;
    insertStruct.item.lParam = static_cast<LPARAM>(itemId);
    insertStruct.item.mask = style;
    insertStruct.item.pszText = const_cast<char*>(text);
    insertStruct.item.cChildren = hasChildren ? 1 : 0;
    // insertStruct.item.iImage = 1;
    // insertStruct.item.iSelectedImage = 1;
    return treeView.InsertItem(&insertStruct);
}
