#ifndef _GE_GUARDIANAPPLICATIONEVENT_H_
#define _GE_GUARDIANAPPLICATIONEVENT_H_
#include "GuardianEventProcesser.h"

namespace GE
{
	class GUARDIAN_API GuardianWindowResizeEvent : public GuardianEvent
	{
	public:
		GuardianWindowResizeEvent()
		{
			this->WindowHandle = null;
			this->ResizeWidth = 0;
			this->ResizeHeight = 0;
		}
		GuardianWindowResizeEvent(HWND windowHandle, int width, int height)
		{
			this->WindowHandle = windowHandle;
			this->ResizeWidth = width;
			this->ResizeHeight = height;
		}
		GuardianWindowResizeEvent(const GuardianWindowResizeEvent& other)
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

	class GUARDIAN_API GuardianWindowMoveEvent : public GuardianEvent
	{
	public:
		GuardianWindowMoveEvent()
		{
			this->WindowHandle = null;
			this->WindowPositionX = 0;
			this->WindowPositionY = 0;
		}
		GuardianWindowMoveEvent(HWND windowHandle, int x, int y)
		{
			this->WindowHandle = windowHandle;
			this->WindowPositionX = x;
			this->WindowPositionY = y;
		}
		GuardianWindowMoveEvent(const GuardianWindowMoveEvent& other)
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

	class GUARDIAN_API GuardianWindowFocusEvent : public GuardianEvent
	{
	public:
		GuardianWindowFocusEvent()
		{
			this->WindowHandle = null;
		}
		GuardianWindowFocusEvent(HWND hwnd)
		{
			this->WindowHandle = hwnd;
		}
		GuardianWindowFocusEvent(const GuardianWindowFocusEvent& other)
		{
			this->WindowHandle = other.WindowHandle;
		}

		GUARDIAN_EVENT_TYPE(GE_EVENT_WINDOW_FOCUS)

		HWND WindowHandle;
	};

	class GUARDIAN_API GuardianWindowLostFocusEvent : public GuardianEvent
	{
	public:
		GuardianWindowLostFocusEvent()
		{
			this->WindowHandle = null;
		}
		GuardianWindowLostFocusEvent(HWND hwnd)
		{
			this->WindowHandle = hwnd;
		}
		GuardianWindowLostFocusEvent(const GuardianWindowLostFocusEvent& other)
		{
			this->WindowHandle = other.WindowHandle;
		}

		GUARDIAN_EVENT_TYPE(GE_EVENT_WINDOW_LOSTFOCUS)

		HWND WindowHandle;
	};

	class GUARDIAN_API GuardianWindowCloseEvent : public GuardianEvent
	{
	public:
		GuardianWindowCloseEvent()
		{
			this->WindowHandle = null;
		}
		GuardianWindowCloseEvent(HWND hwnd)
		{
			this->WindowHandle = hwnd;
		}
		GuardianWindowCloseEvent(const GuardianWindowCloseEvent& other)
		{
			this->WindowHandle = other.WindowHandle;
		}

		GUARDIAN_EVENT_TYPE(GE_EVENT_WINDOW_CLOSE)

		HWND WindowHandle;
	};
}

#endif // !_GE_GUARDIANAPPLICATIONEVENT_H_