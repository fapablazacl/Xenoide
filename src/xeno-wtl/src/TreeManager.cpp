
#include <xeno/wtl/TreeManager.h>

#include <CommonControls.h>
#include <iostream>
#include <map>
#include <boost/filesystem.hpp>


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
    class CTreeManagerControllerMock : public TreeManagerController {
    public:
        CTreeManagerControllerMock() {

        }

        void clicked(const TreeItemId itemId) override {
            
        }

        int getChildCount(const TreeItemId itemId) const override {
            // check for the root
            if (itemId == TreeItemId{}) {
                return 10;
            }

            // each children will have three elements
            return 3;
        }

        TreeItemId getChildId(const TreeItemId parentId, const int i) const override {
            return TreeItemId{(10 * parentId.value + 1) + i};
        }

        std::string getItemCaption(const TreeItemId itemId) const override {
            return "ItemId: " + std::to_string(itemId.value);
        }

        int getItemImage(const TreeItemId itemId) const override {
            return 0;
        }
    };


    class CTreeManagerControllerFileSystem : public TreeManagerController {
    public:
        explicit CTreeManagerControllerFileSystem(const boost::filesystem::path& rootPath) : rootPath{rootPath} {
            fileContextMenu = {
                MenuData::action([]() {}, "&Open"),
                MenuData::action([]() {}, "&Open in Dedicated Window"),
                MenuData::separator(),
                MenuData::action([]() {}, "&Cut"),
                MenuData::action([]() {}, "&Copy"),
                MenuData::action([]() {}, "&Paste"),
                MenuData::separator(),
                MenuData::action([]() {}, "&Rename"),
                MenuData::action([]() {}, "&Delete")
            };

            folderContextMenu = {
                MenuData::action([]() {}, "&New File"),
                MenuData::action([]() {}, "&New Folder"),
                MenuData::separator(),
                MenuData::action([]() {}, "&Cut"),
                MenuData::action([]() {}, "&Copy"),
                MenuData::action([]() {}, "&Paste"),
                MenuData::separator(),
                MenuData::action([]() {}, "&Rename"),
                MenuData::action([]() {}, "&Delete")
            };
        }

        void clicked(const TreeItemId itemId) override {

        }

        int getChildCount(const TreeItemId itemId) const override {
            // determine current path
            // TODO: refactor into an utility method
            const boost::filesystem::path path = (itemId == TreeItemId{} ?  rootPath : itemPathMap[itemId]);

            // if the current path is not a directory, it can't have subpath.
            if (!boost::filesystem::is_directory(path)) {
                return 0;
            }

            // check if the specified item was previously populated, and return that
            if (const auto it = itemChildMap.find(itemId); it != itemChildMap.end()) {
                return static_cast<int>(it->second.size());
            }

            // populate child elements
            int i = 0;

            for (boost::filesystem::directory_iterator end, path_it{ path }; path_it != end; path_it++) {
                const boost::filesystem::path childPath = path_it->path();
                itemChildMap[itemId].push_back(childPath);

                const TreeItemId childId = getChildId(itemId, i);
                itemPathMap[childId] = childPath;

                i++;
            }

            return i + 1;
        }


        TreeItemId getChildId(const TreeItemId parentId, const int i) const override {
            const auto key = std::tuple(parentId, i);

            if (const auto it = parentItemMap.find(key); it != parentItemMap.end()) {
                return it->second;
            }
            
            const TreeItemId itemId = generateItemId();

            parentItemMap.insert({ key, itemId });

            return itemId;
        }


        std::string getItemCaption(const TreeItemId itemId) const override {
            const auto it = itemPathMap.find(itemId);
            assert(it != itemPathMap.end());

            return "ItemId: " + std::to_string(itemId.value) + " " + it->second.filename().string();
        }


        int getItemImage(const TreeItemId itemId) const override {
            const auto it = itemPathMap.find(itemId);
            assert(it != itemPathMap.end());

            return GetIconFromPath(it->second.string().c_str());

            // return boost::filesystem::is_directory(it->second) ? 0 : 1;
        }

        std::vector<MenuData> getItemPopupMenuData(const TreeItemId itemId) const override {
            // TODO: Refactor into an utility method
            const boost::filesystem::path path = (itemId == TreeItemId{} ?  rootPath : itemPathMap[itemId]);

            if (boost::filesystem::is_directory(path)) {
                return folderContextMenu;
            }

            return fileContextMenu;
        }

    private:
        TreeItemId generateItemId() const {
            return TreeItemId { ++count };
        }

    private:
        std::vector<MenuData> fileContextMenu;
        std::vector<MenuData> folderContextMenu;

        const boost::filesystem::path rootPath;

        mutable int count = 0;
        mutable std::map<std::tuple<TreeItemId, int>, TreeItemId> parentItemMap;
        mutable std::map<TreeItemId, boost::filesystem::path> itemPathMap;
        mutable std::map<TreeItemId, std::vector<boost::filesystem::path>> itemChildMap;
    };
}


namespace Xenoide {
    CTreeManager::CTreeManager() {
        m_bMsgHandled = false;
        // controller = new CTreeManagerControllerMock();
        controller = new CTreeManagerControllerFileSystem(boost::filesystem::current_path());
    }


    CTreeManager::~CTreeManager() {
        delete controller;
    }


    LRESULT CTreeManager::OnCreate(LPCREATESTRUCT cs) {
        SetMsgHandled(true);

        const DWORD dwStyle = TVS_FULLROWSELECT | TVS_SHOWSELALWAYS | TVS_HASBUTTONS | TVS_LINESATROOT | WS_VISIBLE | WS_CHILD;
    
        treeView.Create(m_hWnd, rcDefault, "", dwStyle, 0L, ID_FOLDERVIEW_TREEVIEW);
        treeView.SetExtendedStyle(TVS_EX_DOUBLEBUFFER, TVS_EX_DOUBLEBUFFER);

        // obtain 
        SHGetImageList(SHIL_SMALL, IID_IImageList, reinterpret_cast<void**>(&hImageList));
        treeView.SetImageList(hImageList);

        // start with the first level
        for (int i = 0; i < controller->getChildCount({}); i++) {
            const TreeItemId id = controller->getChildId({}, i);

            const std::string name = controller->getItemCaption(id);
            const bool hasChildren = controller->getChildCount(id) > 0;
            const int image = controller->getItemImage(id);

            InsertTreeItem(treeView, name.c_str(), static_cast<LPARAM>(id.value), NULL, hasChildren, image);
        }

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

                        InsertTreeItem(treeView, name.c_str(), static_cast<LPARAM>(childId.value), pnmtv.itemNew.hItem, hasChildren, image);
                    }

                    populated.insert(itemId);
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


    int CTreeManager::OnCommand(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled) {

        return 0;
    }
}
