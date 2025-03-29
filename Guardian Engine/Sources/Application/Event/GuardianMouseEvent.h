#ifndef _GE_GUARDIANMOUSEEVENT_H_
#define _GE_GUARDIANMOUSEEVENT_H_
#include "GuardianKeyboardEvent.h"

namespace GE
{
	enum GUARDIAN_API GuardianMouseButtonCategory
	{
		GE_MOUSEBUTTON_NONE,

		GE_MOUSEBUTTON_LEFT,
		GE_MOUSEBUTTON_MIDDLE,
		GE_MOUSEBUTTON_RIGHT,
	};

	class GUARDIAN_API GuardianMouseButtonClickEvent : public GuardianEvent
	{
	public:
		GuardianMouseButtonClickEvent()
		{
			this->MouseButton = GE_MOUSEBUTTON_NONE;
		}
		GuardianMouseButtonClickEvent(const GuardianMouseButtonCategory& button)
		{
			this->MouseButton = button;
		}
		GuardianMouseButtonClickEvent(const GuardianMouseButtonClickEvent& other)
		{
			this->MouseButton = other.MouseButton;
		}

		GUARDIAN_EVENT_TYPE(GE_EVENT_MOUSE_BUTTONCLICK)

		GuardianMouseButtonCategory MouseButton;
	};

	class GUARDIAN_API GuardianMouseButtonReleaseEvent : public GuardianEvent
	{
	public:
		GuardianMouseButtonReleaseEvent()
		{
			this->MouseButton = GE_MOUSEBUTTON_NONE;
		}
		GuardianMouseButtonReleaseEvent(const GuardianMouseButtonCategory& button)
		{
			this->MouseButton = button;
		}
		GuardianMouseButtonReleaseEvent(const GuardianMouseButtonReleaseEvent& other)
		{
			this->MouseButton = other.MouseButton;
		}

		GUARDIAN_EVENT_TYPE(GE_EVENT_MOUSE_BUTTONRELEASE)

		GuardianMouseButtonCategory MouseButton;
	};

	class GUARDIAN_API GuardianMouseMoveEvent : public GuardianEvent
	{
	public:
		GuardianMouseMoveEvent()
		{
			this->MousePositionX = 0;
			this->MousePositionY = 0;
		}
		GuardianMouseMoveEvent(int x, int y)
		{
			this->MousePositionX = x;
			this->MousePositionY = y;
		}
		GuardianMouseMoveEvent(const GuardianMouseMoveEvent& other)
		{
			this->MousePositionX = other.MousePositionX;
			this->MousePositionY = other.MousePositionY;
		}

		GUARDIAN_EVENT_TYPE(GE_EVENT_MOUSE_MOVE)

		int MousePositionX;
		int MousePositionY;
	};

	class GUARDIAN_API GuardianMouseRawMoveEvent : public GuardianEvent
	{
	public:
		GuardianMouseRawMoveEvent()
		{
			this->MouseMovementX = 0.0f;
			this->MouseMovementY = 0.0f;
		}
		GuardianMouseRawMoveEvent(float movementX, float movementY)
		{
			this->MouseMovementX = movementX;
			this->MouseMovementY = movementY;
		}
		GuardianMouseRawMoveEvent(const GuardianMouseRawMoveEvent& other)
		{
			this->MouseMovementX = other.MouseMovementX;
			this->MouseMovementY = other.MouseMovementY;
		}

		GUARDIAN_EVENT_TYPE(GE_EVENT_MOUSE_RAWMOVE)

		float MouseMovementX;
		float MouseMovementY;
	};
}

#endif // !_GE_GUARDIANMOUSEEVENT_H_