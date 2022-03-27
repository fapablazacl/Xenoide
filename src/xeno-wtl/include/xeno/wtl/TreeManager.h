
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
