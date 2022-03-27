
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
    class CTreeManagerControllerMock : public CTreeManagerController {
    public:
        void clicked(const CTreeItemId itemId) override {
            
        }

        int getChildCount(const CTreeItemId itemId) const override {
            // check for the root
            if (itemId == CTreeItemId{}) {
                return 10;
            }

            // each children will have three elements
            return 3;
        }

        CTreeItemId getChildId(const CTreeItemId parentId, const int i) const override {
            return CTreeItemId{(10 * parentId.value + 1) + i};
        }

        std::string getItemCaption(const CTreeItemId itemId) const override {
            return "ItemId: " + std::to_string(itemId.value);
        }

        int getItemImage(const CTreeItemId itemId) const override {
            return 0;
        }
    };


    class CTreeManagerControllerFileSystem : public CTreeManagerController {
    public:
        explicit CTreeManagerControllerFileSystem(const boost::filesystem::path& rootPath) : rootPath{rootPath} {}

        void clicked(const CTreeItemId itemId) override {}

        int getChildCount(const CTreeItemId itemId) const override {

            // determine current path
            const boost::filesystem::path path = (itemId == CTreeItemId{} ?  rootPath : itemPathMap[itemId]);

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

                const CTreeItemId childId = getChildId(itemId, i);
                itemPathMap[childId] = childPath;

                i++;
            }

            return i + 1;
        }


        CTreeItemId getChildId(const CTreeItemId parentId, const int i) const override {
            const auto key = std::tuple(parentId, i);

            if (const auto it = parentItemMap.find(key); it != parentItemMap.end()) {
                return it->second;
            }
            
            const CTreeItemId itemId = generateItemId();

            parentItemMap.insert({ key, itemId });

            return itemId;
        }


        std::string getItemCaption(const CTreeItemId itemId) const override {
            const auto it = itemPathMap.find(itemId);
            assert(it != itemPathMap.end());

            return "ItemId: " + std::to_string(itemId.value) + " " + it->second.filename().string();
        }


        int getItemImage(const CTreeItemId itemId) const override {
            const auto it = itemPathMap.find(itemId);
            assert(it != itemPathMap.end());

            return GetIconFromPath(it->second.string().c_str());

            // return boost::filesystem::is_directory(it->second) ? 0 : 1;
        }

    private:
        CTreeItemId generateItemId() const {
            return CTreeItemId { ++count };
        }

    private:
        const boost::filesystem::path rootPath;

        mutable int count = 0;
        mutable std::map<std::tuple<CTreeItemId, int>, CTreeItemId> parentItemMap;
        mutable std::map<CTreeItemId, boost::filesystem::path> itemPathMap;
        mutable std::map<CTreeItemId, std::vector<boost::filesystem::path>> itemChildMap;
    };
}


namespace Xenoide {
    union IndexLParamConversionHelper {
        CTreeItemIndex index;
        LRESULT lParam;
    };

    LPARAM create_lParam(const CTreeItemIndex index) {
        static_assert(sizeof(LPARAM) >= sizeof(CTreeItemIndex));

        IndexLParamConversionHelper helper{};

        helper.lParam = 0;
        helper.index = index;

        return helper.lParam;
    }

    CTreeItemIndex create_index(const LPARAM lParam) {
        static_assert(sizeof(LPARAM) >= sizeof(CTreeItemIndex));

        IndexLParamConversionHelper helper{};

        helper.lParam = lParam;

        return helper.index;
    }

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
            const CTreeItemId id = controller->getChildId({}, i);

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


    LRESULT CTreeManager::OnNotify(int idCtrl, LPNMHDR pnmh) {
        SetMsgHandled(true);

        if (pnmh->code == NM_DBLCLK) {
            const HTREEITEM selectedItem = treeView.GetSelectedItem();

            if (selectedItem) {

            }
        }
        else if (pnmh->code == TVN_ITEMEXPANDING) {
            const auto &pnmtv = *reinterpret_cast<LPNMTREEVIEW>(pnmh);

            if (pnmtv.action == TVE_EXPAND) {
                CTreeItemId itemId{static_cast<int>(pnmtv.itemNew.lParam)};

                const auto it = populated.find(itemId);

                if (it == populated.end()) {
                    for (int i = 0; i < controller->getChildCount(itemId); i++) {
                        const CTreeItemId childId = controller->getChildId(itemId, i);

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
}
