
#include <xeno/wtl/TreeManagerControllerFileSystem.h>


namespace Xenoide {
    CTreeManagerControllerFileSystem::CTreeManagerControllerFileSystem(const boost::filesystem::path& rootPath) 
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
    }

    CTreeManagerControllerFileSystem::~CTreeManagerControllerFileSystem() {}

    void CTreeManagerControllerFileSystem::clicked(const TreeItemId itemId) {}

    int CTreeManagerControllerFileSystem::getChildCount(const TreeItemId itemId) const {
        // determine current path
        // TODO: refactor into an utility method
        const boost::filesystem::path path = (itemId == TreeItemId{} ?  rootPath : itemPathMap[itemId]);

        // if the current path is not a directory, it can't have children subpaths.
        if (!boost::filesystem::is_directory(path)) {
            return 0;
        }

        // check if the specified item was previously populated, and return that
        if (const auto it = itemChildMap.find(itemId); it != itemChildMap.end()) {
            return static_cast<int>(it->second.size());
        }

        // populate child elements
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


    TreeItemId CTreeManagerControllerFileSystem::getChildId(const TreeItemId parentId, const int i) const {
        const auto key = std::tuple(parentId, i);

        if (const auto it = parentItemMap.find(key); it != parentItemMap.end()) {
            return it->second;
        }
            
        const TreeItemId itemId = generateItemId();

        parentItemMap.insert({ key, itemId });

        return itemId;
    }


    std::string CTreeManagerControllerFileSystem::getItemCaption(const TreeItemId itemId) const {
        const auto it = itemPathMap.find(itemId);
        assert(it != itemPathMap.end());

        return it->second.filename().string();
    }


    int CTreeManagerControllerFileSystem::getItemImage(const TreeItemId itemId) const {
        const auto it = itemPathMap.find(itemId);
        assert(it != itemPathMap.end());

        // return GetIconFromPath(it->second.string().c_str());

        return boost::filesystem::is_directory(it->second) ? 1 : 0;
    }

    std::vector<MenuData> CTreeManagerControllerFileSystem::getItemPopupMenuData(const TreeItemId itemId) const {
        // TODO: Refactor into an utility method
        const boost::filesystem::path path = (itemId == TreeItemId{} ?  rootPath : itemPathMap[itemId]);

        if (boost::filesystem::is_directory(path)) {
            return folderContextMenu;
        }

        return fileContextMenu;
    }

    TreeItemId CTreeManagerControllerFileSystem::generateItemId() const {
        return TreeItemId { ++count };
    }
}
