
#include "MainFrame.h"

#include <map>

#include <cassert>
#include <gdiplus.h>

constexpr int IMAGE_WIDTH = 48;
constexpr int IMAGE_HEIGHT = 48;

CMainFrame::CMainFrame() {
    const std::wstring parentPath = L"D:\\Assets\\apps\\icons\\VS2012 Modern Image Library\\x--archive--x\\Actions - VS2010\\24bitcolor bitmaps\\";

    mCommandBitmapPathMap[ID_XENOIDE_FILE_NEW] = parentPath + L"Document.bmp";
    mCommandBitmapPathMap[ID_XENOIDE_FILE_OPEN] = parentPath + L"openfolder_24.bmp";
    mCommandBitmapPathMap[ID_XENOIDE_FILE_SAVE] = parentPath + L"Save.bmp";
}


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
    const std::vector<IdeCommands> toolbarCommands = {
        ID_XENOIDE_FILE_NEW,
        ID_XENOIDE_FILE_OPEN,
        ID_XENOIDE_FILE_SAVE,
    };


    SetMenu(CreateMenuBar());
    CreateToolBarImageList(mToolBarImageList, toolbarCommands);

    // Command Bar
    CreateCommandBar(mCommandBar, GetMenu());
    SetMenu(nullptr);

    CreateToolBar(mToolBar, mToolBarImageList, toolbarCommands);

    CreateSimpleStatusBar();

    const DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
    m_hWndClient = mCodeView.Create(m_hWnd, rcDefault, NULL, dwStyle, WS_EX_CLIENTEDGE);

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


void CMainFrame::CreateToolBarImageList(CImageList &imageList, const std::vector<IdeCommands> &commands) const {
    imageList.Create(IMAGE_WIDTH, IMAGE_HEIGHT, ILC_COLOR24 | ILC_MASK, 3, 1);

    for (const IdeCommands command : commands) {
        CBitmapHandle bmp;

        if (!bmp.IsNull()) {
            bmp.DeleteObject();
        }

        const std::wstring fileName = mCommandBitmapPathMap.find(command)->second;

        bmp = LoadImageBitmap(const_cast<LPWSTR>(fileName.c_str()));

        const int imageIndex = imageList.Add(bmp, RGB(255, 0, 255));
        assert(imageIndex >= 0);
    }
}


void CMainFrame::CreateToolBar(CToolBarCtrl &toolbarCtrl, const CImageList &imageList, const std::vector<IdeCommands> &commands) const {
    const DWORD dwStyle = ATL_SIMPLE_TOOLBAR_STYLE | TBSTYLE_TRANSPARENT | CCS_NODIVIDER | CCS_NOPARENTALIGN | CCS_NORESIZE | CCS_TOP;
    const HWND hWnd = toolbarCtrl.Create(m_hWnd, rcDefault, NULL, dwStyle, 0, ATL_IDW_TOOLBAR, nullptr);
    
    toolbarCtrl.SetButtonStructSize(sizeof(TBBUTTON));
    toolbarCtrl.SetImageList(imageList);

    // Define buttons
    std::vector<TBBUTTON> tbbs;

    for (size_t i = 0; i < commands.size(); i++) {
        TBBUTTON tbb = {};

        tbb.idCommand = commands[i];
        tbb.fsState = TBSTATE_ENABLED;
        tbb.fsStyle = BTNS_BUTTON;
        tbb.iBitmap = static_cast<int>(i);

        tbbs.push_back(tbb);
    }

    toolbarCtrl.AddButtons(static_cast<int>(tbbs.size()), tbbs.data());
    toolbarCtrl.AutoSize();
}


static BOOL AppendMenuItemInfo(HMENU hMenu, const UINT pos, const UINT id, const LPWSTR item, HBITMAP hBitmap) {
    MENUITEMINFO info{ 0 };

    info.cbSize = sizeof(MENUITEMINFO);
    info.fMask = MIIM_STRING | MIIM_ID;
    info.wID = id;
    info.dwTypeData = item;

    BOOL result = InsertMenuItem(hMenu, pos, TRUE, &info);
    assert(result);
    
    if (hBitmap != nullptr) {
        SetMenuItemBitmaps(hMenu, id, MF_BYCOMMAND, hBitmap, hBitmap);
    }
    
    return TRUE;
}


static BOOL AppendMenuItemSeparator(HMENU hMenu, const UINT pos) {
    MENUITEMINFO info{ 0 };

    info.cbSize = sizeof(MENUITEMINFO);
    info.fMask = MIIM_TYPE;
    info.fType = MFT_SEPARATOR;
    
    return InsertMenuItem(hMenu, pos, TRUE, &info);
}


static BOOL AppendMenuItem(HMENU hMenu, const UINT pos, HMENU hSubMenu, const LPWSTR item) {
    MENUITEMINFO info{ 0 };
    info.cbSize = sizeof(MENUITEMINFO);
    info.fMask = MIIM_STRING | MIIM_SUBMENU;
    info.dwTypeData = item;
    info.hSubMenu = hSubMenu;
    
    return InsertMenuItem(hMenu, pos, TRUE, &info);
}


HMENU CMainFrame::CreateMenuBar() {
    CMenuHandle test;

    HMENU hMenuBar = CreateMenu();
    HMENU hFileMenu = CreatePopupMenu();
    UINT pos = 0;

    AppendMenuItemInfo(hFileMenu, pos++, ID_XENOIDE_FILE_NEW, _T("&New\tCtrl+N"), GetOrLoadBitmap(ID_XENOIDE_FILE_NEW));
    AppendMenuItemSeparator(hFileMenu, pos++);
    AppendMenuItemInfo(hFileMenu, pos++, ID_XENOIDE_FILE_OPEN, _T("&Open\tCtrl+O"), GetOrLoadBitmap(ID_XENOIDE_FILE_OPEN));
    AppendMenuItemSeparator(hFileMenu, pos++);
    AppendMenuItemInfo(hFileMenu, pos++, ID_XENOIDE_FILE_SAVE, _T("&Save\tCtrl+S"), GetOrLoadBitmap(ID_XENOIDE_FILE_SAVE));
    AppendMenuItemInfo(hFileMenu, pos++, ID_XENOIDE_FILE_SAVEAS, _T("Save &As"), GetOrLoadBitmap(ID_XENOIDE_FILE_SAVEAS));
    AppendMenuItemSeparator(hFileMenu, pos++);
    AppendMenuItemInfo(hFileMenu, pos++, ID_XENOIDE_FILE_EXIT, _T("&Exit\tAlt+F4"), GetOrLoadBitmap(ID_XENOIDE_FILE_EXIT));

    AppendMenuItem(hMenuBar, 0, hFileMenu, _T("&File"));

    return hMenuBar;
}


void CMainFrame::CreateCommandBar(CCommandBarCtrl &commandBarCtrl, const HMENU hMenuBar) const {
    RECT rcCmdBar = { 0, 0, 50, 50 };
    commandBarCtrl.Create(m_hWnd, rcCmdBar, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
    commandBarCtrl.AttachMenu(hMenuBar);
    commandBarCtrl.LoadImages(IDR_MAINFRAME);    
}


CBitmap& CMainFrame::GetOrLoadBitmap(const std::wstring& fullFilePath) {
    const auto it = mFullFilePathBitmapMap.find(fullFilePath);

    if (it == mFullFilePathBitmapMap.end()) {
        CBitmapHandle handle = LoadImageBitmap(const_cast<LPWSTR>(fullFilePath.c_str()));
        CBitmap& bitmap = mFullFilePathBitmapMap[fullFilePath];

        bitmap.Attach(handle);

        return bitmap;    
    }

    return it->second;    
}


CBitmapHandle CMainFrame::GetOrLoadBitmap(const IdeCommands command) {
    const auto it = mCommandBitmapPathMap.find(command);

    if (it == mCommandBitmapPathMap.end()) {
        return {};
    }

    CBitmap& bitmap = GetOrLoadBitmap(it->second);

    return bitmap.m_hBitmap;
}
