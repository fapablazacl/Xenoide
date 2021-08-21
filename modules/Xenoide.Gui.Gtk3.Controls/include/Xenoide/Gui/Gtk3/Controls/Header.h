
#pragma once

#include <gtkmm.h>

namespace Xenoide {
    class Header : public Gtk::HBox {
    public:
        Header();

        ~Header();

        Gtk::Button* get_close_button();

        void set_text(const std::string &value);

        std::string get_text() const;

    private:
        Gtk::Image m_closeImage;
        Gtk::Label m_titleLabel;
        Gtk::Button m_closeButton;
    };
}
