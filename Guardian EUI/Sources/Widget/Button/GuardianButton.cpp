#include "GuardianButton.h"

namespace GE
{
	namespace EUI
	{
		GuardianButton::GuardianButton(
			const std::string& label, const std::function<void()>& callback, const GVector2& size)
		{
			this->ButtonLabel = label;
			this->ButtonCallbackFunction = callback;
			this->ButtonSize = size;
		}

		void GuardianButton::SetButtonLabel(const std::string& label)
		{
			this->ButtonLabel = label;
		}

		void GuardianButton::SetButtonCallbackFunction(const std::function<void()>& callback)
		{
			this->ButtonCallbackFunction = callback;
		}

		void GuardianButton::SetButtonSize(const GVector2& size)
		{
			this->ButtonSize = size;
		}

		void GuardianButton::Update()
		{

		}

		void GuardianButton::Render()
		{
			bool isClicked = ImGui::Button(this->ButtonLabel.c_str(),
				ImVec2(this->ButtonSize.x, this->ButtonSize.y));

			this->CallWidgetEventProcessFunction();

			if (isClicked)
			{
				if (this->ButtonCallbackFunction)
				{
					this->ButtonCallbackFunction();
				}
			}
		}
	}
}
