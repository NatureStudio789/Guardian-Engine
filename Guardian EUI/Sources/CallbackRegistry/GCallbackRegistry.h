#ifndef _GE_GCALLBACKREGISTRY_H_
#define _GE_GCALLBACKREGISTRY_H_
#include "../Widget/Dockspace/GDockspace.h"

namespace GE
{
	namespace EUI
	{
		class GCallbackRegistry
		{
		public:
			static void RegistryCallback(const std::string& triggerName, const std::function<void()>& callback);

			static void TriggerCallback(const std::string& name);

		private:
			static std::unordered_map<std::string, std::vector<std::function<void()>>> CallbackFunctionList;
		};
	}
}

#endif