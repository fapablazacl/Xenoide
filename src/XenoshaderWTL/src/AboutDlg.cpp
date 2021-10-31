
#include "AboutDlg.h"


LRESULT AboutDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
    CenterWindow();

    // let the system set the focus
    return TRUE;
}
 
LRESULT AboutDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
    EndDialog(IDCANCEL);
    return 0;
}
 
LRESULT AboutDlg::OnOKCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
    EndDialog(wID);
    return 0;
}
