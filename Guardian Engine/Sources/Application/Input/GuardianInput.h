#ifndef _GE_GUARDIANINPUT_H_
#define _GE_GUARDIANINPUT_H_
#include "Mouse/GuardianMouse.h"

namespace GE
{
	class GUARDIAN_API GuardianInput : public GuardianMouse, public GuardianKeyboard
	{
	public:
		

	private:
		static void InitializeInput();

		friend class GuardianApplication;
	};
}

#endif