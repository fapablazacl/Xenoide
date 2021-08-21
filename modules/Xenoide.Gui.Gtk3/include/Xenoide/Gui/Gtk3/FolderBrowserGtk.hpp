
#ifndef __FELIDE_GTK3_PROJECTEXPLORER_HPP__
#define __FELIDE_GTK3_PROJECTEXPLORER_HPP__

#include <string>
#include <gtkmm.h>

#include <iostream>
#include <boost/filesystem.hpp>
#include <Xenoide/Gui/FolderBrowser.hpp>
#include <Xenoide/Core/FileService.hpp>

namespace fs = boost::filesystem;

namespace Xenoide {
    /**
     * @brief FolderBrowserGtk that opens a "vanilla" project style (it just open a folder and shows it contents)
     */ 
    class ProjectItemModel : public Gtk::TreeModel::ColumnRecord {
    public:
        ProjectItemModel() {
            add(m_itemName);
            add(m_itemPath);
        }

        Gtk::TreeModelColumn<std::string> m_itemName;
        Gtk::TreeModelColumn<std::string> m_itemPath;
    };

    typedef sigc::signal<void, std::string> signal_item_activated_t;

    class FolderBrowserGtk : public Gtk::Bin, public FolderBrowser {
    public:
        FolderBrowserGtk(FolderBrowser::Presenter *presenter);

        virtual ~FolderBrowserGtk();

        virtual void displayFolder(const std::string &folder) override;

        virtual boost::optional<std::string> getSelectedPath() const override;

        virtual void displayContextualMenu(const Point &point, const Menu &menu) override;

    public:
        signal_item_activated_t signal_item_activated();

    private:
        void OnItemActivated(const Gtk::TreeModel::Path& treePath, Gtk::TreeViewColumn* column);

        void OnItemSelected();

        std::string GetPathName(const fs::path &path);

        void PopulateTreeNode(fs::path path, Gtk::TreeModel::iterator treeIterator);

    private:
        signal_item_activated_t m_signal_item_activated;

        std::string m_projectPath;
        Gtk::ScrolledWindow m_scrolled;

        ProjectItemModel m_treeModel;
        Gtk::TreeView m_treeView;
        Glib::RefPtr<Gtk::TreeStore> m_refTreeStore;
    };
}

#endif
