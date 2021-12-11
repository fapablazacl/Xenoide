
#include <Xenoide/Core/FolderExplorer.h>
#include <Xenoide/Core/FileSystemService.h>

#include <cassert>

namespace Xenoide {
	FolderExplorer::FolderExplorer(Folder folder, FileSystemService* fileSystemService) 
		: folder(folder), fileSystemService(fileSystemService) {

		assert(fileSystemService);
	}
}
