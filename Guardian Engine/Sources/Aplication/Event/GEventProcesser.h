#ifndef _GE_GEVENTPROCESSER_H_
#define _GE_GEVENTPROCESSER_H_
#include "GEventDispatcher.h"
#include "GEvent.h"

namespace GE
{
	class GUARDIAN_API GEventProcesser
	{
	public:
		GEventProcesser() = default;
		GEventProcesser(const GEventProcesser&) = default;
		~GEventProcesser() = default;

		template<typename T>
		void OnEvent(const std::function<void(const T&)>& function);
	};

	template<typename T>
	inline void GEventProcesser::OnEvent(const std::function<void(const T&)>& function)
	{
		GEventDispatcher::DispatcherInstance->RegisterEventProcesser(this, function);
	}
}

#endif