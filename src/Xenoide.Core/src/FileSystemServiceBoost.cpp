
#include <Xenoide/Core/FileSystemServiceBoost.h>

#include <boost/filesystem.hpp>

namespace Xenoide {
	FileSystemServiceBoost::~FileSystemServiceBoost() {}


	bool FileSystemServiceBoost::exists(const Path& path) const {
		return boost::filesystem::exists(
			boost::filesystem::path(path.value)
		);
	}


	void FileSystemServiceBoost::enumerate(const Folder& folder, FileSystemVisitor visitor) {
		// using boost::filesystem::recursive_directory_iterator;
		using boost::filesystem::directory_iterator;
		using boost::filesystem::is_directory;
		using boost::filesystem::path;

		directory_iterator current{folder.path}, end;

		while (current != end) {
			const path currentPath = current->path();

			const bool continue_ = visitor({
				is_directory(currentPath) ? PathType::Folder : PathType::File,
				currentPath.string()
			});

			if (!continue_) {
				break;
			}

			++current;
		}
	}
}
