
#include <vector>
#include <string_view>

#include "MainFrame.h"
#include "Lexilla.h"
#include "LexillaAccess.h"

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")


CAppModule _Module;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    ::SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);
    ::AtlInitCommonControls(ICC_COOL_CLASSES | ICC_TREEVIEW_CLASSES | ICC_BAR_CLASSES);

    HMODULE hModScintilla = ::LoadLibrary(_T("Scintilla.dll"));

    if (hModScintilla == nullptr) {
        constexpr auto msg = _T("Required Scintilla.dll module wasn't found.");
        constexpr auto title = _T("XenoShader");

        ::MessageBox(nullptr, msg, title, MB_OK | MB_ICONERROR);
        return 1;
    }

    Lexilla::SetDefaultDirectory(".");
    Lexilla::Load(".");
    
    _Module.Init(NULL, hInstance);

    {
        // RUN MAIN LOOP

        CMessageLoop theLoop;
        _Module.AddMessageLoop(&theLoop);

        MainFrame wndFrame;

        if(wndFrame.CreateEx() == NULL) {
            ATLTRACE(_T("Frame window creation failed!\n"));
            return EXIT_FAILURE;
        }

        wndFrame.ShowWindow(nCmdShow);
        ::SetForegroundWindow(wndFrame);

        int nRet = theLoop.Run();

        _Module.RemoveMessageLoop();
        return nRet;
    }

    _Module.Term();

    ::FreeLibrary(hModScintilla);
    
    return EXIT_SUCCESS;
}
