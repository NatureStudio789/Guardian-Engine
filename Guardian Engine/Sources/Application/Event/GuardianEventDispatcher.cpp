#include "GuardianEventDispatcher.h"

namespace guardian
{
	std::unique_ptr<GuardianEventDispatcher> GuardianEventDispatcher::DispatcherInstance = std::make_unique<GuardianEventDispatcher>();


	void GuardianEventDispatcher::DestroyEventProcesser(GuardianEventProcesser* processer)
	{
		for (auto& iterator : this->EventProcesserList)
		{
			iterator.second.erase(std::remove_if(iterator.second.begin(), iterator.second.end(),
				[processer](const GuardianEventHandler& handler)
				{
					return (handler.EventProcesser && handler.EventProcesser == processer);
				}), iterator.second.end());
		}
	}

	void GuardianEventDispatcher::DispatchEvent(GuardianEvent& event)
	{
		if (this->EventProcesserList.find(event.GetEventType()) == this->EventProcesserList.end())
		{
			return;
		}

		auto& processers = this->EventProcesserList[event.GetEventType()];
		if (processers.empty())
		{
			return;
		}

		for (auto& processer : processers)
		{
			if (processer.EventProcesser)
			{
				processer.ProcessFunction(event);
			}
		}
	}
}
