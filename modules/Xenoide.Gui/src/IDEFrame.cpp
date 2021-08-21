
#include <Xenoide/Gui/IDEFrame.hpp>

#include <iostream>
#include <cassert>
#include <boost/filesystem.hpp>
#include <Xenoide/Core/FolderService.hpp>
#include <Xenoide/Gui/Menu.hpp>
#include <Xenoide/Gui/MenuPanel.hpp>
#include <Xenoide/Gui/FolderBrowser.hpp>


namespace Xenoide {
    IDEFrame::IDEFrame(IDEFrame::Presenter *presenter) {
        mPresenter = presenter;
    }

    IDEFrame::~IDEFrame() {}
}


namespace Xenoide {
    IDEFrame::Presenter::Presenter(IDEFrame::Model *model) {
        this->model = model;

        // initialize child presenters
        documentManagerPresenter = std::make_unique<DocumentManager::Presenter>(model->getDocumentManagerModel());
        folderBrowserPresenter = std::make_unique<FolderBrowser::Presenter>(model->getFolderBrowserModel());
    }

    IDEFrame::Presenter::~Presenter() {}

    void IDEFrame::Presenter::onInitialized(IDEFrame *view, DialogManager *dialogView, MenuPanel *menuView) {
        assert(view);
        
        this->view = view;
        this->dialogView = dialogView;
        this->menuView = menuView;

        const auto menu = Menu{Menu::menuBar({
            Menu::menu("&File", {
                Menu::action([this] () { this->onFileNew(); }, "&New", {Modifier::Ctrl, Key::N}),
                Menu::separator(),
                Menu::action([this] () { this->onFileOpen(); }, "&Open ...", {Modifier::Ctrl, Key::O}),
                Menu::action([this] () { this->onFileOpenFolder(); }, "Open &Folder ...", {Modifier::CtrlShift, Key::O}),
                Menu::separator(),
                Menu::action([this] () { this->onFileSave(); }, "&Save", {Modifier::Ctrl, Key::S}),
                Menu::action([this] () { this->onFileSaveAs(); }, "Sa&ve As ..."),
                Menu::action([this] () { this->onFileSaveAll(); }, "Save &All"),
                Menu::separator(),
                Menu::action([this] () { this->onFileClose(); }, "&Close"),
                Menu::separator(),
                Menu::action([this] () { this->onFileExit(); }, "&Exit")
            }),
            Menu::menu("&Edit", {
                Menu::action([this] () { this->onEditUndo(); }, "&Undo", {Modifier::Ctrl, Key::Z}),
                Menu::action([this] () { this->onEditRedo(); }, "&Redo", {Modifier::CtrlShift, Key::Z}),
                Menu::separator(),
                Menu::action([this] () { this->onEditCut(); }, "&Cut", {Modifier::Ctrl, Key::X}),
                Menu::action([this] () { this->onEditCopy(); }, "C&opy", {Modifier::Ctrl, Key::C}),
                Menu::action([this] () { this->onEditPaste(); }, "&Paste", {Modifier::Ctrl, Key::V}),
                Menu::separator(),
                Menu::action([] () {}, "Find ..."),
                Menu::action([] () {}, "Replace ..."),
            }),
            Menu::menu("&View", {
                Menu::action([this] () { this->onViewFolderBrowser(); }, "&Folder Browser")
            }),
            Menu::menu("&Tools", {
                Menu::action([this] () { this->onToolsFileSearch(); }, "File &Search ...", {Modifier::CtrlShift, Key::P})
            }),
            Menu::menu("&Build", {
                Menu::action([] () {}, "&Clean"),
                Menu::action([] () {}, "&Execute")
            }),
            Menu::menu("&Help", {
                Menu::action([] () {}, "&About", {Key::F1})
            }),
        })};

        // this->menuView->setupMenuBar(menu);
    }

    void IDEFrame::Presenter::onFileNew() {
        documentManagerPresenter->onNewDocument();
    }

    void IDEFrame::Presenter::onFileOpen() {
        auto fileDialog = FileDialogData{};
        fileDialog.title = "Open File";
        fileDialog.filters = model->getFileFilters();
        fileDialog.type = FileDialogType::OpenFile;

        if (auto filePath = dialogView->showFileDialog(fileDialog)) {
            documentManagerPresenter->onOpenDocument(filePath.get());
        }
    }
    
    void IDEFrame::Presenter::onFileOpen(const std::string &filePath) {
        documentManagerPresenter->onOpenDocument(filePath);
    }

    void IDEFrame::Presenter::onFileOpenFolder() {
        auto folderDialog = FolderDialogData{};
        folderDialog.title = "Open Folder";

        if (auto folderPath = dialogView->showFolderDialog(folderDialog)) {
            folderBrowserPresenter->onDisplayFolder(folderPath.get());
            model->setWorkspaceFolder(folderPath.get());
        }
    }

    void IDEFrame::Presenter::onToolsFileSearch() {
        if (auto workspaceFolder = model->getWorkspaceFolder()) {
            auto fileSearchDialog = FileSearchDialogData{};

            fileSearchDialog.title = "File Search";
            fileSearchDialog.defaultPath = workspaceFolder.get();

            if (auto filePath = dialogView->showFileSearchDialog(fileSearchDialog)) {
                documentManagerPresenter->onOpenDocument(filePath.get());
            }
        }
    }

    void IDEFrame::Presenter::onFileSave() {
        documentManagerPresenter->onSaveDocument();
    }

    void IDEFrame::Presenter::onFileSaveAs() {
        documentManagerPresenter->onSaveAsDocument();
    }
    
    void IDEFrame::Presenter::onFileSaveAll() {
        documentManagerPresenter->onSaveAllDocuments();
    }

    void IDEFrame::Presenter::onEditUndo() {
        // TODO: Add implementation
    }

    void IDEFrame::Presenter::onEditRedo() {
        // TODO: Add implementation
    }

    void IDEFrame::Presenter::onEditCut() {
        // TODO: Add implementation
    }

    void IDEFrame::Presenter::onEditCopy() {
        // TODO: Add implementation
    }

    void IDEFrame::Presenter::onEditPaste() {
        // TODO: Add implementation
    }

    void IDEFrame::Presenter::onFileClose() {
        documentManagerPresenter->onCloseCurrentDocument();
    }

    void IDEFrame::Presenter::onFileExit() {
        view->close();
    }

    bool IDEFrame::Presenter::onCloseRequested() {
        assert(this);
        assert(view->getDialogManager());

        auto messageDialog = MessageDialogData {};
        messageDialog.title = "Xenoide";
        messageDialog.message = "Exit?";
        messageDialog.icon = DialogIcon::Question;
        messageDialog.buttons = DialogButton::YesNo;

        const DialogButton button = dialogView->showMessageDialog(messageDialog);
        
        return button == DialogButton::Yes;
    }
    
    void IDEFrame::Presenter::onViewFolderBrowser() {
        view->showPanel(IDEFrame::FOLDER_BROWSER);
    }

    void IDEFrame::Presenter::openFolder(const std::string &fullPath) {
        view->getFolderBrowser()->displayFolder(fullPath);
    }

    DocumentManager::Presenter*  IDEFrame::Presenter::getDocumentManagerPresenter() {
        return documentManagerPresenter.get();
    }

    FolderBrowser::Presenter* IDEFrame::Presenter::getFolderBrowserPresenter() {
        return folderBrowserPresenter.get();
    }
}



namespace Xenoide {
    class IDEFrameModelImpl : public IDEFrame::Model {
    public:
        IDEFrameModelImpl() {
            this->folderService = FolderService::create();

            this->documentManagerModel = DocumentManager::Model::create();
            this->folderBrowserModel = FolderBrowser::Model::create(this->folderService.get());
        }

        virtual std::vector<FileFilter> getFileFilters() const override {
            return {
                {"All Files", {"*.*"}},
                {"C/C++ Files", {"*.hpp", "*.cpp", "*.hh", "*.cc", "*.h", "*.c"}},
            };
        }

        virtual DocumentManager::Model *getDocumentManagerModel() override {
            return documentManagerModel.get();
        }

        virtual FolderBrowser::Model *getFolderBrowserModel() override {
            return folderBrowserModel.get();
        }

        virtual boost::optional<boost::filesystem::path> getWorkspaceFolder() const override {
            return this->workspaceFolder;
        }

        virtual void setWorkspaceFolder(boost::filesystem::path workspaceFolder) override {
            std::cout << "setWorkspaceFolder(" << workspaceFolder.string() << ")" << std::endl;
            this->workspaceFolder = workspaceFolder;
        }

    private:
        std::unique_ptr<FolderService> folderService;
        std::unique_ptr<DocumentManager::Model> documentManagerModel;
        std::unique_ptr<FolderBrowser::Model> folderBrowserModel;
        boost::optional<boost::filesystem::path> workspaceFolder;
    };

    IDEFrame::Model::~Model() {}

    std::unique_ptr<IDEFrame::Model> IDEFrame::Model::create() {
        return std::make_unique<IDEFrameModelImpl>();
    }
}
