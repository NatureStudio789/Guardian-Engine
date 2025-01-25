#include "GuardianInput.h"

namespace guardian
{
	void GuardianInput::InitializeInput()
	{
		GuardianKeyboard::InitializeKeyboard();

		GuardianMouse::InitializeMouse();
	}
}
