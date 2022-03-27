
#ifndef __XENOIDE_UI_FILEFILTER_HPP__
#define __XENOIDE_UI_FILEFILTER_HPP__

#include <string>
#include <vector>

namespace Xenoide {
    struct FileFilter {
        std::string description;
        std::vector<std::string> wildcards;

        ~FileFilter();
    };
} 

#endif
