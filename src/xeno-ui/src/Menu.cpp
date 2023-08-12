
#include <xeno/ui/Menu.h>

namespace Xenoide {
    MenuData MenuData::menuBar(const std::vector<MenuData> &childs) {
        MenuData menu;

        menu.childs = childs;
        menu.kind = MenuKind::SubMenu;

        return menu;
    }

    MenuData MenuData::menu(const std::string &text, const std::vector<MenuData> &childs) {
        MenuData menu;

        menu.text = text;
        menu.childs = childs;
        menu.kind = MenuKind::SubMenu;
        
        return menu;
    }

    MenuData MenuData::action(const std::function<void()> &callback, const std::string &text, const Shortcut &shortcut) {
        MenuData menu;

        menu.callback = callback;
        menu.text = text;
        menu.shortcut = shortcut;
        menu.kind = MenuKind::Action;

        return menu;
    }

    MenuData MenuData::action(const int id, const std::string &text, const Shortcut &shortcut) {
        MenuData menu;

        menu.id = id;
        menu.text = text;
        menu.shortcut = shortcut;
        menu.kind = MenuKind::Action;

        return menu;
    }

    MenuData MenuData::separator() {
        MenuData menu;

        menu.kind = MenuKind::Separator;

        return menu;
    }
}
