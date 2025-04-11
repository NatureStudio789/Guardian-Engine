#include "GuardianMenuItem.h"

namespace GE
{
	GuardianMenuItem::GuardianMenuItem(const GString& label, const std::function<void()>& callback)
	{
		this->SetMenuItemLabel(label);
		this->SetMenuItemCallback(callback);
	}

	void GuardianMenuItem::SetMenuItemLabel(const GString& label)
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
