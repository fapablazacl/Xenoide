
#pragma once 

#include <memory>
#include <string>
#include <optional>
#include <functional>
#include <set>
#include <cassert>

#include <atlbase.h>
#include <atlapp.h>
#include <atlwin.h>
#include <atlframe.h>
#include <atlctrls.h>
#include <atlctrlx.h>
#include <atluser.h>
#include <atlmisc.h>
#include <atlcrack.h>
#include <atlsplit.h>
#include <atldlgs.h>


namespace Xenoide {
    struct TreeItemId {
        TreeItemId() {}
        
        explicit TreeItemId(const int value) : value(value) {}
        
        bool operator== (const TreeItemId& item) const {
            return value == item.value;
        }

        bool operator!= (const TreeItemId& item) const {
            return value != item.value;
        }
        
        bool operator< (const TreeItemId& item) const {
            return value < item.value;
        }

        const int value = 0;
    };
    

    class TreeManagerController {
    public:
        virtual ~TreeManagerController() {}

        virtual void clicked(const TreeItemId itemId) = 0;

        //! the number of children for a given tree item
        virtual int getChildCount(const TreeItemId itemId) const = 0;

        //! get the unique treeitem ID for a parent and its i-th child
        virtual TreeItemId getChildId(const TreeItemId itemId, const int i) const = 0;

        //! get the caption text from a given item
        virtual std::string getItemCaption(const TreeItemId itemId) const = 0;

        //! get the image text from a given item
        virtual int getItemImage(const TreeItemId itemId) const = 0;
    };


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
        END_MSG_MAP()

    public:
        explicit CTreeManager();

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

    private:
        HIMAGELIST hImageList = NULL;
        std::set<TreeItemId> populated;
        CTreeViewCtrl treeView;
        TreeManagerController *controller = nullptr;
    };
}
