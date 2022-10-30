
#pragma once 

#include <vector>
#include <string>
#include <initializer_list>
#include <boost/filesystem/path.hpp>


class FileNamePatternMatcher {
public:
    FileNamePatternMatcher() {}

    FileNamePatternMatcher(const std::vector<std::string> &patterns) : patterns{patterns} {}

    FileNamePatternMatcher(const std::initializer_list<std::string> &patterns) : patterns{patterns} {}

    bool match(const boost::filesystem::path &filePath) const;

private:
    std::vector<std::string> patterns;
};
