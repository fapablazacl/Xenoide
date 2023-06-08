
#pragma once 

#include <xeno/ui/DialogManager.h>

#include <xeno/wtl/wtl.h>

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
