
#pragma once 

#include "TreeManagerController.h"

#include <vector>
#include <map>
#include <boost/filesystem.hpp>


namespace Xenoide {
    class TreeManagerControllerFileSystem : public TreeManagerController {
    public:
        explicit TreeManagerControllerFileSystem(const boost::filesystem::path& rootPath);

        virtual ~TreeManagerControllerFileSystem();

        void clicked(const TreeItemId itemId) override;

        int getChildCount(const TreeItemId itemId) const override;

        TreeItemId getChildId(const TreeItemId parentId, const int i) const override;

        std::string getItemCaption(const TreeItemId itemId) const override;

        int getItemImage(const TreeItemId itemId) const override;

        std::vector<MenuData> getItemPopupMenuData(const TreeItemId itemId) const override;
        
        int compare(const TreeItemId& item1, const TreeItemId& item2) const override;

    private:
        TreeItemId generateItemId() const;

        const boost::filesystem::path& pathFromItem(const TreeItemId) const;

        int populateChildren(const TreeItemId itemId, const boost::filesystem::path &path) const;

    private:
        std::vector<MenuData> fileContextMenu;
        std::vector<MenuData> folderContextMenu;

        const boost::filesystem::path rootPath;

        mutable int count = 0;
        mutable std::map<std::tuple<TreeItemId, int>, TreeItemId> parentItemMap;
        mutable std::map<TreeItemId, boost::filesystem::path> itemPathMap;
        mutable std::map<TreeItemId, std::vector<boost::filesystem::path>> itemChildMap;
    };
}
