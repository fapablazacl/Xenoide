
#pragma once 

#include <memory>
#include <string>
#include <optional>
#include <functional>
#include <set>
#include <boost/filesystem.hpp>
#include <boost/bimap.hpp>

#include "framework.h"

#include "AppController.h"


class FolderExplorerView {
public:
    virtual ~FolderExplorerView() {}

    virtual void clear() = 0;

    virtual int insert(const std::string &title, const std::optional<int> parentItemId = {}, const bool isDirectory = false) = 0;

    virtual void sort(const int itemId) = 0;

    virtual void sort(const int itemId, std::function<int (int, int)> cmp) = 0;
};


class FolderExplorerPresenter {
public:
    FolderExplorerPresenter(AppController *controller) : mController(controller) {}

    void attachView(FolderExplorerView *view) {
        mView = view;
    }

    void displayFolder(const boost::filesystem::path &folderPath) {
        mView->clear();

        mPathItemsCache.clear();
        mPopulatedItems.clear();

        const std::string folderName = folderPath.filename().string();
        const int itemId = mView->insert(folderName);

        mPathItemsCache.insert({itemId, folderPath});
    }

    void onItemActivated(const int itemId) {
        const auto pathIt = mPathItemsCache.left.find(itemId);

        if (pathIt == mPathItemsCache.left.end()) {
            return;
        }

        if (boost::filesystem::is_regular_file(pathIt->second)) {
            const auto filePath = pathIt->second;
            mController->openFile(filePath);
        }
    }

    void onItemExpanded(const int itemId) {
        if (! itemIsPopulated(itemId)) {
            populateItem(itemId);
        }
    }

public:
    void populateItem(const int parentItemId) {
        using boost::filesystem::recursive_directory_iterator;
        using boost::filesystem::directory_iterator;

        const auto folderPathIt = mPathItemsCache.left.find(parentItemId);

        if (folderPathIt == mPathItemsCache.left.end()) {
            return;
        }

        directory_iterator current{folderPathIt->second}, end;

        while (current != end) {
            const auto currentPath = current->path();

            const auto parentCacheIt = mPathItemsCache.right.find(currentPath.parent_path());

            if (parentCacheIt == mPathItemsCache.right.end()) {
                continue;
            }

            const bool isDirectory = boost::filesystem::is_directory(currentPath);
            const std::string name = currentPath.filename().string();
            const int itemId = mView->insert(name, parentItemId, isDirectory);
            mPathItemsCache.insert({itemId, currentPath});
            
            ++current;
        }

        mPopulatedItems.insert(parentItemId);

        mView->sort(parentItemId, [this](const int i1, const int i2) {
            return this->compare(i1, i2);
        });
    }


    bool itemIsPopulated(const int itemId) const {
        const auto it = mPopulatedItems.find(itemId); 

        return it != mPopulatedItems.end();
    }


    int compare(const int itemId1, const int itemId2) const {
        const auto path1 = mPathItemsCache.left.find(itemId1)->second;
        const auto path2 = mPathItemsCache.left.find(itemId2)->second;

        if (boost::filesystem::is_directory(path1) && !boost::filesystem::is_directory(path2)) {
            return -1;
        }

        if (!boost::filesystem::is_directory(path1) && boost::filesystem::is_directory(path2)) {
            return 1;
        }
        
        return path1.filename().string().compare(path2.filename().string());
    }

private:
    FolderExplorerView *mView = nullptr;
    AppController *mController = nullptr;
    boost::bimap<int, boost::filesystem::path> mPathItemsCache;
    std::set<int> mPopulatedItems;
};


class CXenoFolderView;
class FolderExplorerWTL : public FolderExplorerView {
public:
    FolderExplorerWTL(CXenoFolderView& folderView);

    void clear() override;

    int insert(const std::string& title, const std::optional<int> parentItemId, const bool hasChildren) override;

    void sort(const int itemId) override;

    void sort(const int itemId, std::function<int(int, int)> cmp) override;

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
    std::unique_ptr<FolderExplorerWTL> mView;
    FolderExplorerPresenter mPresenter;

    int mItemCount = 0;

    CTreeViewCtrl mTreeView;
    boost::bimap<HTREEITEM, int> mTreeItemBimap;
};
