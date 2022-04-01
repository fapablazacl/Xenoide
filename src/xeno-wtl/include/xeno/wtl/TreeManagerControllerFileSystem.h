
#pragma once 

#include "TreeManagerController.h"

#include <vector>
#include <map>
#include <boost/filesystem.hpp>


namespace Xenoide {
    class CTreeManagerControllerFileSystem : public TreeManagerController {
    public:
        explicit CTreeManagerControllerFileSystem(const boost::filesystem::path& rootPath);

        virtual ~CTreeManagerControllerFileSystem();

        void clicked(const TreeItemId itemId) override;

        int getChildCount(const TreeItemId itemId) const override;

        TreeItemId getChildId(const TreeItemId parentId, const int i) const override;

        std::string getItemCaption(const TreeItemId itemId) const override;

        int getItemImage(const TreeItemId itemId) const override;

        std::vector<MenuData> getItemPopupMenuData(const TreeItemId itemId) const override;

    private:
        TreeItemId generateItemId() const;

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

