
#include "CDialogManager.h"

#include <atldlgs.h>

namespace Xenoide  {
    CDialogManager::CDialogManager() {}


    CDialogManager::~CDialogManager() {}

    
    UINT CDialogManager::mapMsgBoxIcon(DialogIcon icon) const {
        switch (icon) {
            case DialogIcon::Question:      
                return MB_ICONQUESTION;       

            case DialogIcon::Information:   
                return MB_ICONINFORMATION;    

            case DialogIcon::Warning:       
                return MB_ICONWARNING;        

            case DialogIcon::Error:         
                return MB_ICONERROR;          
        }

        return MB_OK;
    }


    UINT CDialogManager::mapMsgBoxButton(DialogButton button) const {
        switch (button) {
            case DialogButton::YesNoCancel:    
                return MB_YESNOCANCEL;

            case DialogButton::Ok :         
                return MB_OK;           

            case DialogButton::OkCancel:    
                return MB_OKCANCEL;        
        }

        return MB_OK;
    }
    
    
    DialogButton CDialogManager::mapMsgBoxResult(INT result) const {
        switch (result) {
            case IDOK:
                return DialogButton::Ok;

            case IDCANCEL:    
                return DialogButton::Cancel;

            case IDYES:
                return DialogButton::Yes;

            case IDNO:
                return DialogButton::No;
        }

        return DialogButton::Ok;
    }


    DialogButton CDialogManager::showMessageDialog(const MessageDialogData &data) const {
        UINT flags = MB_OK;

        flags |= this->mapMsgBoxIcon(data.icon);
        flags |= this->mapMsgBoxButton(data.buttons);

        /*
        std::wstring wmsg(data.message.begin(), data.message.end());
        std::wstring wtitle(data.title.begin(), data.title.end());
        */

        int result = ::MessageBox(NULL, data.message.c_str(), data.title.c_str(), flags);

        return this->mapMsgBoxResult(result);
    }
    
    
    boost::optional<std::string> CDialogManager::showInputDialog(const InputDialogData &data) const {
        // TODO: Add implementation
        return data.defaultText;
    }


    boost::optional<boost::filesystem::path> CDialogManager::showFileDialog(const FileDialogData &data) const {
        CFileDialog dialog(TRUE, _T("All Files\0*.*"));

        if (dialog.DoModal() == IDOK) {
            std::string fileName = dialog.m_szFileName;
            return boost::filesystem::path{fileName};
        }
        
        return {};
    }

    boost::optional<boost::filesystem::path> CDialogManager::showFolderDialog(const FolderDialogData &data) {
        CFolderDialog dialog{};

        return {};
    }


    boost::optional<boost::filesystem::path> CDialogManager::showFileSearchDialog(const FileSearchDialogData &data) const {
        // TODO: Add implementation
        return {};
    }
}
