
#include "MainFrame.h"

#include <map>

#include <cassert>
#include <gdiplus.h>


constexpr int IMAGE_WIDTH = 48;
constexpr int IMAGE_HEIGHT = 48;


struct ActionData {
    LPSTR lpCaption = nullptr;

    LPSTR lpDescription = nullptr;

    LPSTR lpImagePath = nullptr;

    ActionData() {}

    explicit ActionData(LPSTR lpCaption, LPSTR lpDescription = nullptr, LPSTR lpImagePath = nullptr) {
        assert(lpCaption);

        this->lpCaption = lpCaption;
        this->lpDescription = lpDescription;
        this->lpImagePath = lpImagePath;
    }

    HBITMAP hBitmap = NULL;
};


static std::map<UINT, ActionData> gActionDataMap = {
    {
        ID_XENOIDE_FILE_NEW, 
        ActionData {
            _T("&New"), 
            _T("Creates a New File"), 
            _T("D:\\Assets\\apps\\icons\\VS2012 Modern Image Library\\x--archive--x\\Actions - VS2010\\24bitcolor bitmaps\\Document.bmp")
        }
    }, {
        ID_XENOIDE_FILE_OPEN, 
        ActionData {
            _T("&Open"), 
            _T("Opens an existing file"), 
            _T("D:\\Assets\\apps\\icons\\VS2012 Modern Image Library\\x--archive--x\\Actions - VS2010\\24bitcolor bitmaps\\openfolder_24.bmp")
        }
    }, {
        ID_XENOIDE_FILE_SAVE, 
        ActionData {
            _T("&Save"), 
            _T("Pending explanation"), 
            _T("D:\\Assets\\apps\\icons\\VS2012 Modern Image Library\\x--archive--x\\Actions - VS2010\\24bitcolor bitmaps\\Save.bmp")
        }
    },{
        ID_XENOIDE_FILE_SAVEAS, 
        ActionData {
            _T("Save &As"), 
            _T("Pending"), 
        }
    },{
        ID_XENOIDE_FILE_EXIT, 
        ActionData {
            _T("&Exit"), 
            _T("Pending")
        }
    },
};


BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
    return CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg);
}


BOOL CMainFrame::OnIdle()
{
    return FALSE;
}


LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    SetMenu(CreateMenuBar());
    CreateImageList(mImageList);

    // Command Bar
    CreateCommandBar(mCommandBar, GetMenu());
    SetMenu(nullptr);

    CreateToolBar(mToolBar, mImageList);

    CreateSimpleStatusBar();

    m_hWndClient = mCodeView.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);

    CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
    AddSimpleReBarBand(mCommandBar);
    AddSimpleReBarBand(mToolBar, NULL, TRUE);

    UpdateLayout();

    UIAddToolBar(mToolBar);

    CMessageLoop* pLoop = _Module.GetMessageLoop();
    assert(pLoop);
    pLoop->AddMessageFilter(this);
    pLoop->AddIdleHandler(this);

    return 0;
}


// TODO: Move this into an utility package
struct GdiplusGuard {
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartupInput startupInput;

    GdiplusGuard() {
        Gdiplus::GdiplusStartup(&gdiplusToken, &startupInput, NULL);
    }

    ~GdiplusGuard() {
        Gdiplus::GdiplusShutdown(gdiplusToken);
    }

    std::unique_ptr<Gdiplus::Bitmap> CreateBitmapFromFile(const LPWSTR filePath) {
        std::unique_ptr<Gdiplus::Bitmap> bitmap;
        bitmap.reset(Gdiplus::Bitmap::FromFile(filePath));

        return bitmap;
    }
};

static GdiplusGuard guard;


CBitmapHandle CMainFrame::LoadImageBitmap(const LPWSTR filePath) const {
    const auto flags = LR_LOADFROMFILE | LR_CREATEDIBSECTION;

    HBITMAP hBitmap = (HBITMAP)::LoadImageW(NULL, filePath, IMAGE_BITMAP, IMAGE_WIDTH, IMAGE_HEIGHT, flags);
    assert(hBitmap);

    CBitmapHandle bitmap;
    bitmap.Attach(hBitmap);
    return bitmap;
}


void CMainFrame::CreateImageList(CImageList &imageList) const {
    const std::vector<LPWSTR> bitmapFileNames = {
        L"D:\\Assets\\apps\\icons\\VS2012 Modern Image Library\\x--archive--x\\Actions - VS2010\\24bitcolor bitmaps\\Document.bmp",
        L"D:\\Assets\\apps\\icons\\VS2012 Modern Image Library\\x--archive--x\\Actions - VS2010\\24bitcolor bitmaps\\openfolder_24.bmp",
        L"D:\\Assets\\apps\\icons\\VS2012 Modern Image Library\\x--archive--x\\Actions - VS2010\\24bitcolor bitmaps\\Save.bmp"
    };

    imageList.Create(IMAGE_WIDTH, IMAGE_HEIGHT, ILC_COLOR24 | ILC_MASK, 3, 1);

    CBitmapHandle bmp;

    for (LPWSTR fileName : bitmapFileNames) {
        if (!bmp.IsNull()) {
            bmp.DeleteObject();
        }

        bmp = LoadImageBitmap(fileName);

        const int imageIndex = imageList.Add(bmp, RGB(255, 0, 255));
        assert(imageIndex >= 0);
    }
}


void CMainFrame::CreateToolBar(CToolBarCtrl &toolbarCtrl, const CImageList &imageList) const {
    const DWORD dwStyle = ATL_SIMPLE_TOOLBAR_STYLE | CCS_NOPARENTALIGN;

    HWND hWnd = toolbarCtrl.Create(m_hWnd, rcDefault, NULL, dwStyle, 0, ATL_IDW_TOOLBAR, nullptr);

    toolbarCtrl.SetButtonStructSize(sizeof(TBBUTTON));
    toolbarCtrl.SetImageList(imageList);

    // Define buttons
    std::vector<TBBUTTON> tbb{3};

    tbb[0].idCommand = ID_XENOIDE_FILE_NEW;
    tbb[0].fsState = TBSTATE_ENABLED;
    tbb[0].fsStyle = BTNS_BUTTON;
    tbb[0].iBitmap = 0;

    tbb[1].idCommand = ID_XENOIDE_FILE_OPEN;
    tbb[1].fsState = TBSTATE_ENABLED;
    tbb[1].fsStyle = BTNS_BUTTON;
    tbb[1].iBitmap = 1;

    tbb[2].idCommand = ID_XENOIDE_FILE_SAVE;
    tbb[2].fsState = TBSTATE_ENABLED;
    tbb[2].fsStyle = BTNS_BUTTON;
    tbb[2].iBitmap = 2;

    // Add the buttons to the toolbar
    toolbarCtrl.AddButtons(static_cast<int>(tbb.size()), tbb.data());

    // Resize the toolbar
    toolbarCtrl.AutoSize();
}


static BOOL AppendMenuItemInfo(HMENU hMenu, const UINT pos, const UINT id, const LPSTR item, HBITMAP hBitmap) {
    MENUITEMINFO info{ 0 };

    info.cbSize = sizeof(MENUITEMINFO);
    info.fMask = MIIM_STRING | MIIM_ID | (hBitmap != NULL ? MIIM_BITMAP : 0);
    info.wID = id;
    info.dwTypeData = item;
    info.hbmpItem = hBitmap;

    return InsertMenuItem(hMenu, pos, TRUE, &info);
}


static BOOL AppendMenuItemSeparator(HMENU hMenu, const UINT pos) {
    MENUITEMINFO info{ 0 };

    info.cbSize = sizeof(MENUITEMINFO);
    info.fMask = MIIM_TYPE;
    info.fType = MFT_SEPARATOR;
    
    return InsertMenuItem(hMenu, pos, TRUE, &info);
}

HMENU CMainFrame::CreateMenuBar() const {
    HMENU hMenuBar = CreateMenu();

    HMENU hFileMenu = CreatePopupMenu();

    UINT pos = 0;

    AppendMenuItemInfo(hFileMenu, pos++, ID_XENOIDE_FILE_NEW, _T("&New"), NULL);
    AppendMenuItemSeparator(hFileMenu, pos++);
    AppendMenuItemInfo(hFileMenu, pos++, ID_XENOIDE_FILE_OPEN, _T("&Open"), NULL);
    AppendMenuItemSeparator(hFileMenu, pos++);
    AppendMenuItemInfo(hFileMenu, pos++, ID_XENOIDE_FILE_SAVE, _T("&Save"), NULL);
    AppendMenuItemInfo(hFileMenu, pos++, ID_XENOIDE_FILE_SAVEAS, _T("Save &As"), NULL);
    AppendMenuItemSeparator(hFileMenu, pos++);
    AppendMenuItemInfo(hFileMenu, pos++, ID_XENOIDE_FILE_EXIT, _T("&Exit"), NULL);

    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hFileMenu, _T("&File"));

    return hMenuBar;
}


void CMainFrame::CreateCommandBar(CCommandBarCtrl &commandBarCtrl, const HMENU hMenuBar) const {
    RECT rcCmdBar = { 0, 0, 50, 50 };
    commandBarCtrl.Create(m_hWnd, rcCmdBar, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
    commandBarCtrl.AttachMenu(hMenuBar);
    commandBarCtrl.LoadImages(IDR_MAINFRAME);    
}
