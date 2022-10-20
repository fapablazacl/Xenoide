
#include "FolderBrowserGtk.h"

#include <xeno/ui/TreeManagerControllerFileSystem.h>

namespace Xenoide {
    FolderBrowserGtk::FolderBrowserGtk() {
        // setup UI
        add(m_scrolled);

        m_refTreeStore = Gtk::TreeStore::create(m_treeModel);
        m_treeView.set_model(m_refTreeStore);
        m_treeView.append_column("Name", m_treeModel.m_itemName);
        m_treeView.set_headers_visible(false);

        m_scrolled.set_border_width(5);
        m_scrolled.add(m_treeView);
        m_scrolled.show();
        m_treeView.show();

        show_all_children();

        // connect event handlers
        // m_treeView.get_selection()->signal_changed().connect(sigc::mem_fun(*this, &FolderBrowserGtk::OnItemSelected));
        m_treeView.signal_row_activated().connect(sigc::mem_fun(*this, &FolderBrowserGtk::OnItemActivated));
    }


    FolderBrowserGtk::~FolderBrowserGtk() {}


    void FolderBrowserGtk::displayFolder(const std::string &projectPath) {
        // internal variable cleanup
        controller = std::make_unique<TreeManagerControllerFileSystem>(projectPath);
        m_refTreeStore->clear();

        // populate the model rool model
        rootNodeIterator = appendPathTreeNode({}, {});

        // PopulateTreeNode(path, treeIterator);

        expandRootNode();
    }


    void FolderBrowserGtk::expandRootNode() {
        Gtk::TreeModel::Path treePath;
        treePath.push_back(0);
        m_treeView.expand_row(treePath, false);
    }


    Gtk::TreeModel::iterator FolderBrowserGtk::appendPathTreeNode (
        const TreeItemId itemId,
        Gtk::TreeModel::iterator treeIterator
    ) {
        Gtk::TreeModel::iterator childIterator = m_refTreeStore->append(treeIterator->children());
        Gtk::TreeModel::Row childRow = *childIterator;

        childRow[m_treeModel.m_itemName] = controller->getItemCaption(itemId);
        childRow[m_treeModel.m_itemPath] = "path";
        
        return childIterator;
    }


    std::optional<std::string> FolderBrowserGtk::getSelectedPath() const {
        return {};
    }


    signal_item_activated_t FolderBrowserGtk::signal_item_activated() {
        return m_signal_item_activated;
    }

    void FolderBrowserGtk::OnItemActivated(const Gtk::TreeModel::Path& treePath, Gtk::TreeViewColumn* column) {
        Gtk::TreeModel::iterator iterator = m_treeView.get_model()->get_iter(treePath);

        if (!iterator) {
            return;
        }
        
        Gtk::TreeModel::Row row = *iterator;
        std::string name = row[m_treeModel.m_itemName];
        std::string path = row[m_treeModel.m_itemPath];

        m_signal_item_activated(path);
    }


    void FolderBrowserGtk::OnItemSelected() {
        /*
        Gtk::TreeModel::iterator iterator = m_treeView.get_selection()->get_selected();

        if (iterator) {
            Gtk::TreeModel::Row row = *iterator;
            Glib::ustring name = row[m_treeModel.m_itemName];
            Glib::ustring path = row[m_treeModel.m_itemPath];

            const std::string content = Xenoide::FileUtil::load(path);
            documentManager->OpenEditor(name, content);
        }
        */
    }

    /*
    void FolderBrowserGtk::PopulateTreeNode(fs::path path, Gtk::TreeModel::iterator treeIterator) {
        if (fs::is_directory(path)) {
            fs::directory_iterator subPathIterator(path);
            fs::directory_iterator end;

            while (subPathIterator != end) {
                fs::path subPath = subPathIterator->path();

                Gtk::TreeModel::iterator childIterator = m_refTreeStore->append(treeIterator->children());
                Gtk::TreeModel::Row childRow = *childIterator;

                childRow[m_treeModel.m_itemName] = this->GetPathName(subPath);
                childRow[m_treeModel.m_itemPath] = subPath.string();

                this->PopulateTreeNode(subPath, childIterator);

                subPathIterator++;
            }
        }
    }
    */
}
