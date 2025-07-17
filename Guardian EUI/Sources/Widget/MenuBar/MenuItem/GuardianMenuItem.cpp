#include "GuardianMenuItem.h"

namespace GE
{
	namespace EUI
	{
		GuardianMenuItem::GuardianMenuItem(const std::string& label, const std::function<void()>& callback)
		{
			this->SetMenuItemLabel(label);
			this->SetMenuItemCallback(callback);
		}

		void GuardianMenuItem::SetMenuItemLabel(const std::string& label)
		{
			this->ItemLabel = label;
		}

		void GuardianMenuItem::SetMenuItemCallback(const std::function<void()> callback)
		{
			this->ItemCallback = callback;
		}

		void GuardianMenuItem::Update()
		{

		}

		void GuardianMenuItem::Render()
		{
			bool clicked = ImGui::MenuItem(this->ItemLabel.c_str());

			this->CallWidgetEventProcessFunction();

			if (clicked)
			{
				if (this->ItemCallback)
				{
					this->ItemCallback();
				}
			}
		}
	}
}
