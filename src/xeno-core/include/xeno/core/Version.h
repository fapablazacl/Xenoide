
#ifndef __XENOIDE_VERSION_HPP__
#define __XENOIDE_VERSION_HPP__

#include <string>
#include <xeno/core/Predef.h>

namespace Xenoide {
    enum class VersionComparisonResult {
        Unknown = 255,
        Lesser = -1, 
        Equal = 0, 
        Greater = 1
    };

    class Version {
    public:
        explicit Version() {}

        explicit Version(const int major);

        explicit Version(const int major, const int minor);

        explicit Version(const int major, const int minor, const int revision);

        int getMajor() const {
            return m_major;
        }

        int getMinor() const {
            return m_minor;
        }

        int getRevision() const {
            return m_revision;
        }
        
        Version& setMajor(const int value);

        Version& setMinor(const int value);

        Version& setRevision(const int value);

        VersionComparisonResult compare(const Version &other) const;

        bool operator< (const Version &other) const;

        bool operator> (const Version &other) const;

        bool operator<= (const Version &other) const;
        
        bool operator>= (const Version &other) const;

        bool operator== (const Version &other) const;

        bool operator!= (const Version &other) const;

        std::string toString() const;

    public:
        static Version parse(const std::string &str);

    private:
        int m_major = 0;
        int m_minor = 0;
        int m_revision = 0;
    };
}

#endif 
