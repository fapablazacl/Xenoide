
#ifndef __XENOIDE_UI_DOCUMENT_HPP__
#define __XENOIDE_UI_DOCUMENT_HPP__

#include <string>
#include <memory>
#include <boost/filesystem/path.hpp>
#include <xeno/core/Predef.h>

namespace Xenoide {
    struct DocumentConfig {
        std::string fontName;
        int fontSize;
        int tabWidth;
        bool caretLineVisible;
        bool showLineNumbers;

        static DocumentConfig Default();
    };

    struct TextSelection {
        int start;
        int end;

        static TextSelection all() {
            return {-1, -1};
        }
    };

    class DialogManager;
    class Document {
    public:
        class Model {
        public:
            virtual ~Model();

            virtual int getTag() const = 0;

            virtual int getId() const = 0;

            virtual void setModifiedFlag(const bool value) = 0;

            virtual bool getModifiedFlag() const = 0;

            virtual void modify() = 0;

            virtual void setFilePath(const std::string &value) = 0;

            virtual std::string getFilePath() const = 0;

            virtual bool hasFilePath() const = 0;

            virtual void setContent(const std::string &value) = 0;

            virtual std::string getContent() const = 0;

        public:
            static int getCount();

            static std::unique_ptr<Model> create(int tag);

            static std::unique_ptr<Model> create(const std::string &filePath);

            static std::unique_ptr<Model> create(const std::string &filePath, const std::string &content);

        protected:
            static int count;
        };


        class Presenter {
        public:
            enum class UserResponse {
                Cancel,
                Accept
            };

            Presenter(Model *model);

            ~Presenter();

            void onInitialized(Document *view, DialogManager *dialogView);

            void onContentChanged();

            void onTitleChanged();

            UserResponse onSave();

            UserResponse onSaveAs();

            UserResponse onCloseRequested();

            bool hasFilePath(const boost::filesystem::path &filePath) const;

            Document* getView() const;

            Model* getModel() const;

        private:
            std::string computeFileTitle(Model *model) const;

            std::string computeTitle(Model *model) const;

        private:
            DialogManager *dialogView = nullptr;
            Document *view = nullptr;
            Model *model = nullptr;
        };

    public:
        virtual ~Document();

        virtual void setTitle(const std::string &title) = 0;
        virtual std::string getTitle() const = 0;

        virtual void setContent(const std::string &content) = 0;
        virtual std::string getContent() const = 0;

        virtual void setConfig(const DocumentConfig &config) = 0;
        virtual DocumentConfig getConfig() const = 0;

        virtual void clearAll() = 0;

        virtual void clearUndoBuffer() = 0;

        virtual void undo() = 0;
        virtual void redo() = 0;

        virtual void cut() = 0;
        virtual void copy() = 0;
        virtual void paste() = 0;
        
        virtual void setSelection(const TextSelection &selection) = 0;

        virtual void selectAll() = 0;

        virtual void clearSelection() = 0;

        virtual TextSelection getSelection() const = 0;
    };    
}

#endif
