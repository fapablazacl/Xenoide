
#include <xeno/wtl/TreeManager.h>

#include <CommonControls.h>
#include <iostream>
#include <map>

#include <xeno/wtl/TreeManagerControllerFileSystem.h>


int GetIconFromPath(const char *strPath) {
    SHFILEINFO sfi;
    memset(&sfi, 0, sizeof(sfi));

    // SHGFI_SYSICONINDEX will return the icon's index within the shell image list
    SHGetFileInfo(strPath, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(sfi), SHGFI_SYSICONINDEX | SHGFI_USEFILEATTRIBUTES);

    return sfi.iIcon;
}


HTREEITEM InsertTreeItem(
    CTreeViewCtrl &treeView, 
    const char *text, 
    const LPARAM lParam, 
    const HTREEITEM parentItem, 
    const bool hasChildren,
    const int image) {

    const DWORD style = TVIF_PARAM | TVIF_TEXT | (hasChildren ? TVIF_CHILDREN : 0) | TVIF_IMAGE | TVIF_SELECTEDIMAGE;

    TVINSERTSTRUCT insertStruct = {};

    insertStruct.hParent = parentItem;
    insertStruct.item.lParam = lParam;
    insertStruct.item.mask = style;
    insertStruct.item.pszText = const_cast<char*>(text);
    insertStruct.item.cChildren = hasChildren ? 1 : 0;
    insertStruct.item.iImage = image;
    insertStruct.item.iSelectedImage = image;

    return treeView.InsertItem(&insertStruct);
}


namespace Xenoide {
    CTreeManager::CTreeManager() {
        m_bMsgHandled = false;
    }


    CTreeManager::CTreeManager(const HIMAGELIST hImageList) : hImageList{hImageList} {
        m_bMsgHandled = false;
    }


    CTreeManager::~CTreeManager() {}

    LRESULT CTreeManager::OnCreate(LPCREATESTRUCT cs) {
        SetMsgHandled(true);

        const DWORD dwStyle = TVS_FULLROWSELECT | TVS_SHOWSELALWAYS | TVS_HASBUTTONS | TVS_LINESATROOT | WS_VISIBLE | WS_CHILD;
    
        treeView.Create(m_hWnd, rcDefault, "", dwStyle, 0L, ID_FOLDERVIEW_TREEVIEW);
        treeView.SetExtendedStyle(TVS_EX_DOUBLEBUFFER, TVS_EX_DOUBLEBUFFER);
        treeView.SetImageList(hImageList);

        ReloadContent();

        return 0;
    }


    void CTreeManager::OnDestroy() {
        SetMsgHandled(false);
    }


    void CTreeManager::OnSize(UINT nType, CSize size) {
        const CRect rect = { 0, 0, size.cx, size.cy };

        treeView.SetWindowPos(NULL, rect, 0);
    }


    void CTreeManager::OnContextMenu(CWindow wnd, CPoint point) {
        if (wnd == treeView) {
            const HTREEITEM hItem = treeView.GetSelectedItem();
            const TreeItemId itemId{static_cast<int>(treeView.GetItemData(hItem))};

            CRect itemRect;
            if (treeView.GetItemRect(hItem, &itemRect, FALSE) == FALSE) {
                return;
            }

            const HMENU hMenu = CreatePopupMenu();
            const CMenuFactory factory{ 100000 };
            const std::vector<MenuData> menuItems = controller->getItemPopupMenuData(itemId);

            factory.fillMenu(hMenu, commandMap, menuItems);

            TrackPopupMenuEx(hMenu, TPM_TOPALIGN | TPM_LEFTALIGN, point.x, point.y, m_hWnd, nullptr);
        }
    }


    LRESULT CTreeManager::OnNotify(int idCtrl, LPNMHDR pnmh) {
        SetMsgHandled(true);

        if (pnmh->code == NM_DBLCLK) {
            const HTREEITEM selectedItem = treeView.GetSelectedItem();

            if (selectedItem) {
                // TODO: Call the Click
            }
        }
        else if (pnmh->code == TVN_ITEMEXPANDING) {
            const auto &pnmtv = *reinterpret_cast<LPNMTREEVIEW>(pnmh);
            const HTREEITEM hItem = pnmtv.itemNew.hItem;

            if (pnmtv.action == TVE_EXPAND) {
                // TODO: Refactor to a common function
                TreeItemId itemId{static_cast<int>(pnmtv.itemNew.lParam)};

                const auto it = populated.find(itemId);

                if (it == populated.end()) {
                    for (int i = 0; i < controller->getChildCount(itemId); i++) {
                        const TreeItemId childId = controller->getChildId(itemId, i);

                        const std::string name = controller->getItemCaption(childId);
                        const bool hasChildren = controller->getChildCount(childId) > 0;
                        const int image = controller->getItemImage(childId);

                        InsertTreeItem(treeView, name.c_str(), static_cast<LPARAM>(childId.value), hItem, hasChildren, image);
                    }

                    populated.insert(itemId);

                    // perform a Sort
                    Sort(hItem);
                }
            }
        }
        else {
            SetMsgHandled(false);
        }

        return 0;
    }


    LRESULT CTreeManager::OnEraseBkgnd(HDC hDC) {
        return TRUE;
    }

    void CTreeManager::SetController(TreeManagerController* controller) {
        this->controller = controller;
    }


    int CTreeManager::OnCommand(UINT uNotifyCode, int nID, CWindow wndCtl) {

        return 0;
    }


    static int CALLBACK CTreeManager_CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) {
        const auto controller = reinterpret_cast<TreeManagerController*>(lParamSort);
        const auto item1 = TreeItemId{ static_cast<int>(lParam1) };
        const auto item2 = TreeItemId{ static_cast<int>(lParam2) };

        return controller->compare(item1, item2);
    }


    void CTreeManager::ReloadContent() {
        treeView.DeleteAllItems();

        if (!controller) {
            return;
        }

        for (int i = 0; i < controller->getChildCount({}); i++) {
            const TreeItemId id = controller->getChildId({}, i);

            const std::string name = controller->getItemCaption(id);
            const bool hasChildren = controller->getChildCount(id) > 0;
            const int image = controller->getItemImage(id);

            InsertTreeItem(treeView, name.c_str(), static_cast<LPARAM>(id.value), NULL, hasChildren, image);
        }

        Sort(NULL);
    }

    void CTreeManager::Sort(HTREEITEM hItem) {
        if (!controller) {
            return;
        }

        TVSORTCB sort = {};

        sort.hParent = hItem;
        sort.lpfnCompare = CTreeManager_CompareFunc;
        sort.lParam = reinterpret_cast<LPARAM>(controller);

        treeView.SortChildrenCB(&sort, FALSE);
    }
}
