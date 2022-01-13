
#ifndef __XENOIDE_UI_MENU_HPP__
#define __XENOIDE_UI_MENU_HPP__

#include <string>
#include <vector>
#include <functional>

namespace Xenoide {
    enum class MenuKind {
        SubMenu,
        Action,
        Separator
    };

    enum class Modifier {
        None = 0,
        Ctrl = 1,
        Shift = 2, 
        Alt = 4,
        CtrlShift = Ctrl + Shift,
        CtrlShiftAlt = Ctrl + Shift + Alt,
        ShiftAlt = Shift + Alt
    };

    inline bool operator& (const Modifier &modifiers, const Modifier &modifier) {
        return static_cast<int>(modifiers) & static_cast<int>(modifier);
    }

    enum class Key {
        None = 0,
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
        Q, W, E, R, T, Y, U, I, O, P,
        A, S, D, F, G, H, J, K, L,
        Z, X, C, V, B, N, M
    };

    struct Shortcut {
        Modifier modifier = Modifier::None;
        Key key = Key::None;

        Shortcut() {}

        Shortcut(const Key key) {
            this->key = key;
        }

        Shortcut(const Modifier modifier, const Key key) {
            this->modifier = modifier;
            this->key = key;
        }

        bool isNone() const {
            return key == Key::None;
        }
    };

    struct Menu {
        std::string text;
        Shortcut shortcut;
        std::vector<Menu> childs;
        MenuKind kind;
        std::function<void()> callback;
        
        static Menu menuBar(const std::vector<Menu> &childs);

        static Menu menu(const std::string &text, const std::vector<Menu> &childs);

        static Menu action(const std::function<void()> &callback, const std::string &text, const Shortcut &shortcut = {});

        static Menu separator();
    };
}

#endif
