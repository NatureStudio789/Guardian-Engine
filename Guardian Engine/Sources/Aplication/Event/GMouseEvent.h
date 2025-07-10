#ifndef _GE_GMOUSEEVENT_H_
#define _GE_GMOUSEEVENT_H_
#include "GKeyboardEvent.h"

namespace GE
{
	enum GUARDIAN_API GMouseButtonCategory
	{
		GE_MOUSEBUTTON_NONE,

		GE_MOUSEBUTTON_LEFT,
		GE_MOUSEBUTTON_MIDDLE,
		GE_MOUSEBUTTON_RIGHT,
	};

	class GUARDIAN_API GMouseButtonClickEvent : public GEvent
	{
	public:
		GMouseButtonClickEvent()
		{
			this->MouseButton = GE_MOUSEBUTTON_NONE;
		}
		GMouseButtonClickEvent(const GMouseButtonCategory& button)
		{
			this->MouseButton = button;
		}
		GMouseButtonClickEvent(const GMouseButtonClickEvent& other)
		{
			this->MouseButton = other.MouseButton;
		}

		GUARDIAN_EVENT_TYPE(GE_EVENT_MOUSE_BUTTONCLICK)

		GMouseButtonCategory MouseButton;
	};

	class GUARDIAN_API GMouseButtonReleaseEvent : public GEvent
	{
	public:
		GMouseButtonReleaseEvent()
		{
			this->MouseButton = GE_MOUSEBUTTON_NONE;
		}
		GMouseButtonReleaseEvent(const GMouseButtonCategory& button)
		{
			this->MouseButton = button;
		}
		GMouseButtonReleaseEvent(const GMouseButtonReleaseEvent& other)
		{
			this->MouseButton = other.MouseButton;
		}

		GUARDIAN_EVENT_TYPE(GE_EVENT_MOUSE_BUTTONRELEASE)

		GMouseButtonCategory MouseButton;
	};

	class GUARDIAN_API GMouseMoveEvent : public GEvent
	{
	public:
		GMouseMoveEvent()
		{
			this->MousePositionX = 0;
			this->MousePositionY = 0;
		}
		GMouseMoveEvent(int x, int y)
		{
			this->MousePositionX = x;
			this->MousePositionY = y;
		}
		GMouseMoveEvent(const GMouseMoveEvent& other)
		{
			this->MousePositionX = other.MousePositionX;
			this->MousePositionY = other.MousePositionY;
		}

		GUARDIAN_EVENT_TYPE(GE_EVENT_MOUSE_MOVE)

		int MousePositionX;
		int MousePositionY;
	};

	class GUARDIAN_API GMouseRawMoveEvent : public GEvent
	{
	public:
		GMouseRawMoveEvent()
		{
			this->MouseMovementX = 0.0f;
			this->MouseMovementY = 0.0f;
		}
		GMouseRawMoveEvent(float movementX, float movementY)
		{
			this->MouseMovementX = movementX;
			this->MouseMovementY = movementY;
		}
		GMouseRawMoveEvent(const GMouseRawMoveEvent& other)
		{
			this->MouseMovementX = other.MouseMovementX;
			this->MouseMovementY = other.MouseMovementY;
		}

		GUARDIAN_EVENT_TYPE(GE_EVENT_MOUSE_RAWMOVE)

		float MouseMovementX;
		float MouseMovementY;
	};
}

#endif