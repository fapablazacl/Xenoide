
#ifndef __XENOIDE_UI_FILESEARCHDIALOG_HPP__
#define __XENOIDE_UI_FILESEARCHDIALOG_HPP__

#include <string>
#include <vector>
#include <memory>
#include <boost/filesystem/path.hpp>

namespace Xenoide {
    class FileSearchDialog {
    public:
        class Model {
        public:
            virtual ~Model();

            virtual std::vector<boost::filesystem::path> searchFilePattern(const std::string &filePattern, const int maxResults) = 0;

        public:
            static std::unique_ptr<Model> create(const boost::filesystem::path &basePath);
        };


    public:
        class Presenter {
        public:
            Presenter(Model *model);

            void onInitialized(FileSearchDialog *view);

            void onAccepted(const std::string &filePath);

            void onCancelled();

            void onFilenameFilterRequested(const std::string &fileNamePart);

        private:
            Model *model = nullptr;
            FileSearchDialog *view = nullptr;
        };


    public:
        struct FileViewData {
            //! File to be shown to the User
            std::string fileTitle;
            
            //! Parent folder
            std::string fileFolder;

            //! Full file path
            std::string filePath;
        };

    public:
        explicit FileSearchDialog(Presenter *presenter);

        virtual ~FileSearchDialog();

        virtual void clearFileList() = 0;

        virtual void displayFileList(const std::vector<FileViewData> &files) = 0;

        virtual void hide() = 0;
        
    protected:
        Presenter *presenter = nullptr;
    };
}

#endif
