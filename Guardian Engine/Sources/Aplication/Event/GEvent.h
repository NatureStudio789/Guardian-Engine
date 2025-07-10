#ifndef _GE_GEVENT_H_
#define _GE_GEVENT_H_
#include "../Exception/GException.h"

namespace GE
{
#define GUARDIAN_EVENT_TYPE(type) static Category GetStaticEventType() { return Category::##type; }\
									Category GetEventType() override { return GetStaticEventType(); }\
									virtual const std::string GetEventName() const override { return #type; }

	class GUARDIAN_API GEvent
	{
	public:
		enum Category
		{
			GE_EVENT_NONE,

			GE_EVENT_MODE_SWITCH,

			GE_EVENT_WINDOW_RESIZE,
			GE_EVENT_WINDOW_FINISHRESIZING,
			GE_EVENT_WINDOW_MOVE,
			GE_EVENT_WINDOW_FOCUS,
			GE_EVENT_WINDOW_LOSTFOCUS,
			GE_EVENT_WINDOW_CLOSE,

			GE_EVENT_KEY_PRESS,
			GE_EVENT_KEY_RELEASE,
			GE_EVENT_CHAR,

			GE_EVENT_MOUSE_BUTTONCLICK,
			GE_EVENT_MOUSE_BUTTONRELEASE,
			GE_EVENT_MOUSE_MOVE,
			GE_EVENT_MOUSE_RAWMOVE,
			GE_EVENT_MOUSE_SCROLL,
		};

	public:
		virtual Category GetEventType() = 0;
		virtual const std::string GetEventName() const = 0;

	protected:
	};
}

#endif