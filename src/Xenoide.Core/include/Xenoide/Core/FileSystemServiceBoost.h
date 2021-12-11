
#pragma once 

#include "FileSystemService.h"

namespace Xenoide {
	class FileSystemServiceBoost : public FileSystemService {
	public:
		virtual ~FileSystemServiceBoost();

		bool exists(const Path& path) const override;

		void enumerate(const Folder& folder, FileSystemVisitor visitor);
	};
}
