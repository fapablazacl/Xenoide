
#pragma once 

#include <string>

namespace Xenoide {
    class MainFrame final {
    public:
        MainFrame() = delete;

        enum Window {
            W_OUTPUT = 0,
            W_FOLDERBROWSER
        };

        enum VisibleState {
            VS_SHOW,
            VS_HIDE
        };

        enum AskResult {
            AR_OK,
            AR_CANCEL
        };

        class Presenter;
        class View {
        public:
            explicit View(Presenter *presenter);

            virtual ~View() {}

            virtual void displayTitle(const std::string &value) = 0;

            virtual void changeWindowState(const Window window, const VisibleState state) = 0;

            virtual void show() = 0;

            virtual AskResult askCloseConfirmation(const std::string &title, const std::string &prompt) = 0;

        protected:
            Presenter *presenter = nullptr;
        };

        class Model {
        public:
            virtual ~Model() {}

            virtual std::string getTitle() const = 0;

            virtual bool hasModifiedFiles() const = 0;
        };

        enum CloseRequestAction {
            CRA_CLOSE,
            CRA_IGNORE
        };

        class Presenter {
        public:
            explicit Presenter(MainFrame::Model *model);

            void handleInitializedView(MainFrame::View *view);

            CloseRequestAction handleCloseRequest();

        private:
            View *view = nullptr;
            Model *model = nullptr;
        };
    };
}
