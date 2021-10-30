
#pragma once 

#include <Xenoide/Gui/DialogManager.h>

#include <atlbase.h>
#include <atlapp.h>
#include <atlframe.h>
#include <atlctrls.h>
#include <atlctrlx.h>
#include <atluser.h>
#include <atlcrack.h>
#include <atlsplit.h>

namespace Xenoide {
    class CDialogManager : public DialogManager {
    public:
        CDialogManager();

        virtual ~CDialogManager();
        virtual DialogButton showMessageDialog(const MessageDialogData &data) const override;
        virtual boost::optional<std::string> showInputDialog(const InputDialogData &data) const override;
        virtual boost::optional<boost::filesystem::path> showFileDialog(const FileDialogData &data) const override;
        virtual boost::optional<boost::filesystem::path> showFolderDialog(const FolderDialogData &data) override;
        virtual boost::optional<boost::filesystem::path> showFileSearchDialog(const FileSearchDialogData &data) const override;

    private:
        UINT mapMsgBoxIcon(DialogIcon icon) const;

        UINT mapMsgBoxButton(DialogButton button) const;

        DialogButton mapMsgBoxResult(INT result) const;
    };
}
