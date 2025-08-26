#include "GPopup.h"

namespace GE
{
	namespace EUI
	{
		GPopup::GPopup(const std::string& name)
		{
			this->PopupName = name;
		}

		void GPopup::AddWidgetToPopup(std::shared_ptr<GWidget> widget)
		{
			this->WidgetList.push_back(widget);
		}

		void GPopup::ClearPopupWidget()
		{
			this->WidgetList.clear();
		}

		void GPopup::ActivePopup()
		{
			ImGui::OpenPopup(this->PopupName.c_str());
		}

		void GPopup::Update()
		{
			for (auto& widget : this->WidgetList) 
			{
				widget->Update();
			}
		}

		void GPopup::Render()
		{
			if (ImGui::BeginPopup(this->PopupName.c_str()))
			{
				for (auto& widget : this->WidgetList)
				{
					widget->Render();
				}

				ImGui::EndPopup();
			}
		}
	}
}
