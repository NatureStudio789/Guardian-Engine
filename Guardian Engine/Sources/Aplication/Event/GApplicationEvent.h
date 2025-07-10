#ifndef _GE_GAPPLICATIONEVENT_H_
#define _GE_GAPPLICATIONEVENT_H_
#include "GEventProcesser.h"

namespace GE
{
	class GUARDIAN_API GWindowResizeEvent : public GEvent
	{
	public:
		GWindowResizeEvent()
		{
			this->WindowHandle = null;
			this->ResizeWidth = 0;
			this->ResizeHeight = 0;
		}
		GWindowResizeEvent(HWND windowHandle, int width, int height)
		{
			this->WindowHandle = windowHandle;
			this->ResizeWidth = width;
			this->ResizeHeight = height;
		}
		GWindowResizeEvent(const GWindowResizeEvent& other)
		{
			this->WindowHandle = other.WindowHandle;
			this->ResizeWidth = other.ResizeWidth;
			this->ResizeHeight = other.ResizeHeight;
		}

		GUARDIAN_EVENT_TYPE(GE_EVENT_WINDOW_RESIZE)

		HWND WindowHandle;
		int ResizeWidth;
		int ResizeHeight;
	};

	class GUARDIAN_API GWindowMoveEvent : public GEvent
	{
	public:
		GWindowMoveEvent()
		{
			this->WindowHandle = null;
			this->WindowPositionX = 0;
			this->WindowPositionY = 0;
		}
		GWindowMoveEvent(HWND windowHandle, int x, int y)
		{
			this->WindowHandle = windowHandle;
			this->WindowPositionX = x;
			this->WindowPositionY = y;
		}
		GWindowMoveEvent(const GWindowMoveEvent& other)
		{
			this->WindowHandle = other.WindowHandle;
			this->WindowPositionX = other.WindowPositionX;
			this->WindowPositionY = other.WindowPositionY;
		}

		GUARDIAN_EVENT_TYPE(GE_EVENT_WINDOW_MOVE)

		HWND WindowHandle;
		int WindowPositionX;
		int WindowPositionY;
	};

	class GUARDIAN_API GWindowFocusEvent : public GEvent
	{
	public:
		GWindowFocusEvent()
		{
			this->WindowHandle = null;
		}
		GWindowFocusEvent(HWND hwnd)
		{
			this->WindowHandle = hwnd;
		}
		GWindowFocusEvent(const GWindowFocusEvent& other)
		{
			this->WindowHandle = other.WindowHandle;
		}

		GUARDIAN_EVENT_TYPE(GE_EVENT_WINDOW_FOCUS)

		HWND WindowHandle;
	};

	class GUARDIAN_API GWindowLostFocusEvent : public GEvent
	{
	public:
		GWindowLostFocusEvent()
		{
			this->WindowHandle = null;
		}
		GWindowLostFocusEvent(HWND hwnd)
		{
			this->WindowHandle = hwnd;
		}
		GWindowLostFocusEvent(const GWindowLostFocusEvent& other)
		{
			this->WindowHandle = other.WindowHandle;
		}

		GUARDIAN_EVENT_TYPE(GE_EVENT_WINDOW_LOSTFOCUS)

		HWND WindowHandle;
	};

	class GUARDIAN_API GWindowCloseEvent : public GEvent
	{
	public:
		GWindowCloseEvent()
		{
			this->WindowHandle = null;
		}
		GWindowCloseEvent(HWND hwnd)
		{
			this->WindowHandle = hwnd;
		}
		GWindowCloseEvent(const GWindowCloseEvent& other)
		{
			this->WindowHandle = other.WindowHandle;
		}

		GUARDIAN_EVENT_TYPE(GE_EVENT_WINDOW_CLOSE)

		HWND WindowHandle;
	};
}

#endif