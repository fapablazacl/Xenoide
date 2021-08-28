
#include <Xenoide/Core/FolderService.h>

#include <boost/filesystem.hpp>


namespace Xenoide {
    std::vector<boost::filesystem::path> FolderService::listChildFolders(const boost::filesystem::path &folderPath) const {
        auto childPathVector = std::vector<boost::filesystem::path>{};

        auto subPathIterator = boost::filesystem::directory_iterator{folderPath};
        auto end = boost::filesystem::directory_iterator{};

        while (subPathIterator != end) {
            boost::filesystem::path subPath = subPathIterator->path();

            childPathVector.push_back(subPath);

            subPathIterator++;
        }

        return childPathVector;
    }
}
