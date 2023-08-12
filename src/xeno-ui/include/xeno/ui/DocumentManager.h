
#ifndef __XENOIDE_UI_DOCUMENTMANAGER_HPP__
#define __XENOIDE_UI_DOCUMENTMANAGER_HPP__

#include <cstddef>
#include <vector>
#include <map>
#include <tuple>
#include <list>
#include <xeno/ui/Document.h>

namespace Xenoide {
    class DocumentManager {
    public:
        class Model {
        public:
            virtual ~Model();

            virtual Document::Model* createDocument() = 0;

            virtual Document::Model* createDocument(const boost::filesystem::path &filePath) = 0;

            virtual void closeDocument(Document::Model *documentModel) = 0;

            virtual std::vector<Document::Model*> enumerateDocuments() const = 0;

        public:
            static std::unique_ptr<Model> create();
        };

        class Presenter {
        public:
            explicit Presenter(Model *model);

            ~Presenter();

            void onInitialized(DocumentManager *view, DialogManager *dialogView);

            void onNewDocument();

            void onOpenDocument(const boost::filesystem::path &path);

            void onSaveDocument();

            void onSaveAsDocument();

            void onSaveAllDocuments();

            void onCloseCurrentDocument();

            void onCloseDocument(Document *document);

            void onCloseOtherDocuments(Document *document);

            void onCloseDocumentsToTheRight(Document *document);

            void onCloseAllDocuments();

        private:
            Document::Presenter* createDocumentMVP();

            Document::Presenter* createDocumentMVP(const boost::filesystem::path &filePath);

            Document::Presenter* findDocumentPresenter(Document *document);

            Document::Presenter* findDocumentPresenter(const boost::filesystem::path &filePath);

            void closeDocumentPresenter(Document::Presenter *documentPresenter);

            void closeDocumentMVP(Document::Presenter *documentPresenter);

        private:
            DialogManager *dialogView = nullptr;
            DocumentManager *view = nullptr;
            DocumentManager::Model *model = nullptr;

            std::list<std::unique_ptr<Document::Presenter>> documentPresenters;
        };


    public:
        explicit DocumentManager(Presenter *presenter);

        virtual ~DocumentManager();

        virtual Document* appendDocument(Document::Presenter *presenter) = 0;

        virtual Document* getCurrentDocument() = 0;

        virtual void setCurrentDocument(Document *document) = 0;

        virtual std::size_t getDocumentCount() const = 0;

        virtual Document* getDocument(const std::size_t index) = 0;
        
        virtual void closeDocument(Document *editor) = 0;

        virtual void showDocument(Document *editor) = 0;

        virtual void closeDocuments(const std::vector<Document*> &documents);

        std::vector<Document*> enumerateDocuments();

    protected:
        Presenter *presenter = nullptr;
    };
} 

#endif
