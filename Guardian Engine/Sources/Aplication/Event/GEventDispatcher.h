#ifndef _GE_GEVENTDISPATCHER_H_
#define _GE_GEVENTDISPATCHER_H_
#include "GEvent.h"

namespace GE
{
	class GUARDIAN_API GEventProcesser;
	using GEventProcessFunction = std::function<void(GEvent& e)>;

	struct GUARDIAN_API GEventHandler
	{
		GEventProcesser* EventProcesser;
		GEventProcessFunction ProcessFunction;
	};

	class GUARDIAN_API GEventDispatcher
	{
	public:
		GEventDispatcher() = default;
		GEventDispatcher(const GEventDispatcher&) = delete;
		~GEventDispatcher() = default;

		template<typename T>
		void RegisterEventProcesser(GEventProcesser* processer, const std::function<void(const T&)>& function);
		void DestroyEventProcesser(GEventProcesser* processer);
		void DispatchEvent(GEvent& event);

		static std::unique_ptr<GEventDispatcher> DispatcherInstance;
	
	private:
		std::unordered_map<GEvent::Category, std::vector<GEventHandler>> EventProcesserList;
	};

	template<typename T>
	inline void GEventDispatcher::RegisterEventProcesser(
		GEventProcesser* processer, const std::function<void(const T&)>& function)
	{
		if (!processer)
		{
			throw GUARDIAN_VALUE_EXCEPTION((long long)processer);
		}

		if (!function)
		{
			throw GUARDIAN_VALUE_EXCEPTION(0);
		}

		auto EventFunction = [function](const GEvent& e)
		{
			const T& event = (const T&)e;

			return	function(event);
		};

		GEventHandler handler =
		{
			.EventProcesser = processer,
			.ProcessFunction = EventFunction
		};

		this->EventProcesserList[T::GetStaticEventType()].emplace_back(handler);
	}
}

#endif