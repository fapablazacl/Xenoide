
#ifndef __XENOIDE_UTIL_OS_HPP__
#define __XENOIDE_UTIL_OS_HPP__

#include <xeno/core/Predef.h>

namespace Xenoide {
    enum class OS {
        Unknown,
        Windows,
        Mac,
        Linux
    };

    extern OS getCurrentOS();
}

#endif
