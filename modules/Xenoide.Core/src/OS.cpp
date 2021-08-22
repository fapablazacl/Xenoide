
#include <Xenoide/Core/OS.hpp>

namespace Xenoide {
    OS getCurrentOS() {
#if defined(_WINDOWS)
        return OS::Windows;
#elif defined(__linux__)
        return OS::Linux;
#elif __APPLE__
#include "TargetConditionals.h"
#if defined(TARGET_OS_MAC)
        return OS::Mac;
#else
        return OS::Unknown;
#endif
#else
        return OS::Unknown;
#endif
    }
}
