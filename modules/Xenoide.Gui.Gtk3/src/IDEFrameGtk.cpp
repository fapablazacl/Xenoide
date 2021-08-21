
#include <Xenoide/Gui/Gtk3/IDEFrameGtk.hpp>

#include <cassert>
#include <iostream>
#include <boost/filesystem.hpp>
#include <Xenoide/Core/FileService.hpp>
#include <Xenoide/Gui/IDEFrame.hpp>
#include <Xenoide/Gui/Gtk3/DocumentGtk.hpp>

namespace fs = boost::filesystem;

namespace Xenoide {
    IDEFrameGtk::IDEFrameGtk(IDEFrame::Presenter *presenter) : IDEFrame(presenter), dialogManager(*this), folderBrowser(presenter->getFolderBrowserPresenter()) {
        // setup supported actions
        add_action("file_new", sigc::mem_fun(*this, &IDEFrameGtk::on_action_file_new));
        add_action("file_open", sigc::mem_fun(*this, &IDEFrameGtk::on_action_file_open));
        add_action("file_open_project", sigc::mem_fun(*this, &IDEFrameGtk::on_action_file_open_project));
        add_action("file_save", sigc::mem_fun(*this, &IDEFrameGtk::on_action_file_save));
        add_action("file_save_as", sigc::mem_fun(*this, &IDEFrameGtk::on_action_file_save_as));
        add_action("file_exit", sigc::mem_fun(*this, &IDEFrameGtk::on_action_file_exit));

        // setup client area
        set_border_width(10);
        std::cout << "    Setting up Project Explorer..." << std::endl;

        paned.add1(folderBrowser);
        std::cout << "      Added to the Paned Splitter." << std::endl;

        folderBrowser.show();
        std::cout << "      Shows to the User." << std::endl;

        folderBrowser.signal_item_activated().connect(sigc::mem_fun(*this, &IDEFrameGtk::on_item_activated));
        std::cout << "      Connected Event Handlers" << std::endl;

        std::cout << "    Setting up DocumentGtk Manager..." << std::endl;
        paned.add2(documentManager);
        documentManager.show();

        std::cout << "    Splitter..." << std::endl;
        add(paned);
        paned.show();

        std::cout << "    Done..." << std::endl;
        maximize();

        presenter->onInitialized(this, &dialogManager, nullptr);
    }

    IDEFrameGtk::~IDEFrameGtk() {}

    void IDEFrameGtk::on_action_file_new() {
        m_title = "Untitled";
        m_path = "";
    }

    DocumentManager* IDEFrameGtk::getDocumentManager() {
        // TODO: Add implementation

        return nullptr;
    }

    DialogManager* IDEFrameGtk::getDialogManager() {
        return &dialogManager;
    }

    FolderBrowser* IDEFrameGtk::getFolderBrowser() {
        return &folderBrowser;
    }

    void IDEFrameGtk::close() {
        // TODO: Add implementation
    }

    void IDEFrameGtk::show() {
        // TODO: Add implementation
    }


    void IDEFrameGtk::showPanel(const Panel panel) {
        // TODO: Add implementation
    }

    /*
    void IDEFrameGtk::setupMenuBar(const Menu &menu) {
        
    }
    */

    void IDEFrameGtk::on_action_file_open_project() {
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

    void IDEFrameGtk::on_action_file_save() {
        auto &editor = documentManager.get_current_editor();

        // TODO: Don't always assume that a Key is a Path
        std::string path = editor.get_key();
        std::string text = editor.get_text();

        // Xenoide::FileUtil::save(path, text);

        editor.set_dirty_flag(false);
    }

    void IDEFrameGtk::on_action_file_close() {
        auto &editor = documentManager.get_current_editor();
        documentManager.close_editor(editor);
    }

    void IDEFrameGtk::on_action_file_open() {
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

    void IDEFrameGtk::on_action_file_save_as() {
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

    void IDEFrameGtk::on_action_file_exit() {
        hide();
    }

    void IDEFrameGtk::on_item_activated(std::string path) {
        if (fs::is_directory(path)) {
            return;
        }

        const std::string name = fs::path(path).filename().string();
        const std::string content = "" /*TODO: Implement Xenoide::FileUtil::load(path)*/;

        documentManager.open_editor(path, name, content);
    }
}
