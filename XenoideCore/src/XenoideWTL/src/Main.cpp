
#include "CIdeFrame.h"

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

CAppModule _Module;

using Xenoide::CIdeFrame;
using Xenoide::IDEFrame;    

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    ::SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);

    ::CoInitialize(NULL);

    AtlInitCommonControls(ICC_COOL_CLASSES | ICC_TREEVIEW_CLASSES | ICC_BAR_CLASSES);

    _Module.Init(NULL, hInstance);

    auto ideFrameModel = IDEFrame::Model::create();

    IDEFrame::Presenter ideFramePresenter{ideFrameModel.get()};
    CIdeFrame mainFrame{&ideFramePresenter};
    MSG msg;

    if (NULL == mainFrame.Create(NULL, CWindow::rcDefault, _T("Xenoide"))) {
        return 1;
    }

    mainFrame.ShowWindow(nCmdShow);
    mainFrame.UpdateWindow();

    while (::GetMessage(&msg, NULL, 0, 0) > 0) {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
    }

    _Module.Term();

    ::CoUninitialize();

    return static_cast<int>(msg.wParam);
}
