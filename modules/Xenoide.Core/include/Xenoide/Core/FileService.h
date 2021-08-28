
#ifndef __XENOIDE_UI_FILESERVICE_HPP__
#define __XENOIDE_UI_FILESERVICE_HPP__

#include <memory>
#include <string>
#include <boost/filesystem/path.hpp>

namespace Xenoide {
    class FileService {
    public:
        virtual ~FileService() {}

        virtual std::string load(const boost::filesystem::path &filePath) = 0;

        virtual void save(const boost::filesystem::path &filePath, const std::string &content) = 0;

        virtual void touch(const boost::filesystem::path &filePath) = 0;
    };
}

#endif
