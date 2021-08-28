
#include <Xenoide/Gui/Menu.h>

namespace Xenoide {
    Menu Menu::menuBar(const std::vector<Menu> &childs) {
        Menu menu;

        menu.childs = childs;
        menu.kind = MenuKind::SubMenu;

        return menu;
    }

    Menu Menu::menu(const std::string &text, const std::vector<Menu> &childs) {
        Menu menu;

        menu.text = text;
        menu.childs = childs;
        menu.kind = MenuKind::SubMenu;
        
        return menu;
    }

    Menu Menu::action(const std::function<void()> &callback, const std::string &text, const Shortcut &shortcut) {
        Menu menu;

        menu.callback = callback;
        menu.text = text;
        menu.shortcut = shortcut;
        menu.kind = MenuKind::Action;

        return menu;
    }

    Menu Menu::separator() {
        Menu menu;

        menu.kind = MenuKind::Separator;

        return menu;
    }
}
