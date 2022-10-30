
#include "FileNamePatternMatcher.h"

bool FileNamePatternMatcher::match(const boost::filesystem::path &filePath) const {
    if (! filePath.has_extension()) {
        return false;
    }
        
    const auto fileExtension = filePath.extension();

    for (const auto &pattern : patterns) {
        if (fileExtension == pattern) {
            return true;
        }
    }

    return false;
}
