
#include <xenoide/core/FolderExplorer.h>
#include <xenoide/core/FileSystemService.h>

#include <cassert>

namespace Xenoide {
	FolderExplorer::FolderExplorer(FileSystemService* fileSystemService) : fileSystemService(fileSystemService) {
		assert(fileSystemService);
	}


	void FolderExplorer::setFolder(const Folder rootFolder, FolderExplorerCallback callback) {
		pathItemsCache.clear();
		populatedItems.clear();

		this->rootFolder = rootFolder;

		const Path folderPath { PathType::Folder, rootFolder.path };
		const std::string name { fileSystemService->extractName(folderPath) };

		callback(folderPath, name);
	}


	int FolderExplorer::compare(const int itemId1, const int itemId2) const {
		const auto it1 = pathItemsCache.left.find(itemId1);
		const auto it2 = pathItemsCache.left.find(itemId2);
		const auto end = pathItemsCache.left.end();

		assert(it1 != end && it2 != end);

		const Path path1 = it1->second;
		const Path path2 = it2->second;

		return path1.compare(path2).value;
	}


	bool FolderExplorer::itemIsPopulated(const int itemId) const {
		const auto it = populatedItems.find(itemId); 

		return it != populatedItems.end();
	}


	void FolderExplorer::insertItem(const int itemId, const Path path) {
		pathItemsCache.insert({itemId, path});
	}


	std::vector<FolderExplorerItem> FolderExplorer::populateItem(const int parentItemId) {
		std::vector<FolderExplorerItem> items;

		const auto folderPathIt = pathItemsCache.left.find(parentItemId);

        assert(folderPathIt != pathItemsCache.left.end());
        assert(folderPathIt->second.isFolder());

        const Folder folder { folderPathIt->second.value };
		const Path folderPath{ PathType::Folder, folder.path };

		const std::vector<Path> children = fileSystemService->enumerate(folder);

		for (const Path path : children) {
			const auto parentCacheIt = pathItemsCache.right.find(folderPath);

            if (parentCacheIt == pathItemsCache.right.end()) {
				continue;
            }

            const std::string name = fileSystemService->extractName(path);

			items.push_back(FolderExplorerItem{name, path});
		}

        populatedItems.insert(parentItemId);

		return items;
	}


	void FolderExplorer::populateItem(const int parentItemId, FolderExplorerCallback callback) {
        const auto folderPathIt = pathItemsCache.left.find(parentItemId);

        assert(folderPathIt != pathItemsCache.left.end());
        assert(folderPathIt->second.isFolder());

        const Folder folder { folderPathIt->second.value };
		const Path folderPath{ PathType::Folder, folder.path };

		const std::vector<Path> children = fileSystemService->enumerate(folder);

		for (const Path path : children) {
			const auto parentCacheIt = pathItemsCache.right.find(folderPath);

            if (parentCacheIt == pathItemsCache.right.end()) {
				continue;
            }

            const std::string name = fileSystemService->extractName(path);
            callback(path, name);
		}

        populatedItems.insert(parentItemId);
    }
}
