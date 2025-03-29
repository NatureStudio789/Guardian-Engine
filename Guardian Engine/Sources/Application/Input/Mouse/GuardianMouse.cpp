#include "GuardianMouse.h"

namespace GE
{
	std::shared_ptr<GuardianEventProcesser> GuardianMouse::MouseEventProcesser = std::make_shared<GuardianEventProcesser>();
	std::map<GuardianMouseButtonCategory, GuardianMouseButtonState> GuardianMouse::MouseButtonStateList;
	std::queue<GuardianMouseRawMoveEvent> GuardianMouse::MouseRawMoveEventList;
	int GuardianMouse::MousePositionX;
	int GuardianMouse::MousePositionY;


	const bool GuardianMouse::IsMouseButtonClick(const GuardianMouseButtonCategory& button) noexcept
	{
		return MouseButtonStateList[button] == GE_MOUSEBUTTONSTATE_CLICK;
	}

	const GuardianMouseButtonState& GuardianMouse::GetMouseButtonState(const GuardianMouseButtonCategory& button) noexcept
	{
		return MouseButtonStateList[button];
	}

	void GuardianMouse::GetMousePosition(int& x, int& y) noexcept
	{
		x = MousePositionX;
		y = MousePositionY;
	}

	const bool GuardianMouse::IsMouseRawMoveEventListEmpty() noexcept
	{
		return MouseRawMoveEventList.empty();
	}

	const GuardianMouseRawMoveEvent GuardianMouse::ReadRawMoveEvent() noexcept
	{
		if (IsMouseRawMoveEventListEmpty())
		{
			return GuardianMouseRawMoveEvent();
		}
		else
		{
			GuardianMouseRawMoveEvent event = MouseRawMoveEventList.front();
			MouseRawMoveEventList.pop();
			return event;
		}
	}

	void GuardianMouse::InitializeMouse()
	{
		RAWINPUTDEVICE RawInputDevice = { 0 };
		RawInputDevice.usUsagePage = 0x01;
		RawInputDevice.usUsage = 0x02;
		RawInputDevice.dwFlags = 0;
		RawInputDevice.hwndTarget = null;

		if (!RegisterRawInputDevices(&RawInputDevice, 1, sizeof(RawInputDevice)))
		{
			throw GUARDIAN_FAILURE_EXCEPTION("RegisterRawInputDevices()");
		}

		MouseButtonStateList[GE_MOUSEBUTTON_LEFT] = GE_MOUSEBUTTONSTATE_RELEASE;
		MouseButtonStateList[GE_MOUSEBUTTON_MIDDLE] = GE_MOUSEBUTTONSTATE_RELEASE;
		MouseButtonStateList[GE_MOUSEBUTTON_RIGHT] = GE_MOUSEBUTTONSTATE_RELEASE;

		SetCursorPos(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);

		MouseEventProcesser->OnEvent<GuardianMouseButtonClickEvent>([](const GuardianMouseButtonClickEvent& event)
		{
			MouseButtonStateList[event.MouseButton] = GE_MOUSEBUTTONSTATE_CLICK;
		});

		MouseEventProcesser->OnEvent<GuardianMouseButtonReleaseEvent>([](const GuardianMouseButtonReleaseEvent& event)
		{
			MouseButtonStateList[event.MouseButton] = GE_MOUSEBUTTONSTATE_RELEASE;
		});

		MouseEventProcesser->OnEvent<GuardianMouseMoveEvent>([](const GuardianMouseMoveEvent& event)
		{
			MousePositionX = event.MousePositionX;
			MousePositionY = event.MousePositionY;
		});

		MouseEventProcesser->OnEvent<GuardianMouseRawMoveEvent>([](const GuardianMouseRawMoveEvent& event)
		{
			MouseRawMoveEventList.push(event);
		});
	}
}
