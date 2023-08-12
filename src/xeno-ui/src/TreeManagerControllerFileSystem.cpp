
#include <xeno/ui/TreeManagerControllerFileSystem.h>

#include <boost/algorithm/string/predicate.hpp>

namespace Xenoide {
    TreeManagerControllerFileSystem::TreeManagerControllerFileSystem(const boost::filesystem::path& rootPath) 
        : rootPath{rootPath} {

        fileContextMenu = {
            MenuData::action([]() {}, "&Open"),
            MenuData::action([]() {}, "&Open in Dedicated Window"),
            MenuData::separator(),
            MenuData::action([]() {}, "&Cut"),
            MenuData::action([]() {}, "&Copy"),
            MenuData::action([]() {}, "&Paste"),
            MenuData::separator(),
            MenuData::action([]() {}, "&Rename"),
            MenuData::action([]() {}, "&Delete")
        };

        folderContextMenu = {
            MenuData::action([]() {}, "&New File"),
            MenuData::action([]() {}, "&New Folder"),
            MenuData::separator(),
            MenuData::action([]() {}, "&Cut"),
            MenuData::action([]() {}, "&Copy"),
            MenuData::action([]() {}, "&Paste"),
            MenuData::separator(),
            MenuData::action([]() {}, "&Rename"),
            MenuData::action([]() {}, "&Delete")
        };

        //
        itemPathMap[{}] = rootPath;
        populateChildren({}, rootPath);
    }

    TreeManagerControllerFileSystem::~TreeManagerControllerFileSystem() {}

    void TreeManagerControllerFileSystem::clicked(const TreeItemId itemId) {}

    int TreeManagerControllerFileSystem::getChildCount(const TreeItemId itemId) const {
        // determine current path
        const boost::filesystem::path path = pathFromItem(itemId);

        // if the current path is not a directory, it can't have children subpaths.
        if (!boost::filesystem::is_directory(path)) {
            return 0;
        }

        // check if the specified item was previously populated, and return that
        if (const auto it = itemChildMap.find(itemId); it != itemChildMap.end()) {
            return static_cast<int>(it->second.size());
        }

        // populate child elements
        return populateChildren(itemId, path);
    }


    int TreeManagerControllerFileSystem::populateChildren(const TreeItemId itemId, const boost::filesystem::path &path) const {
        int i = 0;

        for (boost::filesystem::directory_iterator end, path_it{ path }; path_it != end; path_it++) {
            const boost::filesystem::path childPath = path_it->path();
            itemChildMap[itemId].push_back(childPath);

            const TreeItemId childId = getChildId(itemId, i);
            itemPathMap[childId] = childPath;

            i++;
        }

        return i + 1;
    }


    TreeItemId TreeManagerControllerFileSystem::getChildId(const TreeItemId parentId, const int i) const {
        const auto key = std::tuple(parentId, i);

        if (const auto it = parentItemMap.find(key); it != parentItemMap.end()) {
            return it->second;
        }
            
        const TreeItemId itemId = generateItemId();

        parentItemMap.insert({ key, itemId });

        return itemId;
    }


    std::string TreeManagerControllerFileSystem::getItemCaption(const TreeItemId itemId) const {
        const auto it = itemPathMap.find(itemId);
        assert(it != itemPathMap.end());

        return it->second.filename().string();
    }


    int TreeManagerControllerFileSystem::getItemImage(const TreeItemId itemId) const {
        const auto it = itemPathMap.find(itemId);
        assert(it != itemPathMap.end());

        return boost::filesystem::is_directory(it->second) ? 1 : 0;
    }

    std::vector<MenuData> TreeManagerControllerFileSystem::getItemPopupMenuData(const TreeItemId itemId) const {
        const boost::filesystem::path path = pathFromItem(itemId);

        if (boost::filesystem::is_directory(path)) {
            return folderContextMenu;
        }

        return fileContextMenu;
    }

    TreeItemId TreeManagerControllerFileSystem::generateItemId() const {
        return TreeItemId { ++count };
    }

    int TreeManagerControllerFileSystem::compare(const TreeItemId& item1, const TreeItemId& item2) const {
        namespace fs = boost::filesystem;

        if (item1 == item2) {
            return 0;
        }

        const auto path1 = pathFromItem(item1);
        const auto path2 = pathFromItem(item2);

        if (fs::is_directory(path1) && !fs::is_directory(path2)) {
            return -1;
        }

        if (!fs::is_directory(path1) && fs::is_directory(path2)) {
            return 1;
        }

        const auto name1 = path1.filename().string();
        const auto name2 = path2.filename().string();

        return boost::iequals(name1, name2);
    }

    const boost::filesystem::path& TreeManagerControllerFileSystem::pathFromItem(const TreeItemId itemId) const {
        return (itemId == TreeItemId{} ?  rootPath : itemPathMap[itemId]);
    }
}
