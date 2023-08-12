
#pragma once 

#ifndef __XENOIDE_UI_SEARCHREPLACE_DIALOG_HPP__
#define __XENOIDE_UI_SEARCHREPLACE_DIALOG_HPP__

#include <string>
#include <optional>

namespace Xenoide {
    class Document;
    class FindReplaceDialog {
    public:
        class Presenter {
        public:
            void attachView(FindReplaceDialog *view, Document *documentView);

            void handleEditFindReplace();

            void handleFindWhatTextBox_Change(const std::string &value);

            void handleReplaceWithCheckBox_Click(const bool checked);

            void handleReplaceWithTextBox_Change(const std::string &value);

            void handleMatchCaseCheckBox_Click(const bool checked);

            void handleMatchWholeWordCheckBox_Click(const bool checked);

            void handleSelectionScopeOptionBox_Click();

            void handleCurrentDocumentScopeOptionBox_Click();

            void handleFindNextButton_Click();

            void handleReplaceNextButton_Click();

            void handleReplaceAllButton_Click();

            void handleCloseButton_Click();

        private:
            bool findNext(const size_t offset);

        private:
            FindReplaceDialog *view = nullptr;
            Document *documentView = nullptr;

            std::string findWhat;
            std::string replaceWith;
            bool replaceInsteadOfFind = false;
            bool matchCase = false;
            bool matchWholeWord = false;

            enum Scope {
                Selection, CurrentDocument
            } scope = CurrentDocument;
        };


    public:
        enum SearchScope {
            Selection = 0,
            CurrentDocument = 1
        };

        struct ViewData {
            std::string title;
            std::string findWhat;
            std::optional<std::string> replaceWith;

            bool matchCase = false;
            bool matchWholeWorld = false;

            SearchScope scope = CurrentDocument;
        };

    public:
        virtual ~FindReplaceDialog();

        virtual void attachPresenter(Presenter *presenter) = 0;

        virtual void show(const ViewData &viewData) = 0;

        virtual void hide() = 0;

        virtual void toggleReplaceControls(const bool status) = 0;
    };
}

#endif
