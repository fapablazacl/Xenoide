
#include <Xenoide/Gui/FindReplaceDialog.h>
#include <Xenoide/Gui/WindowsUtils.h>
#include <Xenoide/Gui/Document.h>

#include <iostream>


namespace Xenoide {
    FindReplaceDialog::~FindReplaceDialog() {}
}

namespace Xenoide {
    void FindReplaceDialog::Presenter::attachView(FindReplaceDialog *view, Document *documentView) {
        this->view = view;
        this->documentView = documentView;

        view->attachPresenter(this);
    }


    void FindReplaceDialog::Presenter::handleEditFindReplace() {
        FindReplaceDialog::ViewData viewData;

        viewData.title = "Find/Replace in Document ...";
        viewData.findWhat = this->findWhat;
        viewData.matchCase = this->matchCase;
        viewData.matchWholeWorld = this->matchWholeWord;
        viewData.replaceWith = this->replaceWith;

        if (this->scope == Scope::CurrentDocument) {
            viewData.scope = FindReplaceDialog::CurrentDocument;
        } else if (this->scope == Scope::Selection) {
            viewData.scope = FindReplaceDialog::Selection;
        }

        view->show(viewData);
    }


    void FindReplaceDialog::Presenter::handleFindWhatTextBox_Change(const std::string &value) {
        findWhat = value;
    }


    void FindReplaceDialog::Presenter::handleReplaceWithCheckBox_Click(const bool checked) {
        replaceInsteadOfFind = checked;

        view->toggleReplaceControls(checked);
    }


    void FindReplaceDialog::Presenter::handleReplaceWithTextBox_Change(const std::string &value) {
        replaceWith = value;
    }


    void FindReplaceDialog::Presenter::handleMatchCaseCheckBox_Click(const bool checked) {
        matchCase = checked;
    }


    void FindReplaceDialog::Presenter::handleMatchWholeWordCheckBox_Click(const bool checked) {
        matchWholeWord = checked;
    }


    void FindReplaceDialog::Presenter::handleSelectionScopeOptionBox_Click() {
        scope = Selection;
    }


    void FindReplaceDialog::Presenter::handleCurrentDocumentScopeOptionBox_Click() {
        scope = CurrentDocument;
    }


    void FindReplaceDialog::Presenter::handleFindNextButton_Click() {
        const size_t currentOffset = documentView->getSelection().end;

        if (! findNext(currentOffset)) {
            findNext(0);
        }
    }


    bool FindReplaceDialog::Presenter::findNext(const size_t currentOffset) {
        const std::string content = this->documentView->getContent();
        const auto flags = FIND_FLAGS((matchCase ? FF_MATCH_CASE : FF_DEFAULT) | (matchWholeWord ? FF_MATCH_WHOLE_WORD : FF_DEFAULT));

        const std::size_t offset = find(content.c_str(), currentOffset, findWhat.c_str(), flags);

        if (offset == std::string::npos) {
            return false;
        } 

        const std::size_t length = findWhat.size();
        documentView->setSelection({(int)offset, (int)(offset + length)});
        
        return true;
    }


    void FindReplaceDialog::Presenter::handleReplaceNextButton_Click() {
        // TODO: Add implementation
        std::cout << "asdad" << std::endl;
    }


    void FindReplaceDialog::Presenter::handleReplaceAllButton_Click() {
        // TODO: Add implementation
        std::cout << "asdad" << std::endl;
    }


    void FindReplaceDialog::Presenter::handleCloseButton_Click() {
        // TODO: Add implementation
        std::cout << "asdad" << std::endl;
    }

}
