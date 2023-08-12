
#pragma once 

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/stc/stc.h>

enum {
    ID_GREET
};

class MainFrame : public wxFrame {
public:
    MainFrame();

private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

private:
    // wxAuiNotebook *mNotebook = nullptr;
    wxStyledTextCtrl *mTextCtrlCode;
};
