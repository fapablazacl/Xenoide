
#pragma once 

#include <memory>
#include <string>
#include <optional>
#include <functional>
#include <set>
#include <boost/filesystem.hpp>
#include <boost/bimap.hpp>

#include <xeno/wtl/wtl.h>
#include <xenoide/core/FolderExplorer.h>
#include <xenoide/core/FileSystemServiceBoost.h>


struct TreeItemHandle {
    int value = 0;

	bool operator== (const TreeItemHandle& rhs) const {
		return value == rhs.value;
	}

	bool operator!= (const TreeItemHandle& rhs) const {
		return value != rhs.value;
	}

	bool operator< (const TreeItemHandle& rhs) const {
		return value < rhs.value;
	}

	bool operator> (const TreeItemHandle& rhs) const {
		return value > rhs.value;
	}

	bool operator<= (const TreeItemHandle& rhs) const {
		return value <= rhs.value;
	}

	bool operator>= (const TreeItemHandle& rhs) const {
		return value >= rhs.value;
	}
};


class FolderExplorerView {
public:
    virtual ~FolderExplorerView() {}

    virtual void clear() = 0;

    virtual TreeItemHandle insert(const std::string &title, const std::optional<TreeItemHandle> parentItemId = {}, const bool hasChildren = false) = 0;

    virtual void sort(const TreeItemHandle itemId) = 0;

    virtual void sort(const TreeItemHandle itemId, std::function<int (const TreeItemHandle, const TreeItemHandle)> cmp) = 0;
};


class FolderExplorerPresenter {
public:
    FolderExplorerPresenter(Xenoide::FolderExplorer *folderExplorer) : folderExplorer(folderExplorer) {}

    void attachView(FolderExplorerView *view) {
        this->view = view;
    }

    void displayFolder(const Xenoide::Folder &folder) {
        Xenoide::FolderExplorer* model = this->folderExplorer;
        FolderExplorerView* view = this->view;
        
        view->clear();

        folderExplorer->setFolder(folder, [view, model](const Xenoide::Path path, const std::string &name) {
            const TreeItemHandle itemId = view->insert(name);

            model->insertItem(itemId.value, path);
        });
    }


    void onItemActivated(const TreeItemHandle itemId) {
        /*
        const auto pathIt = mPathItemsCache.left.find(itemId);

        if (pathIt == mPathItemsCache.left.end()) {
            return;
        }

        if (pathIt->second.type == Xenoide::PathType::File) {
            const std::string filePath = pathIt->second.value;

            mController->openFile(filePath);
        }
        */
    }


    void onItemExpanded(const TreeItemHandle itemId) {
        if (this->folderExplorer->itemIsPopulated(itemId.value)) {
            return;
        }

        Xenoide::FolderExplorer* folderExplorer = this->folderExplorer;
        
        const std::vector<Xenoide::FolderExplorerItem> items = folderExplorer->populateItem(itemId.value);

        for (const Xenoide::FolderExplorerItem& item : items) {
            const TreeItemHandle childId = view->insert(item.title, itemId, item.path.isFolder());
            folderExplorer->insertItem(childId.value, item.path);
        }

        view->sort(itemId, [folderExplorer](const TreeItemHandle i1, const TreeItemHandle i2) {
            return folderExplorer->compare(i1.value, i2.value);
        });
    }


private:
    FolderExplorerView *view = nullptr;
    Xenoide::FolderExplorer* folderExplorer = nullptr;
};


class CXenoFolderView;
class FolderExplorerWTL : public FolderExplorerView {
public:
    FolderExplorerWTL(CXenoFolderView& folderView);

    void clear() override;

    TreeItemHandle insert(const std::string& title, const std::optional<TreeItemHandle> parentItemId, const bool hasChildren) override;

    void sort(const TreeItemHandle itemId) override;

    void sort(const TreeItemHandle itemId, std::function<int(const TreeItemHandle, const TreeItemHandle)> cmp) override;

private:
    CXenoFolderView& folderView;
};


class FolderExplorerWTL;
class CXenoFolderView : public CWindowImpl<CXenoFolderView> {
    friend class FolderExplorerWTL;

public:
    enum { ID_FOLDERVIEW_TREEVIEW = 10000 };

public:
    DECLARE_WND_CLASS(NULL)

    BEGIN_MSG_MAP_EX(CXenoFolderView)
        MSG_WM_CREATE(OnCreate)
        MSG_WM_DESTROY(OnDestroy)
        MSG_WM_SIZE(OnSize)
        MSG_WM_NOTIFY(OnNotify)
        MSG_WM_ERASEBKGND(OnEraseBkgnd)
    END_MSG_MAP()

public:
    CXenoFolderView();

    LRESULT OnCreate(LPCREATESTRUCT cs);

    void OnDestroy();

    void OnSize(UINT nType, CSize size);

    LRESULT OnNotify(int idCtrl, LPNMHDR pnmh);

    LRESULT OnEraseBkgnd(HDC hDC);

    void DisplayFolder(const boost::filesystem::path& folderPath);

private:
    HTREEITEM InsertTreeItem(CTreeViewCtrl& treeView, const char* text, const int itemId);

    HTREEITEM InsertTreeItem(CTreeViewCtrl& treeView, const char* text, const int itemId, const HTREEITEM parentItem, const bool hasChildren);

private:
    Xenoide::FileSystemServiceBoost fileSystemService;
    Xenoide::FolderExplorer folderExplorer;
    
    std::unique_ptr<FolderExplorerWTL> mView;
    FolderExplorerPresenter presenter;

    int mItemCount = 0;

    CTreeViewCtrl mTreeView;
    boost::bimap<HTREEITEM, int> mTreeItemBimap;
};
