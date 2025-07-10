#include "GEventDispatcher.h"

namespace GE
{
	std::unique_ptr<GEventDispatcher> GEventDispatcher::DispatcherInstance = std::make_unique<GEventDispatcher>();


	void GEventDispatcher::DestroyEventProcesser(GEventProcesser* processer)
	{
		for (auto& iterator : this->EventProcesserList)
		{
			iterator.second.erase(std::remove_if(iterator.second.begin(), iterator.second.end(),
				[processer](const GEventHandler& handler)
				{
					return (handler.EventProcesser && handler.EventProcesser == processer);
				}), iterator.second.end());
		}
	}

	void GEventDispatcher::DispatchEvent(GEvent& event)
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
