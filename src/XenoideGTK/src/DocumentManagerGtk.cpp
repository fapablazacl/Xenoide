
#include "DocumentManagerGtk.h"

#include "DocumentGtk.h"
#include "DocumentHeader.h"

#include <cassert>
#include <map>
#include <iostream>

namespace Xenoide {
    DocumentManagerGtk::DocumentManagerGtk() {
        add(m_notebook);
        m_notebook.show();
    }

    DocumentManagerGtk::~DocumentManagerGtk() {}

    void DocumentManagerGtk::open_editor(const std::string &key, const std::string &title, const std::string &content) {
        auto it = m_editors.find(key);

        DocumentGtk *editor = nullptr;

        if (it == m_editors.end()) {
            editor = Gtk::manage(new DocumentGtk(key));

            editor->set_text(content);
            editor->set_dirty_flag(false);
            editor->show();

            // auto header = DocumentHeader::create(*editor.get(), title);
            auto header = Gtk::manage(new DocumentHeader(*editor, title));
            editor->signal_editor_dirty_changed().connect(sigc::mem_fun(*header, &DocumentHeader::update_title_label));

            // TODO: Find a way to not dynamically instance the editor header
            m_notebook.append_page(*editor, *header);
            m_editors.insert({key, editor});
        } else {
            editor = it->second;
        }

        const int pageIndex = m_notebook.page_num(*editor);
        m_notebook.set_current_page(pageIndex);
    }


    DocumentGtk& DocumentManagerGtk::get_current_editor() {
        const int pageIndex = m_notebook.get_current_page();

        if (pageIndex == -1) {
            // TODO: Fix this nasty hack
            return *static_cast<DocumentGtk*>(nullptr);
        }

        auto page = m_notebook.get_nth_page(pageIndex);

        assert(page);

        return static_cast<DocumentGtk&>(*page);
    }


    void DocumentManagerGtk::close_editor(DocumentGtk &editor) {
        // remove from UI
        int pageIndex = -1;
        for (int i=0; i<m_notebook.get_n_pages(); i++) {
            if (&editor == m_notebook.get_nth_page(i)) {
                pageIndex = i;
                break;
            }
        }

        if (pageIndex > -1) {
            m_notebook.remove_page(pageIndex);
        }

        // remove from local cache
        m_editors.erase(editor.get_key());
    }


    signal_editor_closed_t DocumentManagerGtk::signal_editor_closed() {
        return m_signal_editor_closed;
    }
}
