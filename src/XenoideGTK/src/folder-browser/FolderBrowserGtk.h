
#ifndef __FELIDE_GTK3_PROJECTEXPLORER_HPP__
#define __FELIDE_GTK3_PROJECTEXPLORER_HPP__

#include <string>
#include <optional>
#include <iostream>
#include <memory>
#include <Xenoide/gtkmm.h>
#include <xeno/ui/TreeManagerController.h>

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

    class FolderBrowserGtk : public Gtk::Bin {
    public:
        FolderBrowserGtk();

        virtual ~FolderBrowserGtk();

        void displayFolder(const std::string &folder);

        std::optional<std::string> getSelectedPath() const;

        void expandRootNode();

    public:
        signal_item_activated_t signal_item_activated();

    private:
        Gtk::TreeModel::iterator appendPathTreeNode (
            const TreeItemId itemId,
            Gtk::TreeModel::iterator treeIterator);

        void OnItemActivated(
            const Gtk::TreeModel::Path& treePath, 
            Gtk::TreeViewColumn* column);

        void OnItemSelected();

    private:
        signal_item_activated_t m_signal_item_activated;

        Gtk::ScrolledWindow m_scrolled;

        ProjectItemModel m_treeModel;
        Gtk::TreeModel::iterator rootNodeIterator;
        Gtk::TreeView m_treeView;
        Glib::RefPtr<Gtk::TreeStore> m_refTreeStore;

        std::unique_ptr<TreeManagerController> controller;
    };
}

#endif
