
#pragma once

#include <xeno/wtl/wtl.h>

#include <string>
#include <vector>
#include <map>
#include <xeno/ui/Menu.h>


namespace Xenoide {
	class CCommandMap {
	public:
		CCommandMap();

		explicit CCommandMap(const std::map<int, std::function<void()>>& commandIdCallbackMap);

		~CCommandMap();

		bool OnCommand(const int commandId) const;

	private:
		std::map<int, std::function<void()>> commandIdCallbackMap;
	};
}


namespace Xenoide {
	class CMenuFactory {
	public:
		explicit CMenuFactory(const int commandBase);

		void fillMenu(HMENU hMenu, CCommandMap &commandMap, const std::vector<Xenoide::MenuData>& popupMenuItems) const;

	private:
		void fillMenuItem(CMenuHandle parent, std::map<int, std::function<void()>>& map, const Xenoide::MenuData& menu) const;

		std::string mapMenuTitle(const Xenoide::MenuData& menu) const;

	private:
		int commandBase = 0;
	};

	class CMenuFactory2 {
	public:
		void fillMenu(HMENU hMenu, const std::vector<MenuData>& popupMenuItems) const;

	private:
		void fillMenuItem(CMenuHandle parent, const MenuData& menu) const;

		std::string mapMenuTitle(const MenuData& menu) const;
	};
}
