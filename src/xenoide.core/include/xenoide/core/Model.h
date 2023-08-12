
#pragma once 

#include <memory>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <cassert>
#include <functional>
#include <optional>
#include <boost/bimap.hpp>


namespace Xenoide {
	struct Folder {
		std::string path;
	};


	struct File {
		std::string path;
	};


	enum class PathType {
		File,
		Folder
	};


	struct PathCompareResult {
		int value = 0;

		bool operator== (const PathCompareResult& rhs) const {
			return value == rhs.value;
		}

		bool operator!= (const PathCompareResult& rhs) const {
			return value != rhs.value;
		}

		bool operator< (const PathCompareResult& rhs) const {
			return value < rhs.value;
		}

		bool operator> (const PathCompareResult& rhs) const {
			return value > rhs.value;
		}

		bool operator<= (const PathCompareResult& rhs) const {
			return value <= rhs.value;
		}

		bool operator>= (const PathCompareResult& rhs) const {
			return value >= rhs.value;
		}
	};


	struct Path {
		PathType type;
		std::string value;

		bool isFolder() const {
			return type == PathType::Folder;
		}

		bool isFile() const {
			return type == PathType::File;
		}

		bool operator!= (const Path& other) const {
			return compare(other) != PathCompareResult{ 0 };
		}

		bool operator== (const Path& other) const {
			return compare(other) == PathCompareResult{ 0 };
		}

		bool operator< (const Path& other) const {
			return compare(other) < PathCompareResult{ 0 };
		}

		bool operator<= (const Path& other) const {
			return compare(other) <= PathCompareResult{ 0 };
		}

		bool operator> (const Path& other) const {
			return compare(other) > PathCompareResult{ 0 };
		}

		bool operator>= (const Path& other) const {
			return compare(other) >= PathCompareResult{ 0 };
		}

		PathCompareResult compare(const Path& rhs) const {
			if (type == PathType::Folder && rhs.type != PathType::Folder) {
				return PathCompareResult{ -1 };
			}

			if (type != PathType::Folder && rhs.type == PathType::Folder) {
				return PathCompareResult{ 1 };
			}

			return PathCompareResult{ value.compare(rhs.value) };
		}


		static Path file(const std::string& value) {
			return {PathType::File, value};
		}

		static Path folder(const std::string& value) {
			return {PathType::Folder, value};
		}
	};


	enum class BuildSystemType {
		None,
		CMake
	};

	enum class VersionControlSystem {
		None,
		Git
	};


	struct WorkspaceConfig {
		BuildSystemType buildSystem = BuildSystemType::None;
		VersionControlSystem versionControlSystem = VersionControlSystem::None;
	};


	class Workspace {
	public:
		Workspace(Folder folder, WorkspaceConfig config)
			: folder(folder), config(config) {}

		Folder getFolder() const {
			return folder;
		}


	private:
		Folder folder;
		WorkspaceConfig config;
	};


	struct WorkspaceProbeResult {
		std::vector<BuildSystemType> buildSystems;
		std::vector<VersionControlSystem> versionControlSystems;
	};


	class WorkspaceManager {
	public:
		WorkspaceManager() {}

		WorkspaceProbeResult probe(const std::string& folderPath) {
			return {
				{ BuildSystemType::None },
				{ VersionControlSystem::None }
			};
		}


		Workspace* create(Folder folder, WorkspaceConfig config) {
			auto workspace = std::make_unique<Workspace>(folder, config);
			auto ptr = workspace.get();

			workspaceMap.insert({ folder.path, std::move(workspace) });

			return ptr;
		}


		void close(Workspace* workspace) {
			assert(workspace);
			
			using Pair = std::pair<const std::string, std::unique_ptr<Xenoide::Workspace>>;

			auto it = std::find_if(workspaceMap.begin(), workspaceMap.end(), [workspace](const Pair &pair) {
				return workspace == pair.second.get();
			});

			assert(it == workspaceMap.end());

			workspaceMap.erase(it);
		}

	private:
		std::map<std::string, std::unique_ptr<Workspace>> workspaceMap;
	};
}
