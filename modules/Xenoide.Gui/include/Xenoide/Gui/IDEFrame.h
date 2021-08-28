
#ifndef __XENOIDE_UI_MAINWINDOWVIEW_HPP__
#define __XENOIDE_UI_MAINWINDOWVIEW_HPP__

#include <Xenoide/Core/Predef.h>
#include <Xenoide/Gui/FileFilter.h>
#include <Xenoide/Gui/DialogManager.h>
#include <Xenoide/Gui/Document.h>
#include <Xenoide/Gui/DocumentManager.h>
#include <Xenoide/Gui/FolderBrowser.h>
#include <boost/optional/optional.hpp>
#include <boost/filesystem/path.hpp>

namespace Xenoide {
    class DocumentManager;
    class DialogManager;
    class FolderBrowser;
    class MenuPanel;

    class IDEFrame {
    public:
        class Model {
        public:
            virtual ~Model();

            virtual std::vector<FileFilter> getFileFilters() const = 0;

            virtual DocumentManager::Model *getDocumentManagerModel() = 0;

            virtual FolderBrowser::Model *getFolderBrowserModel() = 0;

            virtual boost::optional<boost::filesystem::path> getWorkspaceFolder() const = 0;

            virtual void setWorkspaceFolder(boost::filesystem::path workspaceFolder) = 0;

        public:
            static std::unique_ptr<Model> create();
        };


        class Presenter {
        public:
            Presenter(Model *model);

            virtual ~Presenter();

            void onInitialized(IDEFrame *view, DialogManager *dialogView, MenuPanel *menuView);

        public:
            void onFileNew();

            void onFileOpen();
        
            void onFileOpen(const std::string &fileName);

            void onFileOpenFolder();

            void onFileSave();

            void onFileSaveAs();

            void onFileSaveAll();

            void onFileClose();

            void onFileExit();

            void onEditUndo();

            void onEditRedo();

            void onEditCut();

            void onEditCopy();

            void onEditPaste();

            void onViewFolderBrowser();

            void onToolsFileSearch();

            bool onCloseRequested();

        public:
            DocumentManager::Presenter* getDocumentManagerPresenter();

            FolderBrowser::Presenter* getFolderBrowserPresenter();

            // TODO: Make it private (used in Main.cpp)
            void openFolder(const std::string &fullPath);

        private:
            DialogManager *dialogView = nullptr;
            MenuPanel *menuView = nullptr;
            IDEFrame *view = nullptr;
            Model *model = nullptr;
        
            std::unique_ptr<DocumentManager::Presenter> documentManagerPresenter;
            std::unique_ptr<FolderBrowser::Presenter> folderBrowserPresenter;
        };

    public:
        enum Panel {
            FOLDER_BROWSER
        };

    public:
        explicit IDEFrame(Presenter *presenter);

        virtual ~IDEFrame();

        virtual DocumentManager* getDocumentManager() = 0;

        virtual DialogManager* getDialogManager() = 0;
        
        virtual FolderBrowser* getFolderBrowser() = 0;

        virtual void close() = 0;

        virtual void show() = 0;

        virtual void showPanel(const Panel panel) = 0;

    protected:
        Presenter *mPresenter;
    };
}

#endif
