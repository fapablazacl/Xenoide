
#include "MainFrame.h"

#include <cassert>
#include <gdiplus.h>


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
    SetMenu(NULL);

    CreateToolBar(mToolBar, mImageList);

    // CreateSimpleStatusBar();

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


// TODO: Move into a utility package
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


CBitmap CMainFrame::LoadImageBitmap(const LPWSTR filePath) const {
    auto gdiplusBitmap = guard.CreateBitmapFromFile(filePath);
    assert(gdiplusBitmap->GetLastStatus() == Gdiplus::Ok);

    HBITMAP hBitmap;
    gdiplusBitmap->GetHBITMAP(NULL, &hBitmap);

    CBitmap bitmap;
    bitmap.Attach(hBitmap);

    return bitmap;
}


void CMainFrame::CreateImageList(CImageList &imageList) const {
    const std::vector<LPWSTR> bitmapFileNames = {
        L"D:\\Assets\\apps\\icons\\VS2019 Image Library\\vswin2019\\Document\\Document_16x.png",
        L"D:\\Assets\\apps\\icons\\VS2019 Image Library\\vswin2019\\Document\\Document_16x.png",
        L"D:\\Assets\\apps\\icons\\VS2019 Image Library\\vswin2019\\Document\\Document_16x.png"
    };

    imageList.Create(16, 16, ILC_COLOR32 | ILC_MASK, 3, 1);

    CBitmap bmp;

    for (LPWSTR fileName : bitmapFileNames) {
        if (!bmp.IsNull()) {
            bmp.DeleteObject();
        }

        bmp = LoadImageBitmap(fileName);

        assert(!bmp.IsNull());

        imageList.Add(bmp, RGB(255, 255, 255));
    }
}


void CMainFrame::CreateToolBar(CToolBarCtrl &toolbarCtrl, const CImageList &imageList) const {
    HWND hWnd = toolbarCtrl.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_TOOLBAR_STYLE | CCS_NOPARENTALIGN, 0, ATL_IDW_TOOLBAR, nullptr);

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


HMENU CMainFrame::CreateMenuBar() const {
    HMENU hMenuBar = CreateMenu();

    HMENU hFileMenu = CreatePopupMenu();
    AppendMenu(hFileMenu, MF_STRING , ID_XENOIDE_FILE_NEW, _T("&New"));
    AppendMenu(hFileMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(hFileMenu, MF_STRING , ID_XENOIDE_FILE_OPEN, _T("&Open"));
    AppendMenu(hFileMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(hFileMenu, MF_STRING , ID_XENOIDE_FILE_SAVE, _T("&Save"));
    AppendMenu(hFileMenu, MF_STRING , ID_XENOIDE_FILE_SAVEAS, _T("Save &As"));
    AppendMenu(hFileMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(hFileMenu, MF_STRING , ID_XENOIDE_FILE_EXIT, _T("&Exit"));

    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hFileMenu, _T("&File"));

    return hMenuBar;
}


void CMainFrame::CreateCommandBar(CCommandBarCtrl &commandBarCtrl, const HMENU hMenuBar) const {
    RECT rcCmdBar = { 0, 0, 50, 50 };
    commandBarCtrl.Create(m_hWnd, rcCmdBar, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
    commandBarCtrl.AttachMenu(hMenuBar);
    commandBarCtrl.LoadImages(IDR_MAINFRAME);    
}
