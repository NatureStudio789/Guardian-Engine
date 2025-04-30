#include "GuardianKeyboard.h"

namespace GE
{
	std::shared_ptr<GuardianEventProcesser> GuardianKeyboard::KeyboardEventProcesser = std::make_shared<GuardianEventProcesser>();
	std::map<UINT, GuardianKeyboardState> GuardianKeyboard::KeyStateList;
	std::vector<UCHAR> GuardianKeyboard::CharEventList;


	const std::vector<UCHAR> GuardianKeyboard::GetCharEventList()
	{
		std::vector<UCHAR> charList = CharEventList;
		CharEventList.clear();
		return charList;
	}

	const bool GuardianKeyboard::IsKeyPress(UINT keyCode) noexcept
	{
		return KeyStateList[keyCode] == GE_KEY_PRESS;
	}

	const GuardianKeyboardState& GuardianKeyboard::GetKeyState(UINT keyCode) noexcept
	{
		return KeyStateList[keyCode];
	}

	void GuardianKeyboard::InitializeKeyboard()
	{
		for (UINT i = 0; i < 256; i++)
		{
			KeyStateList[i] = GE_KEY_RELEASE;
		}

		KeyboardEventProcesser->OnEvent<GuardianKeyPressEvent>([](const GuardianKeyPressEvent& event)
		{
			KeyStateList[event.KeyCode] = GE_KEY_PRESS;
		});

		KeyboardEventProcesser->OnEvent<GuardianKeyReleaseEvent>([](const GuardianKeyReleaseEvent& event)
		{
			KeyStateList[event.KeyCode] = GE_KEY_RELEASE;
		});

		KeyboardEventProcesser->OnEvent<GuardianWindowLostFocusEvent>([](const GuardianWindowLostFocusEvent& event)
		{
			for (UINT i = 0; i < 256; i++)
			{
				KeyStateList[i] = GE_KEY_RELEASE;
			}
		});

		KeyboardEventProcesser->OnEvent<GuardianCharEvent>([](const GuardianCharEvent& event)
		{
			CharEventList.push_back(event.CharCode);
		});
	}
}
