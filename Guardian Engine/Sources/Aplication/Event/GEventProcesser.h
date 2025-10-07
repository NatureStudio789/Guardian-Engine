#ifndef _GE_GEVENTPROCESSER_H_
#define _GE_GEVENTPROCESSER_H_
#include "GEventDispatcher.h"
#include "GEvent.h"

namespace GE
{
	class GWindowResizeEvent;
	class GRenderGraphResizeEvent;

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

	template GUARDIAN_API void GEventProcesser::OnEvent<GWindowResizeEvent>(const std::function<void(const GWindowResizeEvent&)>& function);
	template GUARDIAN_API void GEventProcesser::OnEvent<GRenderGraphResizeEvent>(const std::function<void(const GRenderGraphResizeEvent&)>& function);
}

#endif