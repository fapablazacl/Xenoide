
#ifndef __XENOIDE_UI_FOLDERSERVICE_HPP__
#define __XENOIDE_UI_FOLDERSERVICE_HPP__

#include <memory>
#include <string>
#include <boost/filesystem/path.hpp>

namespace Xenoide {
    class FolderService {
    public:
        virtual ~FolderService() {}

        virtual std::vector<boost::filesystem::path> listChildFolders(const boost::filesystem::path &folderPath) const;
    };
}

#endif
