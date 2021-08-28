
#pragma once

#include <memory>
#include <string>
#include <map>
#include <boost/filesystem/path.hpp>

namespace Xenoide {
    class WorkspaceService {
    public:
        explicit WorkspaceService(boost::filesystem::path basePath) : mPath(basePath) {}

        virtual ~WorkspaceService() {};

        virtual std::vector<boost::filesystem::path> listChildFolders(const boost::filesystem::path &folderPath);

    protected:
        //! base workspace path
        boost::filesystem::path mPath;
    };
}
