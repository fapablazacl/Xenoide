
#include "FolderView.h"


int CALLBACK FolderView_CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) {
    auto &cmp = *reinterpret_cast<std::function<int (int, int)>*>(lParamSort);

    return cmp(static_cast<int>(lParam1), static_cast<int>(lParam2));
}


FolderExplorerWTL::FolderExplorerWTL(CXenoFolderView& folderView): folderView(folderView) {}


void FolderExplorerWTL::clear() {
    folderView.mTreeView.DeleteAllItems();
}


TreeItemHandle FolderExplorerWTL::insert(const std::string &title, const std::optional<TreeItemHandle> parentItemId, const bool hasChildren) {
    const auto itemId = TreeItemHandle{ ++folderView.mItemCount };

    HTREEITEM hItem = NULL;

    if (! parentItemId) {
        hItem = folderView.InsertTreeItem(folderView.mTreeView, title.c_str(), itemId.value);
    } else {
        const HTREEITEM hParentItem = folderView.mTreeItemBimap.right.find(parentItemId->value)->second;

        hItem = folderView.InsertTreeItem(folderView.mTreeView, title.c_str(), itemId.value, hParentItem, hasChildren);
    }

    folderView.mTreeItemBimap.insert({hItem, itemId.value});

    return itemId;
}


void FolderExplorerWTL::sort(const TreeItemHandle itemId) {
    const HTREEITEM hItem = folderView.mTreeItemBimap.right.find(itemId.value)->second;
    folderView.mTreeView.SortChildren(hItem);
}


void FolderExplorerWTL::sort(const TreeItemHandle itemId, std::function<int (const TreeItemHandle, const TreeItemHandle)> cmp) {
    TVSORTCB sort = {};

    sort.hParent = folderView.mTreeItemBimap.right.find(itemId.value)->second;
    sort.lpfnCompare = FolderView_CompareFunc;
    sort.lParam = reinterpret_cast<LPARAM>(&cmp);

    folderView.mTreeView.SortChildrenCB(&sort, FALSE);
}


CXenoFolderView::CXenoFolderView(): folderExplorer(&fileSystemService), presenter(nullptr, &folderExplorer) {
    mView = std::make_unique<FolderExplorerWTL>(*this);
    m_bMsgHandled = false;
}


LRESULT CXenoFolderView::OnCreate(LPCREATESTRUCT cs) {
    SetMsgHandled(true);

    const DWORD dwStyle = TVS_FULLROWSELECT | TVS_SHOWSELALWAYS | TVS_HASBUTTONS | TVS_LINESATROOT | WS_VISIBLE | WS_CHILD;
    
    mTreeView.Create(m_hWnd, rcDefault, "", dwStyle, 0L, ID_FOLDERVIEW_TREEVIEW);
    mTreeView.SetExtendedStyle(TVS_EX_DOUBLEBUFFER, TVS_EX_DOUBLEBUFFER);

    presenter.attachView(mView.get());

    return 0;
}


void CXenoFolderView::OnDestroy() {
    SetMsgHandled(false);
}


void CXenoFolderView::OnSize(UINT nType, CSize size) {
    const CRect rect = { 0, 0, size.cx, size.cy };

    mTreeView.SetWindowPos(NULL, rect, 0);
}


LRESULT CXenoFolderView::OnNotify(int idCtrl, LPNMHDR pnmh) {
    SetMsgHandled(true);

    switch (pnmh->code) {
    case NM_DBLCLK: {
        const HTREEITEM selectedItem = mTreeView.GetSelectedItem();

        if (selectedItem) {
            const TreeItemHandle itemId{ mTreeItemBimap.left.find(selectedItem)->second };
            presenter.onItemActivated(itemId);
        }

        break;
    }

    case TVN_ITEMEXPANDING: {
        const auto &pnmtv = *reinterpret_cast<LPNMTREEVIEW>(pnmh);

        if (pnmtv.action == TVE_EXPAND) {
            const TreeItemHandle itemId{ static_cast<int>(pnmtv.itemNew.lParam) };
            presenter.onItemExpanded(itemId);
        }

        break;
    }

    default:
        SetMsgHandled(false);
    }

    return 0;
}


LRESULT CXenoFolderView::OnEraseBkgnd(HDC hDC) {
    return TRUE;
}


void CXenoFolderView::DisplayFolder(const boost::filesystem::path &folderPath) {
    presenter.displayFolder(Xenoide::Folder{ folderPath.string() });
}


HTREEITEM CXenoFolderView::InsertTreeItem(CTreeViewCtrl &treeView, const char *text, const int itemId) {
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


HTREEITEM CXenoFolderView::InsertTreeItem(CTreeViewCtrl &treeView, const char *text, const int itemId, const HTREEITEM parentItem, const bool hasChildren) {
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
