
#include <xeno/ui/Document.h>

#include <xenoide/core/OS.h>
#include <xenoide/core/FileService.h>
#include <xeno/ui/DialogManager.h>

#include <boost/filesystem/path.hpp>


namespace Xenoide {
    DocumentConfig DocumentConfig::Default() {
        switch (getCurrentOS()) {
            case OS::Linux: return {"Monospace", 10, 4, true, true};
            case OS::Windows: return {"Consolas", 10, 4, true, true};
            default: return {"Consolas", 10, 4, true, true};
        }
    }

    Document::~Document() {}
}


namespace Xenoide {
    class DocumentModelImpl : public Document::Model {
    public:
        DocumentModelImpl(int tag) {
            id = ++count;

            this->tag = tag;
        }

        DocumentModelImpl(const std::string &filePath) {
            id = ++count;
        
            this->setFilePath(filePath);
        }

        DocumentModelImpl(const std::string &filePath, const std::string &content) {
            id = ++count;

            this->setFilePath(filePath);
            this->setContent(content);
        }

        virtual ~DocumentModelImpl() {}

        int getTag() const {
            return tag;
        }

        int getId() const {
            return id;
        }

        void setModifiedFlag(const bool value) {
            modified = value;
        }

        bool getModifiedFlag() const {
            return modified;
        }

        void modify() {
            modified = true;
        }

        void setFilePath(const std::string &value) {
            filePath = value;
        }

        std::string getFilePath() const {
            return filePath;
        }

        bool hasFilePath() const {
            return filePath != "";
        }

        void setContent(const std::string &value) {
            content = value;
        }

        std::string getContent() const {
            return content;        
        }

    private:
        int tag = 0;
        int id = 0;
        bool modified = false;
        std::string filePath;
        std::string content;
    };

    int Document::Model::count = 0;

    Document::Model::~Model() {}

    int Document::Model::getCount() {
        return Document::Model::count;
    }

    std::unique_ptr<Document::Model> Document::Model::create(int tag) {
        return std::make_unique<DocumentModelImpl>(tag);
    }

    std::unique_ptr<Document::Model> Document::Model::create(const std::string &filePath) {
        return std::make_unique<DocumentModelImpl>(filePath);
    }

    std::unique_ptr<Document::Model> Document::Model::create(const std::string &filePath, const std::string &content) {
        return std::make_unique<DocumentModelImpl>(filePath, content);
    }
}


namespace Xenoide {
    Document::Presenter::Presenter(Document::Model *model) {
        this->model = model;
    }


    Document::Presenter::~Presenter() {}


    void Document::Presenter::onInitialized(Document *view, DialogManager *dialogView) {
        this->view = view;
        this->dialogView = dialogView;

        if (model->hasFilePath()) {
            auto fileService = FileService();
            auto content = fileService.load(model->getFilePath());

            model->setContent(content);
        }

        const std::string title = this->computeTitle(model);
        view->setTitle(title);
        view->setContent(model->getContent());
        view->setConfig(DocumentConfig::Default());
    }


    void Document::Presenter::onContentChanged() {
        model->modify();

        const std::string title = this->computeTitle(model);
        view->setTitle(title);
    }


    Document::Presenter::UserResponse Document::Presenter::onSave() {
        // TODO: Put FileFilter getters from a PresenterService

        if (!model->hasFilePath()) {
            auto fileDialog = FileDialogData {};
            fileDialog.title = "Save File";
            fileDialog.type = FileDialogType::SaveFile;
            fileDialog.defaultPath = this->computeFileTitle(model);
            fileDialog.filters = {
                FileFilter{"All Files", {"*"}}
            };

            if (auto filePath = dialogView->showFileDialog(fileDialog)) {
                model->setFilePath(filePath.get().string());
            } else {
                return Document::Presenter::UserResponse::Cancel;
            }
        }

        auto fileService = FileService();

        model->setContent(view->getContent());

        const std::string fileName = model->getFilePath();
        const std::string content = model->getContent();
        
        fileService.save(fileName, content);

        model->setFilePath(fileName);
        model->setModifiedFlag(false);

        const std::string title = this->computeTitle(model);

        view->setTitle(title);

        return Document::Presenter::UserResponse::Accept;
    }


    Document::Presenter::UserResponse Document::Presenter::onSaveAs() {
        // TODO: Setup a PresenterService to support the UI
        auto fileDialog = FileDialogData {};

        fileDialog.title = "Save File";
        fileDialog.type = FileDialogType::SaveFile;
        fileDialog.filters = {
            FileFilter{"All Files", {"*"}}
        };

        if (auto filePath = dialogView->showFileDialog(fileDialog)) {
            model->setFilePath(filePath.get().string());

            this->onSave();

            return Document::Presenter::UserResponse::Accept;
        }

        return Document::Presenter::UserResponse::Cancel;
    }


    Document::Presenter::UserResponse Document::Presenter::onCloseRequested() {
        if (!model->getModifiedFlag()) {
            return Document::Presenter::UserResponse::Accept;
        }

        auto messageDialog = MessageDialogData{};
        messageDialog.title = "Xenoide";
        messageDialog.message = "The file has unsaved changes. Save them before closing?";
        messageDialog.icon = DialogIcon::Question;
        messageDialog.buttons = DialogButton::YesNoCancel;

        auto dialogResult = dialogView->showMessageDialog(messageDialog);

        switch (dialogResult) {
            case DialogButton::Cancel:
                return Document::Presenter::UserResponse::Cancel;

            case DialogButton::Yes:
                return this->onSave();

            case DialogButton::No:
                return Document::Presenter::UserResponse::Accept;
        }

        return Document::Presenter::UserResponse::Cancel;
    }

    void Document::Presenter::onTitleChanged() {
        const std::string title = this->computeTitle(model);
        view->setTitle(title);
    }

    std::string Document::Presenter::computeFileTitle(Document::Model *model) const {
        if (model->hasFilePath()) {
            return boost::filesystem::path(model->getFilePath()).filename().string();
        }

        return "Untitled " + std::to_string(model->getTag());
    }

    std::string Document::Presenter::computeTitle(Document::Model *model) const {
        const std::string prefix = (model->getModifiedFlag() ? "[*]" : "");
        const std::string fileTitle = this->computeFileTitle(model);

        return prefix + fileTitle;
    }

    bool Document::Presenter::hasFilePath(const boost::filesystem::path &filePath) const {
        return boost::filesystem::path(model->getFilePath()) == filePath;
    }
    
    Document* Document::Presenter::getView() const {
        return view;
    }

    Document::Model* Document::Presenter::getModel() const {
        return model;
    }
}
