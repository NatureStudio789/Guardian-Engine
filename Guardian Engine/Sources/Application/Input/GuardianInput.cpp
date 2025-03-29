#include "GuardianInput.h"

namespace GE
{
	void GuardianInput::InitializeInput()
	{
		GuardianKeyboard::InitializeKeyboard();

		GuardianMouse::InitializeMouse();
	}
}
