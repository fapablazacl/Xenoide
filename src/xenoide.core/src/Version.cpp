
#include <xenoide/core/Version.h>

#include <cassert>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <boost/algorithm/string.hpp>

namespace Xenoide {
    Version::Version(const int major) {
        setMajor(major);
    }

    Version::Version(const int major, const int minor) {
        setMajor(major).setMinor(minor);
    }

    Version::Version(const int major, const int minor, const int revision) {
        setMajor(major).setMinor(minor).setRevision(revision);
    }
    
    Version& Version::setMajor(const int value) {
        if (value < 0) {
            throw std::invalid_argument("Argument should be non-negative");
        }

        m_major = value;

        return *this;
    }
    
    Version& Version::setMinor(const int value) {
        if (value < 0) {
            throw std::invalid_argument("Argument should be non-negative");
        }

        m_minor = value;

        return *this;
    }

    Version& Version::setRevision(const int value) {
        if (value < 0) {
            throw std::invalid_argument("Argument should be non-negative");
        }

        m_revision = value;

        return *this;
    }

    VersionComparisonResult Version::compare(const Version &other) const {
        if (*this == other) {
            return VersionComparisonResult::Equal;
        } else if (*this < other) {
            return VersionComparisonResult::Lesser;
        } else if (*this > other) {
            return VersionComparisonResult::Greater;
        } else {
            return VersionComparisonResult::Unknown;
        }
    }

    bool Version::operator< (const Version &other) const {
        if (m_major == other.m_major) {
            if (m_minor == other.m_minor) {
                return m_revision < other.m_revision;
            } else {
                return m_minor < other.m_minor;
            }
        } else {
            return m_major < other.m_major;
        }
    }

    bool Version::operator> (const Version &other) const {
        if (m_major == other.m_major) {
            if (m_minor == other.m_minor) {
                return m_revision > other.m_revision;
            } else {
                return m_minor > other.m_minor;
            }
        } else {
            return m_major > other.m_major;
        }
    }

    bool Version::operator<= (const Version &other) const {
        return *this < other || *this == other;
    }
    
    bool Version::operator>= (const Version &other) const {
        return *this > other || *this == other;
    }

    bool Version::operator== (const Version &other) const {
        if (m_major != other.m_major) {
            return false;
        }

        if (m_minor != other.m_minor) {
            return false;
        }

        if (m_revision != other.m_revision) {
            return false;
        }

        return true;
    }

    bool Version::operator!= (const Version &other) const {
        return ! (*this == other);
    }

    std::string Version::toString() const {
        return std::to_string(m_major) + "." + std::to_string(m_minor) + "." + std::to_string(m_revision);
    }
    
    Version Version::parse(const std::string &str) {
        std::vector<std::string> results;

        boost::split(results, str, boost::is_any_of("."));

        if (results.size() > 3 || results.size() < 1) {
            throw std::runtime_error("Invalid version format");
        }

        return Version {
            std::atoi(results[0].c_str()),
            std::atoi(results[1].c_str()),
            std::atoi(results[2].c_str())
        };
    }
}
