
#include <xeno/ui/DocumentManager.h>

#include <list>
#include <algorithm>
#include <iostream>
#include <xeno/ui/DialogManager.h>


namespace Xenoide {
    DocumentManager::DocumentManager(DocumentManager::Presenter *presenter) {
        this->presenter = presenter;
    }

    DocumentManager::~DocumentManager() {}

    std::vector<Document*> DocumentManager::enumerateDocuments() {
        std::vector<Document*> editors;

        editors.reserve(this->getDocumentCount());

        for (int i=0; i<this->getDocumentCount(); i++) {
            editors.push_back(this->getDocument(i));
        }

        return editors;
    }

    void DocumentManager::closeDocuments(const std::vector<Document*> &documents) {
        for (Document *document : documents) {
            this->closeDocument(document);
        }
    }
}


namespace Xenoide {
    class DocumentManagerModelImpl : public DocumentManager::Model {
    public:
        virtual ~DocumentManagerModelImpl() {}

        virtual Document::Model* createDocument() override {
            auto document = Document::Model::create(++createdDocumentCount);
            auto documentPtr = document.get();

            documents.push_back(std::move(document));

            return documentPtr;
        }

        virtual Document::Model* createDocument(const boost::filesystem::path &filePath) override {
            auto document = this->createDocument();
            document->setFilePath(filePath.string());

            return document;
        }

        virtual void closeDocument(Document::Model *documentModel) override {
            auto documentIt = std::find_if(documents.begin(), documents.end(), [documentModel](auto &document) {
                return document.get() == documentModel;
            });

            if (documentIt != documents.end()) {
                documents.erase(documentIt);
            }
        }

        virtual std::vector<Document::Model*> enumerateDocuments() const override {
            std::vector<Document::Model*> result;

            result.reserve(documents.size());

            for (auto &document : documents) {
                result.push_back(document.get());
            }

            return result;
        }

    private:
        std::list<std::unique_ptr<Document::Model>> documents;
        int createdDocumentCount = 0;
    };

    DocumentManager::Model::~Model() {}

    std::unique_ptr<DocumentManager::Model> DocumentManager::Model::create() {
        return std::make_unique<DocumentManagerModelImpl>();
    }
}


namespace Xenoide {
    DocumentManager::Presenter::Presenter(DocumentManager::Model *model) {
        this->model = model;
    }

    DocumentManager::Presenter::~Presenter() {}

    void DocumentManager::Presenter::onInitialized(DocumentManager *view, DialogManager *dialogView) {
        this->view = view;
        this->dialogView = dialogView;
    }

    void DocumentManager::Presenter::onNewDocument() {
        this->createDocumentMVP();
    }

    void DocumentManager::Presenter::onOpenDocument(const boost::filesystem::path &filePath) {
        Document::Presenter *documentPresenter;

        if (documentPresenter = this->findDocumentPresenter(filePath)) {
            view->setCurrentDocument(documentPresenter->getView());
        } else {
            this->createDocumentMVP(filePath);
        }
    }

    void DocumentManager::Presenter::onSaveDocument() {
        if (auto document = view->getCurrentDocument()) {
            if (auto documentPresenter = this->findDocumentPresenter(document)) {
                documentPresenter->onSave();
            }
        }
    }

    void DocumentManager::Presenter::onSaveAllDocuments() {
        auto documentViews = view->enumerateDocuments();

        for (auto documentView : documentViews) {
            if (auto documentPresenter = this->findDocumentPresenter(documentView)) {
                if (documentPresenter->onSave() == Document::Presenter::UserResponse::Cancel) {
                    break;
                }
            }
        }
    }

    void DocumentManager::Presenter::onSaveAsDocument() {
        if (auto document = view->getCurrentDocument()) {
            if (auto documentPresenter = this->findDocumentPresenter(document)) {
                documentPresenter->onSaveAs();
            }
        }
    }

    void DocumentManager::Presenter::onCloseCurrentDocument() {
        this->onCloseDocument(this->view->getCurrentDocument());
    }

    void DocumentManager::Presenter::onCloseDocument(Document *document) {
        std::cout << "DocumentManager::Presenter::onCloseDocument" << std::endl;

        if (auto documentPresenter = this->findDocumentPresenter(document)) {
            if (documentPresenter->onCloseRequested() == Document::Presenter::UserResponse::Accept) {
                this->closeDocumentMVP(documentPresenter);
            }
        }
    }

    void DocumentManager::Presenter::onCloseOtherDocuments(Document *document) {
        auto documents = view->enumerateDocuments();

        for (auto current : documents) {
            if (current == document) {
                continue;
            }

            if (auto documentPresenter = this->findDocumentPresenter(current)) {
                if (documentPresenter->onCloseRequested() == Document::Presenter::UserResponse::Accept) {
                    this->closeDocumentMVP(documentPresenter);
                }
            }
        }
    }

    void DocumentManager::Presenter::onCloseDocumentsToTheRight(Document *document) {
        auto documents = view->enumerateDocuments();
        bool close = false;

        for (auto current : documents) {
            if (close) {
                if (auto documentPresenter = this->findDocumentPresenter(current)) {
                    if (documentPresenter->onCloseRequested() == Document::Presenter::UserResponse::Accept) {
                        this->closeDocumentMVP(documentPresenter);
                    }
                }
            }

            if (current == document) {
                close = true;
            }
        }
    }

    void DocumentManager::Presenter::onCloseAllDocuments() {
        auto documents = view->enumerateDocuments();

        for (auto document : documents) {
            if (auto documentPresenter = this->findDocumentPresenter(document)) {
                if (documentPresenter->onCloseRequested() == Document::Presenter::UserResponse::Accept) {
                    this->closeDocumentMVP(documentPresenter);
                }
            }
        }
    }

    Document::Presenter* DocumentManager::Presenter::createDocumentMVP() {
        return this->createDocumentMVP("");
    }

    Document::Presenter* DocumentManager::Presenter::createDocumentMVP(const boost::filesystem::path &filePath) {
        auto documentModel = model->createDocument(filePath);
        auto documentPresenter = new Document::Presenter(documentModel);
        auto documentView = view->appendDocument(documentPresenter);

        documentPresenters.emplace_back(documentPresenter);

        return documentPresenter;
    }

    Document::Presenter* DocumentManager::Presenter::findDocumentPresenter(Document *document) {
        auto &dps = documentPresenters;
        auto documentPresenterIt = std::find_if(dps.begin(), dps.end(), [document](std::unique_ptr<Document::Presenter> &dp) {
            return dp->getView() == document;
        });

        if (documentPresenterIt != dps.end()) {
            return documentPresenterIt->get();
        }

        return nullptr;
    }
    
    Document::Presenter* DocumentManager::Presenter::findDocumentPresenter(const boost::filesystem::path &filePath) {
        auto &dps = documentPresenters;
        auto documentPresenterIt = std::find_if(dps.begin(), dps.end(), [filePath](std::unique_ptr<Document::Presenter> &dp) {
            return dp->hasFilePath(filePath);
        });

        if (documentPresenterIt != dps.end()) {
            return documentPresenterIt->get();
        }

        return nullptr;
    }

    void DocumentManager::Presenter::closeDocumentPresenter(Document::Presenter *documentPresenter) {
        auto &dps = documentPresenters;
        auto documentPresenterIt = std::find_if(dps.begin(), dps.end(), [documentPresenter](std::unique_ptr<Document::Presenter> &dp) {
            return dp.get() == documentPresenter;
        });

        documentPresenters.erase(documentPresenterIt);
    }

    void DocumentManager::Presenter::closeDocumentMVP(Document::Presenter *documentPresenter) {
        std::cout << "DocumentManagerPresenter::closeDocumentMVP" << std::endl;

        view->closeDocument(documentPresenter->getView());
        model->closeDocument(documentPresenter->getModel());

        this->closeDocumentPresenter(documentPresenter);
    }
}
