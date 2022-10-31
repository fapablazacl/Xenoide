
#pragma once 

#include <memory>
#include <string>
#include <optional>
#include <functional>
#include <set>
#include <cassert>
#include <vector>

#include <xeno/wtl/wtl.h>
#include <xeno/ui/TreeManagerController.h>

#include "MenuFactory.h"

namespace Xenoide {
    struct CImageListCreateData {
        int cx;
        int cy;
        UINT flags;
        std::vector<HICON> icons;
    };


    inline CImageList CreateImageList(const int cx, const int cy, const UINT flags, const std::vector<HICON>& icons) {
        CImageList imageList;

        imageList.Create(cx, cy, flags, static_cast<int>(icons.size()), 0);

        for (const HICON hIcon : icons) {
            assert(hIcon);
            int index = imageList.AddIcon(hIcon);

            assert(index >= 0);
        }

        return imageList;
    }

    class TreeManagerController;

    /**
     * @brief TreeManager class component. Augment a native TreeView with additional features.
     * 
     * Features:
     * - Initial Item Populate Action from Controller
     * 
     * TODO Features:
     * - Update Tree Item when the underlying model changes.
     * - Implement contextual menu.
     */
    class CTreeManager : public CWindowImpl<CTreeManager> {
    public:
        enum { ID_FOLDERVIEW_TREEVIEW = 10000 };

    public:
        DECLARE_WND_CLASS(NULL)

        BEGIN_MSG_MAP_EX(CTreeManager)
            MSG_WM_CREATE(OnCreate)
            MSG_WM_DESTROY(OnDestroy)
            MSG_WM_SIZE(OnSize)
            MSG_WM_NOTIFY(OnNotify)
            MSG_WM_ERASEBKGND(OnEraseBkgnd)
            MSG_WM_CONTEXTMENU(OnContextMenu)
            MSG_WM_COMMAND(OnCommand);
        END_MSG_MAP()

    public:
        CTreeManager();

        explicit CTreeManager(const HIMAGELIST hImageList);

        ~CTreeManager();

        LRESULT OnCreate(LPCREATESTRUCT cs);

        void OnDestroy();

        void OnSize(UINT nType, CSize size);

        LRESULT OnNotify(int idCtrl, LPNMHDR pnmh);

        LRESULT OnEraseBkgnd(HDC hDC);

        void SetController(TreeManagerController* controller);

        const TreeManagerController* GetController() const {
            return controller;
        }

        void OnContextMenu(CWindow wnd, CPoint point);

        int OnCommand(UINT uNotifyCode, int nID, CWindow wndCtl);

        void ReloadContent();

        void Sort(HTREEITEM hItem);

    private:
        HIMAGELIST hImageList = NULL;
        std::set<TreeItemId> populated;
        CTreeViewCtrl treeView;
        TreeManagerController *controller = nullptr;

        CCommandMap commandMap;  
    };
}
