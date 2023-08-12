
#pragma once 

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class MainApp : public wxApp {
public:
    bool OnInit() override;
};
