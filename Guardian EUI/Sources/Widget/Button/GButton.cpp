#include "GButton.h"

namespace GE
{
	namespace EUI
	{
		GButton::GButton(
			const std::string& label, const std::function<void()>& callback, const GVector2& size)
		{
			this->ButtonLabel = label;
			this->ButtonCallbackFunction = callback;
			this->ButtonSize = size;
		}

		void GButton::SetButtonLabel(const std::string& label)
		{
			this->ButtonLabel = label;
		}

		void GButton::SetButtonCallbackFunction(const std::function<void()>& callback)
		{
			this->ButtonCallbackFunction = callback;
		}

		void GButton::SetButtonSize(const GVector2& size)
		{
			this->ButtonSize = size;
		}

		void GButton::Update()
		{

		}

		void GButton::Render()
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
