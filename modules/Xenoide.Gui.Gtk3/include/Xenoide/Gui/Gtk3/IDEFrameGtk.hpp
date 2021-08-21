
#ifndef __FELIDE_GTK3_MAINWINDOW_HPP__
#define __FELIDE_GTK3_MAINWINDOW_HPP__

#include <gtkmm.h>
#include <string>

#include <Xenoide/Gui/IDEFrame.hpp>

#include "DocumentManagerGtk.hpp"
#include "FolderBrowserGtk.hpp"
#include "DialogManagerGtk.hpp"

namespace Xenoide {
    class IDEFrameGtk : public Gtk::ApplicationWindow, public IDEFrame {
    public:
        IDEFrameGtk(IDEFrame::Presenter *presenter);

        virtual ~IDEFrameGtk();

        virtual DocumentManager* getDocumentManager() override;

        virtual DialogManager* getDialogManager() override;

        virtual FolderBrowser* getFolderBrowser() override;

        virtual void close() override;

        virtual void show() override;

        virtual void showPanel(const Panel panel) override;

    private:
        void on_action_file_new();

        void on_action_file_open_project();

        void on_action_file_save();

        void on_action_file_close();

        void on_action_file_open();

        void on_action_file_save_as();

        void on_action_file_exit();

        void on_item_activated(std::string path);

    private:
        std::string m_title;
        std::string m_path;

        DialogManagerGtk dialogManager;
        DocumentManagerGtk documentManager;
        FolderBrowserGtk folderBrowser;
        // Glib::RefPtr<Gio::Menu> m_mainMenu;

        // AKA Splitter
        Gtk::Paned paned;
    };
}

#endif
