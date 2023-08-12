
#include "MenuFactory.h"

#include <boost/algorithm/string/join.hpp>


namespace Xenoide {
    CCommandMap::CCommandMap() {}


    CCommandMap::CCommandMap(const std::map<int, std::function<void()>>& commandIdCallbackMap) 
	    : commandIdCallbackMap{commandIdCallbackMap} {}


    CCommandMap::~CCommandMap() {}


    bool CCommandMap::OnCommand(const int commandId) const {
	    const auto& map = commandIdCallbackMap;

	    if (auto commandIt = map.find(commandId); commandIt != map.end()) {
            commandIt->second();

		    return true;
        }

	    return false;
    }
}


namespace Xenoide {
    CMenuFactory::CMenuFactory(const int commandBase) : commandBase{ commandBase } {}


    void CMenuFactory::fillMenu(
        HMENU hMenu,
        CCommandMap& commandMap,
        const std::vector<Xenoide::MenuData>& popupMenuItems) const {

        std::map<int, std::function<void()>> map;

        for (const MenuData& menuData : popupMenuItems) {
            fillMenuItem(hMenu, map, menuData);
        }

        commandMap = CCommandMap{ map };
    }


    void CMenuFactory::fillMenuItem(
        CMenuHandle parent,
        std::map<int, std::function<void()>>& map,
        const MenuData& menu) const {

        switch (menu.kind) {
        case MenuKind::Action: {
            const int commandId = commandBase + static_cast<int>(map.size());

            const std::string title = this->mapMenuTitle(menu);
            parent.AppendMenu(MF_STRING, commandId, title.c_str());

            map[commandId] = menu.callback;

            break;
        }

        case MenuKind::Separator: {
            parent.AppendMenuA(MF_SEPARATOR);
            break;
        }

        case MenuKind::SubMenu: {
            CMenuHandle menuItemHandle;
            menuItemHandle.CreateMenu();
            parent.AppendMenu(MF_STRING, menuItemHandle, menu.text.c_str());

            for (const MenuData& child : menu.childs) {
                this->fillMenuItem(menuItemHandle, map, child);
            }

            break;
        }
        }
    }


    std::string CMenuFactory::mapMenuTitle(const MenuData& menu) const {
        if (menu.shortcut.isNone()) {
            return menu.text;
        }

        // base text
        std::string title = menu.text;

        // modifiers
        std::vector<std::string> modifiers;
        if (menu.shortcut.modifier & Modifier::Ctrl) {
            modifiers.push_back("Ctrl");
        }

        if (menu.shortcut.modifier & Modifier::Shift) {
            modifiers.push_back("Shift");
        }

        if (menu.shortcut.modifier & Modifier::Alt) {
            modifiers.push_back("Alt");
        }

        // key
        std::string key;
        switch (menu.shortcut.key) {
        case Key::A: key = "A"; break;
        case Key::B: key = "B"; break;
        case Key::C: key = "C"; break;
        case Key::D: key = "D"; break;
        case Key::E: key = "E"; break;
        case Key::F: key = "F"; break;
        case Key::G: key = "G"; break;
        case Key::H: key = "H"; break;
        case Key::I: key = "I"; break;
        case Key::J: key = "J"; break;
        case Key::K: key = "K"; break;
        case Key::L: key = "L"; break;
        case Key::M: key = "M"; break;
        case Key::N: key = "N"; break;
        case Key::O: key = "O"; break;
        case Key::P: key = "P"; break;
        case Key::Q: key = "Q"; break;
        case Key::R: key = "R"; break;
        case Key::S: key = "S"; break;
        case Key::T: key = "T"; break;
        case Key::U: key = "U"; break;
        case Key::V: key = "V"; break;
        case Key::W: key = "W"; break;
        case Key::X: key = "X"; break;
        case Key::Y: key = "Y"; break;
        case Key::Z: key = "Z"; break;
        case Key::F1: key = "F1"; break;
        case Key::F2: key = "F2"; break;
        case Key::F3: key = "F3"; break;
        case Key::F4: key = "F4"; break;
        case Key::F5: key = "F5"; break;
        case Key::F6: key = "F6"; break;
        case Key::F7: key = "F7"; break;
        case Key::F8: key = "F8"; break;
        case Key::F9: key = "F9"; break;
        case Key::F10: key = "F10"; break;
        case Key::F11: key = "F11"; break;
        case Key::F12: key = "F12"; break;
        }

        if (modifiers.size() == 0) {
            return title + "\t" + key;
        }
        else {
            return title + "\t" + boost::join(modifiers, "+") + "+" + key;
        }
    }
}




namespace Xenoide {
    void CMenuFactory2::fillMenu(
        HMENU hMenu,
        const std::vector<MenuData>& popupMenuItems) const {

        for (const MenuData& menuData : popupMenuItems) {
            fillMenuItem(hMenu, menuData);
        }
    }


    void CMenuFactory2::fillMenuItem(
        CMenuHandle parent,
        const MenuData& menu) const {

        switch (menu.kind) {
        case MenuKind::Action: {
            const int commandId = menu.id;

            const std::string title = this->mapMenuTitle(menu);
            parent.AppendMenu(MF_STRING, commandId, title.c_str());

            break;
        }

        case MenuKind::Separator: {
            parent.AppendMenuA(MF_SEPARATOR);
            break;
        }

        case MenuKind::SubMenu: {
            CMenuHandle menuItemHandle;
            menuItemHandle.CreateMenu();
            parent.AppendMenu(MF_STRING, menuItemHandle, menu.text.c_str());

            for (const MenuData& child : menu.childs) {
                this->fillMenuItem(menuItemHandle, child);
            }

            break;
        }
        }
    }


    std::string CMenuFactory2::mapMenuTitle(const MenuData& menu) const {
        if (menu.shortcut.isNone()) {
            return menu.text;
        }

        // base text
        std::string title = menu.text;

        // modifiers
        std::vector<std::string> modifiers;
        if (menu.shortcut.modifier & Modifier::Ctrl) {
            modifiers.push_back("Ctrl");
        }

        if (menu.shortcut.modifier & Modifier::Shift) {
            modifiers.push_back("Shift");
        }

        if (menu.shortcut.modifier & Modifier::Alt) {
            modifiers.push_back("Alt");
        }

        // key
        std::string key;
        switch (menu.shortcut.key) {
        case Key::A: key = "A"; break;
        case Key::B: key = "B"; break;
        case Key::C: key = "C"; break;
        case Key::D: key = "D"; break;
        case Key::E: key = "E"; break;
        case Key::F: key = "F"; break;
        case Key::G: key = "G"; break;
        case Key::H: key = "H"; break;
        case Key::I: key = "I"; break;
        case Key::J: key = "J"; break;
        case Key::K: key = "K"; break;
        case Key::L: key = "L"; break;
        case Key::M: key = "M"; break;
        case Key::N: key = "N"; break;
        case Key::O: key = "O"; break;
        case Key::P: key = "P"; break;
        case Key::Q: key = "Q"; break;
        case Key::R: key = "R"; break;
        case Key::S: key = "S"; break;
        case Key::T: key = "T"; break;
        case Key::U: key = "U"; break;
        case Key::V: key = "V"; break;
        case Key::W: key = "W"; break;
        case Key::X: key = "X"; break;
        case Key::Y: key = "Y"; break;
        case Key::Z: key = "Z"; break;
        case Key::F1: key = "F1"; break;
        case Key::F2: key = "F2"; break;
        case Key::F3: key = "F3"; break;
        case Key::F4: key = "F4"; break;
        case Key::F5: key = "F5"; break;
        case Key::F6: key = "F6"; break;
        case Key::F7: key = "F7"; break;
        case Key::F8: key = "F8"; break;
        case Key::F9: key = "F9"; break;
        case Key::F10: key = "F10"; break;
        case Key::F11: key = "F11"; break;
        case Key::F12: key = "F12"; break;
        }

        if (modifiers.size() == 0) {
            return title + "\t" + key;
        }
        else {
            return title + "\t" + boost::join(modifiers, "+") + "+" + key;
        }
    }
}
