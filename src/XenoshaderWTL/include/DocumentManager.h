
#pragma once 

#include "framework.h"

class DocumentManager : public CTabViewImpl<DocumentManager> {
public:
	DECLARE_WND_CLASS_EX(_T("WTL_TabView"), 0, COLOR_APPWORKSPACE)

    typedef CTabViewImpl<DocumentManager> Base;

    /*
    void CalcCloseButtonRect(int nItem, RECT& rcClose) {
        Base::CalcCloseButtonRect(nItem, rcClose);

        int width = rcClose.right - rcClose.left;
        int height = rcClose.bottom - rcClose.top;

        rcClose.left -= (width / 2);
        rcClose.right += (width / 2);

        rcClose.top -= (height / 2);
        rcClose.bottom += (height / 2);

        rcClose.left -= (width / 2);
        rcClose.right -= (width / 2);

        int x = 0;
    }*/
};
