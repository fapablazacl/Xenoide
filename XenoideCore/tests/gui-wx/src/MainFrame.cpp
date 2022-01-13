
#include "MainFrame.h"

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, "Hello, World") {
    auto panel = new wxPanel{this};

    mTextCtrlCode = new wxStyledTextCtrl{this};

    auto menuFile = new wxMenu{};
    menuFile->Append(ID_GREET, "&Hello...\tCtrl-H", "Some asdasdsa");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    auto menuHelp = new wxMenu{};
    menuHelp->Append(wxID_ABOUT);

    auto menuBar = new wxMenuBar{};
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar(menuBar);

    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");

    Bind(wxEVT_MENU, &MainFrame::OnHello, this, ID_GREET);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
}


void MainFrame::OnHello(wxCommandEvent& event) {
    wxLogMessage("Hello world from wxWidgets!");
}


void MainFrame::OnExit(wxCommandEvent& event) {
    Close(true);
}


void MainFrame::OnAbout(wxCommandEvent& event) {
    wxMessageBox("Some message", "Title", wxOK | wxICON_INFORMATION);
}
