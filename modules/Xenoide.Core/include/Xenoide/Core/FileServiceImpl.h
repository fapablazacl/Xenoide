
#pragma once

#include "FileService.h"

namespace Xenoide {
    class FileServiceImpl : public FileService {
    public:
        virtual std::string load(const boost::filesystem::path &filePath) override;

        virtual void save(const boost::filesystem::path &filePath, const std::string &content) override;

        virtual void touch(const boost::filesystem::path &filePath) override ;
    };
}
