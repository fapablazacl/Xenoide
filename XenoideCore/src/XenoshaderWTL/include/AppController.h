
#pragma once 

#include <boost/filesystem/path.hpp>

class AppController {
public:
    virtual ~AppController() {}

    virtual void openFile(const boost::filesystem::path& filePath) = 0;
};
