#include "GCallbackRegistry.h"

namespace GE
{
	namespace EUI
	{
		std::unordered_map<std::string, std::vector<std::function<void()>>> GCallbackRegistry::CallbackFunctionList;



		void GCallbackRegistry::RegistryCallback(const std::string& triggerName, const std::function<void()>& callback)
		{
			CallbackFunctionList[triggerName].push_back(callback);
		}

		void GCallbackRegistry::TriggerCallback(const std::string& name)
		{
			if (CallbackFunctionList.count(name))
			{
				for (auto& callback : CallbackFunctionList[name])
				{
					if (callback)
					{
						callback();
					}
				}
			}
		}
	}
}
