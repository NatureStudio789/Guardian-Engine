#ifndef _GE_GUARDIANEVENTDISPATCHER_H_
#define _GE_GUARDIANEVENTDISPATCHER_H_
#include "GuardianEvent.h"

namespace guardian
{
	class GUARDIAN_API GuardianEventProcesser;
	using GuardianEventProcessFunction = std::function<void(GuardianEvent& e)>;

	struct GUARDIAN_API GuardianEventHandler
	{
		GuardianEventProcesser* EventProcesser;
		GuardianEventProcessFunction ProcessFunction;
	};

	class GUARDIAN_API GuardianEventDispatcher
	{
	public:
		GuardianEventDispatcher() = default;
		GuardianEventDispatcher(const GuardianEventDispatcher&) = delete;
		~GuardianEventDispatcher() = default;

		template<typename T>
		void RegisterEventProcesser(GuardianEventProcesser* processer, const std::function<void(const T&)>& function);
		void DestroyEventProcesser(GuardianEventProcesser* processer);
		void DispatchEvent(GuardianEvent& event);

		static std::unique_ptr<GuardianEventDispatcher> DispatcherInstance;
	
	private:
		std::unordered_map<GuardianEventType, std::vector<GuardianEventHandler>> EventProcesserList;
	};

	template<typename T>
	inline void GuardianEventDispatcher::RegisterEventProcesser(
		GuardianEventProcesser* processer, const std::function<void(const T&)>& function)
	{
		if (!processer)
		{
			throw GUARDIAN_VALUE_EXCEPTION((long long)processer);
		}

		if (!function)
		{
			throw GUARDIAN_VALUE_EXCEPTION(0);
		}

		auto EventFunction = [function](const GuardianEvent& e)
		{
			const T& event = (const T&)e;

			return	function(event);
		};

		GuardianEventHandler handler =
		{
			.EventProcesser = processer,
			.ProcessFunction = EventFunction
		};

		this->EventProcesserList[T::GetStaticEventType()].emplace_back(handler);
	}
}

#endif