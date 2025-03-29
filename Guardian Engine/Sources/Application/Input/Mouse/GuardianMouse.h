#ifndef _GE_GUARDIANMOUSE_H_
#define _GE_GUARDIANMOUSE_H_
#include "../Keyboard/GuardianKeyboard.h"

namespace GE
{
	enum GUARDIAN_API GuardianMouseButtonState
	{
		GE_MOUSEBUTTONSTATE_RELEASE,
		GE_MOUSEBUTTONSTATE_CLICK,
	};

	class GUARDIAN_API GuardianMouse
	{
	public:
		static const bool IsMouseButtonClick(const GuardianMouseButtonCategory& button) noexcept;
		static const GuardianMouseButtonState& GetMouseButtonState(const GuardianMouseButtonCategory& button) noexcept;

		static void GetMousePosition(int& x, int& y) noexcept;

		static const bool IsMouseRawMoveEventListEmpty() noexcept;
		static const GuardianMouseRawMoveEvent ReadRawMoveEvent() noexcept;
		
	private:
		static void InitializeMouse();

		static std::shared_ptr<GuardianEventProcesser> MouseEventProcesser;
		static std::map<GuardianMouseButtonCategory, GuardianMouseButtonState> MouseButtonStateList;
		static std::queue<GuardianMouseRawMoveEvent> MouseRawMoveEventList;
		static int MousePositionX; 
		static int MousePositionY;

		friend class GuardianInput;
	};
}

#endif