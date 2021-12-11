
#pragma once 

#include "Model.h"


namespace Xenoide {
	class FileSystemService;
	class FolderExplorer {
	public:
		explicit FolderExplorer(Folder folder, FileSystemService* fileSystemService);

	private:
		Folder folder;
		FileSystemService* fileSystemService = nullptr;
	};
}
