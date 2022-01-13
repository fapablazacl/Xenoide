
#pragma once 

#include <atlbase.h>
#include <atlapp.h>
#include <atlframe.h>
#include <atlctrls.h>
#include <atlctrlx.h>
#include <atluser.h>
#include <atlcrack.h>
#include <atlsplit.h>

namespace Xenoide {
    class CClassView : public CWindowImpl<CClassView, CWindow, CControlWinTraits> {
    public:
        DECLARE_WND_CLASS(_T("Xenoide::CClassBrowser"))

        BEGIN_MSG_MAP(CFolderBrowser)
            MSG_WM_CREATE(OnCreate)
        END_MSG_MAP()

    public:
        CClassView();

        ~CClassView();

        int OnCreate(LPCREATESTRUCT lpCreateStruct);

    private:
        CTreeViewCtrl treeView;
        CImageListManaged imageList;
        CBitmap bitmap1, bitmap2, bitmap3;
    };
}
