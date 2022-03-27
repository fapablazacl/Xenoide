
#include <xeno/ui/FileSearchDialog.h>

#include <iostream>
#include <cassert>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>


namespace Xenoide {
    FileSearchDialog::FileSearchDialog(FileSearchDialog::Presenter *presenter) {
        this->presenter = presenter;
    }

    FileSearchDialog::~FileSearchDialog() {}
}


namespace Xenoide {
    static bool is_logically_hidden(const boost::filesystem::path &path) {
        const std::string title = path.filename().string();

        std::cout << path.string() << " isHidden: " << (title[0] == '.') << std::endl;

        return title[0] == '.';
    }

    class FileSearchDialogModelImpl : public FileSearchDialog::Model {
    public:
        FileSearchDialogModelImpl(const boost::filesystem::path &basePath) {
            this->basePath = basePath;
        }

        virtual ~FileSearchDialogModelImpl() {}

        virtual std::vector<boost::filesystem::path> searchFilePattern(const std::string &filePattern, const int maxResults) override {
            std::string filePatternUppercased = filePattern;
            boost::to_upper(filePatternUppercased);

            return this->search(filePatternUppercased, maxResults);
        }

    private:
        bool testFile(const boost::filesystem::path &filePath, const std::string &filePattern) const {
            std::string testAgaint = filePath.filename().string();
            boost::to_upper(testAgaint);

            return testAgaint.find(filePattern) != std::string::npos;
        }

        std::vector<boost::filesystem::path> search(const std::string &filePattern, const int maxResults) const {
            std::vector<boost::filesystem::path> files;

            this->searchImpl(files, basePath, filePattern, maxResults);

            return files;
        }

        void searchImpl(std::vector<boost::filesystem::path> &files, const boost::filesystem::path &folder, const std::string &filePattern, const int maxResults) const {
            // skips search inside files ...
            if (! boost::filesystem::is_directory(folder)) {
                return;
            }

            // skips hidden directories
            if ( boost::filesystem::is_directory(folder) && is_logically_hidden(folder)) {
                return;
            }

            boost::filesystem::directory_iterator subPathIterator(folder);
            boost::filesystem::directory_iterator end;

            while (subPathIterator != end) {
                boost::filesystem::path subPath = subPathIterator->path();

                if (this->testFile(subPath, filePattern)) {
                    files.push_back(subPath);
                }

                if (files.size() + 1 >= maxResults) {
                    return;
                }

                this->searchImpl(files, subPath, filePattern, maxResults);

                subPathIterator++;
            }
        }

    private:
        boost::filesystem::path basePath;
        std::vector<boost::filesystem::path> files;
    };

    FileSearchDialog::Model::~Model() {}

    std::unique_ptr<FileSearchDialog::Model> FileSearchDialog::Model::create(const boost::filesystem::path &basePath) {
        return std::make_unique<FileSearchDialogModelImpl>(basePath);
    }
}


namespace Xenoide {
    FileSearchDialog::Presenter::Presenter(FileSearchDialog::Model *model) {
        this->model = model;
    }

    void FileSearchDialog::Presenter::onInitialized(FileSearchDialog *view) {
        this->view = view;
    }

    void FileSearchDialog::Presenter::onAccepted(const std::string &filePath) {
        assert(view);

        view->hide();
    }

    void FileSearchDialog::Presenter::onCancelled() {
        assert(view);

        view->hide();
    }

    void FileSearchDialog::Presenter::onFilenameFilterRequested(const std::string &filePattern) {
        assert(view);

        view->clearFileList();

        if (filePattern.size() < 3) {
            return;
        }

        const std::vector<boost::filesystem::path> files = model->searchFilePattern(filePattern, 20);

        std::vector<FileSearchDialog::FileViewData> fileList;

        for (const auto &file : files) {
            FileSearchDialog::FileViewData fileViewData;

            fileViewData.fileTitle = file.filename().string();
            fileViewData.fileFolder = file.parent_path().string();
            fileViewData.filePath = file.string();

            fileList.push_back(fileViewData);
        }

        view->displayFileList(fileList);
    }
}
