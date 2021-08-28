
#ifndef __XENOIDE_UI_MENUPANEL_HPP__
#define __XENOIDE_UI_MENUPANEL_HPP__

#include <Xenoide/Core/Predef.h>

namespace Xenoide {
    struct Menu;

    /**
     * @brief View that shows to the user a MenuBar.
     */
    class MenuPanel {
    public:
        virtual ~MenuPanel();

        virtual void setupMenuBar(const Menu &menu) = 0;
    };
}

#endif
