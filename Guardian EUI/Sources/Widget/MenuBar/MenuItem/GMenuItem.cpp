#include "GMenuItem.h"

namespace GE
{
	namespace EUI
	{
		GMenuItem::GMenuItem(const std::string& label, const std::function<void()>& callback)
		{
			this->SetMenuItemLabel(label);
			this->SetMenuItemCallback(callback);
		}

		void GMenuItem::SetMenuItemLabel(const std::string& label)
		{
			this->ItemLabel = label;
		}

		void GMenuItem::SetMenuItemCallback(const std::function<void()> callback)
		{
			this->ItemCallback = callback;
		}

		void GMenuItem::Update()
		{

		}

		void GMenuItem::Render()
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
