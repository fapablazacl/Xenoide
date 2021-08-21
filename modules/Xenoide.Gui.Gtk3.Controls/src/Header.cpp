
#include <Xenoide/Gui/Gtk3/Controls/Header.h>

namespace Xenoide {
    Header::Header() : m_closeImage(Gtk::Stock::CLOSE, Gtk::IconSize(Gtk::ICON_SIZE_MENU)) {
        m_closeButton.set_image(m_closeImage);
        m_closeButton.set_relief(Gtk::RELIEF_NONE);

        pack_start(m_titleLabel, true, 0);
        pack_end(m_closeButton);
        show_all();
    }


    Header::~Header() {}


    Gtk::Button* Header::get_close_button() {
        return &m_closeButton;
    }

    void Header::set_text(const std::string &value) {
        m_titleLabel.set_text(value);
    }

    std::string Header::get_text() const {
        return m_titleLabel.get_text();
    }
}
