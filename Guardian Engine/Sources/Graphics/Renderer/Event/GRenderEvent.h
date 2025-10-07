#ifndef _GE_GRENDEREVENT_H_
#define _GE_GRENDEREVENT_H_
#include "../../../Aplication/Event/GEventProcesser.h"

namespace GE
{
	class GUARDIAN_API GRenderGraphResizeEvent : public GEvent
	{
	public:
		GRenderGraphResizeEvent()
		{
			this->RenderGraphName = "";
			this->ResizeWidth = 0;
			this->ResizeHeight = 0;
		}
		GRenderGraphResizeEvent(const std::string& name, int width, int height)
		{
			this->RenderGraphName = name;
			this->ResizeWidth = width;
			this->ResizeHeight = height;
		}
		GRenderGraphResizeEvent(const GRenderGraphResizeEvent& other)
		{
			this->RenderGraphName = other.RenderGraphName;
			this->ResizeWidth = other.ResizeWidth;
			this->ResizeHeight = other.ResizeHeight;
		}

		GUARDIAN_EVENT_TYPE(GE_EVENT_RENDER_RENDERGRAPHRESIZE)

		std::string RenderGraphName;
		int ResizeWidth;
		int ResizeHeight;
	};
}

#endif