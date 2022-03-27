
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
    struct CTreeItem {
        CTreeItem() {}
        
        CTreeItem(void* value) : value(value) {}
        
        bool operator!= (const CTreeItem& item) const {
            return value != item.value;
        }

        bool operator== (const CTreeItem& item) const {
            return value == item.value;
        }
        
        bool operator< (const CTreeItem& item) const {
            return value < item.value;
        }
     
        void* value = nullptr;
    };


    class CTreeManagerItemAction {
    public:
        virtual ~CTreeManagerItemAction() {}

        virtual std::string getCaption() const = 0;

        virtual void perform() = 0;
    };


    struct CTreeItemId {
        CTreeItemId() {}
        
        CTreeItemId(const int value) : value(value) {}
        
        bool operator== (const CTreeItemId& item) const {
            return value == item.value;
        }

        bool operator!= (const CTreeItemId& item) const {
            return value != item.value;
        }
        
        bool operator< (const CTreeItemId& item) const {
            return value < item.value;
        }

        const int value = 0;
    };
    
    struct CTreeItemIndex {
        unsigned short level = 0;
        unsigned short row = 0;

        CTreeItemIndex() {}

        CTreeItemIndex(const unsigned short level, const unsigned short row) 
            : level(level), row(row) {
        
            // checks for invalid in
            assert(level != 0 || ((level == 0) && row == 0));
        }

        bool operator== (const CTreeItemIndex& rhs) const {
            if (level != rhs.level) {
                return false;
            }

            if (row != rhs.row) {
                return false;
            }

            return true;
        }

        bool operator!= (const CTreeItemIndex& rhs) const {
            return !(*this == rhs);
        }

        bool operator< (const CTreeItemIndex& rhs) const {
            if (level == rhs.level) {
                return row < rhs.row;
            }

            return level < rhs.level;
        }

        static CTreeItemIndex rootIndex() {
            return { 0, 0 };
        }

        CTreeItemIndex indexAtRow(const unsigned short row) const {
            return { level, row };
        }
    };

    class CTreeManagerController {
    public:
        virtual ~CTreeManagerController() {}

        virtual void clicked(const CTreeItemId itemId) = 0;

        //! the number of children for a given tree item
        virtual int getChildCount(const CTreeItemId itemId) const = 0;

        //! get the unique treeitem ID for a parent and its i-th child
        virtual CTreeItemId getChildId(const CTreeItemId itemId, const int i) const = 0;

        //! get the caption text from a given item
        virtual std::string getItemCaption(const CTreeItemId itemId) const = 0;

        //! get the image text from a given item
        virtual int getItemImage(const CTreeItemId itemId) const = 0;
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

    private:
        HIMAGELIST hImageList = NULL;
        std::set<CTreeItemId> populated;
        CTreeViewCtrl treeView;
        CTreeManagerController *controller = nullptr;
    };
}
