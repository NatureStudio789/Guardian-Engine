#ifndef _GE_GUARDIANEVENTPROCESSER_H_
#define _GE_GUARDIANEVENTPROCESSER_H_
#include "GuardianEventDispatcher.h"
#include "GuardianEvent.h"

namespace guardian
{
	class GUARDIAN_API GuardianEventProcesser
	{
	public:
		GuardianEventProcesser() = default;
		GuardianEventProcesser(const GuardianEventProcesser&) = default;
		~GuardianEventProcesser() = default;

		template<typename T>
		void OnEvent(const std::function<void(const T&)>& function);
	};

	template<typename T>
	inline void GuardianEventProcesser::OnEvent(const std::function<void(const T&)>& function)
	{
		GuardianEventDispatcher::DispatcherInstance->RegisterEventProcesser(this, function);
	}
}

#endif