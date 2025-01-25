#ifndef _GE_GUARDIANKEYBOARD_H_
#define _GE_GUARDIANKEYBOARD_H_
#include "../../Event/GuardianMouseEvent.h"

namespace guardian
{
	enum GUARDIAN_API GuardianKeyboardState
	{
		GE_KEY_RELEASE,
		GE_KEY_PRESS,
	};

	class GUARDIAN_API GuardianKeyboard
	{
	public:
		static const std::vector<UCHAR> GetCharEventList();

		static const bool IsKeyPress(UINT keyCode) noexcept;
		static const GuardianKeyboardState& GetKeyState(UINT keyCode) noexcept;

	private:
		static void InitializeKeyboard();
		
		static std::shared_ptr<GuardianEventProcesser> KeyboardEventProcesser;
		static std::map<UINT, GuardianKeyboardState> KeyStateList;
		static std::vector<UCHAR> CharEventList;
		
		friend class GuardianInput;
	};
}

#endif