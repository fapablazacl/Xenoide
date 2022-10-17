
#include "MainWindow.h"

#include <cassert>
#include <iostream>

namespace fs = std::filesystem;

namespace Xenoide {
    MainWindow::MainWindow() {
        // setup supported actions
        add_action("file_new", sigc::mem_fun(*this, &MainWindow::on_action_file_new));
        add_action("file_open", sigc::mem_fun(*this, &MainWindow::on_action_file_open));
        add_action("file_open_project", sigc::mem_fun(*this, &MainWindow::on_action_file_open_project));
        add_action("file_save", sigc::mem_fun(*this, &MainWindow::on_action_file_save));
        add_action("file_save_as", sigc::mem_fun(*this, &MainWindow::on_action_file_save_as));
        add_action("file_exit", sigc::mem_fun(*this, &MainWindow::on_action_file_exit));

        // setup client area
        set_border_width(10);

        paned.add1(folderBrowser);

        folderBrowser.show();

        folderBrowser.signal_item_activated().connect(sigc::mem_fun(*this, &MainWindow::on_item_activated));

        paned.add2(documentManager);
        documentManager.show();

        add(paned);
        paned.show();
       
        maximize();
    }


    MainWindow::~MainWindow() {}


    void MainWindow::on_action_file_new() {
        m_title = "Untitled";
        m_path = "";
    }


    void MainWindow::on_action_file_open_project() {
        Gtk::FileChooserDialog dialog("Please choose a Folder", Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
        dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
        dialog.add_button("_Open", Gtk::RESPONSE_OK);
        dialog.set_transient_for(*this);

        int result = dialog.run();

        if (result == Gtk::RESPONSE_OK) {
            const std::string folderPath = dialog.get_filename();
            folderBrowser.displayFolder(folderPath);
        }
    }


    void MainWindow::on_action_file_save() {
        auto &editor = documentManager.get_current_editor();

        // TODO: Don't always assume that a Key is a Path
        std::string path = editor.get_key();
        std::string text = editor.get_text();

        // Xenoide::FileUtil::save(path, text);

        editor.set_dirty_flag(false);
    }


    void MainWindow::on_action_file_close() {
        auto &editor = documentManager.get_current_editor();
        documentManager.close_editor(editor);
    }


    void MainWindow::on_action_file_open() {
        auto fileFilter = Gtk::FileFilter::create();
        fileFilter->set_name("Text files");
        fileFilter->add_mime_type("text/plain");

        Gtk::FileChooserDialog dialog("Please choose a file", Gtk::FILE_CHOOSER_ACTION_OPEN);
        dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
        dialog.add_button("_Open", Gtk::RESPONSE_OK);
        dialog.set_transient_for(*this);
        dialog.add_filter(fileFilter);

        int result = dialog.run();

        if (result == Gtk::RESPONSE_OK) {            
            const std::string path = dialog.get_filename();
            const std::string name = fs::path(path).filename().string();
            const std::string content = ""/*Xenoide::FileUtil::load(path)*/;

            documentManager.open_editor(path, name, content);
        }
    }


    void MainWindow::on_action_file_save_as() {
        // trigger dialog
        auto fileFilter = Gtk::FileFilter::create();
        fileFilter->set_name("Text files");
        fileFilter->add_mime_type("text/plain");

        Gtk::FileChooserDialog dialog("Please choose a file", Gtk::FILE_CHOOSER_ACTION_SAVE);
        dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
        dialog.add_button("_Open", Gtk::RESPONSE_OK);
        dialog.set_transient_for(*this);
        dialog.add_filter(fileFilter);

        int result = dialog.run();

        if (result == Gtk::RESPONSE_OK) {
            const std::string filePath = dialog.get_filename();
        }
    }


    void MainWindow::on_action_file_exit() {
        hide();
    }


    void MainWindow::on_item_activated(std::string path) {
        if (fs::is_directory(path)) {
            return;
        }

        const std::string name = fs::path(path).filename().string();
        const std::string content = "" /*TODO: Implement Xenoide::FileUtil::load(path)*/;

        documentManager.open_editor(path, name, content);
    }
}
