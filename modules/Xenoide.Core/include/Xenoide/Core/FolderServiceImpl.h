
#include "FolderService.h"

#include <boost/filesystem.hpp>

namespace Xenoide {
    class FolderServiceImpl : public FolderService {
    public:
        std::vector<boost::filesystem::path> listChildFolders(const boost::filesystem::path &folderPath) const override;
    };
}
