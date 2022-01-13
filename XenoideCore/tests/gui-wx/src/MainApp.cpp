
#include "MainApp.h"
#include "MainFrame.h"

bool MainApp::OnInit() {
    auto frame = new MainFrame{};

    frame->Show(true);

    return true;
}

wxIMPLEMENT_APP(MainApp);
